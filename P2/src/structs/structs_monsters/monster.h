#pragma once
#include "jagruz.h"
#include "ruiz.h"
#include "ruzalo.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    int estocadas;
    Jugador* me_distrajo;
    bool salto_active;
    char* class_str;
} Monster;

Monster* monster_init(int identificador);

void ruzgar_hability(Monster *jagruz, Jugador *player);

void coletazo_hability(Monster *jagruz, Jugador **players, int players_amount);

void copycase_hability(Monster *ruiz, Jugador *copy_player, Jugador *obj_player);

void reprobatron_hability(Monster *ruiz, Jugador *player);

void salto_hability(Monster *ruzalo, Jugador *player);

void espinavenenosa_hability(Monster *ruzalo, Jugador *player);