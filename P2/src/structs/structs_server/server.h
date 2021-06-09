#pragma once
#include <pthread.h>
#include "../structs_shared/jugador.h"
#include "../structs_server/comunication.h"
// #include "../structs/monster.h"

typedef struct server{
    Jugador* lider;
    Jugador** clientes;
    int cantidad_clientes;
    int socket;
    // Monster monster;
} Server;

typedef struct thread_arguments {
  int socket;
  int not_start;
  int espera_cliente;
} ArgumentsCreateThread;

Server* init_server(int socket);

void notify_all_clients(Server* server, char* msg);

void initial_listen(Server* server);

void* leader_start(void *args);