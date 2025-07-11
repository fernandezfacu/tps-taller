#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <utility>

/*
 * Command
 * Struct DTO que representa un comando que recibe el cliente de un usuario
 * que intenta hacer una petición al servidor.
 *
 * */
struct Command {
    const bool is_create;
    const bool is_join;
    const std::string name;
    const bool is_list;
    const bool is_play;
    const std::pair<uint8_t, uint8_t> pos;

    Command(const bool& is_create, const bool& is_join, const std::string& name,
            const bool& is_list, const bool& is_play, const std::pair<uint8_t, uint8_t>& pos);

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
