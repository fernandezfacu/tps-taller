#include <exception>
#include <iostream>

#include "tateti_server.h"

int main(int argc, const char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Bad program call. Expected " << argv[0] << " <port> <protocol-type>\n";
            return EXIT_FAILURE;
        }

        const std::string port = argv[1];

        TatetiServer tateti_server(port);
        tateti_server.run();

        return EXIT_SUCCESS;
    } catch (const std::exception& err) {
        std::cerr << "Something went wrong and an exception was caught: " << err.what() << "\n";
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Something went wrong and an unknown exception was caught.\n";
        return EXIT_FAILURE;
    }
}
