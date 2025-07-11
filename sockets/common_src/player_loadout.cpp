#include "player_loadout.h"

PlayerLoadout::PlayerLoadout(uint16_t money, bool is_knife_equipped, WeaponType primary_weapon,
                             uint16_t primary_weapon_ammo, WeaponType secondary_weapon,
                             uint16_t secondary_weapon_ammo):
        money(money),
        is_knife_equipped(is_knife_equipped),
        primary_weapon(primary_weapon),
        primary_weapon_ammo(primary_weapon_ammo),
        secondary_weapon(secondary_weapon),
        secondary_weapon_ammo(secondary_weapon_ammo) {}

PlayerLoadout::~PlayerLoadout() {}
