#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <string>
#include <vector>

#include "../common_src/buy_request.h"
#include "../common_src/player_loadout.h"
#include "../common_src/protocol_helper.h"
#include "../common_src/socket.h"

/*
 * ServerProtocol
 * Objeto encargado de serializar y enviar al cliente a través de un socket el equipamiento de
 * jugador, y deserializar para el servidor las peticiones que envía el cliente.
 *
 * Implementa el protocolo binario tanto como el protocolo de texto, según el tipo protocolo que
 * se recibe para la construcción del objeto.
 *
 * */
class ServerProtocol {
private:
    uint8_t type;
    Socket srv;
    Socket peer;
    ProtocolHelper helper;

    /*
     * Método para la iniciación del protocolo.
     *
     * Envía al cliente el tipo de protocolo a usar.
     *
     * Lanza una excepción si hubo un error en la comuniación.
     * */
    void send_protocol_type();

    /*
     * Métodos para la serialización del protocolo binario de equipamiento.
     * ServerProtocol::send_binary_player_loadout se encarga de enviar por socket el equipamiento
     * y llama al otro método para serializar.
     * */
    const std::vector<uint8_t> serialize_binary_loadout(const PlayerLoadout& loadout);
    void send_binary_player_loadout(const PlayerLoadout& loadout);

    /*
     * Métodos para la serialización del protocolo de texto de equipamiento.
     * ServerProtocol::send_text_player_loadout se encarga de enviar por socket el equipamiento
     * y llama a los demás métodos para serializar.
     * */
    const std::string serialize_text_money(const uint16_t& money);
    const std::string serialize_text_knife(const bool& is_knife_equipped);
    const std::string serialize_text_weapon(const WeaponType& weapon, const uint16_t& ammo,
                                            const bool& is_primary);
    const std::string serialize_text_loadout(const PlayerLoadout& loadout);
    void send_text_player_loadout(const PlayerLoadout& loadout);

    /*
     * Métodos para la deserialización del protocolo de texto de peticiones de compra del cliente.
     * ServerProtocol::get_binary_buy_request se encarga de recibir por socket la petición
     * y llamar al método que corresponda según el tipo.
     * */
    BuyRequest get_binary_buy_weapon_request();
    BuyRequest get_binary_buy_ammo_request();
    BuyRequest get_binary_buy_request();

    /*
     * Métodos para la deserialización del protocolo de texto de peticiones de compra del cliente.
     *
     * ServerProtocol::get_text_buy_request se encarga de recibir por socket la petición
     * y llamar al método que corresponda según el tipo.
     * */
    BuyRequest get_text_buy_weapon_request(std::string& buy_weapon_line);
    BuyRequest get_text_buy_ammo_request(std::string& buy_ammo_line);
    BuyRequest get_text_buy_request();

public:
    /*
     * Consructor de ServerProtocol
     *
     * Buscará una dirección local válida escuchar y aceptar conexiones automáticamente en el <port>
     * dado.
     *
     * <protocol_type> es el tipo de protocolo (binario o de texto) a utilizar.
     *
     * En caso de error el constructor lanza una excepción.
     *
     * */
    ServerProtocol(const std::string& port, const std::string& protocol_type);

    /*
     * Método para la iniciación del protocolo.
     *
     * Recibe el username desde el cliente y lo devuelve.
     *
     * Lanza una excepción si hubo un error en la comuniación o
     * si no se recibió el username en el formato esperado.
     *
     * */
    const std::string get_username();

    /*
     * Envía por socket al cliente el equipamiento del jugador, serializándolo según el tipo de
     * protocolo. (llama a ServerProtocol::send_text_player_loadout o
     * ServerProtocol::send_binary_player_loadout)
     *
     * Lanza una excepción si ocurre un error en la comunicación.
     *
     * */
    void send_player_loadout(const PlayerLoadout& player_loadout);

    /*
     * Recibe por socket del cliente una petición de compra y la deserializa según el tipo de
     * protocolo.
     *
     * Lanza una excepción si ocurre un error en la comunicación o al deserializar.
     *
     * */
    BuyRequest get_buy_request();

    ServerProtocol(const ServerProtocol&) = delete;
    ServerProtocol& operator=(const ServerProtocol&) = delete;

    ~ServerProtocol();
};

#endif
