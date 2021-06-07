#pragma once
#include "../structs_shared/jugador.h"
#include "../structs_server/comunication.h"

typedef struct server{
    Jugador* lider;
    Jugador** clientes;
    int cantidad_clientes;
} Server;

Server* init_server();

void notify_all_clients(Server* server, char* msg);