#include "tateti_game.h"

#include <sstream>

#include "../common_src/tateti_constants.h"

TatetiGame::TatetiGame():
        board(ROWS, std::vector<char>(ROWS, EMPTY)),
        current_player_mark(STARTING_PLAYER_MARK),
        ended(false),
        winner_mark(std::nullopt) {}

const bool TatetiGame::has_a_winner() const {
    for (int i = 0; i < ROWS; ++i) {
        if (board[i][0] != EMPTY && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return true;

        if (board[0][i] != EMPTY && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return true;
    }

    if (board[0][0] != EMPTY && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return true;

    if (board[0][2] != EMPTY && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return true;

    return false;
}

const bool TatetiGame::is_a_tie() const {
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < ROWS; ++j)
            if (board[i][j] == EMPTY)
                return false;

    return true;
}

const bool TatetiGame::is_valid(int row, int col) const {
    return ((row >= 1 and row <= ROWS) and (col >= 1 or col <= ROWS));
}

const bool TatetiGame::is_empty(int row, int col) const {
    return (board[row - 1][col - 1] == EMPTY);
}

void TatetiGame::change_turn() {
    current_player_mark = (current_player_mark == STARTING_PLAYER_MARK) ? JOINING_PLAYER_MARK :
                                                                          STARTING_PLAYER_MARK;
}

const bool TatetiGame::is_turn(const char mark) { return mark == current_player_mark; }

void TatetiGame::play(const char mark, int row, int col) {
    if (not is_turn(mark))
        throw std::invalid_argument("Invalid mark. Not its turn");
    if (not is_valid(row, col))
        throw std::invalid_argument("Invalid position. Should be two numbers between 1 and 3");
    if (not is_empty(row, col))
        throw std::invalid_argument("Invalid position. It is not empty");

    board[row - 1][col - 1] = current_player_mark;

    if (has_a_winner()) {
        select_winner();
        end_game();
    } else if (is_a_tie()) {
        end_game();
    } else {
        change_turn();
    }
}

void TatetiGame::select_winner() { winner_mark = current_player_mark; }

void TatetiGame::end_game() { ended = true; }

const bool TatetiGame::has_ended() const { return ended; }

const std::string TatetiGame::game_result_msg(const char& mark) {
    if (not winner_mark.has_value())
        return std::string(TIE_MSG) + "\n";

    if (winner_mark.value() != mark)
        return std::string(LOSS_MSG) + "\n";

    return std::string(WIN_MSG) + "\n";
}

std::ostream& operator<<(std::ostream& os, const TatetiGame& game) {
    for (int i = 0; i < ROWS + 1; ++i) {
        if (i == 0) {
            os << "   ";
            for (int j = 0; j < ROWS; ++j) os << " " << j + 1 << " .";
        } else {
            os << i << " |";
            for (int j = 0; j < ROWS; ++j) os << " " << game.board[i - 1][j] << " |";
        }
        os << "\n";
        os << "  +---+---+---+";
        os << "\n";
    }

    return os;
}

TatetiGame::~TatetiGame() {}
