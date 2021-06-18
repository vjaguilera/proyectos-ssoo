#include "server.h"
#include "../structs_monsters/monster.h"
#include "../structs_server/conection.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//#include "../structs_shared/abilities_player.h"

Server* init_server(int socket) {
    Server* server = malloc(sizeof(Server));
    server -> clientes = malloc(sizeof(Jugador) * 5);
    server -> cantidad_clientes = 0;
    server -> socket = socket;
    server -> monster = malloc(sizeof(Monster));
    server -> rounds_without_sudo = 1;
    return server;
}

void notify_all_clients(Server* server, char* msg) {
  // printf("cantidad clients %d\n", server -> cantidad_clientes);
  for (int i = 0; i < server -> cantidad_clientes; i++) {
    if (server -> clientes[i] -> deleted){
      continue;
    } 
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

      sprintf(response, "%s ha ingresado a la partida con la clase %s\n", actual -> nombre, actual -> clase_str);
      if (pos != 0) {
        notify_all_clients(server, response);
      }
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
  server_send_message(info -> socket, 4, "¿Quiere comenzar?");
  while (op ) {
    printf("op %d NOT %d\n", op, info -> not_start);
    if (info -> not_start != 0) {
      printf("Enviar\n");
      msg_code = server_receive_id(info -> socket);
    }
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
  if (server -> monster -> on == 1) {
    monster_clean(server -> monster);
  }
  server -> monster = monster_init(num_monster);
}

// [?] jugadores seria server -> clients
void start_playing(Server* server, Jugador** jugadores){
  // Se asume que recibe una lista de Jugadores 

  // Se instancia el mounstro

  server_send_message(server -> lider -> socket, 7, "Seleccione al monstruo\n1) JagRuz\n2) Ruiz\n3) Ruzalo\n4) Al azar\n");
  int msg_code = server_receive_id(server -> lider -> socket);
  char * client_message = server_receive_payload(server -> lider -> socket);
  server -> num_monster = msg_code;
  set_monster(server, server -> num_monster);
  char msg[40];
  sprintf(msg, "Se ha seleccionado al monstruo: %s", server -> monster -> class_str);
  notify_all_clients(server, msg);

  send_state(server);

  int cantidad_activos = server->cantidad_clientes;
  int rounds = 1;

  // WHILE 
  while (server->monster->current_life > 0 && cantidad_activos > 0){
    server -> ronda_actual = rounds;
    // FOR
    for (int turn = 0; turn < server->cantidad_clientes; turn++){

      if (server -> clientes[turn] -> rendido || server -> clientes[turn] -> deleted){
        continue;
      }

      // Juega cada jugador en orden de llegada
      // El jugadador 0 es el lider
      char currentTurnMessage[50];
      server -> cliente_actual = server -> clientes[turn];
      sprintf(currentTurnMessage, "Es el turno del jugador %s\n", server -> cliente_actual -> nombre);
      notify_all_clients(server, currentTurnMessage);

      printf("Turno de %s round %d \n", server -> cliente_actual -> nombre, rounds);

      // Chequeo!
      // Intoxicacion
      int intoxicated = check_player_intoxicated(server->cliente_actual);
      if (intoxicated) {
        char msg[100];
        sprintf(msg, "El jugador %s ha recibido 400 de dano por intoxicacion\n", server -> cliente_actual->nombre);
        notify_all_clients(server, msg);
      }

      server -> turno_actual = turn;

      server_send_message(server -> cliente_actual -> socket, 8, "Escoge una opcion\n");
      msg_code = server_receive_id(server -> cliente_actual -> socket);
      int option;
      if (msg_code == 1) {
        char * client_message = server_receive_payload(server -> cliente_actual -> socket);
        // Turn corresponde al id del cliente
        printf("El cliente %d dice: %s\n", turn, client_message);
        option = atoi(client_message);
      }

      printf("Opcion elegida es %d\n", option);

      // Manejo de ifs
      if (option == 1){
        // HACKER 
        if (server -> clientes[turn] -> clase_str[0] == 'h'){
          server_send_message(server -> cliente_actual -> socket, 11, "Escoge una habilidad\n");
          msg_code = server_receive_id(server -> cliente_actual -> socket);
          int optionHacker;
          if (msg_code == 1) {
            char * client_message = server_receive_payload(server -> cliente_actual -> socket);
            // Turn corresponde al id del cliente
            printf("El cliente %d dice: %s\n", turn, client_message);
            optionHacker = atoi(client_message);
          }
          // Inyeccion SQL
          if (optionHacker == 1){
            char duplicateMessage[100];
            int numeroJugador = 0;
            // [?] Como acceder a las habilidades de la clase del jugador actual
            char* textoInicial = "Escoge a quien quieres duplicarle el ataque\n";
            strcat(duplicateMessage, textoInicial);
            char playerToDuplicate[50];
            for (int i = 0; i < server -> cantidad_clientes; i++){
              if (server ->clientes[i] -> rendido){
                printf("El jugador rendido es %i: %s", numeroJugador, server->clientes[i]->nombre);
                numeroJugador += 1;
                continue;
              }
              else{
                if (numeroJugador == turn){
                    numeroJugador += 1;
                    continue;
                }
                sprintf(playerToDuplicate, "%i: %s\n", numeroJugador, server->clientes[i]->nombre);
                strcat(duplicateMessage, playerToDuplicate);
                numeroJugador += 1;
              }
            }
            server_send_message(server -> cliente_actual -> socket, 12, duplicateMessage);
            msg_code = server_receive_id(server -> cliente_actual -> socket);

            int optionSQL;
            if (msg_code == 1) {
              char * client_message = server_receive_payload(server -> cliente_actual -> socket);
              // Turn corresponde al id del cliente
              printf("El cliente %d dice: %s\n", turn, client_message);
              optionSQL = atoi(client_message);
              inyeccion_sql_ability(server -> cliente_actual, server->clientes[optionSQL]);
              char mensaje[100];
              sprintf(mensaje, "Usó inyección sql sobre %s\n", server->clientes[optionSQL]->nombre);
              notify_all_clients(server, mensaje);
            }
          } 
          // Ataque DDOS
          else if (optionHacker == 2){
            printf("Entre a ataque ddos\n");
            ataque_ddos_ability(server -> cliente_actual, server -> monster);
            notify_all_clients(server, "Usó Ataque DDOS");
          }
          // Fuerza Bruta
          else if (optionHacker == 3){
            fuerza_bruta_ability(server -> cliente_actual, server -> monster);
            notify_all_clients(server, "Usó Fuerza Bruta");
          }
          else{
            printf("Ingresa una opcion valida");
          }
        }

        // MEDICO
        else if (server -> clientes[turn] -> clase_str[0] == 'm'){
          server_send_message(server -> cliente_actual -> socket, 13, "Escoge una habilidad\n");
          msg_code = server_receive_id(server -> cliente_actual -> socket);
          int optionMedico;
          if (msg_code == 1) {
            char * client_message = server_receive_payload(server -> cliente_actual -> socket);
            // Turn corresponde al id del cliente
            printf("El cliente %d dice: %s\n", turn, client_message);
            optionMedico = atoi(client_message);
          }
          // Curar
          if (optionMedico == 1){
            char cureMessage[100];
            int numeroJugador = 0;
            // [?] Como acceder a las habilidades de la clase del jugador actual
            char* textoInicial = "Escoge a quien quieres curar\n";
            strcat(cureMessage, textoInicial);
            char playerToCure[50];
            for (int i = 0; i < server -> cantidad_clientes; i++){
              if (server ->clientes[i] -> rendido){
                numeroJugador += 1;
                continue;
              }
              else{
                if (numeroJugador == turn){
                    numeroJugador += 1;
                    continue;
                }
                sprintf(playerToCure, "%i: %s\n", numeroJugador, server->clientes[i]->nombre);
                strcat(cureMessage, playerToCure);
                numeroJugador += 1;
              }
            }
            server_send_message(server -> cliente_actual -> socket, 14, cureMessage);
            msg_code = server_receive_id(server -> cliente_actual -> socket);

            int cliente_a_curar;
            if (msg_code == 1) {
              char * client_message = server_receive_payload(server -> cliente_actual -> socket);
              // Turn corresponde al id del cliente
              printf("El cliente %d dice: %s\n", turn, client_message);
              cliente_a_curar = atoi(client_message);
              curar_ability(server -> cliente_actual, server -> clientes[cliente_a_curar]);
              char mensaje[100];
              sprintf(mensaje, "Usó Curar sobre %s\n", server->clientes[cliente_a_curar]->nombre);
              notify_all_clients(server, mensaje);
            }
          } 
          // Destello regenerador
          else if (optionMedico == 2){
            // Asignar target_recuperacion al azar
            int jugador_valido = 1;
            int jugador_recuperado;
            while(jugador_valido){
              jugador_recuperado = (rand() % (server -> cantidad_clientes));
              if (server -> clientes[jugador_recuperado] -> rendido){
                continue;
              }
              else{
                jugador_valido = 0;
              }
            }
            destello_regenerador_ability(server -> cliente_actual, server -> monster, server -> clientes[jugador_recuperado]);
            notify_all_clients(server, "Usó Destello Regenerador\n");
          }
          // Descarga Vital
          else if (optionMedico == 3){
            descarga_vital_ability(server -> cliente_actual, server -> monster);
            notify_all_clients(server, "Usó Descarga Vital\n");
          }
          else{
            printf("Ingresa una opcion valida");
          }
        }
        // CAZADOR
        else if (server -> cliente_actual -> clase_str[0] == 'c'){

          server_send_message(server -> cliente_actual -> socket, 15, "Escoge una habilidad\n");
          msg_code = server_receive_id(server -> cliente_actual -> socket);
          int optionCazador;
          if (msg_code == 1) {
            char * client_message = server_receive_payload(server -> cliente_actual -> socket);
            // Turn corresponde al id del cliente
            printf("El cliente %d dice: %s\n", turn, client_message);
            optionCazador = atoi(client_message);
          }
          // Estocada
          if (optionCazador == 1){
            estocada_ability(server -> cliente_actual, server -> monster);
            notify_all_clients(server, "Usó Estocada\n");
          } 
          // Corte Cruzado
          else if (optionCazador == 2){
            corte_cruzado_ability(server -> cliente_actual, server -> monster);
            notify_all_clients(server, "Usó Corte Cruzado\n");
          }
          // Distraer
          else if (optionCazador == 3){
            distraer_ability(server -> cliente_actual, server -> monster);
            notify_all_clients(server, "Usó Distraer\n");
          }
          else{
            printf("Ingresa una opcion valida");
          }
        }

      }
      else if (option == 2){
        // server -> cantidad_clientes -= 1;
        printf("El jugador %i se rindió\n", turn);
        server -> cliente_actual -> rendido = 1;
        char playerSurrender[60];
        sprintf(playerSurrender, "El jugador %s se rindio\n", server -> cliente_actual -> nombre);
        notify_all_clients(server, playerSurrender);
      }
    }
    // FIN DEL FOR DE JUGADORES


    // Aqui juega el MONSTER
    printf("Es el turno del Monstruo \n");
    printf("El Monstruo escogido es un %s \n", server->monster->class_str);
    printf("El Monstruo tiene %d vida\n", server->monster->current_life);

    // Agregar sangrado
    check_monster_sangrado(server->monster);
    // Verificar si murio con el sangrado
    int monster_bleeded = check_monster_dead(server->monster);

    if (monster_bleeded)
    {
      printf("EL MONSTRUO MURIO DESANGRADO\n");
    } else {
      // Select ability
      int monster_ability = monster_choose_ability(server->monster);
      // Use ability
      if (server -> monster->is_jagruz == 1){
        if (monster_ability == 0){
          // RUZGAR
          // Obtener cliente random
          Jugador* affected_player = monster_choose_random_player(server);
          int damage = ruzgar_hability(server->monster, affected_player);
          printf("[JAGRUZ] - [RUZGAR]\n");
          char msg[100];
          sprintf(msg, "El JagRuz ha utilizado RUZGAR contra %s inflingiendo %d\n", affected_player->nombre, damage);
          notify_all_clients(server, msg);
        } 
        else if (monster_ability == 1){
          // COLETAZO
          int damage = coletazo_hability(server->monster, server->clientes, server->cantidad_clientes);
          printf("[JAGRUZ] - [COLETAZO]\n");
          char msg[100];
          sprintf(msg, "El JagRuz ha utilizado COLETAZO contra TODOS LOS JUGADORES inflingiendo %d\n", damage);
          notify_all_clients(server, msg);
        }
        else{
          printf("Problemas procesando el ataque del monstruo JAGRUZ");
        }
      }
      else if (server -> monster->is_ruiz == 1){
        if (monster_ability == 0){
          // COPY CASE
          // Obtener jugador objetivo random
          Jugador* affected_player = monster_choose_random_player(server);
          // Obtener jugador para copiar
          Jugador* copy_player = monster_choose_random_player(server);
          char habilidadText[30];
          int damage = copycase_hability(server->monster, copy_player, affected_player, habilidadText);
          // TODO castear numero de hjabilidad a su nombre
          printf("[RUIZ] - [COPYCASE]\n");
          char msg[200];
          sprintf(msg, "El Ruiz ha utilizado CASO DE COPIA contra %s inflingiendo %d\nCopio la habilidad %s del jugador %s\n", affected_player->nombre, damage, habilidadText, copy_player->nombre);
          notify_all_clients(server, msg);
        } 
        else if (monster_ability == 1) {
          // Reprobatron
          // Obtener jugador random
          Jugador* affected_player = monster_choose_random_player(server);
          reprobatron_hability(server->monster, affected_player);
          printf("[RUIZ] - [REPROBATRON]\n");
          char msg[100];
          sprintf(msg, "El Ruiz ha utilizado REPROBRATRON contra %s inflingiendo 0\n", affected_player->nombre);
          notify_all_clients(server, msg);
        }
        else if (monster_ability == 2){
          // SUDO RM RF
          int damage = sudormrf_hability(server->monster, server, server->clientes, server->cantidad_clientes);
          printf("[RUIZ] - [SUDO RM RF]\n");
          char msg[100];
          sprintf(msg, "El Ruiz ha utilizado REPROBRATRON contra TODOS inflingiendo %d\n", damage);
          notify_all_clients(server, msg);
        }
        else{
          printf("Problemas procesando el ataque del monstruo\n");
        }
      }
      else if (server -> monster->is_ruzalo == 1){
        if (monster_ability == 0){
          // SALTO
          // Obtener jugador random
          Jugador* affected_player = monster_choose_random_player(server);
          int damage = salto_hability(server->monster, affected_player);
          printf("[RUZALO] - [SALTO]\n");
          char msg[100];
          sprintf(msg, "El Ruzalo ha utilizado SALTO contra %s inflingiendo %d\n", affected_player->nombre, damage);
          notify_all_clients(server, msg);
        } 
        else if (monster_ability == 1){
          // Espina venenosa
          // Obtener jugador random
          Jugador* affected_player = monster_choose_random_player(server);
          int damage = espinavenenosa_hability(server->monster, affected_player);
          printf("[RUZALO] - [ESPINA VENENOSA]\n");
          char msg[100];
          sprintf(msg, "El Ruzalo ha utilizado ESPINA VENENOSA contra %s inflingiendo %d\n", affected_player->nombre, damage);
          notify_all_clients(server, msg);
        }
        else{
          printf("Problemas procesando el ataque del monstruo\n");
        }
      }
    }

    // FIN DEL MONSTER

    // Calculamos nueva cantidad de activos
    int cantidad_activos_aux = 0;
    for (int i = 0; i < server->cantidad_clientes; i++){
      if (server->clientes[i]->rendido || server->clientes[i] -> current_life <= 0){
        continue;
      }
      else{
        cantidad_activos_aux += 1;
      }
    }
    cantidad_activos = cantidad_activos_aux;
    printf("Cantidad activos %d\n", cantidad_activos);
    rounds += 1;
    server->rounds_without_sudo += 1;
    printf("-----------------------------------\n");
    send_state(server);
  }
  // FIN DEL WHILE


  // GANAMOS 
  if (server->monster->current_life <= 0){
    char * winMessage = "\nEl monstruo ha sido derrotado, FELIICITACIONES A LOS JUGADORES\n";  
    notify_all_clients(server, winMessage);
    for (int i = 0; i < server -> cantidad_clientes; i++) {
      if (server -> clientes[i] -> deleted){
        continue;
      } 
      send_loot(server -> clientes[i] -> socket);
    }
  }
  // PERDIMOS :(
  else{
    // [tomas] si pierden la vida, que se dejen como rendido = 1
    char * lossMessage = "\n¡EL MONSTRUO NOS HA DERROTADO!\n";
    notify_all_clients(server, lossMessage);
  }
}

void notify_players(Server* server, char* message){
  // Funcion que recibe un mensaje y lo envia a todos los jugadores
  for (int jugador = 0; jugador < server->cantidad_clientes; jugador++){
    server_send_message(server -> clientes[jugador] -> socket , jugador, message);
  }
}

Jugador* choose_random_player(Server* server) {
  // Retorna un jugador al azar dentro de los que estan inscritos
  int random_num;

  random_num = rand() % server -> cantidad_clientes;
  return server->clientes[random_num];

}

Jugador* monster_choose_random_player(Server *server)
{
  // Retorna un jugador al hacer, si esque no hay una distraccion
  // Si existe una distraccion lo retorna y deja la distraccion en NULL
  if (server->monster->distraido == 1)
  {
    for(int jg=0; jg<server->cantidad_clientes;jg++)
    {
      if (server->clientes[jg]->nombre == server->monster->me_distrajo->nombre)
      {
        server->monster->distraido = 0;
        return server->clientes[jg];
      }
    }
  } else {
    return choose_random_player(server);
  }
  return NULL;

}

int sudormrf_hability(Monster *ruiz, Server *server, Jugador **players, int players_amount)
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
      if (players[jg]->demoralized){
        damage *= 1.5;
      }
        update_player_life(players[jg], -damage); // Update -damage player life
    }

    // Set server rounds to 0
    server->rounds_without_sudo = 0;
    return damage;
}

