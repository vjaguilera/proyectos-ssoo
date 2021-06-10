#include <unistd.h>
#include <stdio.h>
#include "../structs/structs_client/conection.h"
#include "../structs/structs_client/comunication.h"
#include "../structs/structs_shared/jugador.h"


int main (int argc, char *argv[]){

    //Se obtiene la ip y el puerto donde está escuchando el servidor (la ip y puerto de este cliente da igual)
    char * IP = "0.0.0.0";
    int PORT = 8080;
    int po, ip = 0;
    if (0) {
        for (int i = 1; i < argc; i++) {
            /* Check for a switch (leading "-"). */
            if (argv[i][0] == '-') {
                /* Use the next character to decide what to do. */
                switch (argv[i][1]) {
                    case 'p':
                        po = 1;
                        PORT = atoi(argv[i + 1]);
                        break;
                    case 'i':
                        ip = 1;
                        IP = argv[i + 1];
                        break;
                }
            }
        }
        if (po == 0 || ip == 0){
            printf("Missing parameters\n");
            return 0;
        } else {
            printf("Conectando servidor en IP: %s, PORT: %d\n", IP, PORT);
            return 0;
        }

    }

    // Se prepara el socket
    int server_socket = prepare_socket(IP, PORT);
    Jugador* jugador = init_jugador();
    set_socket(jugador, server_socket);
    printf("Esperando al servidor...\n Mi socket %d\n", server_socket);
    listen_client(jugador, server_socket);

    clean_jugador(jugador);

    return 0;
}
