#include <unistd.h>
#include <stdio.h>
#include "../structs/structs_client/conection.h"
#include "../structs/structs_client/comunication.h"

char * get_input(){
    char * response = malloc(20);
    int pos=0;
    while (1){
        char c = getchar();
        if (c == '\n') break;
        response[pos] = c;
        pos++;
    }
    response[pos] = '\0';
    return response;
}


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
    printf("%d server socket\n", server_socket);

    // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto
    while (1){
        int msg_code = client_receive_id(server_socket);
        printf("Msg receive client: %d\n", msg_code);

        if (msg_code == 1) { //Recibimos un mensaje del servidor
            char * message = client_receive_payload(server_socket);
            printf("El servidor dice: %s\n", message);
            free(message);

            printf("¿Qué desea hacer?\n   1)Enviar mensaje al servidor\n   2)Enviar mensaje al otro cliente\n");
            int option = getchar() - '0';
            getchar(); //Para capturar el "enter" que queda en el buffer de entrada stdin

            printf("Ingrese su mensaje: ");
            char * response = get_input();

            client_send_message(server_socket, option, response);
        }

        if (msg_code == 2) { //Recibimos un mensaje que proviene del otro cliente
            char * message = client_receive_payload(server_socket);
            printf("El otro cliente dice: %s\n", message);
            free(message);

            printf("¿Qué desea hacer?\n   1)Enviar mensaje al servidor\n   2)Enviar mensaje al otro cliente\n");
            int option = getchar() - '0';
            getchar(); //Para capturar el "enter" que queda en el buffer de entrada stdin

            printf("Ingrese su mensaje: ");
            char * response = get_input();

            client_send_message(server_socket, option, response);
        }

        if (msg_code == 0) {
            break;
        }
        printf("------------------\n");
    }

    // Se cierra el socket
    if (server_socket != 3) {
        close(server_socket);
    }
    close(server_socket);

    return 0;
}
