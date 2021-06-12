#include "server.h"
#include "../structs_monsters/monster.h"
#include "../structs_server/conection.h"
//#include "../structs_shared/abilities_player.h"

Server* init_server(int socket) {
    Server* server = malloc(sizeof(Server));
    server -> clientes = malloc(sizeof(Jugador) * 5);
    server -> cantidad_clientes = 0;
    server -> socket = socket;
    server -> monster = malloc(sizeof(Monster));
    server -> rounds_without_sudo = 0;
    return server;
}

void notify_all_clients(Server* server, char* msg) {
  printf("cantidad clients %d\n", server -> cantidad_clientes);
    for (int i = 0; i < server -> cantidad_clientes; i++) {
        server_send_message(server -> clientes[i] -> socket, 3, msg);
    }
}

void initial_listen(Server* server) {
  int new_socket;
  int pos;
  int msg_code;
  char response[70];
  pthread_t tid[1];
  ArgumentsCreateThread* thrgs = malloc(sizeof(ArgumentsCreateThread));
  thrgs -> not_start = 1;
  
  char * welcome = "¡Bienvenido Cliente!\nEscriba su nombre:";
  Jugador* actual;
  while (thrgs -> not_start) {
    // Se definen las estructuras para almacenar info sobre los sockets de los clientes
    struct sockaddr_in client1_addr;
    socklen_t addr_size = sizeof(client1_addr);

    fd_set set;
    struct timeval timeout;
    int rv;
    FD_ZERO(&set); /* clear the set */
    FD_SET(server -> socket, &set); /* add our file descriptor to the set */

    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    rv = select(server -> socket + 1, &set, NULL, NULL, &timeout);
    if (rv == -1)
    {
        perror("select"); /* an error occurred */
    }
    else if (rv == 0)
    {
      // printf("timeout occurred (20 second) \n"); /* a timeout occurred */
    }
    else
    {
      new_socket = accept(server -> socket, (struct sockaddr *)&client1_addr, &addr_size);
    

      thrgs -> espera_cliente = 1;
      printf("Enviamos mensaje bienvenida a %d\n", new_socket);
      pos = server -> cantidad_clientes;
      server -> clientes[server -> cantidad_clientes] = init_jugador();
      actual = server -> clientes[server -> cantidad_clientes];
      set_socket(actual, new_socket);
      
      // ask for name
      printf("Mandar a %d\n", actual -> socket);
      server_send_message(actual -> socket, 1, welcome);
      msg_code = server_receive_id(new_socket);
      if (msg_code == 1) {
        char * client_message = server_receive_payload(new_socket);
        printf("El cliente %d dice: %s\n", server -> cantidad_clientes, client_message);
        actual -> nombre = client_message;

      }
      server_send_message(new_socket, 2, "Seleccione su clase");
      msg_code = server_receive_id(new_socket);
      if (msg_code == 1) {
        char * client_message = server_receive_payload(new_socket);
        printf("El cliente %d dice: %s\n", server -> cantidad_clientes, client_message);
        actual -> num_clase = atoi(client_message);
        set_class(actual, actual -> num_clase);
      }
      thrgs -> espera_cliente = 0;

      server -> cantidad_clientes += 1;

      // quiero que esto sea en paralelo
      if (pos == 0) {
        server -> lider = actual;
        thrgs -> socket = new_socket;
        if ( pthread_create(&tid[0], NULL, leader_start, thrgs) != 0 ) {
          printf("Failed to create thread\n");
        }
      }

      sprintf(response, "%s ha ingresado a la partida con la clase %d\n", actual -> nombre, actual -> num_clase);
      notify_all_clients(server, response);
    }

    if (thrgs -> not_start == 0) {
      printf("Termina la elección de clientes\n");
      break;
    }
  }
  printf("Empezará la partida\n");
  notify_all_clients(server, "¡Comenzará la partida!");
}

void * leader_start(void *args) {
  int msg_code;
  ArgumentsCreateThread * info = args;

  int op = 1;
  while (op ) {
    server_send_message(info -> socket, 4, "¿Quiere comenzar?");
    msg_code = server_receive_id(info -> socket);
    if (msg_code == 1) {
      char * client_message = server_receive_payload(info -> socket);
      printf("El cliente lider dice: %s\n", client_message);
      if (info -> espera_cliente == 1) {
        server_send_message(info -> socket, 4, "Hay un cliente registrándose.\n¿Quiere comenzar?");
      } else if (atoi(client_message) == 1) {
        op = 0;
        info -> not_start = 0;
        break;
      } else {
      }
    }
  }
  printf("Termine la función del lider\n");
  pthread_exit(NULL);
}

