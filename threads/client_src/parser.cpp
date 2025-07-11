#include "parser.h"

#include <iostream>
#include <sstream>
#include <utility>

Parser::Parser() {}

int Parser::get_int(const std::string& arg2) const {
    try {
        size_t pos;
        int count = std::stoi(arg2, &pos);
        return count;
    } catch (const std::exception&) {
        return -1;
    }
}


std::optional<Command> Parser::get_command_from(const std::string& line) const {
    std::istringstream iss(line);
    std::string comando, arg1, arg2, extra;

    if (!(iss >> comando))
        return std::nullopt;
    if (comando == CREATE_INPUT) {
        if (!(iss >> arg1) || (iss >> extra)) {
            std::cout << "The 'create' command requires 1 argument: <game-name>." << std::endl;
        } else {
            return Command(true, false, arg1, false, false, std::pair<uint8_t, uint8_t>{4, 4});
        }
    } else if (comando == LIST_INPUT) {
        if ((iss >> extra)) {
            std::cout << "The 'list' command does not accept any argument." << std::endl;
        } else {
            return Command(false, false, "", true, false, std::pair<uint8_t, uint8_t>{4, 4});
        }
    } else if (comando == JOIN_INPUT) {
        if (!(iss >> arg1) || (iss >> extra)) {
            std::cout << "The 'join' command requires 1 argument: <game-name>." << std::endl;
        } else {
            return Command(false, true, arg1, false, false, std::pair<uint8_t, uint8_t>{4, 4});
        }
    } else if (comando == PLAY_INPUT) {
        if (!(iss >> arg1 >> arg2) || (iss >> extra)) {
            std::cout << "The 'play' command requires 2 arguments: <row> <col>." << std::endl;
        } else {
            int row = get_int(arg1);
            int col = get_int(arg2);
            return Command(false, false, "", false, true, std::pair<uint8_t, uint8_t>{row, col});
        }
    } else {
        std::cout << "Unknown command: " << comando << std::endl;
    }
    return std::nullopt;
}

Parser::~Parser() {}
