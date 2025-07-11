#ifndef REQUEST_H
#define REQUEST_H

#include <cstdint>
#include <string>
#include <utility>

/*
 * Request
 * Struct DTO que representa una petición del cliente al servidor
 *
 *
 * */
struct Request {
    const bool is_create;
    const bool is_join;
    const std::string name;
    const bool is_list;
    const bool is_play;
    const std::pair<uint8_t, uint8_t> pos;

    Request(const bool& is_create, const bool& is_join, const std::string& name,
            const bool& is_list, const bool& is_play, const std::pair<uint8_t, uint8_t>& pos);

    Request(const Request&) = delete;
    Request& operator=(const Request&) = delete;

    ~Request();
};

#endif
