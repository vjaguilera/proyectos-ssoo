#pragma once
#include <stdio.h>
#include <stdlib.h>


typedef struct player{
  int socket;
  char* nombre;
  int num_clase;
  int clase; // Deberia ser un enum
  int mi_turno; // 0 no 1 si
} Jugador;

Jugador* init_jugador(char* ip, int port); // crea al jugador

void set_name(Jugador* jugador, char* name); // define su nombre

void set_class(Jugador* jugador, int num_clase); // define su clase

void listen_server(Jugador* jugador); // acá escucha los mensajes y los recibe

void send_msg(Jugador* jugador, char* msg); // acá escucha los mensajes y los recibe

void show_menu(char* menu); // muestra el menu

int pick_option(Jugador* jugador); // elige la opcion y envia el número