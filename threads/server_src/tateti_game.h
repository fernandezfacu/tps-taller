#ifndef TATETI_GAME_H
#define TATETI_GAME_H

#include <optional>
#include <string>
#include <vector>

#define ROWS 3
#define EMPTY ' '

/*
 * Clase TatetiGame
 *
 * Representa un juego de tateti, pueden hacerse jugadas y decide si el juego finaliza o no
 * (por haber un ganador o ser un empate).
 *
 * Además, puede pedírsele una representación del tablero en formato de string.
 *
 * */
class TatetiGame {
private:
    std::vector<std::vector<char>> board;
    char current_player_mark;
    bool ended;
    std::optional<char> winner_mark;

    /*
     * Devuelve true si algún jugador ganó el juego, false en caso contrario.
     *
     * */
    const bool has_a_winner() const;

    /*
     * Devuelve true si todos los casilleros están llenos, false en caso contrario.
     *
     * Debe ser llamado luego de comprobar que no haya un ganador, ya que no hace ese chequeo.
     *
     * */
    const bool is_a_tie() const;

    /*
     * Devuelve true si la posicion está dentro de los límites del tablero,
     * false en caso contrario.
     *
     * */
    const bool is_valid(int row, int col) const;

    /*
     * Devuelve true si la posicion está vacía, false en caso contrario.
     *
     * */
    const bool is_empty(int row, int col) const;

    /*
     * Cambia el jugador actual (cambia current_player_mark de CROSS a CIRCLE y viceversa, según
     * corresponda).
     *
     * */
    void change_turn();

    /*
     * Setea el ganador, según cuál sea current_player_mark.
     *
     * Debe ser llamado luego de comprobar que hay un ganador, sin cambiar turnos en el medio.
     * */
    void select_winner();

    /*
     * Finaliza el juego (setea has_ended a true).
     *
     * */
    void end_game();

    friend std::ostream& operator<<(std::ostream& os, const TatetiGame& game);

public:
    /*
     * Consructor de TatetiGame
     *
     * Inicializa una partida de ta-te-ti con el tablero vacío
     * y con turno para el jugador O.
     *
     * */
    TatetiGame();

    /*
     * Devuelve true si el la marca es la del jugador del que es el turno,
     * false en caso contrario.
     *
     * */
    const bool is_turn(const char mark);

    /*
     * Hace una jugada para el jugador actual, en la posición indicada.
     *
     * Es decir, ocupa la posición en el tablero con la marca del jugador del que sea el turno.
     *
     * Si la posición no es válida para hacer una jugada (fuera de los límites del tablero u
     * ocupada), lanza una excepción.
     *
     * */
    void play(const char mark, int row, int col);

    /*
     * Devuelve true si el juego ha terminado, false si se puede continuar jugando.
     *
     * */
    const bool has_ended() const;

    /*
     * Devuelve un string con el resultado de la partida para el jugador que
     * jugó con la marca pasada por parámetro.
     *
     * */
    const std::string game_result_msg(const char& mark);

    TatetiGame(const TatetiGame&) = delete;
    TatetiGame& operator=(const TatetiGame&) = delete;

    TatetiGame(TatetiGame&&) = default;
    TatetiGame& operator=(TatetiGame&&) = default;

    ~TatetiGame();
};

#endif
