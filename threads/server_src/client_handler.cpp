#include "client_handler.h"

#include <sstream>

#include "../common_src/liberror.h"

ClientHandler::ClientHandler(Socket&& peer, GameListProtected& games):
        peer(std::move(peer)),
        protocol(this->peer),
        games(games),
        state(std::make_unique<WaitingForGameState>()) {}

const std::string ClientHandler::game_names_msg(const std::vector<std::string>& game_names) {
    std::ostringstream oss;
    oss << "Partidas:\n";
    for (const auto& name: game_names) oss << " - " << name << "\n";
    return oss.str();
}

void ClientHandler::create_game(const std::string& name) {
    try {
        const auto result = games.create_game(name);
        TatetiGameProtected& game = result.first;
        const char mark = result.second;

        state = std::make_unique<PlayingState>(game, mark);
        std::string board_msg = game.get_board(mark);
        this->protocol.send_msg(board_msg);
    } catch (const std::invalid_argument& e) {
        this->protocol.send_msg("Error al crear la partida: " + std::string(e.what()));
    }
}

void ClientHandler::join_game(const std::string& name) {
    try {
        const auto result = games.join_game(name);
        TatetiGameProtected& game = result.first;
        const char mark = result.second;

        state = std::make_unique<PlayingState>(game, mark);
        std::string board_msg = game.get_board(mark);
        if (game.has_ended()) {
            state = std::make_unique<GameOverState>();
            stop();
        }
        this->protocol.send_msg(board_msg);
    } catch (const std::invalid_argument& e) {
        this->protocol.send_msg("Error al unirse a la partida: " + std::string(e.what()));
    }
}

void ClientHandler::list_games() {
    const std::string games_names = game_names_msg(games.list_names());
    this->protocol.send_msg(games_names);
}

void ClientHandler::handle_request_waiting_for_game() {
    try {
        const Request req = this->protocol.get_client_request();
        if (req.is_create) {
            create_game(req.name);
        } else if (req.is_join) {
            join_game(req.name);
        } else if (req.is_list) {
            list_games();
        } else {
            this->protocol.send_msg("No se deberia hacer una jugada mientras no se juega.");
        }
    } catch (const std::runtime_error& e) {
        state = std::make_unique<GameOverState>();
        stop();
    }
}

void ClientHandler::make_play(TatetiGameProtected& game, std::pair<uint8_t, uint8_t> pos,
                              const char& mark) {
    try {
        std::string board_msg = game.make_play_and_wait_turn(mark, pos.first + 1, pos.second + 1);
        if (game.has_ended()) {
            state = std::make_unique<GameOverState>();
            stop();
        }
        this->protocol.send_msg(board_msg);
    } catch (std::invalid_argument& e) {
        this->protocol.send_msg(e.what());
    }
}

void ClientHandler::play_tateti(TatetiGameProtected& game, const char& mark) {
    while (not game.has_ended()) {  // se intenta jugar la partida completa (solo no se completa por
                                    // finalizaciones forzadas)!
        try {
            const Request req = this->protocol.get_client_request();
            if (req.is_play) {
                make_play(game, req.pos, mark);
            } else {
                this->protocol.send_msg(
                        "No se deberia crear partida, listar partidas o unirse a una "
                        "mientras se juega");
            }
        } catch (const std::runtime_error& e) {
            game.force_end();  // sólo aca debería usar el clientHandler este método. En otro
                               // lado, NO.
            state = std::make_unique<GameOverState>();
            stop();
        }
    }
}

void ClientHandler::handle_request() { state->handle_request(*this); }

void ClientHandler::run() {
    while (should_keep_running()) handle_request();
}

void ClientHandler::kill() { stop(); }

ClientHandler::~ClientHandler() {}
