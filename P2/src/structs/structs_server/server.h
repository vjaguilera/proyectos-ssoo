#pragma once
#include "../structs_shared/jugador.h"
#include "../structs_server/comunication.h"

typedef struct server{
    Jugador* lider;
    Jugador** clientes;
    int cantidad_clientes;
    int socket;
} Server;

Server* init_server(int socket);

void notify_all_clients(Server* server, char* msg);

void initial_listen(Server* server);