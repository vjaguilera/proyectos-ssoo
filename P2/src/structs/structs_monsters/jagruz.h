#pragma once
#include "../structs_shared/jugador.h"

typedef struct jag_ruz
{
    int initial_life;
    int current_life;
} JagRuz;

JagRuz *jagruz_init();

int choose_hability_jagruz();

void jagruz_clean(JagRuz *jagruz);