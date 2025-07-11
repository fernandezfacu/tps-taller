#ifndef COMMAND_H
#define COMMAND_H

#include <cstdint>

#include "../common_src/weapon_type.h"

/*
 * Command
 * Struct DTO que representa un comando que recibe el cliente de un usuario
 * que intenta hacer una compra de arma o munición.
 *
 * Si el comando es de compra de arma, is_buy_weapon debe ser true y ammo_count 0.
 * Si el comando es de compra de munición, is_buy_weapon debe ser false.
 * (el constructor lanza una excepción si esto no se cumple).
 *
 * */
struct Command {
    const bool is_buy_weapon;
    const WeaponType weapon;
    const uint16_t ammo_count;

    Command(bool is_weapon, WeaponType weapon, uint16_t ammo_count);

    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

    /*
     * Habilito el movimiento para poder utilizar std::optional<Command> en la comunicación del
     * cliente con el parser.
     *
     * */
    Command(Command&&) = default;
    Command& operator=(Command&&) = default;

    ~Command();
};

#endif
