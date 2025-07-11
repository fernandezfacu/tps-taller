#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <list>
#include <map>
#include <string>

#include "../common_src/socket.h"

#include "client_handler.h"
#include "game_list_protected.h"
#include "tateti_game_protected.h"
#include "thread.h"

/*
 * Acceptor
 *
 * Objeto encargado de aceptar clientes y lanzar threads para el manejo de cada uno de ellos.
 * Guarda una lista de partidas que será usada por los clientes para crear partidas o unirse
 * para jugar. Se encarga de borrar clientes que finalizaron su ejecución y partidas que terminaron,
 * además de ejecutar un shutdown polite si se cierra el servidor (se espera a que terminen las
 * partidas que se están jugando).
 *
 *
 * */
class Acceptor: public Thread {
private:
    Socket& skt;
    GameListProtected& games;
    std::list<ClientHandler> clients;

    /*
     * Método que borra clientes que finalizaron su ejecución de la lista de clientes, y partidas
     * que ya no se están jugando de la lista de partidas.
     *
     * */
    void reap_dead();

    /*
     * Método que es llamado al cerrarse el server, elimina las partidas que no legaron a comenzarse
     * y espera a que los clientes que están jugando terminen las suyas.
     *
     * */
    void join_all();

public:
    Acceptor(Socket& skt, GameListProtected& games);

    /*
     * run del Acceptor.
     *
     * Acepta clientes, lanza thread por cada uno (usando ClientHandler), hace el reap de clientes
     * y partidas al aceptar uno y ejecuta el shutdown polite si se cierra el socket del servidor.
     * */
    void run() override;

    ~Acceptor() override;
};

#endif
