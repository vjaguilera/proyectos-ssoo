#include "medico.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>



Medico *medico_init(){
    Medico *medico = malloc(sizeof(Medico));
    medico -> initial_life = 3000;
    medico -> current_life = 3000;
    medico -> duplicado = 0;
    return medico;
}


void curar_ability(Player *player){
// da 2000 de vida al jugador elegido
    if (player -> tipo_player -> current_life > player -> tipo_player -> initial_life - 2000){
        player -> tipo_player -> current_life = player -> tipo_player -> initial_life;
    }
    else {
        player -> tipo_player -> current_life += 2000;
    }
}


void destello_regenerador_ability(Medico *medico, Server *server){
// inflinge entre 750 y 2000 de daño al monstruo.
// elige un jugador al azar para darle la mitad del numero anterior como vida
    srand(time(0));
    int cantidad_ataque = (rand() % (2500 - 750 + 1)) + 750;
    if (medico -> duplicado != 0){
        if (server -> monster -> current_life < 2 * cantidad_ataque){
            server -> monster -> current_life = 0;
            // enviar señal de que el monstruo murio ? 
        }
        else {
            server -> monster -> current_life -= 2 * cantidad_ataque;
        }
    }
    else{
        if (server -> monster -> current_life < cantidad_ataque){
            server -> monster -> current_life = 0;
            // enviar señal de que el monstruo murio ? 
        }
        else {
            server -> monster -> current_life -= cantidad_ataque;    
        }
    }

    int cantidad_jugadores = server -> cantidad_clientes;
    int jugador_recuperado = (rand() % cantidad_jugadores + 1);
    int vida_recuperada = (int)cantidad_ataque/2;

    if (server -> clientes[jugador_recuperado] -> current_life + vida_recuperada > server -> clientes[jugador_recuperado] -> initial_life){
        server -> clientes[jugador_recuperado] -> current_life = server -> clientes[jugador_recuperado] -> initial_life;
    }
    else{
        server -> clientes[jugador_recuperado] -> current_life += vida_recuperada;
    }
}



void descarga_vital_ability(Medico *medico, Server *server){
    int dano = 2 * (medico -> initial_life - medico -> current_life);
    if (medico -> duplicado != 0){
        if (server -> monster -> current_life <= 2 * dano){
            server -> monster -> current_life = 0; 
        }
        else{
            server -> monster -> current_life -= 2 * dano;
        }
    }
    else{
        if (server -> monster -> current_life <= dano){
            server -> monster -> current_life = 0; 
        }
        else{
            server -> monster -> current_life -= dano;
        }
    }
}


void medico_clean(Medico *medico){
    free(medico);
};
