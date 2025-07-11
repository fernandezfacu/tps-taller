#ifndef CLIENT_HANDLER_STATE_H
#define CLIENT_HANDLER_STATE_H

#include "../common_src/request.h"

#include "tateti_game_protected.h"

class ClientHandler;  // Forward declaration

/*
 * ClientHandlerState.
 *
 * Ver documentación de ClientHandler y el uso que hace de esta clase y sus subclases.
 * */
class ClientHandlerState {
public:
    virtual void handle_request(ClientHandler& client_handler) = 0;
    virtual ~ClientHandlerState() {}
};

class WaitingForGameState: public ClientHandlerState {
public:
    WaitingForGameState();
    void handle_request(ClientHandler& client_handler) override;
};

class PlayingState: public ClientHandlerState {
private:
    TatetiGameProtected& game;
    const char mark;

public:
    PlayingState(TatetiGameProtected& game, const char& mark);

    void handle_request(ClientHandler& client_handler) override;
};

class GameOverState: public ClientHandlerState {
public:
    GameOverState();
    void handle_request(ClientHandler& client_handler) override;
};

#endif
