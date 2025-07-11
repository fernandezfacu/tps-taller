#include "command.h"

#include <stdexcept>

Command::Command(bool is_buy_weapon, WeaponType weapon, uint16_t ammo_count):
        is_buy_weapon(is_buy_weapon), weapon(weapon), ammo_count(ammo_count) {
    if (is_buy_weapon and ammo_count != 0)
        throw std::invalid_argument("Buy weapon command should not have an ammo count");
    if (weapon == WeaponType::None)
        throw std::invalid_argument("Buy command should have a buyable weapon type");
}

Command::~Command() {}
