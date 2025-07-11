#ifndef CS2D_GAME_SERVER_H
#define CS2D_GAME_SERVER_H

#include <string>

#include "loadout_manager.h"
#include "server_protocol.h"

/*
 * CS2DGameClient
 * Servidor de juego para el counter-strike 2D
 *
 * Maneja la lógica de negocio del equipamiento y
 * la comunicación con el cliente para la fase de compras del juego.
 *
 * */
class CS2DGameServer {
private:
    ServerProtocol protocol;
    LoadoutManager loadout_manager;

    /*
     * Imprime por pantalla el mensaje de compra rechazada.
     *
     * */
    void rejected_purchase_message(const std::string& rejected);

    /*
     * Imprime por pantalla el mensaje de saludo al usuario.
     *
     * */
    void say_hello_to(const std::string& username);

public:
    /*
     * Constructor para `CS2DGameServer` para esperar conexión del cliente
     *
     * El protocolo buscará una dirección local válida escuchar y aceptar conexiones automáticamente
     * en el <port> dado.
     *
     * <protocol_type> es el tipo (binario o de texto) a utilizar por el protocolo.
     *
     * Saluda al usuario al conextarse con el cliente, imprimiendo por pantalla.
     *
     * En caso de error el constructor lanza una excepción.
     * */
    CS2DGameServer(const std::string& port, const std::string& protocol_type);

    /*
     * CS2DGameClient::buy_phase maneja la fase de compras desde el servidor.
     *
     * Se espera que el cliente envíe una petición de compra y se pide al negocio que se efectúe.
     *
     * Si no puede efectuarse, se imprime por pantalla un mensaje correspondiente al error.
     *
     * Se envía al cliente el estado del equipamiento se efectúe o no la compra.
     *
     * Se termina de pedir al cliente cuando se corta la conexión, lanzado una excepción.
     * Si se cortó la conexión, se lanza std::runtime_error.
     * Si ocurrió un error se lanza la correspondiente.
     * */
    void buy_phase();

    CS2DGameServer(const CS2DGameServer&) = delete;
    CS2DGameServer& operator=(const CS2DGameServer&) = delete;

    ~CS2DGameServer();
};

#endif
