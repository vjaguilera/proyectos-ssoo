#pragma once
#include "../structs_shared/jugador.h"

typedef struct jag_ruz
{
    int initial_life;
    int current_life;
} JagRuz;

JagRuz *jagruz_init();

void ruzgar_hability(JagRuz *jagruz, Jugador *player);

void coletazo_hability(JagRuz *jagruz, Jugador **players, int players_amount);

int choose_hability_jagruz();

void jagruz_clean(JagRuz *jagruz);