#ifndef GAME_LIST_PROTECTED_H
#define GAME_LIST_PROTECTED_H

#include <map>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "tateti_game_protected.h"

/*
 * GameListProtected
 *
 * Monitor de lista de partidas.
 *
 * Maneja las critical sections para que un cliente cree una partida, se una a una partida
 * u obtenga la lista de las partidas activas hasta el momento.
 *
 * Además tiene dos métodos que son para el uso del Acceptor del servidor, para hacer un reap de
 * partidas finalizadas y finalizar partidas no empezadas al cerrar el servidor.
 *
 * */
class GameListProtected {
private:
    std::map<std::string, TatetiGameProtected> games;
    std::mutex mtx;

public:
    GameListProtected();

    std::pair<TatetiGameProtected&, char> create_game(const std::string& name);
    std::pair<TatetiGameProtected&, char> join_game(const std::string& name);
    const std::vector<std::string> list_names();

    /* Sólo deberían ser llamadas por acceptor */
    void remove_ended_games();
    void end_non_started_games();

    GameListProtected(const GameListProtected&) = delete;
    GameListProtected& operator=(const GameListProtected&) = delete;

    ~GameListProtected();
};

#endif