void fix_clients_pos(Server* server, int total) {
  int c = -1;
  for (int i = 0; i < server -> cantidad_clientes; i++) {
    if (i == total) {
      break;
    }
    if (c != i && server -> clientes[i] -> deleted == 1 && server -> clientes[i + 1] -> deleted == 0) {
      server -> clientes[i] = server -> clientes[i + 1];
      c = i + 1;
    } else if (c == i) {
      while (c < server -> cantidad_clientes - 1) {
        server -> clientes[c] = server -> clientes[c + 1];
        c += 1;
      }
      c = -1;
    }
  }
  for (int i = 0; i < total; i++) {
    printf("Sigue jugando %s\n", server -> clientes[i] -> nombre);
  }
}

void end_listen(Server* server) {
  int total = server -> cantidad_clientes;
  for (int i = 0; i < server -> cantidad_clientes; i++) {
    server_send_message(server -> clientes[i] -> socket, 4, "¿Quieres continuar?");
    int msg_code = server_receive_id(server -> clientes[i] -> socket);

    char * client_message = server_receive_payload(server -> clientes[i] -> socket);
    if (atoi(client_message) == 1) {
      server_send_message(server -> clientes[i] -> socket, 2, "Seleccione su clase");
      msg_code = server_receive_id(server -> clientes[i] -> socket);
      if (msg_code == 1) {
        char * client_message = server_receive_payload(server -> clientes[i] -> socket);
        printf("El cliente %d dice: %s\n", server -> cantidad_clientes, client_message);
        server -> clientes[i] -> num_clase = atoi(client_message);
        set_class(server -> clientes[i], server -> clientes[i] -> num_clase);
        char response[50];
        sprintf(response, "%s ha ingresado a la partida con la clase %s\n",
          server -> clientes[i] -> nombre,
          server -> clientes[i] -> clase_str
        );
        notify_all_clients(server, response);
      }
    } else {
      server -> clientes[i] -> deleted = 1;
      total -= 1;
      server_send_message(server -> clientes[i] -> socket, 16, "¡Gracias por jugar!");
      if (total != 0 && server -> lider -> nombre == server -> clientes[i] -> nombre) {
        server -> lider = server -> clientes[i + 1];
        server_send_message(server -> clientes[i + 1] -> socket, 3, "Ahora eres líder");
      }
    }
  }
  fix_clients_pos(server, total);
  server -> cantidad_clientes = total;
}

