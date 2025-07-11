#ifndef PROTOCOL_HELPER_H
#define PROTOCOL_HELPER_H

#include <string>
#include <vector>

#include "socket.h"

/*
 * ProtocolHelper
 * Clase que implementa métodos que comparten el protocolo del cliente y el protocolo del servidor.
 *
 * 1 - Aclaración sobre esta clase:
 *
 * Al no usar herencia y no tener una clase abstracta "Protocol" que las implemente para que cada
 * protocolo herede de ella, esta clase soluciona ese problema siendo usada como atributo de cada
 * protocolo.
 * */
class ProtocolHelper {
private:
public:
    ProtocolHelper();

    void recv_byte(Socket& skt, uint8_t& byte);
    void send_byte(Socket& skt, const uint8_t& byte);
    void recv_all_bytes(Socket& skt, std::vector<uint8_t>& buf);
    void send_all_bytes(Socket& skt, const std::vector<uint8_t>& buf);

    uint16_t get_two_bytes_in_little_endian(std::vector<uint8_t>& buf, unsigned int& index) const;
    void save_two_bytes_in_big_endian(std::vector<uint8_t>& buf, unsigned int& index,
                                      const uint16_t& bytes) const;
    void save_string(std::vector<uint8_t>& buf, unsigned int& index, const std::string& string);

    ProtocolHelper(const ProtocolHelper&) = delete;
    ProtocolHelper& operator=(const ProtocolHelper&) = delete;
    ~ProtocolHelper();
};

#endif
