#pragma once
#include "jugador.h"
#include "../structs_monsters/monster.h"
typedef struct hacker
{
    int initial_life;
    int current_life;
    int duplicado;
    int fuerza_bruta;
    char clase_str;
    char* habilidades[3];
} Hacker;

Hacker *hacker_init();

void inyeccion_sql_ability(Jugador *jugador_origen, Jugador *jugador_destino);

void ataque_ddos_ability(Jugador *jugador, Monster *monster);

void fuerza_bruta_ability(Jugador *jugador, Monster *monster);

void hacker_clean(Hacker *hacker);