#include "tateti_game_protected.h"

#include <sstream>
#include <utility>

TatetiGameProtected::TatetiGameProtected(): started(false), ended(false) {}


const std::string TatetiGameProtected::get_board(const char player_mark) {
    std::unique_lock<std::mutex> lck(mtx);
    while (not game.is_turn(player_mark) and not ended) turn_changed_cv.wait(lck);
    std::ostringstream oss;
    oss << game;
    if (ended)
        oss << game.game_result_msg(player_mark);
    return oss.str();
}

void TatetiGameProtected::end_locked() {
    ended = true;
    turn_changed_cv.notify_all();
}

const std::string TatetiGameProtected::make_play_and_wait_turn(const char player_mark, int row,
                                                               int col) {
    /* llamada siempre que ese el turno (por el wait en get_board() que se usa previamente) */
    game.play(player_mark, row, col);

    std::unique_lock<std::mutex> lck(mtx);
    if (game.has_ended()) {
        end_locked();
    } else {
        turn_changed_cv.notify_all();
        while (not game.is_turn(player_mark) and not ended) turn_changed_cv.wait(lck);
    }
    std::ostringstream oss;
    oss << game;
    if (ended)
        oss << game.game_result_msg(player_mark);
    return oss.str();
}

bool TatetiGameProtected::has_ended() {
    std::unique_lock<std::mutex> lck(mtx);
    return ended;
}

void TatetiGameProtected::start() {
    std::unique_lock<std::mutex> lck(mtx);
    started = true;
}

bool TatetiGameProtected::has_started() {
    std::unique_lock<std::mutex> lck(mtx);
    return started;
}

void TatetiGameProtected::force_end() {
    std::unique_lock<std::mutex> lck(mtx);
    if (not ended)
        end_locked();
}

TatetiGameProtected::TatetiGameProtected(TatetiGameProtected&& other) noexcept:
        game(std::move(other.game)), started(other.started), ended(other.ended) {}

TatetiGameProtected& TatetiGameProtected::operator=(TatetiGameProtected&& other) noexcept {
    if (this != &other) {
        game = std::move(other.game);
        started = other.started;
        ended = other.ended;
    }
    return *this;
}

TatetiGameProtected::~TatetiGameProtected() {}
