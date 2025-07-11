#include "request.h"

#include <stdexcept>

Request::Request(const bool& is_create, const bool& is_join, const std::string& name,
                 const bool& is_list, const bool& is_play, const std::pair<uint8_t, uint8_t>& pos):
        is_create(is_create),
        is_join(is_join),
        name(name),
        is_list(is_list),
        is_play(is_play),
        pos(pos) {
    if ((is_create or is_join) and name == "")
        throw std::invalid_argument("Cannot send create or join game request without a name");
    if (not(is_create or is_join) and name != "")
        throw std::invalid_argument("Play or list request should not have a name");
    if (is_play and pos == std::pair<uint8_t, uint8_t>{4, 4})
        throw std::invalid_argument("Play request should have a valid pos");
    if (not is_play and pos != std::pair<uint8_t, uint8_t>{4, 4})
        throw std::invalid_argument("Play request should not have a valid pos");
}

Request::~Request() {}
