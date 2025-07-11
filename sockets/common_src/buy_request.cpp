#include "buy_request.h"

#include <stdexcept>

BuyRequest::BuyRequest(bool is_weapon, WeaponType weapon, bool is_primary, uint16_t ammo_count):
        is_weapon(is_weapon), weapon(weapon), is_primary(is_primary), ammo_count(ammo_count) {
    if (is_weapon and weapon == WeaponType::None)
        throw std::invalid_argument("Cannot buy none weapon");
    if (is_weapon and ammo_count != 0)
        throw std::invalid_argument("Buy weapon request should not have an ammo count");
    if (not is_weapon and weapon != WeaponType::None)
        throw std::invalid_argument("Buy ammo request should not have a weapon type");
}

BuyRequest::~BuyRequest() {}
