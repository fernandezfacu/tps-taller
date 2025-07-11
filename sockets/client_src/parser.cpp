#include "parser.h"

#include <iostream>
#include <sstream>

Parser::Parser(): weapon_translator() {}

const WeaponType Parser::get_weapon_type(const std::string& arg1) const {
    try {
        auto weapon = this->weapon_translator.get_weapon_from_string(arg1);
        return weapon;
    } catch (const std::out_of_range& e) {
        return WeaponType::None;
    }
}

std::optional<Command> Parser::buy_weapon_command(const std::string& arg1) const {
    auto weapon = this->weapon_translator.get_weapon_from_string(arg1);
    if (weapon == WeaponType::None) {
        std::cout << "Invalid weapon argument: " << arg1 << std::endl;
        return std::nullopt;
    }
    return Command(true, weapon, 0);
}

int Parser::get_ammo_count(const std::string& arg2) const {
    try {
        size_t pos;
        int count = std::stoi(arg2, &pos);
        return count;
    } catch (const std::exception&) {
        return -1;
    }
}

std::optional<Command> Parser::buy_ammo_command(const std::string& arg1,
                                                const std::string& arg2) const {
    auto weapon = get_weapon_type(arg1);
    if (weapon == WeaponType::None) {
        std::cout << "Invalid weapon argument: " << arg1 << std::endl;
        return std::nullopt;
    }

    int count = get_ammo_count(arg2);
    if (count <= 0) {
        std::cout << "Invalid ammo count: " << arg2 << ". It must be a positive number."
                  << std::endl;
        return std::nullopt;
    }
    count = static_cast<uint16_t>(count);

    return Command(false, weapon, count);
}

std::optional<Command> Parser::get_command_from(const std::string& line, bool& should_exit) const {
    std::istringstream iss(line);
    std::string comando, arg1, arg2, extra;

    if (!(iss >> comando))
        return std::nullopt;
    if (comando == EXIT_INPUT) {
        if (iss >> arg1) {
            std::cout << "The 'exit' command does not accept arguments." << std::endl;
        } else {
            should_exit = true;
        }
    } else if (comando == BUY_WEAPON_INPUT) {
        if (!(iss >> arg1) || (iss >> extra)) {
            std::cout << "The 'buy' command requires 1 argument: <weapon>." << std::endl;
        } else {
            return buy_weapon_command(arg1);
        }
    } else if (comando == BUY_AMMO_INPUT) {
        if (!(iss >> arg1 >> arg2) || (iss >> extra)) {
            std::cout << "The 'ammo' command requires 2 arguments: <weapon> <count>." << std::endl;
        } else {
            return buy_ammo_command(arg1, arg2);
        }
    } else {
        std::cout << "Unknown command: " << comando << std::endl;
    }
    return std::nullopt;
}

Parser::~Parser() {}
