#pragma once
#include "../structs_shared/jugador.h"

typedef struct ruiz
{
    int initial_life;
    int current_life;
} Ruiz;

Ruiz *ruiz_init();

int choose_hability_ruiz();

void ruiz_clean(Ruiz *ruiz);