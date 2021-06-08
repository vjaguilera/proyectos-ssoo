#include "cazador.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>



Cazador *cazador_init()
{
    Cazador *cazador = malloc(sizeof(Cazador));
    cazador -> initial_life = 5000;
    cazador -> current_life = 5000;
    // duplicado no estaria aceptando que se pueda cuaduplicar o octuplicar o ... tiene que permitirlo?
    cazador -> duplicado = 0; // cantidad de turnos que le quedan estando duplicado. Si es 0, no se duplica su ataque
    return cazador;
}


void estocada_ability(Cazador *cazador, Server *server){
// estocada al mounstruo. 1000 de daño y dejando sangrado que quita 500 cada ronda, hasta maximo
// 3 acumulados
    if (cazador -> duplicado != 0){
        if (server -> monster -> current_life > 2000){
            server -> monster -> current_life -= 2000;
            if (server -> monster -> estocadas < 3){
                server -> monster -> estocadas += 1 ; // agrega una estocada al monstruo, revisar el quite de vida en monstruo
            }   
        }
        else {
            server -> monster -> current_life = 0; // avisar que la vida le llega a 0 ? 
        }
    }
    else{
        if (server -> monster -> current_life > 1000){
            server -> monster -> current_life -= 1000;
            if (server -> monster -> estocadas < 3){
                server -> monster -> estocadas += 1 ; // agrega una estocada al monstruo
            }   
        }
        else {
            server -> monster -> current_life = 0; // avisar que la vida le llega a 0 ? 
        
    }

}


void corte_cruzado_ability(Cazador *cazador, Server *server){
// inflinge 3000 de daño al monstruo
    if (cazador -> duplicado != 0){
        if (server -> monster -> current_life > 6000){
            server -> monster -> current_life -= 6000;   
        }
        else {
            server -> monster -> current_life = 0; // avisar que la vida le llega a 0 ? 
        }
    }
    else {
        if (server -> monster -> current_life > 3000){
            server -> monster -> current_life -= 3000;   
        }
        else {
            server -> monster -> current_life = 0; // avisar que la vida le llega a 0 ? 
        
    }
}


void distraer_ability(Cazador *cazador, Server *server){
    server -> me_distrajo = cazador;
}


void cazador_clean(Cazador *cazador)
{
    free(cazador);
};