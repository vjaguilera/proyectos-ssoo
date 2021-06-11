#pragma once
#include "jugador.h"
#include "../structs_monsters/monster.h"

typedef struct medico
{
    int initial_life;
    int current_life;
    int duplicado;
    char clase_str;
} Medico;

Medico *medico_init();

void curar_ability(Jugador *player_medico, Jugador *receptor);

void destello_regenerador_ability(Jugador *jugador_actual, Monster *monster, Jugador *jugador_recuperar);

void descarga_vital_ability(Jugador *jugador, Monster *monster);

void medico_clean(Medico *medico);