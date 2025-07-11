#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <string>

#include "../common_src/protocol_helper.h"
#include "../common_src/request.h"
#include "../common_src/socket.h"

/*
 * ServerProtocol
 * Objeto encargado de serializar y enviar al cliente estados del juego de ta-te-ti o de las
 * partidas actuales; y deserializar para el servidor las peticiones que envía el cliente.
 *
 * */
class ServerProtocol {
private:
    Socket& peer;
    ProtocolHelper helper;


    /*
     * Métodos para la deserialización de requests, según su tipo.
     *
     * */

    const Request get_create_game_request();
    const Request get_join_game_request();
    const std::string get_name();
    const Request get_list_games_request();
    const Request get_play_request();

public:
    /*
     * Consructor de ServerProtocol
     *
     * Buscará una dirección local válida escuchar y aceptar conexiones automáticamente en el <port>
     * dado.
     *
     * En caso de error el constructor lanza una excepción.
     *
     * */
    explicit ServerProtocol(Socket& peer);

    /*
     * Recibe una request por socket, la deserializa y devuelve.
     *
     * Lanza una excepción si ocurrió un error de conexión o si el tipo de request
     * fue inválido.
     * */
    const Request get_client_request();

    /*
     * Serializa un mensaje en formato string y lo envía por socket.
     *
     * Lanza una excepción si ocurrió un error de conexión.
     * */
    void send_msg(const std::string& msg);

    ServerProtocol(const ServerProtocol&) = delete;
    ServerProtocol& operator=(const ServerProtocol&) = delete;

    ~ServerProtocol();
};

#endif
