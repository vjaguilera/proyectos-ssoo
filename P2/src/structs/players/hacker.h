#pragma once
#include "../structs_monsters/monster.h"
#include "../structs_shared/jugador.h"

typedef struct hacker
{
    int initial_life;
    char* clase_str;
} Hacker;

Hacker *hacker_init();

void inyeccion_sql_ability(Jugador *jugador_origen, Jugador *jugador_destino);

void ataque_ddos_ability(Jugador *jugador, Monster *monster);

void fuerza_bruta_ability(Jugador *jugador, Monster *monster);

void hacker_clean(Hacker *hacker);