#include "cazador.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>



Cazador *cazador_init()
{
    Cazador *cazador = malloc(sizeof(Cazador));
    cazador -> initial_life = 5000; 
    cazador -> current_life = 5000; // se tienen que ir como atributo del player
    cazador -> clase_str = "cazador";
    // duplicado no estaria aceptando que se pueda cuaduplicar o octuplicar o ... tiene que permitirlo?
    cazador -> duplicado = 0; // cantidad de turnos que le quedan estando duplicado. Si es 0, no se duplica su ataque
    return cazador;
}


void estocada_ability(Jugador *jugador, Monster *monster){
// estocada al mounstruo. 1000 de daño y dejando sangrado que quita 500 cada ronda, hasta maximo
// 3 acumulados
    if (jugador -> duplicado != 0){
        if (monster -> current_life > 2000){
            monster -> current_life -= 2000;
            if (monster -> estocadas < 3){
                monster -> estocadas += 1 ; // agrega una estocada al monstruo, revisar el quite de vida en monstruo
            }   
        }
        else {
            monster -> current_life = 0; // avisar que la vida le llega a 0 ? 
        }
        if (monster -> current_life > 1000){
            monster -> current_life -= 1000;
            if (monster -> estocadas < 3){
                monster -> estocadas += 1 ; // agrega una estocada al monstruo
            }   
        }
        else {
            monster -> current_life = 0; // avisar que la vida le llega a 0 ?    
    }
}


void corte_cruzado_ability(Jugador *jugador, Monster *monster){
// inflinge 3000 de daño al monstruo
    if (jugador -> duplicado != 0){
        if (monster -> current_life > 6000){
            monster -> current_life -= 6000;   
        }
        else {
            monster -> current_life = 0; // avisar que la vida le llega a 0 ? 
        }
    }
    else {
        if (monster -> current_life > 3000){
            monster -> current_life -= 3000;   
        }
        else {
            monster -> current_life = 0; // avisar que la vida le llega a 0 ? 
        
    }
}


void distraer_ability(Jugador *jugador, Monster *Monster){
    mosnter -> me_distrajo = jugador;
}


void cazador_clean(Cazador *cazador)
{
    free(cazador);
};