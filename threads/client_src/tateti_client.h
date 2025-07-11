#ifndef TATETI_CLIENT_H
#define TATETI_CLIENT_H

#include <string>

#include "client_protocol.h"
#include "parser.h"

#define ROWS 3

/*
 * TatetiClient
 * Cliente de juego para el ta-te-ti.
 * Se conecta a un servidor y maneja la lógica de comunicación con el mismo
 * para jugar al tateti con un contrincante online.
 *
 * */
class TatetiClient {
private:
    const Parser parser;
    ClientProtocol protocol;

    /*
     * Muestra por pantalla un mensaje del server, tal cual es recibido.
     * */
    void show_server_msg(const std::string& msg) const;

    /*
     * Chequea que el comando de jugada pueda enviarse al servidor según el
     * las posiciones indicadas para jugar.
     *
     * Devuelve true y envía la petición de jugada si las posiciones están dentro
     * de los rangos del tablero de ta-te-ti, false en caso contrario.
     *
     * */
    bool try_send_play_request(const Command& play_command);

    /*
     * Obtiene el input del usuario y pide al Parser que devuelva un comando si el input es válido.
     *
     * Si se ejecuta el comando y se envía request al servidor, devuelve true.
     *
     *
     * Si el comando no es válido o no se puede ejecutar, devuelve false.
     *
     * */
    bool read_client_and_try_send_request();

    bool contains_ended_game_msg(const std::string& msg);

public:
    /*
     * Constructor para `TatetiClient` para conectarse a un servidor
     *
     * <hostname>/<service> es la dirección de la máquina remota a la cual se quiere conectar un
     * socket para comunicarse con el servidor.
     *
     * En caso de error el constructor lanza una excepción.
     * */
    TatetiClient(const std::string& hostname, const std::string& service);

    /*
     * TatetiClient::run
     *
     *
     * */
    void run();

    TatetiClient(const TatetiClient&) = delete;
    TatetiClient& operator=(const TatetiClient&) = delete;

    ~TatetiClient();
};

#endif
