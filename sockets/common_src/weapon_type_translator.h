#ifndef WEAPON_TYPE_TRANSLATOR_H
#define WEAPON_TYPE_TRANSLATOR_H

#include <map>
#include <string>

#include "weapon_type.h"

/*
 * WeaponTypeTranslator
 * traductor de los tipo de arma del juego para protocolo de texto y binario
 *
 * Tiene dos map, uno con las traducciones de tipo de arma a binario y otro
 * con las traducciones de tipo de arma a texto.
 *
 * Las traducciones de texto son también usadas por el cliente para parsear comandos del usuario y
 * mostrar por pantalla el equipamiento. Leer aclaración sobre este uso en CS2DGameClient
 * */
class WeaponTypeTranslator {
private:
    const std::map<WeaponType, uint8_t> weapon_type_codes;
    const std::map<WeaponType, std::string> weapon_type_strings;

public:
    WeaponTypeTranslator();

    /*
     * Traduce un WeaponType a su representación en string.
     *
     * */
    const std::string get_string_from_weapon(const WeaponType& weapon) const;

    /*
     * Traduce un string a un WeaponType.
     *
     * Si no existe la traducción, lanza una excepción.
     * */
    const WeaponType get_weapon_from_string(const std::string& weapon_string) const;

    /*
     * Traduce un WeaponType a su representación binaria (un byte).
     *
     * */
    const uint8_t get_code_from_weapon(const WeaponType& weapon) const;
    /*
     * Traduce un byte a un WeaponType.
     *
     * Si no existe la traducción, lanza una excepción.
     * */
    const WeaponType get_weapon_from_code(const uint8_t& weapon_code) const;


    WeaponTypeTranslator(const WeaponTypeTranslator&) = delete;
    WeaponTypeTranslator& operator=(const WeaponTypeTranslator&) = delete;

    ~WeaponTypeTranslator();
};

#endif
