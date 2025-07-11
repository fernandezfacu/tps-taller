#ifndef LOADOUT_MANAGER_H
#define LOADOUT_MANAGER_H

#include "../common_src/player_loadout.h"

#define AMMO_PRICE 1
#define WEAPON_PRICE 100
#define INITIAL_AMMO 30
#define INITIAL_MONEY 500

/*
 * LoadoutManager
 * Objeto encargado del manejo del negocio del juego.
 *
 * Se encarga de manejar el equipamiento del jugador para el servidor,
 * efectuando las compras de armas y munición en caso de ser estas posibles.
 *
 * Puede devolver un struct DTO con el estado actual del equipamiento del jugador.
 *
 * */
class LoadoutManager {
private:
    uint16_t money;
    bool is_knife_equipped;
    WeaponType primary_weapon;
    uint16_t primary_weapon_ammo;
    WeaponType secondary_weapon;
    uint16_t secondary_weapon_ammo;

    /*
     * Disminuye el dinero en ammount_of_money.
     *
     * */
    void decrease_money_by(const uint16_t& ammount_of_money);
    /*
     * Cambia el arma primaria a weapon, y la cantidad de balas del arma primaria a 30.
     *
     * */
    void new_primary_weapon(const WeaponType& weapon);
    /*
     * Aumenta la cantidad de balas del arma primaria en ammo_count.
     *
     * */
    void add_ammo_to_primary_weapon(const uint16_t& ammo_count);
    /*
     * Aumenta la cantidad de balas del arma secundaria en ammo_count.
     *
     * */
    void add_ammo_to_secondary_weapon(const uint16_t& ammo_count);

    /*
     * Devuelve true si el jugador tiene el suficiente dinero para comprar un arma,
     * false en caso contrario.
     *
     * */
    bool can_buy_weapon() const;

    /*
     * Devuelve true si el jugador tiene el suficiente dinero para comprar las balas pedidas,
     * false en caso contrario.
     *
     * */
    bool can_buy_ammo(const uint16_t& ammo_count) const;

public:
    /*
     * Constructor de LoadoutManager
     *
     * El equipamiento de un jugador inicialmente es:
     *
     * Dinero: 500.
     * Cuchillo equipado: sí.
     * Arma primaria: WeaponType::None.
     * Balas de arma primaria: 0.
     * Arma secundaria: Glock.
     * Balas de arma secundaria: 30.
     *
     * */
    LoadoutManager();

    /*
     * Compra un arma primaria para el equipamiento.
     *
     * Si la compra puede ser efectuada, disminuye el dinero en 100, cambia el arma primaria
     * que comenzará con 30 de munición, sin importar la munición del anterior arma.
     * Devuelve true en este caso.
     *
     * Devuelve false si no se tiene el suficiente dinero para comprar el arma.
     *
     * */
    bool buy_primary_weapon(const WeaponType& weapon);

    /*
     * Compra munición para algún arma del equipamiento.
     *
     * Si for_primary es true, compra balas para el arma primaria, caso contrario para el arma
     * secundaria.
     *
     * Si la compra puede ser efectuada, disminuye el dinero en la cantidad de balas compradas
     * (el precio de cada bala es 1), aumenta la cantidad de balas del arma que corresponda y
     * devuelve true.
     *
     * Devuelve false si el jugador no tiene el suficiente dinero para comprar las balas pedidas.
     *
     * */
    bool buy_ammo(const uint16_t& ammo_count, bool for_primary);

    /*
     * Devuelve struct DTO con el estado actual del equipamiento.
     * */
    const PlayerLoadout get_loadout() const;

    LoadoutManager(const LoadoutManager&) = delete;
    LoadoutManager& operator=(const LoadoutManager&) = delete;

    ~LoadoutManager();
};

#endif
