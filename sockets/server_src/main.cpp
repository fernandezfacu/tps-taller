#include <exception>
#include <iostream>

#include "game_server.h"

int main(int argc, const char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Bad program call. Expected " << argv[0] << " <port> <protocol-type>\n";
            return EXIT_FAILURE;
        }

        const std::string port = argv[1];
        const std::string protocol_type = argv[2];

        CS2DGameServer game_server(port, protocol_type);

        game_server.buy_phase();

        return EXIT_SUCCESS;
    } catch (const std::runtime_error& err) {
        // se cerro la conexion con el cliente
        return 0;
    } catch (const std::exception& err) {
        std::cerr << "Something went wrong and an exception was caught: " << err.what() << "\n";
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Something went wrong and an unknown exception was caught.\n";
        return EXIT_FAILURE;
    }
}
