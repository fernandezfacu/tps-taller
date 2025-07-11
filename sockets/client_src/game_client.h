#ifndef CS2D_GAME_CLIENT_H
#define CS2D_GAME_CLIENT_H

#include <string>

#include "client_protocol.h"
#include "parser.h"

/*
 * CS2DGameClient
 * Cliente de juego para el counter-strike 2D
 * Se conecta a un servidor y maneja la lógica de comunicación con el cliente para la
 * fase de compras del mismo.
 *
 *
 *
 * Aclaración sobre el uso de WeaponTypeTranslator:
 *
 * Es usado para traducir los tipos de arma en strings, del mismo modo que se traducen para
 * el protocolo de texto (aquí, usado para la impresión por stdout del equipamiento).
 * Si bien esto debería estar "separado", uso objeto de la misma clase ya que la traducción de
 * las entradas por stdin a armas y del protocolo de texto a armas son las mismas (es decir, un
 * "ak-47" se traduce en WeaponType::AK47 tanto para el protocolo de texto como para las entradas
 * del usuario). Si esto cambiara en un futuro, debería implementar un traductor distinto para cada
 * caso.
 *
 * */
class CS2DGameClient {
private:
    const Parser parser;
    ClientProtocol protocol;
    const WeaponTypeTranslator weapon_translator;

    /*
     * Chequea que el comando de compra de arma pueda enviarse al servidor según el
     * equipamiento actual del jugador.
     *
     * Devuelve true y envía la petición de compra si el comando era ejecutable, false en caso
     * contrario.
     * */
    bool try_send_buy_weapon_request(const Command& buy_weapon_command,
                                     const PlayerLoadout& loadout);
    /*
     * Chequea que el comando de compra de munición pueda enviarse al servidor según el
     * equipamiento actual del jugador.
     *
     * Devuelve true y envía la petición de compra si el comando era ejecutable, false en caso
     * contrario.
     *
     * */
    bool try_send_buy_ammo_request(const Command& buy_ammo_command, const PlayerLoadout& loadout);
    /*
     * Obtiene el input del usuario y pide al Parser que devuelva un comando si el input es válido.
     *
     * Si se ejecuta el comando y se envía request al servidor, devuelve true.
     *
     *
     * Si el comando no es válido o no se puede ejecutar, devuelve false.
     *
     * Si el comando es exit, devuelve false y guarda true en should_exit.
     *
     * */
    bool read_client_and_try_send_buy_request(const PlayerLoadout& loadout, bool& should_exit);

    std::string format_weapon_info(const WeaponType& weapon, uint16_t ammo) const;
    /*
     * Muestra por pantalla el equipamiento del usuario.
     * Usa CS2DGameClient::format_weapon_info como función auxiliar para el formateo de
     * la información de un arma (tipo y munición disponible).
     * */
    void show_player_loadout(const PlayerLoadout& player_loadout) const;

public:
    /*
     * Constructor para `CS2DGameClient` para conectarse a un servidor
     *
     * <hostname>/<service> es la dirección de la máquina remota a la cual se quiere conectar un
     * socket para comunicarse con el servidor.
     *
     * <username> es el nombre de usuario que debe recibir el servidor para reconocer al usuario.
     *
     * En caso de error el constructor lanza una excepción.
     * */
    CS2DGameClient(const std::string& hostname, const std::string& service,
                   const std::string& username);

    /*
     * CS2DGameClient::buy_phase maneja la fase de compras del cliente.
     *
     * Se pide al usuario que ingrese comandos por stdin y se envían requests al servidor
     * según los comandos válidos recibidos.
     *
     * Muestra por pantalla el equipamiento del usuario al comenzar la fase de compras
     * y cada vez luego de realizar una operación de compra (cada vez que el servidor lo envía).
     *
     * Se termina de pedir al usuario cuando este ingresa 'exit'.
     *
     * Si se cortó la conexión con el servidor, se lanza std::runtime_error.
     *
     * Se lanza la excepción correspndiente si ocurre un error.
     *
     *
     * */
    void buy_phase();

    CS2DGameClient(const CS2DGameClient&) = delete;
    CS2DGameClient& operator=(const CS2DGameClient&) = delete;

    ~CS2DGameClient();
};

#endif
