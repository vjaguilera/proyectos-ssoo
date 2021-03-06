#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "../structs/structs_server/comunication.h"
#include "../structs/structs_server/conection.h"
#include "../structs/structs_server/server.h"
#include "../helpers/revert.h"


int main(int argc, char *argv[]){
  // Se define una IP y un puerto
  char * IP = "0.0.0.0";
  int PORT = 8080;
  int po = 0;
  int ip = 0;
  if (1) {
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
          printf("Missing parameters -p or -i\n");
          return 0;
      } else {
          printf("Creando servidor en IP: %s, PORT: %d\n", IP, PORT);
      }

  }

  // Se crea el servidor y se obtienen los sockets de ambos clientes.
  int server_socket = prepare_sockets(IP, PORT);

  Server* server = init_server(server_socket);

  printf("Servidor iniciado\n");
  
  // Se inicializa una estructura propia para guardar los n°s de sockets de los clientes.
  // PlayersInfo * players_info = malloc(sizeof(PlayersInfo));

  ArgumentsCreateThread* thrgs = initial_listen(server);
  
  while (server -> cantidad_clientes > 0) {
    start_playing(server, server -> clientes);
    end_listen(server);
  }
  free(thrgs);
  printf("A limpiar\n");
  server_clean(server);

  return 0;
}

