#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../structs_client/comunication.h"
#include "../structs_client/conection.h"
#include "hacker.h"
#include "medico.h"
#include "cazador.h"

typedef struct jugador{
  int socket;
  char* nombre;
  int num_clase;
  int clase; // Deberia ser un enum PENDIENTE
  int current_life;
  int is_cazador;
  int is_medico;
  int is_hacker;
  Cazador* cazador;
  Medico* medico;
  Hacker* hacker;
  int mi_turno; // 0 no 1 si
  int rendido; // 0 no rendido, 1 rendido
  int duplicado;
  int fuerza_bruta;
} Jugador;

Jugador* init_jugador(int identificador); // crea al jugador

void set_socket(Jugador* jugador, int socket); // define el socket

void set_name(Jugador* jugador, char* name); // define su nombre

void set_class(Jugador* jugador, int num_clase); // define su clase

void listen_client(Jugador* jugador, int socket); // acá escucha los mensajes y los recibe

void send_msg(Jugador* jugador, int option, char* msg); // acá envia los mensajes

void show_menu(char* text, int option); // muestra el menu

int pick_option(); // elige la opcion y envia el número

void clean_jugador(Jugador* jugador); // free

void update_player_life(Jugador* jugador, int damage);

void intoxicate_player(Jugador* jugador, int intoxication);