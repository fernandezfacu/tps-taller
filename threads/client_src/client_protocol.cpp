#include "client_protocol.h"

#include <stdexcept>
#include <vector>

#include "../common_src/protocol_constants.h"

ClientProtocol::ClientProtocol(const std::string& hostname, const std::string& service):
        skt(hostname.c_str(), service.c_str()) {}

void ClientProtocol::send_name_with_header(const uint8_t& header, const std::string& name) {
    uint16_t name_length = name.size();

    std::vector<uint8_t> name_buf(sizeof(header) + sizeof(name_length) + name.size(), 0);
    unsigned int index = 0;
    name_buf.at(index++) = header;
    this->helper.save_two_bytes_in_big_endian(name_buf, index, name_length);
    this->helper.save_string(name_buf, index, name);
    this->helper.send_all_bytes(this->skt, name_buf);
}

void ClientProtocol::send_create_game_request(const Request& request) {
    send_name_with_header(CREATE_HEADER, request.name);
}

void ClientProtocol::send_join_game_request(const Request& request) {
    send_name_with_header(JOIN_HEADER, request.name);
}

void ClientProtocol::send_list_games_request() {
    uint8_t header_list = LIST_HEADER;
    this->helper.send_byte(this->skt, header_list);
}

void ClientProtocol::send_play_request(const Request& request) {
    uint8_t header_play = PLAY_HEADER;

    uint8_t row = request.pos.first;
    uint8_t col = request.pos.second;
    // col y row debrían ser válidos (>= 0, <= 3)
    uint8_t pos_byte = (row << 4) | col;

    std::vector<uint8_t> play_buf = {header_play, pos_byte};
    this->helper.send_all_bytes(this->skt, play_buf);
}

void ClientProtocol::send_request(const Request& request) {
    if (request.is_create) {
        return send_create_game_request(request);
    } else if (request.is_join) {
        return send_join_game_request(request);
    } else if (request.is_list) {
        return send_list_games_request();
    } else if (request.is_play) {
        return send_play_request(request);
    } else {
        throw std::invalid_argument("Invalid request");
    }
}

const std::string ClientProtocol::get_server_msg() {
    std::vector<uint8_t> msg_length_buf(2, 0);
    this->helper.recv_all_bytes(this->skt, msg_length_buf);
    unsigned int index = 0;
    uint16_t msg_length = this->helper.get_two_bytes_in_little_endian(msg_length_buf, index);

    std::vector<uint8_t> msg_buf(msg_length);
    this->helper.recv_all_bytes(this->skt, msg_buf);
    const std::string msg(reinterpret_cast<char*>(msg_buf.data()), msg_buf.size());
    return msg;
}

ClientProtocol::~ClientProtocol() {}
