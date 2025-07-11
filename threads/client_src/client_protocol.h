#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <string>
#include <utility>

#include "../common_src/protocol_helper.h"
#include "../common_src/request.h"
#include "../common_src/socket.h"

/*
 * ClientProtocol
 * Objeto encargado de serializar y enviar al servidor a través de un socket las peticiones del
 * cliente, y deserializar para el cliente lo que envía el servidor.
 *
 * */
class ClientProtocol {
private:
    Socket skt;
    ProtocolHelper helper;


    /*
     * Métodos para la serialización de requests, según su tipo
     *
     * */

    void send_create_game_request(const Request& request);
    void send_join_game_request(const Request& request);
    void send_name_with_header(const uint8_t& header, const std::string& name);
    void send_list_games_request();
    void send_play_request(const Request& request);

public:
    /*
     * Consructor de ClientProtocol
     *
     * <hostname>/<service> es la dirección de la máquina remota a la cual se quiere conectar un
     * socket para comunicarse con el servidor.
     *
     * En caso de error el constructor lanza una excepción.
     *
     * */
    ClientProtocol(const std::string& hostname, const std::string& service);

    /*
     * Serializa y envía una request por socket, según el tipo de la misma.
     *
     * Lanza una excepción si ocurrió un error de conexión o si el tipo de request
     * fue inválido.
     * */
    void send_request(const Request& request);

    /*
     * Obtiene un mensaje del servidor y lo deserializa,
     * devolviéndolo en formato string.
     *
     * Lanza una excepción si ocurrió un error de conexión.
     * */
    const std::string get_server_msg();

    ClientProtocol(const ClientProtocol&) = delete;
    ClientProtocol& operator=(const ClientProtocol&) = delete;

    ~ClientProtocol();
};

#endif
