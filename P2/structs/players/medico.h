#pragma once

typedef struct medico
{
    int initial_life;
    int current_life;
    int duplicado;
} Medico;

Medico *medico_init();

void curar_ability(Player *player);

void destello_regenerador_ability(Medico *medico, Server *server);

void descarga_vital_ability(Medico *medico, Server *server);

void medico_clean(Medico *medico);