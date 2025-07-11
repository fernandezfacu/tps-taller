#ifndef PLAYER_LOADOUT_H
#define PLAYER_LOADOUT_H

#include <cstdint>

#include "weapon_type.h"

/*
 * PlayerLoadout
 * Struct DTO que representa el equipamiento de un jugador.
 *
 * */
struct PlayerLoadout {
public:
    const uint16_t money;
    const bool is_knife_equipped;
    const WeaponType primary_weapon;
    const uint16_t primary_weapon_ammo;
    const WeaponType secondary_weapon;
    const uint16_t secondary_weapon_ammo;

    PlayerLoadout(uint16_t money, bool is_knife_equipped, WeaponType primary_weapon,
                  uint16_t primary_weapon_ammo, WeaponType secondary_weapon,
                  uint16_t secondary_weapon_ammo);

    PlayerLoadout(const PlayerLoadout&) = delete;
    PlayerLoadout& operator=(const PlayerLoadout&) = delete;

    ~PlayerLoadout();
};

#endif
