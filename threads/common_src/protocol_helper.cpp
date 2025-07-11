#include "protocol_helper.h"

#include <cstring>
#include <sstream>
#include <stdexcept>

#include <arpa/inet.h>

ProtocolHelper::ProtocolHelper() {}

void ProtocolHelper::recv_byte(Socket& skt, uint8_t& byte) {
    skt.recvall(&byte, sizeof(byte));
    if (skt.is_stream_recv_closed())
        throw std::runtime_error("Recv stream is closed");
}

void ProtocolHelper::send_byte(Socket& skt, const uint8_t& byte) {
    skt.sendall(&byte, sizeof(byte));
    if (skt.is_stream_send_closed())
        throw std::runtime_error("Recv stream is closed");
}

void ProtocolHelper::send_all_bytes(Socket& skt, const std::vector<uint8_t>& buf) {
    skt.sendall(buf.data(), buf.size());
    if (skt.is_stream_send_closed())
        throw std::runtime_error("Send stream is closed");
}

void ProtocolHelper::recv_all_bytes(Socket& skt, std::vector<uint8_t>& buf) {
    skt.recvall(buf.data(), buf.size());
    if (skt.is_stream_recv_closed())
        throw std::runtime_error("Recv stream is closed");
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

ProtocolHelper::~ProtocolHelper() {}
