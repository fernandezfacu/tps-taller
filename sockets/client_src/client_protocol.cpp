#include "client_protocol.h"

#include <sstream>
#include <stdexcept>
#include <vector>

#include "../common_src/protocol_constants.h"

ClientProtocol::ClientProtocol(const std::string& hostname, const std::string& service,
                               const std::string& username):
        skt(hostname.c_str(), service.c_str()) {
    send_username(username);
    get_protocol_type();
}

void ClientProtocol::get_protocol_type() {
    std::vector<uint8_t> protocol_buf(2, 0);
    unsigned int index = 0;
    this->helper.recv_all_bytes(this->skt, protocol_buf);

    uint8_t protocol_header = protocol_buf.at(index++);
    uint8_t protocol_type = protocol_buf.at(index++);
    this->helper.check_header(protocol_header, PROTOCOL_HEADER);
    if (protocol_type != BINARY_PROTOCOL and protocol_type != TEXT_PROTOCOL) {
        throw std::invalid_argument("Invalid protocol type");
    }

    this->type = protocol_type;
}

void ClientProtocol::send_username(const std::string& username) {
    uint8_t header_username = USERNAME_HEADER;
    uint16_t length_username = username.size();

    std::vector<uint8_t> username_buf(
            sizeof(header_username) + sizeof(length_username) + username.size(), 0);
    unsigned int index = 0;
    username_buf.at(index++) = header_username;
    this->helper.save_two_bytes_in_big_endian(username_buf, index, length_username);
    this->helper.save_string(username_buf, index, username);

    this->helper.send_all_bytes(this->skt, username_buf);
}

const PlayerLoadout ClientProtocol::get_binary_player_loadout() {
    std::vector<uint8_t> loadout_buf(10, 0);
    unsigned int index = 0;
    this->helper.recv_all_bytes(this->skt, loadout_buf);
    uint8_t loadout_header = loadout_buf.at(index++);

    this->helper.check_header(loadout_header, LOADOUT_BINARY);

    uint16_t money = this->helper.get_two_bytes_in_little_endian(loadout_buf, index);
    uint8_t is_knife = loadout_buf.at(index++);
    if (is_knife > 1)
        throw std::invalid_argument("Invalid value for is_knife_equipped: must be 0x00 or 0x01");
    WeaponType primary_weapon = this->helper.get_weapon_from_code(loadout_buf.at(index++));
    uint16_t primary_weapon_ammo = this->helper.get_two_bytes_in_little_endian(loadout_buf, index);
    WeaponType secondary_weapon = this->helper.get_weapon_from_code(loadout_buf.at(index++));
    uint16_t secondary_weapon_ammo =
            this->helper.get_two_bytes_in_little_endian(loadout_buf, index);

    return PlayerLoadout(money, is_knife, primary_weapon, primary_weapon_ammo, secondary_weapon,
                         secondary_weapon_ammo);
}

uint16_t ClientProtocol::deserialize_money_line(std::string& money_line) {
    const std::string money_prefix = std::string(MONEY_TEXT) + VALUE_SEPARATOR_TEXT;
    this->helper.check_prefix(money_line, money_prefix);

    return this->helper.get_number_from_string(money_line);
}

bool ClientProtocol::deserialize_knife_line(std::string& knife_line) {
    const std::string knife_prefix = std::string(KNIFE_TEXT) + VALUE_SEPARATOR_TEXT;
    this->helper.check_prefix(knife_line, knife_prefix);

    if (knife_line == KNIFE_EQUIPPED_TEXT) {
        return true;
    } else if (knife_line == "false") {
        return false;
    } else {
        throw std::invalid_argument("Knife equipped must be '" + std::string(KNIFE_EQUIPPED_TEXT) +
                                    "' or 'false'");
    }
}

std::pair<WeaponType, uint16_t> ClientProtocol::deserialize_weapon_line(std::string& weapon_line,
                                                                        bool is_primary) {
    const std::string weapon_prefix =
            std::string(is_primary ? PRIMARY_WEAPON_TEXT : SECONDARY_WEAPON_TEXT) +
            VALUE_SEPARATOR_TEXT;
    this->helper.check_prefix(weapon_line, weapon_prefix);

    size_t comma_pos = weapon_line.find(AMMO_SEPARATOR_TEXT);
    if (comma_pos == std::string::npos)
        throw std::invalid_argument("Expected ',' between weapon and ammo");
    std::string weapon_str = weapon_line.substr(0, comma_pos);
    std::string ammo_str = weapon_line.substr(comma_pos + 1);

    WeaponType weapon = this->helper.get_weapon_from_string(weapon_str);
    uint16_t ammo = this->helper.get_number_from_string(ammo_str);

    return {weapon, ammo};
}

