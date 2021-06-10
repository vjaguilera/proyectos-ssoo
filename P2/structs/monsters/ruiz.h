#pragma once

typedef struct ruiz
{
    int initial_life;
    int current_life;
} Ruiz;

Ruiz *ruiz_init();

void copycase_hability(Ruiz *ruiz, Player *copy_player, Player *obj_player);

void reprobatron_hability(Ruiz *ruiz, Player *player);

void sudormrf_hability(Ruiz *ruiz, Player **players, int players_amount);

int choose_hability();

void ruiz_clean(Ruiz *ruiz);