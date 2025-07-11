#include "command.h"

#include <stdexcept>

Command::Command(const bool& is_create, const bool& is_join, const std::string& name,
                 const bool& is_list, const bool& is_play, const std::pair<uint8_t, uint8_t>& pos):
        is_create(is_create),
        is_join(is_join),
        name(name),
        is_list(is_list),
        is_play(is_play),
        pos(pos) {
    if ((is_create or is_join) and name == "")
        throw std::invalid_argument("create or join game command should have a game name");
    if (not(is_create or is_join) and name != "")
        throw std::invalid_argument("Play or list command should not have a name");
}

Command::~Command() {}
