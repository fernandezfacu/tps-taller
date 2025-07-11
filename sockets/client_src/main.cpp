#include <exception>
#include <iostream>
#include <string>

#include "game_client.h"

int main(int argc, const char* argv[]) {
    try {
        if (argc != 4) {
            std::cerr << "Bad program call. Expected " << argv[0]
                      << " <hostname> <service> <username>\n";
            return EXIT_FAILURE;
        }

        const std::string hostname = argv[1];
        const std::string service = argv[2];
        const std::string username = argv[3];
        CS2DGameClient game_client(hostname, service, username);
        game_client.buy_phase();
        return EXIT_SUCCESS;
    } catch (const std::runtime_error& err) {
        // se cerro la conexion con el cliente
        return EXIT_SUCCESS;
    } catch (const std::exception& err) {
        std::cerr << "Something went wrong and an exception was caught: " << err.what() << "\n";
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Something went wrong and an unknown exception was caught.\n";
        return EXIT_FAILURE;
    }
}
