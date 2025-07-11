#include "server_protocol.h"

#include <sstream>
#include <stdexcept>

#include "../common_src/protocol_constants.h"

ServerProtocol::ServerProtocol(const std::string& port, const std::string& protocol_type):
        type((protocol_type == "binary") ? BINARY_PROTOCOL :
             (protocol_type == "text")   ? TEXT_PROTOCOL :
                                           throw std::invalid_argument("Invalid protocol type")),
        srv(port.c_str()),
        peer(Socket(srv.accept())) {
    send_protocol_type();
}


const std::string ServerProtocol::get_username() {
    std::vector<uint8_t> username_header_buf(3, 0);
    this->helper.recv_all_bytes(this->peer, username_header_buf);
    unsigned int index = 0;

    uint8_t username_header = username_header_buf.at(index++);
    this->helper.check_header(username_header, USERNAME_HEADER);

    uint16_t username_length =
            this->helper.get_two_bytes_in_little_endian(username_header_buf, index);

    std::vector<uint8_t> username_buf(username_length);
    this->helper.recv_all_bytes(this->peer, username_buf);
    std::string username(reinterpret_cast<char*>(username_buf.data()), username_buf.size());
    return username;
}

void ServerProtocol::send_protocol_type() {
    std::vector<uint8_t> protocol_buf(2, 0);
    unsigned int index = 0;
    uint8_t header_protocol = PROTOCOL_HEADER;
    protocol_buf.at(index++) = header_protocol;
    protocol_buf.at(index++) = this->type;
    this->helper.send_all_bytes(this->peer, protocol_buf);
}

const std::vector<uint8_t> ServerProtocol::serialize_binary_loadout(const PlayerLoadout& loadout) {
    std::vector<uint8_t> loadout_buf(10, 0);
    unsigned int index = 0;

    uint8_t header_loadout = LOADOUT_BINARY;
    loadout_buf.at(index++) = header_loadout;

    this->helper.save_two_bytes_in_big_endian(loadout_buf, index, loadout.money);

    if (loadout.is_knife_equipped)
        loadout_buf.at(index) = KNIFE_EQUIPPED_BINARY;
    index++;

    loadout_buf.at(index++) = this->helper.get_code_from_weapon(loadout.primary_weapon);
    this->helper.save_two_bytes_in_big_endian(loadout_buf, index, loadout.primary_weapon_ammo);

    loadout_buf.at(index++) = this->helper.get_code_from_weapon(loadout.secondary_weapon);
    this->helper.save_two_bytes_in_big_endian(loadout_buf, index, loadout.secondary_weapon_ammo);

    return loadout_buf;
}

void ServerProtocol::send_binary_player_loadout(const PlayerLoadout& loadout) {
    std::vector<uint8_t> loadout_buf = serialize_binary_loadout(loadout);
    this->helper.send_all_bytes(this->peer, loadout_buf);
}

const std::string ServerProtocol::serialize_text_money(const uint16_t& money) {
    std::ostringstream stream;

    stream << LOADOUT_TEXT << WORD_DELIMITER_TEXT << MONEY_TEXT << VALUE_SEPARATOR_TEXT << money
           << LINE_DELIMITER_TEXT;

    return stream.str();
}
const std::string ServerProtocol::serialize_text_knife(const bool& is_knife_equipped) {
    std::ostringstream stream;

    stream << LOADOUT_TEXT << WORD_DELIMITER_TEXT << KNIFE_TEXT << VALUE_SEPARATOR_TEXT
           << (is_knife_equipped ? KNIFE_EQUIPPED_TEXT : "false") << LINE_DELIMITER_TEXT;

    return stream.str();
}
const std::string ServerProtocol::serialize_text_weapon(const WeaponType& weapon,
                                                        const uint16_t& ammo,
                                                        const bool& is_primary) {
    std::ostringstream stream;

    stream << LOADOUT_TEXT << WORD_DELIMITER_TEXT
           << (is_primary ? PRIMARY_WEAPON_TEXT : SECONDARY_WEAPON_TEXT) << VALUE_SEPARATOR_TEXT
           << this->helper.get_string_from_weapon(weapon) << AMMO_SEPARATOR_TEXT
           << static_cast<int>(ammo) << LINE_DELIMITER_TEXT;

    return stream.str();
}

const std::string ServerProtocol::serialize_text_loadout(const PlayerLoadout& loadout) {
    std::ostringstream stream;
    stream << serialize_text_money(loadout.money);
    stream << serialize_text_knife(loadout.is_knife_equipped);
    stream << serialize_text_weapon(loadout.primary_weapon, loadout.primary_weapon_ammo, true);
    stream << serialize_text_weapon(loadout.secondary_weapon, loadout.secondary_weapon_ammo, false);
    return stream.str();
}

void ServerProtocol::send_text_player_loadout(const PlayerLoadout& loadout) {
    const std::string loadout_text = serialize_text_loadout(loadout);
    std::vector<uint8_t> loadout_buf(loadout_text.size());
    unsigned int index = 0;
    this->helper.save_string(loadout_buf, index, loadout_text);
    this->helper.send_all_bytes(this->peer, loadout_buf);
}

