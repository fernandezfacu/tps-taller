#include "tateti_server.h"

#include <sstream>

TatetiServer::TatetiServer(const std::string& port):
        skt(port.c_str()), games(), acceptor(skt, games) {}

bool TatetiServer::should_close() {
    char c;
    std::cin.get(c);
    return c == CLOSE_SERVER_INPUT;
}

void TatetiServer::close() {
    skt.shutdown(2);
    skt.close();
    acceptor.join();
}

void TatetiServer::run() {
    acceptor.start();
    while (not should_close()) {}
    close();
}

TatetiServer::~TatetiServer() {}
