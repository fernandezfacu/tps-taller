#ifndef PROTOCOL_HELPER_H
#define PROTOCOL_HELPER_H

#include <string>
#include <vector>

#include "socket.h"
#include "weapon_type_translator.h"

#define BUFFER_SIZE 64

/*
 * ProtocolHelper
 * Clase que implementa métodos que comparten el protocolo del cliente y el protocolo del servidor.
 *
 * 1 - Aclaración sobre esta clase:
 *
 * Al no usar herencia y no tener una clase abstracta "Protocol" que las implemente para que cada
 * protocolo herede de ella, esta clase soluciona ese problema siendo usada como atributo de cada
 * protocolo.
 *
 *
 * 2 - Aclaración sobre los protocolos binario y de texto:
 *
 * Cada protocolo implementa métodos para el protocolo binario y el de texto, cosa que también
 * podría haber sido resuelta con herencia (evitando esta mezcla de protocolos, haciendo uso de
 * polimorfismo en vez de ifs y una interfaz privada más cargada)
 *
 * Un ejemplo: En vez de tener get_weapon_from_code y get_weapon_from_string, se podría haber tenido
 * un único método get_weapon o deserialize_weapon que se implemente en cada subclase de protocolo
 * (de texto o binario) según corresponda. Desconozco si esto se puede hacer así en C++, ya que no
 * vimos herencia en clase, pero son ideas que tuve y no pude probar.
 * */
class ProtocolHelper {
private:
    const WeaponTypeTranslator weapon_translator;

public:
    ProtocolHelper();

    uint16_t get_two_bytes_in_little_endian(std::vector<uint8_t>& buf, unsigned int& index) const;
    void save_two_bytes_in_big_endian(std::vector<uint8_t>& buf, unsigned int& index,
                                      const uint16_t& bytes) const;

    void save_string(std::vector<uint8_t>& buf, unsigned int& index, const std::string& string);
    std::string recv_lines(Socket& skt, const int lines);
    void check_prefix(std::string& line, const std::string& prefix);
    uint16_t get_number_from_string(const std::string& line);
    void recv_byte(Socket& skt, uint8_t& byte);
    void recv_all_bytes(Socket& skt, std::vector<uint8_t>& buf);
    void send_all_bytes(Socket& skt, std::vector<uint8_t>& buf);
    void check_header(const uint8_t& recvd_header, uint8_t expected_header);
    const std::string get_string_from_weapon(const WeaponType& weapon) const;
    const WeaponType get_weapon_from_string(const std::string& weapon_string) const;
    const uint8_t get_code_from_weapon(const WeaponType& weapon) const;
    const WeaponType get_weapon_from_code(const uint8_t& weapon_code) const;

    ProtocolHelper(const ProtocolHelper&) = delete;
    ProtocolHelper& operator=(const ProtocolHelper&) = delete;
    ~ProtocolHelper();
};

#endif
