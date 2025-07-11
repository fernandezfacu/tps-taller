#include "acceptor.h"

#include <utility>

#include "../common_src/liberror.h"

Acceptor::Acceptor(Socket& skt, GameListProtected& games): skt(skt), games(games), clients() {}

void Acceptor::reap_dead() {
    clients.remove_if([](ClientHandler& c) {
        if (not c.is_alive()) {
            c.join();
            return true;
        }
        return false;
    });
    games.remove_ended_games();
}

void Acceptor::join_all() {
    games.end_non_started_games();

    for (auto& c: clients)  // no hace falta llamar al kill, espero que todas las partidas que se
                            // están jugando terminen.
        c.join();
    clients.clear();
}

void Acceptor::run() {
    while (true) {
        try {
            Socket peer = skt.accept();
            reap_dead();
            clients.emplace_back(std::move(peer), games);
            clients.back().start();
        } catch (const LibError& err) {
            if (err.get_error_code() == EINVAL) {
                break;
            } else {
                throw;
            }
        }
    }
    join_all();
}

Acceptor::~Acceptor() {}
