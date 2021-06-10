#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "../structs_client/comunication.h"
#include "../structs_client/conection.h"


typedef struct player{
  int socket;
  char* nombre;
  int num_clase;
  int clase; // Deberia ser un enum
  int mi_turno; // 0 no 1 si
  int rendido;
} Jugador;

Jugador* init_jugador(); // crea al jugador

void set_socket(Jugador* jugador, int socket); // define el socket

void set_name(Jugador* jugador, char* name); // define su nombre

void set_class(Jugador* jugador, int num_clase); // define su clase

void listen_client(Jugador* jugador, int socket); // acá escucha los mensajes y los recibe

void send_msg(Jugador* jugador, int option, char* msg); // acá envia los mensajes

void show_menu(char* text, int option); // muestra el menu

int pick_option(); // elige la opcion y envia el número

void clean_jugador(Jugador* jugador); // free