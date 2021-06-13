#include "monster.h"

Monster* monster_init(int identificador) {
    Monster* monster = malloc(sizeof(Monster));
    monster -> current_life = 10; // harcodeada
    monster -> estocadas = 0;
    monster -> me_distrajo = malloc(sizeof(Jugador));
    int num = identificador;
    monster -> salto_active = false;
    if (identificador == 4) {
        num = rand() % 3;
    }
    switch (num)
    {
    case 1:
        printf("Monstruo escogido: JagRuz\n");
        monster -> is_jagruz = 1;
        monster -> is_ruiz = 0;
        monster -> is_ruzalo = 0;
        monster -> jagruz = jagruz_init();
        monster -> initial_life = monster -> jagruz -> initial_life;
        monster -> class_str = "JagRuz";
        monster->class_int = 1;
        break;
    case 2:
        printf("Monstruo escogido: Ruiz\n");
        monster -> is_jagruz = 0;
        monster -> is_ruiz = 1;
        monster -> is_ruzalo = 0;
        monster -> ruiz = ruiz_init();
        monster -> initial_life = monster -> ruiz -> initial_life;
        monster -> class_str = "Ruiz";
        monster->class_int = 2;
        break;
    case 3:
        printf("Monstruo escogido: Ruzalo\n");
        monster -> is_jagruz = 0;
        monster -> is_ruiz = 0;
        monster -> is_ruzalo = 1;
        monster -> ruzalo = ruzalo_init();
        monster -> initial_life = monster -> ruzalo -> initial_life;
        monster -> class_str = "Ruzalo";
        monster->class_int = 3;
        break;
    
    default:
        break;
    }
    return monster;
}

void monster_clean(Monster* monster) {
    free(monster -> me_distrajo);
    free(monster -> jagruz);
    free(monster -> ruiz);
    free(monster -> ruzalo);
    free(monster);
}

int choose_ability(int* options, int* percentage, int options_amount)
{
    float random_num;

    random_num = rand() % 10;
    printf("%f\n", random_num);

    if (options_amount == 1)
    {
        return options[0];
    }

    int current_percent = 0;
    for (int opt=0; opt<options_amount; opt++)
    {
        if (random_num >= current_percent && random_num < percentage[opt] + current_percent)
        {
            return options[opt];
        }
        current_percent += percentage[opt];

    }
    return 0;
}

int monster_choose_ability(Monster* monster)
{
    // Se escoge la habilidad del monstruo con valores entre 0 y 2 dependiendo del tipo
    switch (monster->class_int)
    {
    case 1:
    {
      // JagRuz
      // 0: ruzgar 50%
      // 1: coletazo 50%
      int options[2] = {0, 1};
      int percentage[2] = {5, 5};
      int ability = choose_ability(options, percentage, 2);
      return ability;
    }
    case 2:
    {
      // Ruiz
      // 0: copycase 40%
      // 1: reprobatron 20%
      // 2: sudo rmrf 40%
      int options[3] = {0, 1, 2};
      int percentage[3] = {4, 2, 4};
      int ability = choose_ability(options, percentage, 3);
      return ability;
    }
    case 3:
    {
      // Ruzalo
      // 0: salto 40%
      // 1: espinavenenosa 60%
      if (monster->ruzalo->salto_active){
          return 1;
      }
      int options[2] = {0, 1};
      int percentage[2] = {4, 6};
      int ability = choose_ability(options, percentage, 2);
      return ability;
    }
    default:
        break;
    }
    return 0;
}

