#pragma once
#include "../structs_shared/jugador.h"

typedef struct ruzalo
{
    int initial_life;
    int current_life;
    int salto_active;
} Ruzalo;

Ruzalo *ruzalo_init();

int choose_hability_ruzalo();

void ruzalo_clean(Ruzalo *ruzalo);