const PlayerLoadout ClientProtocol::get_text_player_loadout() {
    std::string loadout_text = this->helper.recv_lines(this->skt, 4);
    std::istringstream loadout_text_ss(loadout_text);

    std::vector<std::string> lines(4);
    const std::string loadout_prefix = std::string(LOADOUT_TEXT) + WORD_DELIMITER_TEXT;
    for (int i = 0; i < 4; ++i) {
        std::getline(loadout_text_ss, lines[i]);
        this->helper.check_prefix(lines[i], loadout_prefix);
    }

    const uint16_t money = deserialize_money_line(lines[0]);
    const bool is_knife = deserialize_knife_line(lines[1]);
    const auto [primary_weapon, primary_weapon_ammo] = deserialize_weapon_line(lines[2], true);
    const auto [secondary_weapon, secondary_weapon_ammo] = deserialize_weapon_line(lines[3], false);

    return PlayerLoadout(money, is_knife, primary_weapon, primary_weapon_ammo, secondary_weapon,
                         secondary_weapon_ammo);
}

const PlayerLoadout ClientProtocol::get_player_loadout() {
    if (this->type == BINARY_PROTOCOL) {
        return get_binary_player_loadout();
    } else {
        return get_text_player_loadout();
    }
}

void ClientProtocol::send_binary_buy_weapon_request(const WeaponType& weapon) {
    uint8_t header_buy_weapon = BUY_WEAPON_BINARY;
    uint8_t weapon_code = this->helper.get_code_from_weapon(weapon);

    std::vector<uint8_t> buy_weapon_buf(2, 0);
    unsigned int index = 0;
    buy_weapon_buf.at(index++) = header_buy_weapon;
    buy_weapon_buf.at(index++) = weapon_code;

    this->helper.send_all_bytes(this->skt, buy_weapon_buf);
}

void ClientProtocol::send_binary_buy_ammo_request(bool is_primary, uint16_t ammo_count) {
    uint8_t header_buy_ammo = BUY_AMMO_BINARY;
    uint8_t type_weapon = (is_primary) ? PRIMARY_WEAPON_BINARY : SECONDARY_WEAPON_BINARY;

    std::vector<uint8_t> buy_ammo_buf(4, 0);
    unsigned int index = 0;
    buy_ammo_buf.at(index++) = header_buy_ammo;
    buy_ammo_buf.at(index++) = type_weapon;
    this->helper.save_two_bytes_in_big_endian(buy_ammo_buf, index, ammo_count);

    this->helper.send_all_bytes(this->skt, buy_ammo_buf);
}

void ClientProtocol::send_text_buy_weapon_request(const WeaponType& weapon) {
    std::ostringstream stream;

    stream << BUY_TEXT << WORD_DELIMITER_TEXT << WEAPON_TEXT << VALUE_SEPARATOR_TEXT
           << this->helper.get_string_from_weapon(weapon) << LINE_DELIMITER_TEXT;

    std::string loadout_text = stream.str();
    std::vector<uint8_t> loadout_buf(loadout_text.size());
    unsigned int index = 0;
    this->helper.save_string(loadout_buf, index, loadout_text);

    this->helper.send_all_bytes(this->skt, loadout_buf);
}
void ClientProtocol::send_text_buy_ammo_request(bool is_primary, uint16_t ammo_count) {
    std::ostringstream stream;

    stream << BUY_TEXT << WORD_DELIMITER_TEXT << AMMO_TEXT << WORD_DELIMITER_TEXT
           << (is_primary ? PRIMARY_WEAPON_TEXT : SECONDARY_WEAPON_TEXT) << VALUE_SEPARATOR_TEXT
           << ammo_count << LINE_DELIMITER_TEXT;

    std::string loadout_text = stream.str();
    std::vector<uint8_t> loadout_buf(loadout_text.size());
    unsigned int index = 0;
    this->helper.save_string(loadout_buf, index, loadout_text);

    this->helper.send_all_bytes(this->skt, loadout_buf);
}

void ClientProtocol::send_buy_weapon_request(const WeaponType& weapon) {
    if (this->type == BINARY_PROTOCOL) {
        send_binary_buy_weapon_request(weapon);
    } else {
        send_text_buy_weapon_request(weapon);
    }
}

void ClientProtocol::send_buy_ammo_request(bool is_primary, uint16_t ammo_count) {
    if (this->type == BINARY_PROTOCOL) {
        send_binary_buy_ammo_request(is_primary, ammo_count);
    } else {
        send_text_buy_ammo_request(is_primary, ammo_count);
    }
}

void ClientProtocol::send_buy_request(const BuyRequest& buy_request) {
    if (buy_request.is_weapon) {
        send_buy_weapon_request(buy_request.weapon);
    } else {
        send_buy_ammo_request(buy_request.is_primary, buy_request.ammo_count);
    }
}

ClientProtocol::~ClientProtocol() {}