int player_choose_ability(Jugador *player)
{
  // Se escoge la habilidad del jugador con valores entre 0 y 2 dependiendo del tipo
  switch (player->num_clase)
  {
  case 1:
  {
    // Cazador
    // 0: estocada 33%
    // 1: corte cruzado 33%
    // 2: distraer 33%
    int options[3] = {0, 1, 2};
    int percentage[3] = {3, 3, 3};
    int ability = choose_ability(options, percentage, 3);
    return ability;
  }
  case 2:
  {
    // Hacker
    // 3: SQL Injection 33%
    // 4: DDOS 33%
    // 5: Brute Force 33%
    int options[3] = {3, 4, 5};
    int percentage[3] = {3, 3, 3};
    int ability = choose_ability(options, percentage, 3);
    return ability;
  }
  case 3:
  {
    // Medico
    // 6: curar 33%
    // 7: destello regenerado 33%
    // 8: descarga vital  33%
    int options[3] = {6, 7, 8};
    int percentage[3] = {3, 3, 3};
    int ability = choose_ability(options, percentage, 3);
    return ability;
  }
  default:
      break;
  }
  return 0;

}

void ruzgar_hability(Monster *jagruz, Jugador *player)
{
    // El JagRuz ocupa sus poderosas garRaz para atacar a un enemigo 1000 puntos de daño"""
    int damage = 1000;
    if (player->demoralized){
        damage *= 1.5;
    }
    update_player_life(player, -damage); // Update -damage player life
}

void coletazo_hability(Monster *jagruz, Jugador **players, int players_amount)
{
    int damage = 500;
    // El JagRuz golpea a todos los objetivos con su cola 500 puntos de daño
    for (int jg = 0; jg < players_amount; jg++)
    {
        if (players[jg]->demoralized){
            damage *= 1.5;
        }
        update_player_life(players[jg], -damage); // Update -damage player life
    }
}

void copycase_hability(Monster *monster, Jugador *copy_player, Jugador *obj_player)
{
    // Ruiz rompe el Código de Honor y copia de forma aleatoria una de las habilidades de
    // algún jugador y la usa contra alguno de los jugadores causando los efectos asociados. Si la habilidad normal-
    // mente involucra ayudar a un jugador aliado, Ruiz recibe el efecto sobre si mismo.
    int ability = player_choose_ability(copy_player);

    // UTILIZAR HABILIDAD
    switch (ability)
    {
    case 0:
    {
      // Cazador - estocada
      estocada_ability_copy(monster->ruiz, obj_player);
      break;
    }
    case 1:
    {
      // Cazador - corte cruzado
      corte_cruzado_ability_copy(monster->ruiz, obj_player);
      break;
    }
    case 2:
    {
      // Cazador - distraer
      distraer_ability_copy(monster->ruiz, obj_player);
      break;
    }
    case 3:
    {
      // Hacker - sql injection
      inyeccion_sql_ability_copy(monster->ruiz);
      break;
    }
    case 4:
    {
      // Hacker - ddos
      ataque_ddos_ability_copy(monster->ruiz, obj_player);
      break;
    }
    case 5:
    {
      // Hacker - brute force
      fuerza_bruta_ability_copy(monster->ruiz, obj_player);
      break;
    }
    case 6:
    {
      // Medico - curar
      curar_ability_copy(monster);
      break;
    }
    case 7:
    {
      // Medico - destello regenerador
      destello_regenerador_ability_copy(monster, obj_player);
      break;
    }
    case 8:
    {
      // Medico - descarga vital
      descarga_vital_ability_copy(monster, obj_player);
      break;
    }
    default:
      break;
    }
    
    // Disminuir turnos duplicados
    if (monster->ruiz->duplicado > 0) {
      monster->ruiz->duplicado -= 1;
    }

}

void reprobatron_hability(Monster *ruiz, Jugador *player)
{
    // Ruiz reprueba instantáneamente a un jugador. El jugador queda con estado repro-
    // bado hasta el fin del siguiente turno de Ruiz. Un jugador con estado reprobado está desmoralizado y recibe
    // 50 % de daño extra de todas las fuentes e inflige 50 % menos de puntos de vida con sus habilidades, ya sea al
    // curar o dañar

    // Champion desmoralization
    player -> demoralized = true;
}

void salto_hability(Monster *ruzalo, Jugador *player)
{
    // El Ruzalos salta sobre su objetivo, cayendo con gran fuerza sobre el, este recibe 1500 puntos de
    // daño. Ruzalos no puede usar esta habilidad 2 o más veces seguidas, es decir, si usa esta habilidad, en su próximo
    // turno no puede usarla
    int damage = 1500;
    if (player->demoralized){
        damage *= 1.5;
    }
    update_player_life(player, -damage); // Update -damage player life
    ruzalo->salto_active = true;
}

