#pragma once
#include <pthread.h>
#include "../structs_shared/jugador.h"
#include "../structs_server/comunication.h"
#include "../structs_monsters/monster.h"
#include "../players/medico.h"
#include "../players/hacker.h"
#include "../players/cazador.h"

typedef struct server{
    Jugador* lider;
    Jugador** clientes;
    int cantidad_clientes;
    int socket;
    Monster* monster;
    int num_monster;
    int turno_actual;
    Jugador* cliente_actual;
    int ronda_actual;
    int active_match;
    int rounds_without_sudo;
    int cant_initial;
    int has_monster;
} Server;

typedef struct thread_arguments {
  int socket;
  int not_start;
  int espera_cliente;
} ArgumentsCreateThread;

Server* init_server(int socket);

void notify_all_clients(Server* server, char* msg);

ArgumentsCreateThread* initial_listen(Server* server);

void start_playing(Server* server, Jugador** jugadores);

void* leader_start(void *args);

void notify_players(Server* server, char* message);

Jugador* choose_random_player(Server* server);

Jugador* monster_choose_random_player(Server* server);

void end_listen(Server* server);

void send_state(Server* server);

void server_clean(Server *server);


// ----------- Monsters actions

int sudormrf_hability(Monster *ruiz, Server *server, Jugador **players, int players_amount);

void send_loot(int socket);