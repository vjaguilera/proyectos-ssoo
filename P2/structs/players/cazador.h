#pragma once

typedef struct cazador
{
    int initial_life; 
    int current_life; // se tienen que ir como atributo del player
    int duplicado;
    char clase_str;
} Cazador;

Cazador *cazador_init();

void estocada_ability(Jugador *jugador, Monster *monster);

void corte_cruzado_ability(Jugador *jugador, Monster *monster);

void distraer_ability(Jugador *jugador, Monster *monster);

void jagruz_clean(Cazador *cazador);