#include "server.h"

Server* init_server() {
    Server* server = malloc(sizeof(Server));
    server -> clientes = malloc(sizeof(Jugador) * 5);
    server -> cantidad_clientes = 0;
    return server;
}

void notify_all_clients(Server* server, char* msg) {
    for (int i = 0; i < server -> cantidad_clientes; i++) {
        server_send_message(server -> clientes[i] -> socket, 3, msg);
    }
}