#pragma once

typedef struct hacker
{
    int initial_life;
    int current_life;
    int duplicado;
    int fuerza_bruta;
} Hacker;

Hacker *hacker_init();

void inyeccion_sql_ability(Player *player);

void ataque_ddos_ability(Hacker *hacker, Server *server);

void fuerza_bruta_ability(Hacker *hacker, Server *server);

void hacker_clean(Hacker *hacker);