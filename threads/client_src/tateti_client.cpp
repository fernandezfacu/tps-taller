#include "tateti_client.h"

#include <iostream>
#include <utility>

#include "../common_src/tateti_constants.h"

TatetiClient::TatetiClient(const std::string& hostname, const std::string& service):
        protocol(hostname, service) {}

void TatetiClient::show_server_msg(const std::string& msg) const { std::cout << msg; }

bool TatetiClient::try_send_play_request(const Command& play_command) {
    int row = play_command.pos.first;
    int col = play_command.pos.second;
    if (row < 1 or row > ROWS or col < 1 or col > ROWS) {
        std::cout << "Invalid position. Should be two numbers between 1 and 3" << std::endl;
        return false;
    }
    row = row - 1;
    col = col - 1;
    std::pair<uint8_t, uint8_t> pos{row, col};
    this->protocol.send_request(Request(false, false, play_command.name, false, true, pos));
    return true;
}

bool TatetiClient::read_client_and_try_send_request() {
    std::string line;
    std::getline(std::cin, line);
    const std::optional<Command> command = this->parser.get_command_from(line);

    if (not command.has_value())
        return false;

    const Command& comm = command.value();
    if (comm.is_create) {
        this->protocol.send_request(Request(true, false, comm.name, false, false, comm.pos));
        return true;
    } else if (comm.is_list) {
        this->protocol.send_request(Request(false, false, comm.name, true, false, comm.pos));
        return true;
    } else if (comm.is_join) {
        this->protocol.send_request(Request(false, true, comm.name, false, false, comm.pos));
        return true;
    } else {
        return try_send_play_request(comm);
    }
}

bool TatetiClient::contains_ended_game_msg(const std::string& msg) {
    return (msg.find(TIE_MSG) != std::string::npos) or (msg.find(LOSS_MSG) != std::string::npos) or
           (msg.find(WIN_MSG) != std::string::npos);
}

void TatetiClient::run() {
    bool should_exit = false;
    while (not should_exit) {
        while (not read_client_and_try_send_request()) {}
        const std::string msg = this->protocol.get_server_msg();
        if (contains_ended_game_msg(msg))
            should_exit = true;
        show_server_msg(msg);
    }
}

TatetiClient::~TatetiClient() {}
