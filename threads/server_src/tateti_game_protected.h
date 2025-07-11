#ifndef TATERI_GAME_PROTECTED_H
#define TATERI_GAME_PROTECTED_H

#include <condition_variable>
#include <mutex>
#include <optional>
#include <string>

#include "tateti_game.h"

/*
 * GameListProtected
 *
 * Monitor de lista de partidas.
 *
 * Maneja las critical sections para que un cliente haga una jugada y espere su turno
 * (make_play_and_wait_turn), o para que pida el tablero al entrar a una partida (antes de tener que
 * hacer una jugada, get_board).
 *
 * Además se maneja el inicio de una partida (cuando ingresa un segundo jugador al unirse) y la
 * finalización forzada de una partida, que debe ser usada en casos de errores de conexión en los
 * clientes o para el cierre del servidor.
 *
 * */
class TatetiGameProtected {
private:
    TatetiGame game;
    std::mutex mtx;
    std::condition_variable turn_changed_cv;
    bool started;
    bool ended;

    void end_locked();

public:
    TatetiGameProtected();

    const std::string get_board(const char player_mark);
    const std::string make_play_and_wait_turn(const char player_mark, int row, int col);

    void start();
    bool has_started();

    bool has_ended();
    void force_end();

    TatetiGameProtected(const TatetiGameProtected&) = delete;
    TatetiGameProtected& operator=(const TatetiGameProtected&) = delete;

    TatetiGameProtected(TatetiGameProtected&&) noexcept;
    TatetiGameProtected& operator=(TatetiGameProtected&&) noexcept;

    ~TatetiGameProtected();
};


#endif
