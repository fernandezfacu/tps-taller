#include "protocol_helper.h"

#include <algorithm>
#include <cstring>
#include <sstream>
#include <stdexcept>

#include <arpa/inet.h>

#include "protocol_constants.h"

ProtocolHelper::ProtocolHelper(): weapon_translator() {}

std::string ProtocolHelper::recv_lines(Socket& skt, const int lines) {
    std::ostringstream partial;
    int lines_read = 0;
    while (lines_read < lines) {
        std::vector<char> buf(BUFFER_SIZE);
        size_t bytes_received = skt.recvsome(buf.data(), buf.size());
        if (skt.is_stream_recv_closed())
            throw std::runtime_error("Recv stream is closed");

        std::string received(buf.data(), bytes_received);
        for (char& c: received)
            if (!isascii(c))
                c = '@';

        lines_read += std::count(received.begin(), received.end(), LINE_DELIMITER_TEXT);
        partial << received;
    }

    std::string text = partial.str();
    size_t pos = 0;
    for (int count = 0; count < lines; ++count) {
        pos = text.find(LINE_DELIMITER_TEXT, pos);
        ++pos;
    }
    if (pos < text.size())
        throw std::invalid_argument("Unexpected extra data after last line.");
    return text;
}

void ProtocolHelper::check_prefix(std::string& line, const std::string& prefix) {
    if (line.rfind(prefix, 0) != 0)
        throw std::invalid_argument("Line must start with '" + prefix + "'");
    line = line.substr(prefix.size());
}

uint16_t ProtocolHelper::get_number_from_string(const std::string& line) {
    try {
        int value = std::stoi(line);
        if (value < 0)
            throw std::invalid_argument("value must be a non-negative number");
        return static_cast<uint16_t>(
                value);  // Supongo que, si se recibe un numero positivo, es de 2 bytes.
    } catch (const std::exception& e) {
        throw std::invalid_argument("value must be a number: " + std::string(e.what()));
    }
}

void ProtocolHelper::recv_all_bytes(Socket& skt, std::vector<uint8_t>& buf) {
    skt.recvall(buf.data(), buf.size());
    if (skt.is_stream_recv_closed())
        throw std::runtime_error("Recv stream is closed");
}

void ProtocolHelper::recv_byte(Socket& skt, uint8_t& byte) {
    skt.recvall(&byte, sizeof(byte));
    if (skt.is_stream_recv_closed())
        throw std::runtime_error("Recv stream is closed");
}

void ProtocolHelper::send_all_bytes(Socket& skt, std::vector<uint8_t>& buf) {
    skt.sendall(buf.data(), buf.size());
    if (skt.is_stream_send_closed())
        throw std::runtime_error("Send stream is closed");
}

uint16_t ProtocolHelper::get_two_bytes_in_little_endian(std::vector<uint8_t>& buf,
                                                        unsigned int& index) const {
    uint16_t bytes;
    std::memcpy(&bytes, buf.data() + index, sizeof(bytes));
    index += 2;
    return ntohs(bytes);
}

void ProtocolHelper::save_two_bytes_in_big_endian(std::vector<uint8_t>& buf, unsigned int& index,
                                                  const uint16_t& bytes) const {
    uint16_t bytes_be = htons(bytes);
    std::memcpy(buf.data() + index, &bytes_be, sizeof(bytes_be));
    index += 2;
}

void ProtocolHelper::save_string(std::vector<uint8_t>& buf, unsigned int& index,
                                 const std::string& string) {
    std::memcpy(buf.data() + index, string.c_str(), string.size());
    index += string.size();
}

void ProtocolHelper::check_header(const uint8_t& recvd_header, uint8_t expected_header) {
    if (recvd_header != expected_header) {
        throw std::invalid_argument("Invalid header");
    }
}

const std::string ProtocolHelper::get_string_from_weapon(const WeaponType& weapon) const {
    return this->weapon_translator.get_string_from_weapon(weapon);
}
const WeaponType ProtocolHelper::get_weapon_from_string(const std::string& weapon_string) const {
    return this->weapon_translator.get_weapon_from_string(weapon_string);
}
const uint8_t ProtocolHelper::get_code_from_weapon(const WeaponType& weapon) const {
    return this->weapon_translator.get_code_from_weapon(weapon);
}
const WeaponType ProtocolHelper::get_weapon_from_code(const uint8_t& weapon_code) const {
    return this->weapon_translator.get_weapon_from_code(weapon_code);
}

ProtocolHelper::~ProtocolHelper() {}
