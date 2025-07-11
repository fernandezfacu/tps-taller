#ifndef TATETI_SERVER_H
#define TATETI_SERVER_H

#include <string>

#include "../common_src/socket.h"

#include "acceptor.h"

#define CLOSE_SERVER_INPUT 'q'

/*
 * TatetiServer
 * Servidor de juego para el ta-te-ti
 *
 * Maneja la lógica de juego de ta-te-ti entre dos personas.
 * Maneja la comunicación con clientes para el desarrollo de las partidas.
 * Soporta múltiples partidas de ta-te-ti en simultáneo.
 *
 * */
class TatetiServer {
private:
    Socket skt;
    GameListProtected games;
    Acceptor acceptor;

    bool should_close();

    void close();

public:
    /*
     * Constructor para `TatetiServer` para esperar conexión del cliente
     *
     * El protocolo buscará una dirección local válida escuchar y aceptar conexiones automáticamente
     * en el <port> dado.
     *
     * En caso de error el constructor lanza una excepción.
     * */
    explicit TatetiServer(const std::string& port);

    /*
     * TatetiServer::run
     *
     * Lanza el aceptador para aceptar clientes y espera a que se reciba 'q' por
     * stdin para cerrar el servidor.
     * */
    void run();

    TatetiServer(const TatetiServer&) = delete;
    TatetiServer& operator=(const TatetiServer&) = delete;

    ~TatetiServer();
};

#endif