void espinavenenosa_hability(Monster *ruzalo, Jugador *player)
{
    // El Ruzalos golpea al objetivo con la espina en su cola intoxicándolo, recibiendo 400
    // puntos de daño cada turno (dura 3 turnos este efecto). Si el objetivo ya está intoxicado, inflije 500 puntos de
    // daño
    int damage = 500;
    // PENDIENTE
    if (player -> intoxicated)
    {
        if (player->demoralized){
            damage *= 1.5;
        }
        // Si el objetivo se encontraba intoxicado, inflingir 500 de dano
        update_player_life(player, -damage); // Update -damage player life
    }
    // Intoxicar jugador
    int intoxication = 400;
    intoxicate_player(player, intoxication);
    ruzalo->salto_active = false;
}

void estocada_ability_copy(Ruiz* ruiz, Jugador *player)
{
  // estocada al jugador. 1000 de daño y dejando sangrado que quita 500 cada ronda, hasta maximo
  // 3 acumulados
  int damage = 1000;
  if (player->demoralized){
      damage *= 1.5;
  }
  if (ruiz->duplicado > 0)
  {
    damage *= 2;
  }
  update_player_life(player, -damage); // Update -damage player life
  if (player->estocadas < 3) {
    player->estocadas += 1;
  }
}

void corte_cruzado_ability_copy(Ruiz* ruiz, Jugador *player)
{
  // Inflige 3000 de dano al jugador
  int damage = 3000;
  if (player->demoralized){
      damage *= 1.5;
  }
  if (ruiz->duplicado > 0)
  {
    damage *= 2;
  }
  update_player_life(player, -damage); // Update -damage player life
}

void distraer_ability_copy(Ruiz* ruiz, Jugador *player)
{
  // TODO
  // Como se hace esta implementacion
}

void inyeccion_sql_ability_copy(Ruiz* ruiz)
{
  ruiz->duplicado +=2;
}

void ataque_ddos_ability_copy(Ruiz* ruiz, Jugador* player)
{
  // Inflinge 1500 de dano al jugador
  int damage = 1500;
  if (player->demoralized){
      damage *= 1.5;
  }
  if (ruiz->duplicado > 0)
  {
    damage *= 2;
  }
  update_player_life(player, -damage); // Update -damage player life
}

void fuerza_bruta_ability_copy(Ruiz* ruiz, Jugador* player)
{
  // TODO
  // Como se hace esta implemetacion
}

void curar_ability_copy(Monster* monster)
{
  monster->current_life += 2000;
  monster->ruiz->current_life += 2000;
}

void destello_regenerador_ability_copy(Monster *monster, Jugador *player)
{
  // inflinge entre 750 y 2000 de dano al jugador
  // se cura con la mitad
  srand(time(0));
  int damage = (rand() % (2500 - 750 + 1)) + 750;
  if (player->demoralized){
      damage *= 1.5;
  }
  if (monster->ruiz->duplicado > 0)
  {
    damage *= 2;
  }
  update_player_life(player, -damage); // Update -damage player life

  // Recuperar mitad de la vida
  int vida_recuperada = (int)damage/2;
  monster->current_life += vida_recuperada;
}

void descarga_vital_ability_copy(Monster *monster, Jugador *player)
{
  int damage = 2 * (monster->initial_life - monster -> current_life);
  if (player->demoralized){
      damage *= 1.5;
  }
  if (monster->ruiz->duplicado > 0)
  {
    damage *= 2;
  }
  update_player_life(player, -damage); // Update -damage player life
}

void check_monster_sangrado(Monster *monster)
{
  if (monster->estocadas > 0)
  {
    monster->current_life -= 500;
    monster->estocadas -= 1;
  }

}

int check_monster_dead(Monster *monster)
{
  return monster->current_life <= 0;
}
