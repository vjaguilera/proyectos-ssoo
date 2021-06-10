#pragma once
#include "../structs_shared/jugador.h"

typedef struct ruiz
{
    int initial_life;
    int current_life;
} Ruiz;

Ruiz *ruiz_init();

void copycase_hability(Ruiz *ruiz, Jugador *copy_player, Jugador *obj_player);

void reprobatron_hability(Ruiz *ruiz, Jugador *player);

int choose_hability_ruiz();

void ruiz_clean(Ruiz *ruiz);