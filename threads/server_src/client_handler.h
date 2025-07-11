#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "client_state.h"
#include "game_list_protected.h"
#include "server_protocol.h"
#include "thread.h"

/*
 * ClientHandler
 *
 * Objeto encargado de manejar la lógica de un cliente desde el servidor,
 * tanto para el estado de lobby como para cuando se empieza a jugar una partida.
 * Esto se maneja mediante polimorfismo con un ClientHandlerState, que es WaitingForGameState cuando
 * se está en el lobby, PlayingState cuando se está jugando y GameOverState cuando se termina de
 * jugar la partida.
 *
 *
 * */
class ClientHandler: public Thread {
private:
    Socket peer;
    ServerProtocol protocol;
    GameListProtected& games;
    std::unique_ptr<ClientHandlerState> state;


    /*
     * Métodos para manejar requests en "estado de lobby", antes de ingresar a jugar una partida.
     */

    void create_game(const std::string& name);
    void join_game(const std::string& name);
    void list_games();
    const std::string game_names_msg(const std::vector<std::string>& game_names);

    /*
     * Método para manejar request jugando, luego de ingresar a una partida.
     */
    void make_play(TatetiGameProtected& game, std::pair<uint8_t, uint8_t> pos, const char& mark);

    /*
     * Handler de request. se llama a state->handle_request() para manejar cada request según el
     * estado del cliente.
     */
    void handle_request();

public:
    ClientHandler(Socket&& peer, GameListProtected& games);

    /*
     * Métodos para double dispatch con State.
     * Lo ideal sería que sean privados y sólo objetos de clase State puedan llamarlos,
     * probablemente el uso de friend sería conveniente en casos como este, pero no se usó
     * al no ser visto en clase aún.
     */

    void handle_request_waiting_for_game();
    void play_tateti(TatetiGameProtected& game, const char& mark);

    /*
     * run del ClientHandler
     *
     * Llama a handle_request() si should_keep_runninf() devuelve true.
     * */
    void run() override;

    void kill();  // polite

    ~ClientHandler() override;
};

#endif