void send_state(Server* server) {
  notify_all_clients(server, "\n$$$$$$$$$$$$$ ESTADISTICAS $$$$$$$$$$$$$$$");
  for (int i = 0; i < server -> cantidad_clientes; i++) {
    char message[100];
    sprintf(message, "%s[%s] -> VIDA: %d / %d",
      server -> clientes[i] ->nombre,
      server -> clientes[i] ->clase_str,
      server -> clientes[i] ->current_life,
      server -> clientes[i] ->initial_life
    );
    notify_all_clients(server, message);
  }
  char message[100];
  sprintf(message, "Monstruo[%s] -> VIDA: %d / %d",
    server -> monster -> class_str,
    server -> monster -> current_life,
    server -> monster -> initial_life
  );
  notify_all_clients(server, message);
  notify_all_clients(server, "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
}

struct stat st1 = {0};

void send_loot(int socket) {
  int images_to_send = rand() % 3 + 3;
  int numero;
  for (int i = 0; i < images_to_send; i++) {
    numero = rand() % 10;
    char images_path[40];
    if (stat("src/loot", &st1) == -1) {
      printf("LOOT NO EXISTE\n");
    } else {
      printf("LOOT EXISTE\n");
      sprintf(images_path, "src/loot/loot%d.PNG", numero);
      // https://stackoverflow.com/questions/14002954/c-programming-how-to-read-the-whole-file-contents-into-a-buffer

      FILE *f = fopen(images_path, "r");
      fseek(f, 0, SEEK_END);
      long fsize = ftell(f);
      fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

      char *string = malloc(fsize + 1);
      fread(string, 1, fsize, f);
      fclose(f);

      string[fsize] = 0;

      int inicio = 0;
      int current = 1;
      int total_packages = fsize / 255;
      if (total_packages * 255 < fsize) {
        total_packages += 1;
      }

      // printf("Tamaño %ld, total_packages %d\n", fsize, total_packages);

      int sending = 1;
      int pay_size;
      while (sending == 1) {
        char msg_to_send[259];
        msg_to_send[0] = 17;
        msg_to_send[1] = total_packages;
        msg_to_send[2] = current;
        if (fsize - 255 > 0) {
          pay_size = 255;
          fsize -= 255;
        } else {
          pay_size = fsize;
          sending = 0;
        }
        msg_to_send[3] = pay_size;
        for (int j = 0; j < pay_size; j++) {
          msg_to_send[4 + j] = string[inicio + j];
          // if (current <= 2) {
            // printf("Envia %d\n", string[inicio + j]);
          // }
        }
        inicio += pay_size;
        // printf("Enviar pedazo %d de %d\n", current, total_packages);
        // server_send_message(socket, 17, msg_to_send);
        send(socket, msg_to_send, pay_size + 4, 0);
        current += 1;
      }

    }
    
  }
}
