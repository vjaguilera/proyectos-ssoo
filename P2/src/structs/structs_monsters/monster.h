#pragma once
#include "jagruz.h"
#include "ruiz.h"
#include "ruzalo.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct monster
{
    int initial_life;
    int current_life;
    int is_jagruz;
    int is_ruiz;
    int is_ruzalo;
    JagRuz* jagruz;
    Ruiz* ruiz;
    Ruzalo* ruzalo;
    Jugador* me_distrajo;
} Monster;

Monster* monster_init(int identificador);