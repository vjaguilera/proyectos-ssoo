#pragma once

typedef struct cazador
{
    int initial_life;
    int current_life;
    int duplicado;
} Cazador;

Cazador *cazador_init();

void estocada_ability(Cazador *cazador, Server *server);

void corte_cruzado_ability(Cazador *cazador, Server *server);

void distraer_ability(Cazador *cazador, Server *server);

void jagruz_clean(Cazador *cazador);