void ServerProtocol::send_player_loadout(const PlayerLoadout& player_loadout) {
    if (this->type == BINARY_PROTOCOL) {
        send_binary_player_loadout(player_loadout);
    } else {
        send_text_player_loadout(player_loadout);
    }
}

BuyRequest ServerProtocol::get_binary_buy_weapon_request() {
    uint8_t weapon_code;
    this->helper.recv_byte(this->peer, weapon_code);
    WeaponType weapon = this->helper.get_weapon_from_code(weapon_code);
    if (weapon == WeaponType::None)
        throw std::invalid_argument("Invalid weapon to be bought");
    return BuyRequest(true, weapon, false, 0);
}

BuyRequest ServerProtocol::get_binary_buy_ammo_request() {
    std::vector<uint8_t> buy_ammo_buf(3, 0);
    this->helper.recv_all_bytes(this->peer, buy_ammo_buf);
    unsigned int index = 0;

    uint8_t type_weapon = buy_ammo_buf.at(index++);
    bool is_primary;
    if (type_weapon == PRIMARY_WEAPON_BINARY) {
        is_primary = true;
    } else if (type_weapon == SECONDARY_WEAPON_BINARY) {
        is_primary = false;
    } else {
        throw std::invalid_argument("Invalid type weapon");
    }
    uint16_t ammo_count = this->helper.get_two_bytes_in_little_endian(buy_ammo_buf, index);

    return BuyRequest(false, WeaponType::None, is_primary, ammo_count);
}

BuyRequest ServerProtocol::get_binary_buy_request() {
    uint8_t buy_header;
    this->helper.recv_byte(this->peer, buy_header);

    if (buy_header == BUY_WEAPON_BINARY) {
        return get_binary_buy_weapon_request();
    } else if (buy_header == BUY_AMMO_BINARY) {
        return get_binary_buy_ammo_request();
    } else {
        throw std::invalid_argument("Invalid buy request header");
    }
}

BuyRequest ServerProtocol::get_text_buy_weapon_request(std::string& buy_weapon_line) {
    const std::string weapon_prefix = std::string(WEAPON_TEXT) + VALUE_SEPARATOR_TEXT;
    this->helper.check_prefix(buy_weapon_line, weapon_prefix);

    WeaponType weapon = this->helper.get_weapon_from_string(buy_weapon_line);

    return BuyRequest(true, weapon, false, 0);
}

BuyRequest ServerProtocol::get_text_buy_ammo_request(std::string& buy_ammo_line) {
    const std::string ammo_prefix = std::string(AMMO_TEXT) + WORD_DELIMITER_TEXT;
    this->helper.check_prefix(buy_ammo_line, ammo_prefix);

    const std::string primary_prefix = std::string(PRIMARY_WEAPON_TEXT) + VALUE_SEPARATOR_TEXT;
    const std::string secondary_prefix = std::string(SECONDARY_WEAPON_TEXT) + VALUE_SEPARATOR_TEXT;
    bool is_primary;
    if (buy_ammo_line.rfind(primary_prefix, 0) == 0) {
        is_primary = true;
        buy_ammo_line = buy_ammo_line.substr(primary_prefix.size());
    } else if (buy_ammo_line.rfind(secondary_prefix, 0) == 0) {
        is_primary = false;
        buy_ammo_line = buy_ammo_line.substr(secondary_prefix.size());
    } else {
        throw std::invalid_argument(
                "Invalid buy ammo request: weapon-type must be 'primary' or 'secondary'");
    }

    uint16_t ammo_count = this->helper.get_number_from_string(buy_ammo_line);

    return BuyRequest(false, WeaponType::None, is_primary, static_cast<uint16_t>(ammo_count));
}

BuyRequest ServerProtocol::get_text_buy_request() {
    std::string buy_request_text = this->helper.recv_lines(this->peer, 1);
    std::istringstream buy_request_text_ss(buy_request_text);
    std::string buy_request_line;

    std::getline(buy_request_text_ss, buy_request_line);

    const std::string buy_prefix = std::string(BUY_TEXT) + WORD_DELIMITER_TEXT;
    if (buy_request_line.rfind(buy_prefix, 0) != 0)
        throw std::invalid_argument("Line must start with '" + buy_prefix + "'");

    buy_request_line = buy_request_line.substr(buy_prefix.size());

    if (buy_request_line.rfind(WEAPON_TEXT, 0) == 0) {
        return get_text_buy_weapon_request(buy_request_line);
    } else if (buy_request_line.rfind(AMMO_TEXT, 0) == 0) {
        return get_text_buy_ammo_request(buy_request_line);
    } else {
        throw std::invalid_argument("Invalid buy request: must start with 'weapon' or 'ammo'");
    }
}

BuyRequest ServerProtocol::get_buy_request() {
    if (this->type == BINARY_PROTOCOL) {
        return get_binary_buy_request();
    } else {
        return get_text_buy_request();
    }
}

ServerProtocol::~ServerProtocol() {}
