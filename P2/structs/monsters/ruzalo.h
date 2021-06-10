#pragma once

typedef struct ruzalo
{
    int initial_life;
    int current_life;
    int salto_active;
} Ruzalo;

Ruzalo *ruzalo_init();

void salto_hability(Ruzalo *ruzalo, Player *player);

void espinavenenosa_hability(Ruzalo *ruzalo, Player **players, int players_amount);

int choose_hability();

void ruzalo_clean(Ruzalo *ruzalo);