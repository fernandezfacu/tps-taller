#ifndef BUY_REQUEST_H
#define BUY_REQUEST_H

#include <cstdint>

#include "weapon_type.h"

/*
 * BuyRequest
 * Struct DTO que representa una petición de compra de arma o munición enviada
 * desde el cliente al servidor.
 *
 * Si la petición es de compra de arma, is_weapon debe ser true, weapon distinto de WeaponType::None
 * y ammo_count 0. Si la petición es de compra de munición, is_weapon debe ser false y weapon
 * WeaponType::None. (el constructor lanza una excepción si esto no se cumple).
 *
 * */
struct BuyRequest {
    const bool is_weapon;
    const WeaponType weapon;
    const bool is_primary;
    const uint16_t ammo_count;

    BuyRequest(bool is_weapon, WeaponType weapon, bool is_primary, uint16_t ammo_count);

    BuyRequest(const BuyRequest&) = delete;
    BuyRequest& operator=(const BuyRequest&) = delete;

    ~BuyRequest();
};

#endif
