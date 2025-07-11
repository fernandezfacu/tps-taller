#include "loadout_manager.h"

LoadoutManager::LoadoutManager():
        money(INITIAL_MONEY),
        is_knife_equipped(true),
        primary_weapon(WeaponType::None),
        primary_weapon_ammo(0),
        secondary_weapon(WeaponType::Glock),
        secondary_weapon_ammo(INITIAL_AMMO) {}

void LoadoutManager::decrease_money_by(const uint16_t& ammount_of_money) {
    this->money -= ammount_of_money;
}

void LoadoutManager::new_primary_weapon(const WeaponType& weapon) {
    this->primary_weapon = weapon;
    this->primary_weapon_ammo = INITIAL_AMMO;
}

void LoadoutManager::add_ammo_to_primary_weapon(const uint16_t& ammo_count) {
    this->primary_weapon_ammo += ammo_count;
}

void LoadoutManager::add_ammo_to_secondary_weapon(const uint16_t& ammo_count) {
    this->secondary_weapon_ammo += ammo_count;
}

bool LoadoutManager::can_buy_weapon() const { return this->money >= WEAPON_PRICE; }

bool LoadoutManager::can_buy_ammo(const uint16_t& ammo_count) const {
    return this->money >= AMMO_PRICE * ammo_count;
}

bool LoadoutManager::buy_primary_weapon(const WeaponType& weapon) {
    if (not can_buy_weapon())
        return false;
    new_primary_weapon(weapon);
    decrease_money_by(WEAPON_PRICE);
    return true;
}

bool LoadoutManager::buy_ammo(const uint16_t& ammo_count, bool for_primary) {
    if (not can_buy_ammo(ammo_count))
        return false;
    if (for_primary)
        add_ammo_to_primary_weapon(ammo_count);
    if (not for_primary)
        add_ammo_to_secondary_weapon(ammo_count);
    decrease_money_by(AMMO_PRICE * ammo_count);
    return true;
}

const PlayerLoadout LoadoutManager::get_loadout() const {
    return PlayerLoadout(this->money, this->is_knife_equipped, this->primary_weapon,
                         this->primary_weapon_ammo, this->secondary_weapon,
                         this->secondary_weapon_ammo);
}

LoadoutManager::~LoadoutManager() {}
