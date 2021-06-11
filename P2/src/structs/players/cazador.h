#pragma once
#include "../structs_monsters/monster.h"
#include "../structs_shared/jugador.h"

typedef struct cazador
{
    int initial_life; 
    char* clase_str;
} Cazador;

Cazador *cazador_init();

void estocada_ability(Jugador *jugador, Monster *monster);

void corte_cruzado_ability(Jugador *jugador, Monster *monster);

void distraer_ability(Jugador *jugador, Monster *monster);

void cazador_clean(Cazador *cazador);