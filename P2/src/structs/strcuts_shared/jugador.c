#include "jugador.h"
#include "../structs_client/comunication.h"
#include "../structs_client/conection.h"

Jugador* init_jugador(char* ip, int port) {
    Jugador* jugador = malloc(sizeof(Jugador));
    int server_socket = prepare_socket(ip, port);
    jugador -> socket = server_socket;
    jugador -> mi_turno = 0;
    return jugador;
};

void set_name(Jugador* jugador, char* name) {
    jugador -> nombre = name;
}; 

void set_class(Jugador* jugador, int num_clase) {
    jugador -> num_clase = num_clase;
    jugador -> clase = 0; // deberia ser de enum
}

void listen_server(Jugador* jugador) {
    while (1){

        int msg_code = client_receive_id(jugador -> socket);
        printf("Msg receive client: %d\n", msg_code);

        if (msg_code != 0) {
            char * message = client_receive_payload(jugador -> socket);
            printf("El servidor dice: %s\n", message);
            free(message);
        } else {
            break;
        }

        printf("------------------\n");
    }
}

void send_msg(Jugador* jugador, char* msg) {

}

void show_menu(char* menu) {
    
}

int pick_option(Jugador* jugador) {
    return 0;
}