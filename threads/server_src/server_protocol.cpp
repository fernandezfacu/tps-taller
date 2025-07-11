#include "server_protocol.h"

#include <stdexcept>
#include <utility>
#include <vector>

#include "../common_src/protocol_constants.h"

ServerProtocol::ServerProtocol(Socket& peer): peer(peer) {}

const std::string ServerProtocol::get_name() {
    std::vector<uint8_t> name_length_buf(2, 0);
    this->helper.recv_all_bytes(this->peer, name_length_buf);
    unsigned int index = 0;

    uint16_t name_length = this->helper.get_two_bytes_in_little_endian(name_length_buf, index);

    std::vector<uint8_t> name_buf(name_length);
    this->helper.recv_all_bytes(this->peer, name_buf);
    const std::string name(reinterpret_cast<char*>(name_buf.data()), name_buf.size());
    return name;
}

const Request ServerProtocol::get_create_game_request() {
    const std::string name = get_name();
    return Request(true, false, name, false, false, std::pair<uint8_t, uint8_t>{4, 4});
}

const Request ServerProtocol::get_join_game_request() {
    const std::string name = get_name();
    return Request(false, true, name, false, false, std::pair<uint8_t, uint8_t>{4, 4});
}

const Request ServerProtocol::get_list_games_request() {
    return Request(false, false, "", true, false, std::pair<uint8_t, uint8_t>{4, 4});
}

const Request ServerProtocol::get_play_request() {
    uint8_t pos_byte;
    this->helper.recv_byte(this->peer, pos_byte);

    uint8_t col = (pos_byte >> 4) & 0x0F;
    uint8_t row = pos_byte & 0x0F;
    return Request(false, false, "", false, true, std::pair<uint8_t, uint8_t>{row, col});
}

const Request ServerProtocol::get_client_request() {
    uint8_t request_header;
    this->helper.recv_byte(this->peer, request_header);

    if (request_header == CREATE_HEADER) {
        return get_create_game_request();
    } else if (request_header == JOIN_HEADER) {
        return get_join_game_request();
    } else if (request_header == LIST_HEADER) {
        return get_list_games_request();
    } else if (request_header == PLAY_HEADER) {
        return get_play_request();
    } else {
        throw std::invalid_argument("Invalid request header");
    }
}

void ServerProtocol::send_msg(const std::string& msg) {
    uint16_t msg_length = msg.size();
    std::vector<uint8_t> msg_buf(sizeof(msg_length) + msg.size(), 0);
    unsigned int index = 0;
    this->helper.save_two_bytes_in_big_endian(msg_buf, index, msg_length);
    this->helper.save_string(msg_buf, index, msg);

    this->helper.send_all_bytes(this->peer, msg_buf);
}

ServerProtocol::~ServerProtocol() {}
