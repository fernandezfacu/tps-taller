#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <string>
#include <utility>

#include "../common_src/buy_request.h"
#include "../common_src/player_loadout.h"
#include "../common_src/protocol_helper.h"
#include "../common_src/socket.h"

/*
 * ClientProtocol
 * Objeto encargado de serializar y enviar al servidor a través de un socket las peticiones de
 * compra del cliente, y deserializar para el cliente lo que envía el servidor.
 *
 * Implementa el protocolo binario tanto como el protocolo de texto, según el tipo protocolo que
 * indica el servidor en la construcción del objeto.
 *
 * */
class ClientProtocol {
private:
    Socket skt;
    uint8_t type;
    ProtocolHelper helper;

    /*
     * Métodos para la iniciación del protocolo.
     *
     * ClientProtocolo::send_username envía por al servidor el username
     *
     * ClientProtocolo::get_protocol_type recibe del servidor el tipo de protocolo a usar.
     *
     * */
    void send_username(const std::string& username);
    void get_protocol_type();

    /*
     * Método para la deserilización del protocolo binario de equipamiento.
     * */
    const PlayerLoadout get_binary_player_loadout();


    /*
     * Métodos para la deserialización del protocolo de texto de equipamiento.
     * */
    uint16_t deserialize_money_line(std::string& money_line);
    bool deserialize_knife_line(std::string& kinife_line);
    std::pair<WeaponType, uint16_t> deserialize_weapon_line(std::string& weapon_line,
                                                            bool is_primary);
    const PlayerLoadout get_text_player_loadout();


    /*
     * Métodos para la serialización del protocolo de texto de peticiones de compra.
     * */
    void send_text_buy_weapon_request(const WeaponType& weapon);
    void send_text_buy_ammo_request(bool is_primary, uint16_t ammo_count);

    /*
     * Métodos para la serialización del protocolo binario de peticiones de compra.
     * */
    void send_binary_buy_weapon_request(const WeaponType& weapon);
    void send_binary_buy_ammo_request(bool is_primary, uint16_t ammo_count);

    /*
     * Envia por socket al servidor una petición de compra de arma serializada según el tipo de
     * protocolo. (llama a ClientProtocol::send_text_buy_weapon_request o
     * ClientProtocol::send_binary_buy_weapon_request)
     *
     * Lanza una excepción si ocurre un error en la comunicación.
     *
     * */
    void send_buy_weapon_request(const WeaponType& weapon);
    /*
     * Envia por socket al servidor una petición de compra de munición serializada según el tipo de
     * protocolo. (llama a ClientProtocol::send_text_buy_ammo_request o
     * ClientProtocol::send_binary_buy_ammo_request)
     *
     * Lanza una excepción si ocurre un error en la comunicación.
     *
     * */
    void send_buy_ammo_request(bool is_primary, uint16_t ammo_count);

public:
    /*
     * Consructor de ClientProtocol
     *
     * <hostname>/<service> es la dirección de la máquina remota a la cual se quiere conectar un
     * socket para comunicarse con el servidor.
     *
     * <username> es el nombre de usuario que debe recibir el servidor para reconocer al usuario.
     *
     * Se recibe del servidor el tipo de protocolo (binario o de texto) a utilizar.
     *
     * En caso de error el constructor lanza una excepción.
     *
     * */
    ClientProtocol(const std::string& hostname, const std::string& service,
                   const std::string& username);


    /*
     * Recibe del servidor el equipamiento del jugador, lo deserializa según el tipo de protocolo
     * y lo devuelve como un struct DTO.
     * (llama a ClientProtocol::get_text_player_loadout o ClientProtocol::get_binary_player_loadout)
     *
     * Lanza una excepción si ocurre un error en la comunicación o al deserializar.
     *
     * */
    const PlayerLoadout get_player_loadout();

    /*
     * Envia por socket al servidor una petición de compra serializada según el tipo de protocolo.
     *
     * Lanza una excepción si ocurre un error en la comunicación.
     *
     * */
    void send_buy_request(const BuyRequest& buy_request);

    ClientProtocol(const ClientProtocol&) = delete;
    ClientProtocol& operator=(const ClientProtocol&) = delete;

    ~ClientProtocol();
};

#endif
