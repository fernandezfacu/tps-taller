#include <exception>
#include <iostream>
#include <string>

#include "tateti_client.h"

int main(int argc, const char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Bad program call. Expected " << argv[0]
                      << " <hostname> <service> <username>\n";
            return EXIT_FAILURE;
        }

        const std::string hostname = argv[1];
        const std::string service = argv[2];

        TatetiClient tateti_client(hostname, service);
        tateti_client.run();

        return EXIT_SUCCESS;
    } catch (const std::runtime_error& err) {
        std::cout << "Se cerró la conexión con el servidor :(\n";
        return EXIT_SUCCESS;
    } catch (const std::exception& err) {
        std::cerr << "Something went wrong and an exception was caught: " << err.what() << "\n";
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Something went wrong and an unknown exception was caught.\n";
        return EXIT_FAILURE;
    }
}
