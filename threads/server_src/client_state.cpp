#include "client_state.h"

#include "client_handler.h"

WaitingForGameState::WaitingForGameState() {}

void WaitingForGameState::handle_request(ClientHandler& client_handler) {
    client_handler.handle_request_waiting_for_game();
}

PlayingState::PlayingState(TatetiGameProtected& game, const char& mark): game(game), mark(mark) {}

void PlayingState::handle_request(ClientHandler& client_handler) {
    client_handler.play_tateti(game, mark);
}

GameOverState::GameOverState() {}

void GameOverState::handle_request(ClientHandler& client_handler) {
    throw(std::runtime_error("El juego terminó. No deberían enviarse más requests."));
}
