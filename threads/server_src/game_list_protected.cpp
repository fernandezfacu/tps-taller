#include "game_list_protected.h"

#include <stdexcept>

#include "../common_src/tateti_constants.h"

GameListProtected::GameListProtected() {}

std::pair<TatetiGameProtected&, char> GameListProtected::create_game(const std::string& name) {
    std::unique_lock<std::mutex> lock(mtx);

    if (games.find(name) != games.end()) {
        throw std::invalid_argument("No se pudo crear la partida. Ya existe una con ese nombre.");
    }

    games.emplace(name, TatetiGameProtected());

    char mark = STARTING_PLAYER_MARK;
    TatetiGameProtected& game = games.at(name);
    return {game, mark};
}

std::pair<TatetiGameProtected&, char> GameListProtected::join_game(const std::string& name) {
    std::unique_lock<std::mutex> lock(mtx);
    auto it = games.find(name);

    if (it == games.end()) {
        throw std::invalid_argument("No se pudo unir a la partida. No existe uno con ese nombre");
    }

    if (it->second.has_started()) {
        throw std::invalid_argument("No se pudo unir a la partida, ya está completa");
    }

    char mark = JOINING_PLAYER_MARK;
    it->second.start();
    return {it->second, mark};
}

const std::vector<std::string> GameListProtected::list_names() {
    std::unique_lock<std::mutex> lock(mtx);
    std::vector<std::string> names;
    names.reserve(games.size());
    for (const auto& [name, _]: games) {
        names.push_back(name);
    }
    return names;
}

void GameListProtected::remove_ended_games() {
    for (auto it = games.begin(); it != games.end();) {
        if (it->second.has_ended()) {
            it = games.erase(it);
        } else {
            ++it;
        }
    }
}
void GameListProtected::end_non_started_games() {
    for (auto it = games.begin(); it != games.end(); ++it) {
        if (!it->second.has_started()) {
            it->second.force_end();
        }
    }
}

GameListProtected::~GameListProtected() {}