void set_monster(Server* server, int num_monster) {
  server -> monster = monster_init(num_monster);
}

// [?] jugadores seria server -> clients
void start_playing(Server* server, Jugador** jugadores){
  // Se asume que recibe una lista de Jugadores 

  // Se instancia el mounstro

  server_send_message(server -> lider -> socket, 7, "Seleccione al monstruo\n1) JagRuz\n2) Ruiz\n3) Ruzalo\n4) Al azar\n");
  int msg_code = server_receive_id(server -> lider -> socket);
  if (msg_code == 1) {
    char * client_message = server_receive_payload(server -> lider -> socket);
    server -> num_monster = atoi(client_message);
    set_monster(server, server -> num_monster);
    char msg[40];
    sprintf(msg, "Se ha seleccionado al monstruo: %s", server -> monster -> class_str);
    notify_all_clients(server, msg);
  }

  int cantidad_activos = server->cantidad_clientes;
  int rounds = 1;

  // WHILE 
  while (server->monster->current_life > 0 && cantidad_activos > 0){
    server -> ronda_actual = rounds;
    // FOR
    for (int turn = 0; turn < server->cantidad_clientes; turn++){

      if (server -> clientes[turn] -> rendido){
        continue;
      }

      // Juega cada jugador en orden de llegada
      // El jugadador 0 es el lider
      char currentTurnMessage[50];
      sprintf(currentTurnMessage, "Es el turno del jugador %i\n", turn);
      notify_all_clients(server, currentTurnMessage);

      char * options = "Escoge una opcion\n1) Realizar una habilidad\n2) Rendirse\n";

      printf("Turno de %d round %d \n", turn, rounds);
      server -> turno_actual = turn;
      server -> cliente_actual = server -> clientes[turn];

      // [?] Que es el id que se entrega, el segundo parametro
      server_send_message(server -> cliente_actual -> socket , 7, options);

      // [?] El servidor espera respuesta del cliente actual
      // int opc_code = server_receive_id(server -> cliente_actual -> socket); // definimos como 1 siempre
      // El cliente responde esto
      char * client_response = server_receive_payload(server -> cliente_actual -> socket);
      // Manejo de ifs
      if (client_response[0] == '1'){
        char optionsPlay[100];
        // [?] Como acceder a las habilidades de la clase del jugador actual
        sprintf(optionsPlay, "Escoge tu habilidad\n1)%s\n2)%s\n3)%s\n",
        "Habilidad 1", "Habilidad 2", "Habilidad 3"
        );
          // server -> cliente_actual -> clase -> habilidades[0],
          // server -> cliente_actual -> clase -> habilidades[1],
          // server -> cliente_actual -> clase -> habilidades[2]);
        // [?] Que es el id que se entrega, el segundo parametro
        server_send_message(server -> cliente_actual -> socket , 7, optionsPlay);
        char * _client_response_ability = server_receive_payload(server -> cliente_actual -> socket);
        int client_response_ability = (int) _client_response_ability[0];
        // ver como se comporta la habilidad
        // [ToDo] notify_players de la habilidad realizada (jugador, habilidad, consecuencia, target, turno)
        if (server -> clientes[turn] -> is_hacker == 1){
          if (client_response_ability == 1){
            char optionsSQL[100];
            int opcion = 0;
            // [?] Como acceder a las habilidades de la clase del jugador actual
            for (int i = 0; i < server -> cantidad_clientes; i++){
              if (server ->clientes[i] -> rendido){
                opcion += 1;
                continue;
              }
              else{
                sprintf(optionsSQL, "Escoge a quien quieres duplicarle el ataque\n%i) %s\n", opcion, server->clientes[i]->nombre);
                opcion += 1;
              }
            }
            // [?] Que es el id que se entrega, el segundo parametro
            server_send_message(server -> cliente_actual -> socket , 7, optionsSQL);
            char * client_response_sql = server_receive_payload(server -> cliente_actual -> socket);
            // [?] Se castea asi a int? o con atoi
            int cliente_a_duplicar = (int)client_response_sql[0];
            printf("Que se castea cliente_a_duplicar %d\n", cliente_a_duplicar);
            inyeccion_sql_ability(server -> cliente_actual, server->clientes[cliente_a_duplicar]);
          } 
          else if (client_response_ability == 2){
            ataque_ddos_ability(server -> cliente_actual, server -> monster);
          }
          else if (client_response_ability == 3){
            fuerza_bruta_ability(server -> cliente_actual, server -> monster);
          }
          else{
            printf("Ingresa una opcion valida");
          }
        }
        else if (server -> clientes[turn] -> is_medico == 1){
          if (client_response_ability == 1){
            // A quien quieres curar
          char optionsCure[100];
          int opcion = 0;
          // [?] Como acceder a las habilidades de la clase del jugador actual
          for (int i = 0; i < server -> cantidad_clientes; i++){
            if (server ->clientes[i] -> rendido){
              opcion += 1;
              continue;
            }
            else{
              sprintf(optionsCure, "Escoge a quien quieres curar\n%i) %s\n", opcion, server->clientes[i]->nombre);
              opcion += 1;
            }
          }
          // [?] Que es el id que se entrega, el segundo parametro
          server_send_message(server -> cliente_actual -> socket , 7, optionsCure);
          char * client_response_cure = server_receive_payload(server -> cliente_actual -> socket);
          // [?] Se castea asi a int? o con atoi
          int cliente_a_curar = (int)client_response_cure[0];
          curar_ability(server -> cliente_actual, server -> clientes[cliente_a_curar]);
          } 
          else if (client_response_ability == 2){
            // Asignar target_recuperacion al azar
            int jugador_valido = 1;
            int jugador_recuperado;
            while(jugador_valido){
              jugador_recuperado = (rand() % (server -> cantidad_clientes + 1));
              if (server -> clientes[jugador_recuperado] -> rendido){
                continue;
              }
              else{
                jugador_valido = 0;
              }
            }
            destello_regenerador_ability(server -> cliente_actual, server -> monster, server -> clientes[jugador_recuperado]);
          }
          else if (client_response_ability == 3){
            descarga_vital_ability(server -> cliente_actual, server -> monster);
          }
          else{
            printf("Ingresa una opcion valida");
          }
        }
        else if (server -> cliente_actual -> is_cazador == 1){
          if (client_response_ability == 1){
            estocada_ability(server -> cliente_actual, server -> monster);
          } 
          else if (client_response_ability == 2){
            corte_cruzado_ability(server -> cliente_actual, server -> monster);
          }
          else if (client_response_ability == 3){
            distraer_ability(server -> cliente_actual, server -> monster);
          }
          else{
            printf("Ingresa una opcion valida");
          }
        }

      }
      else if (client_response[0] == '2'){
        // server -> cantidad_clientes -= 1;
        server -> cliente_actual -> rendido = 1;
        char playerSurrender[50];
        sprintf(playerSurrender, "El jugador %i se rindio\n", turn);
        notify_all_clients(server, playerSurrender);
      }
    }
    // FIN DEL FOR DE JUGADORES


    // [vicho] Aqui juega el moster

    // FIN DEL MONSTER

    // Calculamos nueva cantidad de activos
    int cantidad_activos_aux = 0;
    for (int i = 0; i < server->cantidad_clientes; i++){
      if (server->clientes[i]->rendido){
        continue;
      }
      else{
        cantidad_activos_aux += 1;
      }
    }
    int cantidad_activos = cantidad_activos_aux;
    rounds += 1;
  }
  // FIN DEL WHILE

  // GANAMOS 
  if (server->monster->current_life <= 0){
    char * winMessage = "El monstruo ha sido derrotado, FELIICITACIONES A LOS JUGADORES\n";  
    notify_all_clients(server, winMessage);
  }
  // PERDIMOS :(
  else{
    // [tomas] si pierden la vida, que se dejen como rendido = 1
    char * lossMessage = "El monstruo nos ha derrotado\n";
    notify_all_clients(server, lossMessage);
  }
}

void notify_players(Server* server, char* message){
  // Funcion que recibe un mensaje y lo envia a todos los jugadores
  for (int jugador = 0; jugador < server->cantidad_clientes; jugador++){
    server_send_message(server -> clientes[jugador] -> socket , jugador, message);
  }
}

void sudormrf_hability(Monster *ruiz, Server *server, Jugador **players, int players_amount)
{
    // Ruiz borra todas las rondas ocurridas hasta ahora para infligir daño a todos los jugadores.
    // Hace 100·(número de rondas desde el inicio del combate hasta ahora, sin considerar usos anteriores de esta
    // habilidad) de daño a todos los jugadores.

    // Get rounds until now and get damage
    int rounds = server->rounds_without_sudo; // Rounds from the beginning til now without usage of sudo rmrf

    int damage = 100 * rounds;

    // Update players life
    for (int jg = 0; jg < players_amount; jg++)
    {
        update_player_life(players[jg], -damage); // Update -damage player life
    }

    // Set server rounds to 0
    server->ronda_actual = 0;
}