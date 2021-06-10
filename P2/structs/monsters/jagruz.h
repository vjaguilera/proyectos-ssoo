#pragma once

typedef struct jag_ruz
{
    int initial_life;
    int current_life;
} JagRuz;

JagRuz *jagruz_init();

void ruzgar_hability(JagRuz *jagruz, Player *player);

void coletazo_hability(JagRuz *jagruz, Player **players, int players_amount);

int choose_hability();

void jagruz_clean(JagRuz *jagruz);