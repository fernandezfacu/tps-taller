#ifndef PARSER_H
#define PARSER_H

#include <optional>
#include <string>

#include "command.h"

#define CREATE_INPUT "crear"
#define LIST_INPUT "listar"
#define JOIN_INPUT "unirse"
#define PLAY_INPUT "jugar"

/*
 * Parser
 * Parser para el cliente de juego, encargado de recibir lineas y parsearlas
 * a comandos que el cliente puede interpretar para enviar requests al servidor.
 *
 *
 * */
class Parser {
private:
    /*
     * Obtiene un entero dado un string
     *
     * Si no puede traducir el string, devuelve -1
     * */
    int get_int(const std::string& arg2) const;


public:
    Parser();

    /*
     * Recibe una string que representa una linea ingresada por el usuario.
     *
     * Si la línea puede traducirse en un comando de juego (Command), lo devuelve. En caso
     * contrario, devuelve std::nullopt.
     *
     * */
    std::optional<Command> get_command_from(const std::string& line) const;

    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;

    ~Parser();
};

#endif
