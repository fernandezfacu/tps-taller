#ifndef PARSER_H
#define PARSER_H

#include <optional>
#include <string>

#include "../common_src/weapon_type_translator.h"

#include "command.h"

#define EXIT_INPUT "exit"
#define BUY_WEAPON_INPUT "buy"
#define BUY_AMMO_INPUT "ammo"

/*
 * Parser
 * Parser para el cliente de juego, encargado de recibir lineas y parsearlas
 * a comandos que el cliente puede interpretar para enviar requests al servidor.
 *
 *
 * Leer Aclaración sobre el uso de WeaponTypeTranslator en Cliente.
 *
 * */
class Parser {
private:
    const WeaponTypeTranslator weapon_translator;

    /*
     * Obtiene el tipo de arma dado un string
     *
     * Si no puede traducir el string, devuelve WeaponType::None
     * */
    const WeaponType get_weapon_type(const std::string& arg1) const;
    /*
     * Recibe los argumentos de un comando 'buy'
     *
     * Si los argumentos son válidos, develve un Command con is_weapon seteado en true,
     * valor válido de tipo de arma y cantidad de balas en 0.
     *
     * Caso contrario, devuelve std::nullopt
     *
     * */
    std::optional<Command> buy_weapon_command(const std::string& arg1) const;

    /*
     * Obtiene un entero dado un string
     *
     * Si no puede traducir el string, devuelve -1
     * */
    int get_ammo_count(const std::string& arg2) const;

    /*
     * Recibe los argumentos de un comando 'ammo'
     *
     * Si los argumentos son válidos, develve un Command con is_weapon seteado en false y
     * valores válidos de tipo de arma y cantidad de balas.
     *
     * Caso contrario, devuelve std::nullopt
     *
     * */
    std::optional<Command> buy_ammo_command(const std::string& arg1, const std::string& arg2) const;

public:
    Parser();

    /*
     * Recibe una string que representa una linea ingresada por el usuario.
     *
     * Si la línea puede traducirse en un comando de juego (Command), lo devuelve. En caso
     * contrario, devuelve std::nullopt.
     *
     * Si se recibe un comando 'exit', devuelve std::nullopt y setea should_exit a true.
     *
     * */
    std::optional<Command> get_command_from(const std::string& line, bool& should_exit) const;

    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;

    ~Parser();
};

#endif
