#include "game_server.h"

#include <iostream>

CS2DGameServer::CS2DGameServer(const std::string& port, const std::string& protocol_type):
        protocol(port, protocol_type) {
    const std::string username = this->protocol.get_username();
    say_hello_to(username);
}

void CS2DGameServer::say_hello_to(const std::string& username) {
    std::cout << username << " has arrived!" << std::endl;
}

void CS2DGameServer::rejected_purchase_message(const std::string& rejected) {
    std::cout << "Not enough money to buy " << rejected << std::endl;
}

void CS2DGameServer::buy_phase() {
    while (true) {
        this->protocol.send_player_loadout(this->loadout_manager.get_loadout());
        BuyRequest buy_request = this->protocol.get_buy_request();

        if (buy_request.is_weapon) {
            if (not this->loadout_manager.buy_primary_weapon(buy_request.weapon)) {
                rejected_purchase_message("weapon");
                continue;
            }
        } else {
            if (not this->loadout_manager.buy_ammo(buy_request.ammo_count,
                                                   buy_request.is_primary)) {
                rejected_purchase_message("ammo");
                continue;
            }
        }
    }
}

CS2DGameServer::~CS2DGameServer() {}
