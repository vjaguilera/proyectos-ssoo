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
    int distraido;
    bool salto_active;
    char* class_str;
    int class_int;
    int duplicado;
} Monster;

Monster* monster_init(int identificador);

int monster_choose_ability(Monster* monster);

void ruzgar_hability(Monster *jagruz, Jugador *player);

void coletazo_hability(Monster *jagruz, Jugador **players, int players_amount);

void copycase_hability(Monster *ruiz, Jugador *copy_player, Jugador *obj_player);

void reprobatron_hability(Monster *ruiz, Jugador *player);

void salto_hability(Monster *ruzalo, Jugador *player);

void espinavenenosa_hability(Monster *ruzalo, Jugador *player);

// COPY CASE

void estocada_ability_copy(Monster* ruiz, Jugador *player);

void corte_cruzado_ability_copy(Monster* ruiz, Jugador *player);

void distraer_ability_copy(Monster* ruiz, Jugador *player);

void inyeccion_sql_ability_copy(Monster* ruiz);

void ataque_ddos_ability_copy(Monster* ruiz, Jugador* player);

void fuerza_bruta_ability_copy(Monster* ruiz, Jugador* player);

void curar_ability_copy(Monster* monster);

void destello_regenerador_ability_copy(Monster *monster, Jugador *player);

void descarga_vital_ability_copy(Monster *monster, Jugador *player);

// General

void check_monster_sangrado(Monster *monster);

int check_monster_dead(Monster *monster);