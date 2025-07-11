#include "game_client.h"

#include <iostream>

CS2DGameClient::CS2DGameClient(const std::string& hostname, const std::string& service,
                               const std::string& username):
        protocol(hostname, service, username) {}

std::string CS2DGameClient::format_weapon_info(const WeaponType& weapon, uint16_t ammo) const {
    if (weapon == WeaponType::None) {
        return "not_equipped";
    }
    return this->weapon_translator.get_string_from_weapon(weapon) + ", " + std::to_string(ammo);
}

void CS2DGameClient::show_player_loadout(const PlayerLoadout& player_loadout) const {
    std::string knife_status = player_loadout.is_knife_equipped ? "equipped" : "not_equipped";

    std::string primary_weapon_str =
            format_weapon_info(player_loadout.primary_weapon, player_loadout.primary_weapon_ammo);
    std::string secondary_weapon_str = format_weapon_info(player_loadout.secondary_weapon,
                                                          player_loadout.secondary_weapon_ammo);

    std::cout << "money: $" << player_loadout.money << " | knife: " << knife_status
              << " | primary: " << primary_weapon_str << " | secondary: " << secondary_weapon_str
              << std::endl;
}

bool CS2DGameClient::try_send_buy_weapon_request(const Command& buy_weapon_command,
                                                 const PlayerLoadout& loadout) {
    const WeaponType weapon = buy_weapon_command.weapon;

    if (weapon == loadout.primary_weapon or weapon == loadout.secondary_weapon) {
        std::cout << "Weapon already owned: "
                  << this->weapon_translator.get_string_from_weapon(weapon) << std::endl;
        return false;
    } else {
        this->protocol.send_buy_request(BuyRequest(true, weapon, false, 0));
        return true;
    }
}

bool CS2DGameClient::try_send_buy_ammo_request(const Command& buy_ammo_command,
                                               const PlayerLoadout& loadout) {

    const WeaponType weapon = buy_ammo_command.weapon;
    const uint16_t ammo_count = buy_ammo_command.ammo_count;

    if (weapon == loadout.primary_weapon) {
        this->protocol.send_buy_request(BuyRequest(false, WeaponType::None, true, ammo_count));
        return true;
    } else if (weapon == loadout.secondary_weapon) {
        this->protocol.send_buy_request(BuyRequest(false, WeaponType::None, false, ammo_count));
        return true;
    } else {
        std::cout << "Weapon: " << this->weapon_translator.get_string_from_weapon(weapon)
                  << " is not_equipped" << std::endl;
        return false;
    }
}

bool CS2DGameClient::read_client_and_try_send_buy_request(const PlayerLoadout& loadout,
                                                          bool& should_exit) {
    std::string line;
    std::getline(std::cin, line);
    const std::optional<Command> command = this->parser.get_command_from(line, should_exit);

    if (should_exit)
        return false;

    if (not command.has_value())
        return false;

    const Command& comm = command.value();
    if (comm.is_buy_weapon) {
        return try_send_buy_weapon_request(comm, loadout);
    } else {
        return try_send_buy_ammo_request(comm, loadout);
    }
}

void CS2DGameClient::buy_phase() {
    bool should_exit = false;
    while (not should_exit) {
        const PlayerLoadout loadout = this->protocol.get_player_loadout();
        show_player_loadout(loadout);
        while (not read_client_and_try_send_buy_request(loadout, should_exit) and not should_exit) {
        }
    }
}


CS2DGameClient::~CS2DGameClient() {}
