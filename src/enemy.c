#include "config.h"

#include "allegro.h"

#include "globvars.h"

#include "stuff.h"
#include "ebullet.h"

#include "sound.h"

#include "palette.h"
#include "cloud.h"
#include <math.h>
#include "stuff.h"
#include "level.h"

#define DIFF - arena.difficulty *

int create_enemy(int type);
void destroy_enemy(int e);
void enemy_explode(int e);
void move_enemy(int e);
void enemy_fire(int e);
int attack_angle(int e);
void move_dancer(int e);
void move_stroller2(int e);
void move_diver(int e);
void move_dswooper(int e);
void move_stopper(int e);
void move_cruiser(int e);
void move_dart_stopper(int e);
void move_drifter(int e);
void move_walker(int e);
void move_boss(int e);
void move_boss2_1_dead(int e);
void move_darter(int e);

void run_boss1_1(int e);
void boss1_1_fire1(int e, int x, int y, int a);
void boss1_1_fire2(int e, int x, int y, int a);

void run_boss1_2(int e);
void boss1_2_fire1(int e, int x, int y, int a);
void boss1_2_fire2(int e, int x, int y, int a);

void run_boss1_3(int e);
void boss1_3_fire1(int e, int x, int y, int a);
void boss1_3_fire2(int e);

void run_boss2_1(int e);
void boss2_1_fire1(int e, int x, int y, int a);
void boss2_1_fire2(int e, int x, int y, int a);
void boss2_1_fire3(int e, int x, int y, int a);
void boss2_1_fire4(int e, int x, int y, int a);

void run_boss2_2(int e);
void boss2_2_fire1(int e);
void boss2_2_fire2(int e);
void boss2_2_fire3(int e);
void boss2_2_fire4(int e);

void run_boss3_1(int e);
void boss3_1_fire1(int e, int x, int y, int angle);
void boss3_1_fire2(int e, int x, int y, int angle);
void boss3_1_fire3(int e, int x, int y, int angle);

void run_boss3_2(int e);
void boss3_2_fire1(int e, int x, int y, int angle);
void boss3_2_fire2(int e, int x, int y, int angle);
void boss3_2_fire3(int e, int x, int y, int angle);

void run_boss3_0(int e);
//void boss3_0_fire1(int e, int x, int y, int angle);

int get_e_angle(int e, int x, int y);
void drag_enemy(int e, int drag);
void drag_enemy_y(int e, int drag);
int hurt_enemy(int e, int dam, int source);
void register_destroyed(int e);
void allocate_xp(int e);
void gain_score(int s);
void part_explode(int e);
void launch_part(int pe, int index, int se, int xs, int ys);
void parts_must_explode(int e);
void enemy_pulse(int e, int p_event);
void enemy_pulse_slow(int e, int p_event);
void set_wpixels(void);

void delay_cloud(int x, int y, int col, int delay, int size);


void aim_at_target(int e, int x, int y, int tx, int ty, int turn, int aiming);
void track_target(int e, int x, int y, int angle, int turn);
void track_target_zero(int e, int x, int y, int angle, int turn);

int next_enemy;



void init_enemies(void)
{

 int e;

  for (e = 0; e < NO_ENEMIES; e ++)
  {
   enemy[e].type = ENEMY_NONE;
  }

 next_enemy = 5;
}

/*
Tries to create an enemy, and returns its index if successful (or -1 if not).
The calling function can use that index to set the enemy's properties.
*/
int create_enemy(int type)
{
  int e;

  for (e = 0; e < NO_ENEMIES + 1; e ++)
  {
    if (e == NO_ENEMIES)
     return -1;
    if (enemy[e].type == ENEMY_NONE)
     break;
  }

 enemy[e].type = type;
 enemy[e].persist = 0;
 enemy[e].recycle = 40;
 enemy[e].burst = 0;
 enemy[e].hp = eclass[type].max_hp;
// enemy[e].pulse1 = 0;
// enemy[e].pulse2 = 0;
 enemy[e].frame = 0;
 enemy[e].whurt [0] = 0;
 enemy[e].whurt [1] = 0;
 enemy[e].whurt [2] = 0;
 enemy[e].parent = -1;
 enemy[e].status = 0;
// enemy[e].angle_turning_count = 2;
// enemy[e].angle1_turning_count = 2;
 enemy[e].escorting = -1;
 int i;
 for (i = 0; i < NO_PARTS; i ++)
 {
  enemy[e].part [i] = -1;
 }
 for (i = 0; i < AIMS; i ++)
 {
  enemy[e].aim [i] = ANGLE_4;
 }

 arena.max_stage_score += eclass[type].score;

 return e;

}



void run_enemies(void)
{

 int e;
/*
// maybe we want to create an enemy!
 next_enemy --;
 if (next_enemy <= 0)
 {
  e = create_enemy(ENEMY_EXAMPLE);
  if (e != -1)
  {
   enemy[e].x = grand(600000) + 20000;
   enemy[e].y = grand(440000) + 20000;
   enemy[e].x_speed = grand(5000) - 2500;
   enemy[e].y_speed = grand(5000) - 2500;
  }
  // for now we'll rely on create_enemy failing if there are too many enemies.
  next_enemy = 500;
 }
*/
// now let's run through all enemies and make them work:
 for (e = 0; e < NO_ENEMIES; e ++)
 {
  if (enemy[e].type == ENEMY_NONE)
   continue;

   switch(eclass[enemy [e].type].ai)
   {
    case AI_SWOOPER:
    case AI_HOOK:
    case AI_STROLLER:
         move_dancer(e);
         break;
    case AI_STROLLER2:
         move_stroller2(e);
         break;
    case AI_DSWOOPER:
         move_dswooper(e);
         break;
    case AI_DARTER:
         move_darter(e);
         break;
    case AI_STOPPER:
         move_stopper(e);
         break;
    case AI_DIVER:
         move_diver(e);
         break;
    case AI_DART_STOPPER:
         move_dart_stopper(e);
         break;
    case AI_DRIFTER:
         move_drifter(e);
         break;
    case AI_CRUISER:
         move_cruiser(e);
         break;
    case AI_BOSS:
         move_boss(e);
         break;
    case AI_BOSS2_1_DEAD:
         move_boss2_1_dead(e);
         break;
   }

    if (enemy[e].persist == 0 &&
           (enemy[e].x <= (eclass[enemy[e].type].xsize * -1) - 50000
         || enemy[e].y <= (eclass[enemy[e].type].ysize * -1) - 50000
         || (enemy[e].x >= 650000 + eclass[enemy[e].type].xsize)
         || enemy[e].y >= 650000 + eclass[enemy[e].type].ysize))
       {
        register_destroyed(e);
        destroy_enemy(e);
       }

/*
  switch(enemy[e].type)
  {
   case ENEMY_EXAMPLE:
    enemy[e].recycle --;
    if (enemy[e].recycle <= 0)
    {
     enemy_fire(e);
     enemy[e].recycle = 20 + grand(5);
    }
    move_enemy(e);
    break;


  }
*/
 }

}

void move_boss(int e)
{

 if (boss.hp_bar_size < 100)
  boss.hp_bar_size ++;

 switch(enemy[e].type)
 {
  case ENEMY_BOSS1_1:
   run_boss1_1(e);
   break;
  case ENEMY_BOSS1_2:
   run_boss1_2(e);
   break;
  case ENEMY_BOSS1_3:
   run_boss1_3(e);
   break;
  case ENEMY_BOSS2_1:
   run_boss2_1(e);
   break;
  case ENEMY_BOSS2_2:
   run_boss2_2(e);
   break;
  case ENEMY_BOSS3_1:
   run_boss3_1(e);
   break;
  case ENEMY_BOSS3_2:
   run_boss3_2(e);
   break;
  case ENEMY_BOSS3_0:
   run_boss3_0(e);
   break;
/*  case ENEMY_BOSS2:
   run_boss2(e);
   break;
  case ENEMY_BOSS3:
   run_boss3(e);
   break;
  case ENEMY_BOSS4:
   run_boss4(e);
   break;
*/
 }

}


void move_boss2_1_dead(int e)
{

 drag_enemy(e, 1010);
 move_enemy(e);

 enemy[e].phase --;

 enemy[e].y_speed += 50;

 if (enemy[e].phase < 0)
 {
  if (enemy[e].phase < 0)
  {
   enemy[e].status += 280;
   enemy[e].burst -= enemy[e].status;
  }
 }

 quick_cloud(CLOUD_CIRCLE2, enemy[e].x + grand(170000) - grand(170000), enemy[e].y + grand(150000) - grand(150000), enemy[e].x_speed + grand(1000) - grand(1000), enemy[e].y_speed + grand(1000) - grand(1000), 30 + grand(30), CLOUD_COL_3, 960);
 if ((arena.counter & 7) == 7 && grand(4) == 0)
    play_effectwfvx(WAV_BANG1, 600 + grand(200), 180, enemy[e].x + grand(170000) - grand(170000));

 if (enemy[e].y > 800000 && enemy[e].y + enemy[e].burst < -200000)
 {
  register_destroyed(e);
  destroy_enemy(e);
 }

}


/*

*************************************************************

Boss - stage 1 part 1

*************************************************************

*/



void run_boss1_1(int e)
{

 boss.phase_count --;
 boss.recycle --;
 if (boss.recycle < 0)
  boss.recycle = 0;
 boss.recycle2 --;
 if (boss.recycle2 < 0)
  boss.recycle2 = 0;

 if (boss.phase_count <= 0)
 {
  switch(boss.phase)
  {
   case 0: // entering
    boss.phase = 2;
    boss.phase_count = 150 + grand(200);
    enemy[e].x_target [0] = RAND_X_IN;
    enemy[e].y_target [0] = 120000 + grand(180000);
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    break;
   case 1: // 1, 2 and 3 are attack phases
   case 2:
   case 3:
   case 4:
    boss.phase = 1 + grand(4);
    boss.phase_count = 150 + grand(200);
    enemy[e].x_target [0] = RAND_X_IN;
    enemy[e].y_target [0] = 120000 + grand(180000);
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
//    enemy[e].x_speed = 0;
//    enemy[e].y_speed = 0;
    break;
  }


 }

   aim_at_target(e, enemy[e].x - 76000, enemy[e].y - 11000, player.x, player.y, 5, 0);
   aim_at_target(e, enemy[e].x + 76000, enemy[e].y - 11000, player.x, player.y, 5, 1);

//   quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 53000, enemy[e].y + 50000, enemy[e].x_speed, enemy[e].y_speed + 4000 + grand(4000), 15 + grand(10), CLOUD_COL_1, 980);
//   quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 53000, enemy[e].y + 50000, enemy[e].x_speed, enemy[e].y_speed + 4000 + grand(4000), 15 + grand(10), CLOUD_COL_1, 980);


 switch(boss.phase)
 {
//  case 0: // entering;
//   break;
  case 1: // attack 1  - both red
   if (boss.recycle <= 0)
   {
    boss1_1_fire1(e, enemy[e].x - 76000, enemy[e].y - 11000, enemy[e].aim [0] + grand(24) - grand(24));
    boss.recycle = 4 + coin() DIFF 1;
   }
   if (boss.recycle2 <= 0)
   {
    boss1_1_fire1(e, enemy[e].x + 76000, enemy[e].y - 11000, enemy[e].aim [1] + grand(24) - grand(24));
    boss.recycle2 = 4 + coin() DIFF 1;
   }
   break;
  case 2: // attack 2 - both blue
   if (boss.recycle <= 0)
   {
    boss1_1_fire2(e, enemy[e].x - 76000, enemy[e].y - 11000, enemy[e].aim [0]);
    boss.recycle = 30 DIFF 7;
   }
   if (boss.recycle2 <= 0)
   {
    boss1_1_fire2(e, enemy[e].x + 76000, enemy[e].y - 11000, enemy[e].aim [1]);
    boss.recycle2 = 30 DIFF 7;
   }
   break;
  case 3: // attack 3 - red left, right blue
   if (boss.recycle <= 0)
   {
    boss1_1_fire1(e, enemy[e].x - 76000, enemy[e].y - 11000, enemy[e].aim [0] + grand(24) - grand(24));
    boss.recycle = 4 + coin() DIFF 1;
   }
   if (boss.recycle2 <= 0)
   {
    boss1_1_fire2(e, enemy[e].x + 76000, enemy[e].y - 11000, enemy[e].aim [1]);
    boss.recycle2 = 30 DIFF 7;
   }
   break;
  case 4: // attack 4 - reverse 3
   if (boss.recycle <= 0)
   {
    boss1_1_fire2(e, enemy[e].x - 76000, enemy[e].y - 11000, enemy[e].aim [0]);
    boss.recycle = 30 DIFF 7;
   }
   if (boss.recycle2 <= 0)
   {
    boss1_1_fire1(e, enemy[e].x + 76000, enemy[e].y - 11000, enemy[e].aim [1] + grand(24) - grand(24));
    boss.recycle2 = 4 + coin() DIFF 1;
   }
   break;

 }

 if (boss.phase != 0)
 {
   drag_enemy(e, 990);
//    if (arena.counter
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    enemy[e].x_speed += xpart(enemy[e].angle, 110);
    enemy[e].y_speed += ypart(enemy[e].angle, 110);
    if (enemy[e].x > enemy[e].x_target [0] - 20000
     && enemy[e].x < enemy[e].x_target [0] + 20000
     && enemy[e].y > enemy[e].y_target [0] - 20000
     && enemy[e].y < enemy[e].y_target [0] + 20000)
     {
      enemy[e].x_target [0] = RAND_X_IN;
      enemy[e].y_target [0] = 120000 + grand(180000);
      enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
//      enemy[e].x_speed = 0;
//      enemy[e].y_speed = 0;
     }
 }

    move_enemy(e);

}



void boss1_1_fire1(int e, int x, int y, int a)
{

         int b = create_ebullet(EBULLET_SHOT);
         if (b != -1)
         {
          ebullet[b].x = x;
          ebullet[b].y = y;
          ebullet[b].x_speed = enemy[e].x_speed + xpart(a, 4000);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(a, 4000);
          ebullet[b].angle = radians_to_angle(atan2(ebullet[b].y_speed, ebullet[b].x_speed));
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_1, 930);
          ebullet[b].status = 6 + grand (5);
          play_effectwfvx(WAV_CHIRP, 2000 + grand(200), 120, enemy[e].x);
         }

}


void boss1_1_fire2(int e, int x, int y, int a)
{

    int speed, angle, b, i;

         play_effectwfvx(WAV_FIRE, 1100 + grand(200), 140, x);
         b = create_ebullet(EBULLET_SHOT2);
         if (b != -1)
         {
          ebullet[b].x = x;
          ebullet[b].y = y;
          ebullet[b].x_speed = enemy[e].x_speed + xpart(a, 9500 + arena.difficulty * 500);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(a, 9500 + arena.difficulty * 500);
          ebullet[b].angle = radians_to_angle(atan2(ebullet[b].y_speed, ebullet[b].x_speed));
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_3;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 30 + grand(10), CLOUD_COL_3, 900);
          for (i = 0; i < 4; i ++)
          {
           angle = ebullet[b].angle + ANGLE_4 + grand(100);
           speed = 3000 + grand(1000);
           quick_cloud(CLOUD_FCIRCLE4, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed + xpart(angle, speed), ebullet[b].y_speed + ypart(angle, speed), 30 + grand(10), CLOUD_COL_3, 900);
           angle = ebullet[b].angle - ANGLE_4 - grand(100);
           speed = 3000 + grand(1000);
           quick_cloud(CLOUD_FCIRCLE4, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed + xpart(angle, speed), ebullet[b].y_speed + ypart(angle, speed), 30 + grand(10), CLOUD_COL_3, 900);
          }
         }

}






/*

*************************************************************

Boss - stage 1 part 2

*************************************************************

*/





void run_boss1_2(int e)
{

 int angle, i, b, speed, angle2, angle3;

 boss.phase_count --;

 boss.recycle --;
 if (boss.recycle < 0)
  boss.recycle = 0;

 boss.recycle2 --;
 if (boss.recycle2 < 0)
  boss.recycle2 = 0;

 boss.recycle3 --;
 if (boss.recycle3 < 0)
  boss.recycle3 = 0;

 if (boss.phase_count <= 0)
 {
  switch(boss.phase)
  {
   case 0: // entering
    boss.phase = 2;
    boss.phase_count = 150 + grand(200);
    enemy[e].x_target [0] = RAND_X_IN;
    enemy[e].y_target [0] = 120000 + grand(180000);
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    break;
   case 1: // 1, 2 and 3 are attack phases
   case 2:
   case 3:
   case 4:
    boss.phase = 1 + grand(4);
    boss.phase_count = 150 + grand(200);
    enemy[e].x_target [0] = RAND_X_IN;
    enemy[e].y_target [0] = 120000 + grand(180000);
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
//    enemy[e].x_speed = 0;
//    enemy[e].y_speed = 0;
    break;
  }


 }

   aim_at_target(e, enemy[e].x - 76000, enemy[e].y - 11000, player.x, player.y, 7, 0);
   aim_at_target(e, enemy[e].x + 76000, enemy[e].y - 11000, player.x, player.y, 7, 1);

//   quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 53000, enemy[e].y + 50000, enemy[e].x_speed, enemy[e].y_speed + 4000 + grand(4000), 15 + grand(10), CLOUD_COL_1, 980);
//   quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 53000, enemy[e].y + 50000, enemy[e].x_speed, enemy[e].y_speed + 4000 + grand(4000), 15 + grand(10), CLOUD_COL_1, 980);

 if (boss.phase > 0)
 {
  if (boss.recycle3 <= 30)
   drag_enemy(e, 910);
  if (boss.recycle3 <= 0)
  {
   angle = grand(ANGLE_1);
   angle2 = grand(ANGLE_1);
   angle3 = attack_angle(e);
    play_effectwfvx(WAV_DOUBLE, 600 + grand(10), 190, enemy[e].x);
    for (i = 0; i < 32; i ++)
    {
         b = create_ebullet(EBULLET_IBULLET_2);
         if (b != -1)
         {
          angle += ANGLE_32;
          angle2 += ANGLE_32;
          ebullet[b].x = enemy[e].x + xpart(angle, 10000);
          ebullet[b].y = enemy[e].y + ypart(angle, 10000) + 41000;
          ebullet[b].angle = angle;
          speed = 2500 + xpart(angle2 * 8, 250);
          ebullet[b].x_speed = enemy[e].x_speed + xpart(ebullet[b].angle, speed);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(ebullet[b].angle, speed);

          ebullet[b].x_speed += xpart(angle3, 1200);
          ebullet[b].y_speed += ypart(angle3, 1200);
//          ebullet[b].x_speed += xpart(i * 96, 1200);
//          ebullet[b].y_speed += xpart(i * 96, 1200);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = 1;
          ebullet[b].status = 6;
         }
    }

   angle = grand(ANGLE_1);
   angle2 = grand(ANGLE_1);
//   angle3 = attack_angle(e);
    for (i = 0; i < 32; i ++)
    {
         b = create_ebullet(EBULLET_IBULLET_2);
         if (b != -1)
         {
          angle += ANGLE_32;
          angle2 += ANGLE_32;
          ebullet[b].x = enemy[e].x + xpart(angle, 10000);
          ebullet[b].y = enemy[e].y + ypart(angle, 10000) + 41000;
          ebullet[b].angle = angle;
          speed = 1500 + xpart(angle2 * 16, 350);
          ebullet[b].x_speed = enemy[e].x_speed + xpart(ebullet[b].angle, speed);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(ebullet[b].angle, speed);

          ebullet[b].x_speed += xpart(angle3, 1200);
          ebullet[b].y_speed += ypart(angle3, 1200);
//          ebullet[b].x_speed += xpart(i * 96, 1200);
//          ebullet[b].y_speed += xpart(i * 96, 1200);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = 1;
          ebullet[b].status = 0;
         }
    }
/*    angle += ANGLE_3;
    for (i = 0; i < 32; i ++)
    {
         b = create_ebullet(EBULLET_IBULLET_2);
         if (b != -1)
         {
          angle += ANGLE_32;
          ebullet[b].x = enemy[e].x;
          ebullet[b].y = enemy[e].y - 25000;
          ebullet[b].angle = angle;
          speed = 1200;
          ebullet[b].x_speed = enemy[e].x_speed + xpart(ebullet[b].angle, speed);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(ebullet[b].angle, speed);
          ebullet[b].x_speed += xpart(i * 128, 600);
          ebullet[b].y_speed += ypart(i * 128, 600);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_2;
          ebullet[b].status = 0;
         }
    }*/
    quick_cloud(CLOUD_FCIRCLE2, enemy[e].x, enemy[e].y + 41000, enemy[e].x_speed, enemy[e].y_speed, 50 + grand(10), CLOUD_COL_3, 950);
    boss.recycle3 = 300 DIFF 75;
   }

 }

 switch(boss.phase)
 {
//  case 0: // entering;
//   break;
  case 1: // attack 1  - both red
   if (boss.recycle <= 0)
   {
    boss1_2_fire1(e, enemy[e].x - 76000, enemy[e].y - 11000, enemy[e].aim [0] + grand(24) - grand(24));
    boss.recycle = 32 DIFF 8;
   }
   if (boss.recycle2 <= 0)
   {
    boss1_2_fire1(e, enemy[e].x + 76000, enemy[e].y - 11000, enemy[e].aim [1] + grand(24) - grand(24));
    boss.recycle2 = 32 DIFF 8;
   }
   break;
  case 2: // attack 2 - both blue
   if (boss.recycle <= 0)
   {
    boss1_2_fire2(e, enemy[e].x - 76000, enemy[e].y - 11000, enemy[e].aim [0]);
    boss.recycle = 50 DIFF 12;
   }
   if (boss.recycle2 <= 0)
   {
    boss1_2_fire2(e, enemy[e].x + 76000, enemy[e].y - 11000, enemy[e].aim [1]);
    boss.recycle2 = 50 DIFF 12;
   }
   break;
  case 3: // attack 3 - red left, right blue
   if (boss.recycle <= 0)
   {
    boss1_2_fire1(e, enemy[e].x - 76000, enemy[e].y - 11000, enemy[e].aim [0] + grand(24) - grand(24));
    boss.recycle = 32 DIFF 8;
   }
   if (boss.recycle2 <= 0)
   {
    boss1_2_fire2(e, enemy[e].x + 76000, enemy[e].y - 11000, enemy[e].aim [1]);
    boss.recycle2 = 50 DIFF 8;
   }
   break;
  case 4: // attack 4 - reverse 3
   if (boss.recycle <= 0)
   {
    boss1_2_fire2(e, enemy[e].x - 76000, enemy[e].y - 11000, enemy[e].aim [0]);
    boss.recycle = 50 DIFF 12;
   }
   if (boss.recycle2 <= 0)
   {
    boss1_2_fire1(e, enemy[e].x + 76000, enemy[e].y - 11000, enemy[e].aim [1] + grand(24) - grand(24));
    boss.recycle2 = 32 DIFF 12;
   }
   break;

 }

 if (boss.phase != 0)
 {
   drag_enemy(e, 980);
//    if (arena.counter
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    enemy[e].x_speed += xpart(enemy[e].angle, 90);
    enemy[e].y_speed += ypart(enemy[e].angle, 90);
    if (enemy[e].x > enemy[e].x_target [0] - 20000
     && enemy[e].x < enemy[e].x_target [0] + 20000
     && enemy[e].y > enemy[e].y_target [0] - 20000
     && enemy[e].y < enemy[e].y_target [0] + 20000)
     {
      enemy[e].x_target [0] = RAND_X_IN;
      enemy[e].y_target [0] = 120000 + grand(120000);
      enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
//      enemy[e].x_speed = 0;
//      enemy[e].y_speed = 0;
     }
 }

    move_enemy(e);

}



void boss1_2_fire1(int e, int x, int y, int a)
{

         int b = create_ebullet(EBULLET_SHOT);
         play_effectwfvx(WAV_CHIRP, 1200 + grand(200), 70, x);

         if (b != -1)
         {
          ebullet[b].x = x;
          ebullet[b].y = y;
          ebullet[b].x_speed = enemy[e].x_speed + xpart(a, 4000);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(a, 4000);
          ebullet[b].angle = radians_to_angle(atan2(ebullet[b].y_speed, ebullet[b].x_speed));
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_3;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_3, 930);
          ebullet[b].status = 6 + grand (5);
         }
         b = create_ebullet(EBULLET_SHOT);
         if (b != -1)
         {
          ebullet[b].x = x;
          ebullet[b].y = y;
          ebullet[b].x_speed = enemy[e].x_speed + xpart(a - ANGLE_16, 3000);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(a - ANGLE_16, 3000);
          ebullet[b].angle = radians_to_angle(atan2(ebullet[b].y_speed, ebullet[b].x_speed));
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_2;
//          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_1, 930);
          ebullet[b].status = 6 + grand (5);
         }
         b = create_ebullet(EBULLET_SHOT);
         if (b != -1)
         {
          ebullet[b].x = x;
          ebullet[b].y = y;
          ebullet[b].x_speed = enemy[e].x_speed + xpart(a + ANGLE_16, 3000);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(a + ANGLE_16, 3000);
          ebullet[b].angle = radians_to_angle(atan2(ebullet[b].y_speed, ebullet[b].x_speed));
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_2;
//          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_1, 930);
          ebullet[b].status = 6 + grand (5);
         }

}


void boss1_2_fire2(int e, int x, int y, int a)
{

    int b, i;

    play_effectwfvx(WAV_CHIRP, 600 + grand(200), 80, x);

    for (i = 0; i < 4; i ++)
    {

         b = create_ebullet(EBULLET_SHOT);
         if (b != -1)
         {
          ebullet[b].x = x;
          ebullet[b].y = y;
          ebullet[b].x_speed = enemy[e].x_speed + xpart(a, 2000 + i * 800);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(a, 2000 + i * 800);
          ebullet[b].angle = radians_to_angle(atan2(ebullet[b].y_speed, ebullet[b].x_speed));
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_1, 930);
          ebullet[b].status = 6 + grand (5);
          if (i < 3)
           ebullet[b].status = 0;
          if (i == 0)
           quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 30 + grand(10), CLOUD_COL_1, 930);
         }
    }


}






/*

*************************************************************

Boss - stage 1 part 3

*************************************************************

*/





void run_boss1_3(int e)
{

 int angle = 0, i, b, speed, angle2, j;

 boss.phase_count --;

 enemy[e].recycle --;
 if (enemy[e].recycle < 0)
  enemy[e].recycle = 0;
// note different types of recycle...

 boss.recycle --;
 if (boss.recycle < 0)
  boss.recycle = 0;

 boss.recycle2 --;
 if (boss.recycle2 < 0)
  boss.recycle2 = 0;

 boss.recycle3 --;
 if (boss.recycle3 < 0)
  boss.recycle3 = 0;

 if (boss.phase_count <= 0)
 {
  switch(boss.phase)
  {
   case 0: // entering
    boss.phase = 1;
    boss.phase2 = 1;
    boss.phase_count = 150 + grand(200);
    enemy[e].x_target [0] = RAND_X_IN;
    enemy[e].y_target [0] = 120000 + grand(180000);
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    break;
   case 1: // 1, 2 and 3 are attack phases
    boss.phase = 2 + grand(2);
    boss.phase_count = 150 + grand(200);
    break;
   case 2:
    boss.phase = 1;
    boss.phase_count = 150 + grand(200);
//    enemy[e].x_target [0] = RAND_X_IN;
//    .enemy[e].y_target [0] = 120000 + grand(180000);
//..    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    boss.phase2 = coin();
    break;
   case 3:
    boss.phase = 1;
    boss.phase_count = 150 + grand(200);
    boss.phase2 = coin();
    break;
  }


 }

//   aim_at_target(e, enemy[e].x - 76000, enemy[e].y - 11000, player.x, player.y, 7, 0);
//   aim_at_target(e, enemy[e].x + 76000, enemy[e].y - 11000, player.x, player.y, 7, 1);

//   quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 53000, enemy[e].y + 50000, enemy[e].x_speed, enemy[e].y_speed + 4000 + grand(4000), 15 + grand(10), CLOUD_COL_1, 980);
//   quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 53000, enemy[e].y + 50000, enemy[e].x_speed, enemy[e].y_speed + 4000 + grand(4000), 15 + grand(10), CLOUD_COL_1, 980);

 if (boss.phase > 0)
 {
  if (boss.recycle3 <= 30)
   drag_enemy(e, 910);
  if (boss.recycle3 <= 0)
  {
     switch(boss.phase2)
     {
         case 0:
           b = create_ebullet(EBULLET_FLAKE2);
           if (b != -1)
           {
            if (coin())
            {
             play_effectwfvx(WAV_INTER, 400 + grand(50), 190, enemy[e].x - 99000);
             ebullet[b].x = enemy[e].x - 99000;
             angle = radians_to_angle(atan2(player.y - (enemy[e].y + 42000), player.x - (enemy[e].x - 99000)));
             quick_cloud(CLOUD_FCIRCLE2, enemy[e].x - 99000, enemy[e].y + 42000, enemy[e].x_speed, enemy[e].y_speed, 50 + grand(10), CLOUD_COL_3, 950);
            }
             else
             {
              play_effectwfvx(WAV_INTER, 400 + grand(50), 190, enemy[e].x + 99000);
              ebullet[b].x = enemy[e].x + 99000;
              angle = radians_to_angle(atan2(player.y - (enemy[e].y + 42000), player.x - (enemy[e].x + 99000)));
              quick_cloud(CLOUD_FCIRCLE2, enemy[e].x + 99000, enemy[e].y + 42000, enemy[e].x_speed, enemy[e].y_speed, 50 + grand(10), CLOUD_COL_3, 950);
             }
            ebullet[b].y = enemy[e].y + 42000;
            speed = 2000;
            ebullet[b].x_speed = xpart(angle, speed);// + enemy[e].x_speed;
            ebullet[b].y_speed = ypart(angle, speed);// + enemy[e].y_speed;
            ebullet[b].timeout = 4000; // important!
            ebullet[b].status = 1;
            ebullet[b].colour = CLOUD_COL_3;
            ebullet[b].angle2 = coin(); // rotation direction
           }
    boss.recycle3 = 50 DIFF 9;

   break;
   case 1:
    play_effectwfvx(WAV_DOUBLE, 500 + grand(10), 190, enemy[e].x);
    for (j = 0; j < 2; j ++)
    {
     for (i = 0; i < 24; i ++)
     {
         b = create_ebullet(EBULLET_IBULLET_2);
         if (b != -1)
         {
          angle += ANGLE_24;
          angle2 += ANGLE_24;
          ebullet[b].x = enemy[e].x + xpart(angle, 10000) - 99000;
          if (j == 1)
           ebullet[b].x = enemy[e].x + xpart(angle, 10000) + 99000;
          ebullet[b].y = enemy[e].y + ypart(angle, 10000) + 42000;
          ebullet[b].angle = angle;
          speed = 2500;
          ebullet[b].x_speed = enemy[e].x_speed + xpart(ebullet[b].angle, speed);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(ebullet[b].angle, speed);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = 0;
          ebullet[b].status = 6;
         }
    }

   angle = grand(ANGLE_1);
   angle2 = grand(ANGLE_1);
//   angle3 = attack_angle(e);
    for (i = 0; i < 16; i ++)
    {
         b = create_ebullet(EBULLET_IBULLET_2);
         if (b != -1)
         {
          angle += ANGLE_16;
          angle2 += ANGLE_16;
          ebullet[b].x = enemy[e].x + xpart(angle, 10000) - 99000;
          if (j == 1)
           ebullet[b].x = enemy[e].x + xpart(angle, 10000) + 99000;
          ebullet[b].y = enemy[e].y + ypart(angle, 10000) + 42000;
          ebullet[b].angle = angle;
          speed = 1700;
          ebullet[b].x_speed = enemy[e].x_speed + xpart(ebullet[b].angle, speed);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(ebullet[b].angle, speed);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = 2;
          ebullet[b].status = 0;
         }
    }
    quick_cloud(CLOUD_FCIRCLE2, enemy[e].x - 99000, enemy[e].y + 42000, enemy[e].x_speed, enemy[e].y_speed, 50 + grand(10), CLOUD_COL_3, 950);
    quick_cloud(CLOUD_FCIRCLE2, enemy[e].x + 99000, enemy[e].y + 42000, enemy[e].x_speed, enemy[e].y_speed, 50 + grand(10), CLOUD_COL_3, 950);
    boss.recycle3 = 120 DIFF 20;
   }
    break;
  }
  }
 }

 switch(boss.phase)
 {
//  case 0: // entering;
//   break;
  case 1: // attack 1  - both red
   aim_at_target(e, enemy[e].x - 76000, enemy[e].y - 11000, player.x, player.y, 7, 0);
   aim_at_target(e, enemy[e].x + 76000, enemy[e].y - 11000, player.x, player.y, 7, 1);
   if (boss.recycle <= 0)
   {
    if (boss.side == 0)
    {
     boss1_3_fire1(e, enemy[e].x - 76000, enemy[e].y - 11000, enemy[e].aim [0]);
     boss.side = 1;
    }
      else
      {
       boss1_3_fire1(e, enemy[e].x + 76000, enemy[e].y - 11000, enemy[e].aim [1]);
       boss.side = 0;
      }
    boss.recycle = 22 DIFF 4;
   }
   break;
  case 2: // attack 2 - both beams
  case 3: // attack 2 - both beams - reverse rotation
   boss1_3_fire2(e);
   if (boss.phase_count < 3)
   {
    if (enemy[e].burst == 0)
     boss.phase_count = 0;
      else
       boss.phase_count = 2;
   }
   break;

 }

 if (boss.phase != 0)
 {
   drag_enemy(e, 980);
//    if (arena.counter
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    enemy[e].x_speed += xpart(enemy[e].angle, 90);
    enemy[e].y_speed += ypart(enemy[e].angle, 90);
    if (enemy[e].x > enemy[e].x_target [0] - 20000
     && enemy[e].x < enemy[e].x_target [0] + 20000
     && enemy[e].y > enemy[e].y_target [0] - 20000
     && enemy[e].y < enemy[e].y_target [0] + 20000)
     {
      enemy[e].x_target [0] = RAND_X_IN;
      enemy[e].y_target [0] = 120000 + grand(120000);
      enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
//      enemy[e].x_speed = 0;
//      enemy[e].y_speed = 0;
     }
 }

    move_enemy(e);

}



void boss1_3_fire1(int e, int x, int y, int a)
{

         int b = create_ebullet(EBULLET_B13SHOT);
         if (b != -1)
         {
          ebullet[b].x = x;
          ebullet[b].y = y;
          ebullet[b].x_speed = enemy[e].x_speed + xpart(a, 4500);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(a, 4500);
          ebullet[b].angle = radians_to_angle(atan2(ebullet[b].y_speed, ebullet[b].x_speed));
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 40 + grand(10), CLOUD_COL_1, 930);
          ebullet[b].status = pos_or_neg(12);
          ebullet[b].angle = grand(ANGLE_1);
         }

    play_effectwfvx(WAV_FIRE, 1500 + grand(10), 190, x);

}


void boss1_3_fire2(int e)
{

    int angle, b;

   if (enemy[e].status == 1 || enemy[e].burst <= 0)
    aim_at_target(e, enemy[e].x - 76000, enemy[e].y - 11000, player.x, player.y, 5, 0);
   if (enemy[e].status == 0 || enemy[e].burst <= 0)
    aim_at_target(e, enemy[e].x + 76000, enemy[e].y - 11000, player.x, player.y, 5, 1);
   if (enemy[e].recycle <= 0)
   {
       enemy[e].burst = 120;
       enemy[e].recycle = 124;
       play_effectwfvx(WAV_BEAM3, 250, 220, enemy[e].x);
       if (enemy[e].status == 0)
       {
        enemy[e].status = 1;
        enemy[e].angle1 = 1;
        if (arena.difficulty == 2)
         enemy[e].angle1 = 2;
       }
         else
         {
          enemy[e].status = 0;
          enemy[e].angle1 = -1;
          if (arena.difficulty == 2)
           enemy[e].angle1 = -2;
         }
//       enemy[e].angle1 = delta_turn_towards_xy(enemy[e].x, enemy[e].y, player.x, player.y, enemy[e].aim [enemy[e].status], 1);
        // inexact but doesn't really matter
   }
   if (enemy[e].burst > 0)
   {
         b = create_ebullet(EBULLET_BEAM);
         if (b != -1)
         {
          angle += ANGLE_32;
          if (enemy[e].status == 1)
          {
           ebullet[b].x = enemy[e].x + 76000 + xpart(enemy[e].aim [1], 6000);
           ebullet[b].y = enemy[e].y - 11000 + ypart(enemy[e].aim [1], 6000);
           if (boss.phase == 2)
            enemy[e].aim [1] += enemy[e].angle1;
             else
              enemy[e].aim [1] -= enemy[e].angle1;
          }
           else
           {
            ebullet[b].x = enemy[e].x - 76000 + xpart(enemy[e].aim [0], 6000);
            ebullet[b].y = enemy[e].y - 11000 + ypart(enemy[e].aim [0], 6000);
            if (boss.phase == 2)
             enemy[e].aim [0] += enemy[e].angle1;
              else
               enemy[e].aim [0] -= enemy[e].angle1;
           }
          ebullet[b].angle = enemy[e].aim [enemy[e].status];
          ebullet[b].timeout = 2;
          ebullet[b].colour = 0;
          ebullet[b].status = enemy[e].burst;
         }
         enemy[e].burst --;
   }

}





/*

*************************************************************

Boss - stage 2 part 1

*************************************************************

*/



void run_boss2_1(int e)
{



 boss.phase_count --;

 boss.recycle --;
 if (boss.recycle < 0)
  boss.recycle = 0;

 boss.recycle2 --;
 if (boss.recycle2 < 0)
  boss.recycle2 = 0;

 boss.recycle3 --;
 if (boss.recycle3 < 0)
  boss.recycle3 = 0;

 boss.recycle4 --;
 if (boss.recycle4 < 0)
  boss.recycle4 = 0;

 if (boss.phase_count <= 0)
 {
  switch(boss.phase)
  {
   case 0: // entering
    boss.phase = 1;
    boss.phase2 = 4;
    boss.phase_count = 150 + grand(200);
    enemy[e].x_target [0] = RAND_X_IN;
    enemy[e].y_target [0] = 120000 + grand(180000);
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    break;
   case 1: // 1, 2 and 3 are attack phases
   case 2:
   case 3:
   case 4:
    if (coin())
     boss.phase = 1 + grand(4);
      else
       boss.phase2 = 1 + grand(4);
    boss.phase_count = 150 + grand(150);
    enemy[e].x_target [0] = RAND_X_IN;
    enemy[e].y_target [0] = 120000 + grand(180000);
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
//    enemy[e].x_speed = 0;
//    enemy[e].y_speed = 0;
    break;
  }


 }

   aim_at_target(e, enemy[e].x - 90000, enemy[e].y + 27000, player.x, player.y, 7, 0);
   aim_at_target(e, enemy[e].x + 90000, enemy[e].y + 27000, player.x, player.y, 7, 1);

   aim_at_target(e, enemy[e].x - 50000, enemy[e].y - 13000, player.x, player.y, 7, 2);
   aim_at_target(e, enemy[e].x + 50000, enemy[e].y - 13000, player.x, player.y, 7, 3);

//   quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 53000, enemy[e].y + 50000, enemy[e].x_speed, enemy[e].y_speed + 4000 + grand(4000), 15 + grand(10), CLOUD_COL_1, 980);
//   quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 53000, enemy[e].y + 50000, enemy[e].x_speed, enemy[e].y_speed + 4000 + grand(4000), 15 + grand(10), CLOUD_COL_1, 980);

 if (boss.phase > 0)
 {
 }

 switch(boss.phase)
 {
//  case 0: // entering;
//   break;
  case 1: // attack 1
   if (boss.recycle <= 0)
   {
    boss2_1_fire1(e, enemy[e].x - 90000, enemy[e].y + 27000, enemy[e].aim [0] + grand(24) - grand(24));
    boss.recycle = 32 DIFF 8;
   }
   if (boss.recycle2 <= 0)
   {
    boss2_1_fire1(e, enemy[e].x + 90000, enemy[e].y + 27000, enemy[e].aim [1] + grand(24) - grand(24));
    boss.recycle2 = 32 DIFF 8;
   }
   break;
  case 2: // attack 2
   if (boss.recycle <= 0)
   {
    boss2_1_fire2(e, enemy[e].x - 90000, enemy[e].y + 27000, enemy[e].aim [0] + grand(24) - grand(24));
    boss.recycle = 32 DIFF 8;
   }
   if (boss.recycle2 <= 0)
   {
    boss2_1_fire2(e, enemy[e].x + 90000, enemy[e].y + 27000, enemy[e].aim [1] + grand(24) - grand(24));
    boss.recycle2 = 32 DIFF 8;
   }
   break;
  case 3: // attack 3 - red left, right blue
   if (boss.recycle <= 0)
   {
    boss2_1_fire3(e, enemy[e].x - 90000, enemy[e].y + 27000, enemy[e].aim [0] + grand(24) - grand(24));
    boss.recycle = 62 DIFF 12;
   }
   if (boss.recycle2 <= 0)
   {
    boss2_1_fire3(e, enemy[e].x + 90000, enemy[e].y + 27000, enemy[e].aim [1] + grand(24) - grand(24));
    boss.recycle2 = 62 DIFF 12;
   }
   break;
  case 4: // attack 4 - reverse 3
   if (boss.recycle <= 0)
   {
    boss2_1_fire4(e, enemy[e].x - 90000, enemy[e].y + 27000, enemy[e].aim [0] + grand(24) - grand(24));
    boss.recycle = 32 DIFF 8;
   }
   if (boss.recycle2 <= 0)
   {
    boss2_1_fire4(e, enemy[e].x + 90000, enemy[e].y + 27000, enemy[e].aim [1] + grand(24) - grand(24));
    boss.recycle2 = 32 DIFF 8;
   }
   break;

 }

 switch(boss.phase2)
 {
//  case 0: // entering;
//   break;
  case 1: // attack 1
   if (boss.recycle4 <= 0)
   {
    boss2_1_fire1(e, enemy[e].x - 50000, enemy[e].y - 27000, enemy[e].aim [2] + grand(24) - grand(24));
    boss.recycle4 = 32 DIFF 8;
   }
   if (boss.recycle3 <= 0)
   {
    boss2_1_fire1(e, enemy[e].x + 50000, enemy[e].y - 27000, enemy[e].aim [3] + grand(24) - grand(24));
    boss.recycle3 = 32 DIFF 8;
   }
   break;
  case 2: // attack 2
   if (boss.recycle4 <= 0)
   {
    boss2_1_fire2(e, enemy[e].x - 50000, enemy[e].y - 27000, enemy[e].aim [2] + grand(24) - grand(24));
    boss.recycle4 = 32 DIFF 8;
   }
   if (boss.recycle3 <= 0)
   {
    boss2_1_fire2(e, enemy[e].x + 50000, enemy[e].y - 27000, enemy[e].aim [3] + grand(24) - grand(24));
    boss.recycle3 = 32 DIFF 8;
   }
   break;
  case 3: // attack 3 - red left, right blue
   if (boss.recycle4 <= 0)
   {
    boss2_1_fire3(e, enemy[e].x - 50000, enemy[e].y - 27000, enemy[e].aim [2] + grand(24) - grand(24));
    boss.recycle4 = 62 DIFF 12;
   }
   if (boss.recycle3 <= 0)
   {
    boss2_1_fire3(e, enemy[e].x + 50000, enemy[e].y - 27000, enemy[e].aim [3] + grand(24) - grand(24));
    boss.recycle3 = 62 DIFF 12;
   }
   break;
  case 4: // attack 4 - reverse 3
   if (boss.recycle4 <= 0)
   {
    boss2_1_fire4(e, enemy[e].x - 50000, enemy[e].y - 27000, enemy[e].aim [2] + grand(24) - grand(24));
    boss.recycle4 = 32 DIFF 8;
   }
   if (boss.recycle3 <= 0)
   {
    boss2_1_fire4(e, enemy[e].x + 50000, enemy[e].y - 27000, enemy[e].aim [3] + grand(24) - grand(24));
    boss.recycle3 = 32 DIFF 8;
   }
   break;

 }


 if (boss.phase != 0)
 {
   drag_enemy(e, 970);
//    if (arena.counter
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    enemy[e].x_speed += xpart(enemy[e].angle, 110);
    enemy[e].y_speed += ypart(enemy[e].angle, 110);
    if (enemy[e].x > enemy[e].x_target [0] - 20000
     && enemy[e].x < enemy[e].x_target [0] + 20000
     && enemy[e].y > enemy[e].y_target [0] - 20000
     && enemy[e].y < enemy[e].y_target [0] + 20000)
     {
      enemy[e].x_target [0] = RAND_X_IN;
      enemy[e].y_target [0] = 120000 + grand(180000);
      enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
//      enemy[e].x_speed = 0;
//      enemy[e].y_speed = 0;
     }
 }

    move_enemy(e);

}


void boss2_1_fire1(int e, int x, int y, int a)
{

         play_effectwfvx(WAV_FIRE, 1900 + grand(300), 140, x);
         int b = create_ebullet(EBULLET_SHOT2);
         if (b != -1)
         {
          ebullet[b].x = x + xpart(a, 8000);
          ebullet[b].y = y + ypart(a, 8000);
          ebullet[b].x_speed = enemy[e].x_speed + xpart(a, 7300);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(a, 7300);
          ebullet[b].angle = radians_to_angle(atan2(ebullet[b].y_speed, ebullet[b].x_speed));
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 40 + grand(10), CLOUD_COL_1, 930);
          ebullet[b].angle = a;
         }

}


void boss2_1_fire2(int e, int x, int y, int a)
{

         int b = create_ebullet(EBULLET_B21SHOT);
         if (b != -1)
         {
          ebullet[b].x = x + xpart(a, 8000);
          ebullet[b].y = y + ypart(a, 8000);
          ebullet[b].x_speed = enemy[e].x_speed + xpart(a, 4300);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(a, 4300);
//          ebullet[b].angle = radians_to_angle(atan2(ebullet[b].y_speed, ebullet[b].x_speed));
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_4;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 40 + grand(10), CLOUD_COL_4, 930);
          ebullet[b].angle = a + grand(356) - grand(356);
         }
    play_effectwfvx(WAV_FIRE, 1200 + grand(10), 190, x);

}



void boss2_1_fire3(int e, int x, int y, int a)
{

 int i, speed, angle, b;

 for (i = 0; i < 9; i ++)
 {

         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          angle = a + grand(24) - grand(24);
          speed = 2500 + grand(3000);
          ebullet[b].x = x + xpart(a, 8000);
          ebullet[b].y = y + ypart(a, 8000);
          ebullet[b].x_speed = enemy[e].x_speed + xpart(angle, speed);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(angle, speed);
//          ebullet[b].angle = radians_to_angle(atan2(ebullet[b].y_speed, ebullet[b].x_speed));
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_3;
          ebullet[b].status = 4 + grand(12);
//          ebullet[b].angle = grand(ANGLE_1);
         }

 }

    play_effectwfvx(WAV_FIRE, 900 + grand(10), 190, x);

 if (b != -1)
  quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 40 + grand(10), CLOUD_COL_3, 930);
 // can be any bullet.

}

void boss2_1_fire4(int e, int x, int y, int a)
{

         int b = create_ebullet(EBULLET_B13SHOT);
//         play_effectwfvx(WAV_DOUBLE, 1200 + grand(150), 190, x);
         if (b != -1)
         {
          ebullet[b].x = x + xpart(a, 8000);
          ebullet[b].y = y + ypart(a, 8000);
          ebullet[b].x_speed = enemy[e].x_speed + xpart(a, 5300);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(a, 5300);
          ebullet[b].angle = radians_to_angle(atan2(ebullet[b].y_speed, ebullet[b].x_speed));
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_2;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 40 + grand(10), CLOUD_COL_2, 930);
          ebullet[b].status = pos_or_neg(16);
          ebullet[b].angle = grand(ANGLE_1);
         }

    play_effectwfvx(WAV_FIRE, 1500 + grand(10), 190, x);

}







/*

*************************************************************

Boss - stage 2 part 2

*************************************************************

*/




void run_boss2_2(int e)
{

 int i, b;

 boss.phase_count --;

 enemy[e].recycle --;
 if (enemy[e].recycle < 0)
  enemy[e].recycle = 0;

 boss.recycle --;
 if (boss.recycle < 0)
  boss.recycle = 0;

 boss.recycle2 --;
 if (boss.recycle2 < 0)
  boss.recycle2 = 0;

 boss.recycle3 --;
 if (boss.recycle3 < 0)
  boss.recycle3 = 0;

 boss.recycle4 --;
 if (boss.recycle4 < 0)
  boss.recycle4 = 0;


 if (boss.phase_count <= 0)
 {
  switch(boss.phase)
  {
   case 0: // entering
    boss.phase = 1;
    boss.phase2 = 0;
    boss.phase_count = 150 + grand(200);
    enemy[e].x_target [0] = RAND_X_IN;
    enemy[e].y_target [0] = 120000 + grand(180000);
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    break;
   case 1: // 1, 2 and 3 are attack phases
   case 2:
   case 3:
   case 4:
    if (boss.phase == 1)
     boss.phase = 2;
      else
       boss.phase = 1;

     boss.phase2 = 2 + grand(3);
     if (boss.hp < 35000)
      boss.phase2 = 1;

    boss.phase_count = 250 + grand(100);
    enemy[e].x_target [0] = RAND_X_IN;
    enemy[e].y_target [0] = 120000 + grand(180000);
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
//    enemy[e].x_speed = 0;
//    enemy[e].y_speed = 0;
    break;
  }


 }


 int x = enemy[e].x;
 int y = enemy[e].y - 15000;

 switch(boss.phase)
 {

     case 1:
//     default:
      if (enemy[e].recycle <= 0)
      {
          enemy[e].angle1 += enemy[e].status;
          play_effectwfvx(WAV_CHIRP, 1100 + grand(200), 120, enemy[e].x);
          for (i = 0; i < 5; i ++)
          {
           b = create_ebullet(EBULLET_BLOB);
           if (b != -1)
           {
            ebullet[b].x = x;
            ebullet[b].y = y;
            ebullet[b].angle = enemy[e].angle1;
            ebullet[b].timeout = 4000;
            ebullet[b].colour = CLOUD_COL_3;
            if (enemy[e].burst == 9 || enemy[e].burst == 1)
             ebullet[b].status = 6;
            if (enemy[e].burst == 8 || enemy[e].burst == 2)
             ebullet[b].status = 8;
            if (enemy[e].burst == 7 || enemy[e].burst == 3)
             ebullet[b].status = 10;
            if (enemy[e].burst == 6 || enemy[e].burst == 4)
             ebullet[b].status = 12;
            if (enemy[e].burst == 5)
             ebullet[b].status = 14;
            ebullet[b].x_speed = enemy[e].x_speed + xpart(ebullet[b].angle + i * ANGLE_5, 2000 + ebullet[b].status * 200);
            ebullet[b].y_speed = enemy[e].y_speed + ypart(ebullet[b].angle + i * ANGLE_5, 2000 + ebullet[b].status * 200);
            quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 30 + grand(10), CLOUD_COL_3, 950);
           }
          }

         enemy[e].recycle = 4;
         enemy[e].burst --;
         if (enemy[e].burst <= 0)
         {
          enemy[e].recycle = 45 DIFF 9;
          enemy[e].burst = 9;
          enemy[e].angle1 = grand(ANGLE_1);
          enemy[e].status *= -1;
         }
         break;

        case 2:
      if (enemy[e].recycle <= 0)
      {
         play_effectwfvx(WAV_CHIRP, 1000 + grand(200), 120, enemy[e].x);
          enemy[e].angle1 += enemy[e].status;
          for (i = 0; i < 3; i ++)
          {
           b = create_ebullet(EBULLET_BLOB);
           if (b != -1)
           {
            ebullet[b].x = x;
            ebullet[b].y = y;
            ebullet[b].angle = enemy[e].angle1;
            ebullet[b].timeout = 4000;
            ebullet[b].colour = CLOUD_COL_3;
            ebullet[b].status = 9;
            ebullet[b].x_speed = enemy[e].x_speed + xpart(ebullet[b].angle + i * ANGLE_3, 3500);
            ebullet[b].y_speed = enemy[e].y_speed + ypart(ebullet[b].angle + i * ANGLE_3, 3500);
            quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 30 + grand(10), CLOUD_COL_3, 950);
           }

           b = create_ebullet(EBULLET_BLOB);
           if (b != -1)
           {
            ebullet[b].x = x;
            ebullet[b].y = y;
            ebullet[b].angle = ANGLE_1 - (enemy[e].angle1 * 2);
            ebullet[b].timeout = 4000;
            ebullet[b].colour = CLOUD_COL_1;
            ebullet[b].status = 7;
            ebullet[b].x_speed = enemy[e].x_speed + xpart(ebullet[b].angle + i * ANGLE_3, 3200);
            ebullet[b].y_speed = enemy[e].y_speed + ypart(ebullet[b].angle + i * ANGLE_3, 3200);
            quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 30 + grand(10), CLOUD_COL_1, 950);
           }
          }

         enemy[e].recycle = 4;
         enemy[e].burst --;
         if (enemy[e].burst <= 0)
         {
          enemy[e].recycle = 45 DIFF 9;
          enemy[e].burst = 9;
          enemy[e].angle1 = grand(ANGLE_1);
          enemy[e].status *= -1;
         }
      }
         break;
      }

 }




 if (boss.phase > 0)
 {
 }


 switch(boss.phase2)
 {
  case 1: // beam
   boss2_2_fire1(e);
   if (boss.recycle2 <= 0)
   {
       boss.beam = 120;
       boss.recycle2 = 122;
       enemy[e].aim [0] = attack_angle(e) + grand(ANGLE_16) - grand(ANGLE_16);
       enemy[e].aim [1] = attack_angle(e) + grand(ANGLE_16) - grand(ANGLE_16);
   }
   break;
  case 2:
   boss2_2_fire2(e);
   boss2_2_fire2(e);
   break;
  case 3:
   if (boss.recycle2 <= 0)
   {
    boss2_2_fire3(e);
    boss2_2_fire3(e);
    boss.recycle2 = 90 DIFF 15;
   }
   break;
  case 4:
   if (boss.recycle2 <= 0)
   {
    boss2_2_fire4(e);
    boss2_2_fire4(e);
    boss.recycle2 = 13 DIFF 3;
   }
   break;
/*  case 2: // attack 2
   if (boss.recycle4 <= 0)
   {
    boss2_1_fire2(e, enemy[e].x - 50000, enemy[e].y - 27000, enemy[e].aim [2] + grand(24) - grand(24));
    boss.recycle4 = 32 DIFF 8;
   }
   if (boss.recycle3 <= 0)
   {
    boss2_1_fire2(e, enemy[e].x + 50000, enemy[e].y - 27000, enemy[e].aim [3] + grand(24) - grand(24));
    boss.recycle3 = 32 DIFF 8;
   }
   break;
  case 3: // attack 3 - red left, right blue
   if (boss.recycle4 <= 0)
   {
    boss2_1_fire3(e, enemy[e].x - 50000, enemy[e].y - 27000, enemy[e].aim [2] + grand(24) - grand(24));
    boss.recycle4 = 62 DIFF 12;
   }
   if (boss.recycle3 <= 0)
   {
    boss2_1_fire3(e, enemy[e].x + 50000, enemy[e].y - 27000, enemy[e].aim [3] + grand(24) - grand(24));
    boss.recycle3 = 62 DIFF 12;
   }
   break;
  case 4: // attack 4 - reverse 3
   if (boss.recycle4 <= 0)
   {
    boss2_1_fire4(e, enemy[e].x - 50000, enemy[e].y - 27000, enemy[e].aim [2] + grand(24) - grand(24));
    boss.recycle4 = 32 DIFF 8;
   }
   if (boss.recycle3 <= 0)
   {
    boss2_1_fire4(e, enemy[e].x + 50000, enemy[e].y - 27000, enemy[e].aim [3] + grand(24) - grand(24));
    boss.recycle3 = 32 DIFF 8;
   }
   break;*/

 }



 if (boss.phase != 0)
 {
   drag_enemy(e, 990);
//    if (arena.counter
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    enemy[e].x_speed += xpart(enemy[e].angle, 80);
    enemy[e].y_speed += ypart(enemy[e].angle, 80);
    if (enemy[e].x > enemy[e].x_target [0] - 20000
     && enemy[e].x < enemy[e].x_target [0] + 20000
     && enemy[e].y > enemy[e].y_target [0] - 20000
     && enemy[e].y < enemy[e].y_target [0] + 20000)
     {
      enemy[e].x_target [0] = RAND_X_IN;
      enemy[e].y_target [0] = 120000 + grand(180000);
      enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
//      enemy[e].x_speed = 0;
//      enemy[e].y_speed = 0;
     }
 }

    move_enemy(e);

}


void boss2_2_fire1(int e)
{

 int b;

   if (boss.beam > 0)
   {
         b = create_ebullet(EBULLET_BEAM2);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x - 33000;
          ebullet[b].y = enemy[e].y + 12000;
          ebullet[b].angle = enemy[e].aim [0];
          ebullet[b].timeout = 2;
          ebullet[b].colour = 0;
          ebullet[b].status = boss.beam;
         }

         b = create_ebullet(EBULLET_BEAM2);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x + 33000;
          ebullet[b].y = enemy[e].y + 12000;
          ebullet[b].angle = enemy[e].aim [1];
          ebullet[b].timeout = 2;
          ebullet[b].colour = 0;
          ebullet[b].status = boss.beam;
         }
         if (boss.beam == 85)
          play_effectwfvx(WAV_BEAM2, 1000, 110, enemy[e].x);
         boss.beam --;
         if (boss.beam < 60)
          boss.beam --;
   }


}

void boss2_2_fire2(int e)
{

    int b;

      if (boss.recycle2 <= 0)
      {
         play_effectwfvx(WAV_CHIRP, 1500 + grand(200), 120, enemy[e].x);

          enemy[e].angle2 += ANGLE_32;
           b = create_ebullet(EBULLET_SHOT);
           if (b != -1)
           {
            ebullet[b].x = enemy[e].x - 33000;
            ebullet[b].y = enemy[e].y + 13000;
            ebullet[b].angle = enemy[e].angle2;
            ebullet[b].timeout = 4000;
            ebullet[b].colour = CLOUD_COL_1;
            ebullet[b].x_speed = enemy[e].x_speed + xpart(ebullet[b].angle, 4000);
            ebullet[b].y_speed = enemy[e].y_speed + ypart(ebullet[b].angle, 4000);
            quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 30 + grand(10), CLOUD_COL_1, 0);
           }

           b = create_ebullet(EBULLET_SHOT);
           if (b != -1)
           {
            ebullet[b].x = enemy[e].x + 33000;
            ebullet[b].y = enemy[e].y + 13000;
            ebullet[b].angle = ANGLE_1 - enemy[e].angle2;
            ebullet[b].timeout = 4000;
            ebullet[b].colour = CLOUD_COL_1;
            ebullet[b].x_speed = enemy[e].x_speed + xpart(ebullet[b].angle, 4000);
            ebullet[b].y_speed = enemy[e].y_speed + ypart(ebullet[b].angle, 4000);
            quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 30 + grand(10), CLOUD_COL_1, 0);
           }

         boss.recycle2 = 2;
//         boss.recycle3 --;
         if (boss.recycle3 <= 0)
         {
          boss.recycle2 = 20 DIFF 5;
          boss.recycle3 = 42;
          enemy[e].angle2 = grand(ANGLE_1);
         }
      }

}


void boss2_2_fire3(int e)
{

 int i, speed, b, angle = attack_angle(e) + grand(ANGLE_16) - grand(ANGLE_16);

 int x = enemy[e].x - 33000;
 int y = enemy[e].y + 13000;

 for (i = 0; i < 6; i ++)
 {

         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          angle = angle + grand(16) - grand(16);
          speed = 3500 + grand(2000);
          ebullet[b].x = x + xpart(angle, 8000);
          ebullet[b].y = y + ypart(angle, 8000);
          ebullet[b].x_speed = enemy[e].x_speed + xpart(angle, speed);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(angle, speed);
//          ebullet[b].angle = radians_to_angle(atan2(ebullet[b].y_speed, ebullet[b].x_speed));
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
          ebullet[b].status = 6 + grand(10);
//          ebullet[b].angle = grand(ANGLE_1);
         }

 }

 if (b != -1)
  quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 40 + grand(10), CLOUD_COL_1, 930);
 // can be any bullet.

 angle = attack_angle(e) + grand(ANGLE_16) - grand(ANGLE_16);

 x = enemy[e].x + 33000;

 for (i = 0; i < 6; i ++)
 {

         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          angle = angle + grand(16) - grand(16);
          speed = 3500 + grand(2000);
          ebullet[b].x = x + xpart(angle, 8000);
          ebullet[b].y = y + ypart(angle, 8000);
          ebullet[b].x_speed = enemy[e].x_speed + xpart(angle, speed);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(angle, speed);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
          ebullet[b].status = 6 + grand(10);
         }

 }

 if (b != -1)
  quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 40 + grand(10), CLOUD_COL_1, 930);

    play_effectwfvx(WAV_FIRE, 1200 + grand(10), 190, enemy[e].x);

}



void boss2_2_fire4(int e)
{

 int x = enemy[e].x - 33000;
 int y = enemy[e].y;
 int a = grand(ANGLE_1);

         int b = create_ebullet(EBULLET_B21SHOT);
         if (b != -1)
         {
          ebullet[b].x = x;
          ebullet[b].y = y;
          ebullet[b].x_speed = enemy[e].x_speed + xpart(a, 4300);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(a, 4300);
//          ebullet[b].angle = radians_to_angle(atan2(ebullet[b].y_speed, ebullet[b].x_speed));
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_4;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 40 + grand(10), CLOUD_COL_4, 930);
          ebullet[b].angle = a + grand(356) - grand(356);
         }

 x = enemy[e].x + 33000;
 a = grand(ANGLE_1);

         b = create_ebullet(EBULLET_B21SHOT);
         if (b != -1)
         {
          ebullet[b].x = x;
          ebullet[b].y = y;
          ebullet[b].x_speed = enemy[e].x_speed + xpart(a, 4300);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(a, 4300);
//          ebullet[b].angle = radians_to_angle(atan2(ebullet[b].y_speed, ebullet[b].x_speed));
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_4;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 40 + grand(10), CLOUD_COL_4, 930);
          ebullet[b].angle = a + grand(356) - grand(356);
         }

    play_effectwfvx(WAV_FIRE, 1000 + grand(10), 190, enemy[e].x);

}




/*

*************************************************************

Boss - stage 3 part 1

*************************************************************

*/



void run_boss3_1(int e)
{

    int i, speed, b;

 boss.phase_count --;
 boss.recycle --;
 if (boss.recycle < 0)
  boss.recycle = 0;
 boss.recycle2 --;
 if (boss.recycle2 < 0)
  boss.recycle2 = 0;

 if (boss.phase_count <= 0)
 {
  switch(boss.phase)
  {
   case 0: // entering
    boss.phase = 4;
    boss.phase_count = 250 + grand(100);
    boss.phase2 = 0;
    enemy[e].x_target [0] = RAND_X_IN;
    enemy[e].y_target [0] = 120000 + grand(180000);
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    break;
   case 1: // moving
    boss.phase = 2 + grand(3);
    boss.phase_count = 250 + grand(100);
    boss.phase2 = 1 + grand(3);
    break;
   case 2: // 2 etc are attack phases - boss sits still and fires
   case 3:
   case 4:
    if (grand(3) == 0)
    {
     boss.phase = 2 + grand(3);
     boss.phase_count = 250 + grand(100);
     boss.phase2 = 1 + grand(3);
     break;
    }
    boss.phase = 1;
    boss.phase_count = 150;// + grand(200);
    enemy[e].x_target [0] = RAND_X_IN;
    enemy[e].y_target [0] = 100000 + grand(120000);
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    enemy[e].burst = 4;
//    enemy[e].x_speed = 0;
//    enemy[e].y_speed = 0;
    break;
  }


 }

//   aim_at_target(e, enemy[e].x - 76000, enemy[e].y - 11000, player.x, player.y, 5, 0);
//   aim_at_target(e, enemy[e].x + 76000, enemy[e].y - 11000, player.x, player.y, 5, 1);

//   quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 53000, enemy[e].y + 50000, enemy[e].x_speed, enemy[e].y_speed + 4000 + grand(4000), 15 + grand(10), CLOUD_COL_1, 980);
//   quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 53000, enemy[e].y + 50000, enemy[e].x_speed, enemy[e].y_speed + 4000 + grand(4000), 15 + grand(10), CLOUD_COL_1, 980);


   aim_at_target(e, enemy[e].x - 106000, enemy[e].y + 65000, player.x, player.y, 8, 0);
   aim_at_target(e, enemy[e].x + 106000, enemy[e].y + 65000, player.x, player.y, 8, 1);


 switch(boss.phase)
 {
  //case 0: // entering;
//   break;
  case 1: // moving
    boss.phase2 = 0;
    if (enemy[e].x > enemy[e].x_target [0] - 20000
     && enemy[e].x < enemy[e].x_target [0] + 20000
     && enemy[e].y > enemy[e].y_target [0] - 20000
     && enemy[e].y < enemy[e].y_target [0] + 20000)
     {
      boss.phase_count = 0;
     }
     drag_enemy(e, 990);
     enemy[e].x_speed += xpart(enemy[e].angle, 130);
     enemy[e].y_speed += ypart(enemy[e].angle, 130);
   break;
  case 2: // triple attack
      if (boss.recycle <= 0 && enemy[e].x_speed == 0 && enemy[e].y_speed == 0)
      {
         if (enemy[e].burst <= 0)
         {
          enemy[e].angle1 = enemy[e].angle1 + 70 + grand(60);
          enemy[e].burst = 6 DIFF 1;
         }
         play_effectwfvx(WAV_SHOT, 2500 + grand(100), 90, enemy[e].x);
        for (i = 0; i < 5; i ++)
        {
         b = create_ebullet(EBULLET_SHOT);
         if (b != -1)
         {
          ebullet[b].angle = enemy[e].angle1 + i * ANGLE_5;
          ebullet[b].x_speed = xpart(ebullet[b].angle, 5000 + arena.difficulty * 1000);
          ebullet[b].y_speed = ypart(ebullet[b].angle, 5000 + arena.difficulty * 1000);
          ebullet[b].x = enemy[e].x - 56300 + xpart(ebullet[b].angle, 10000);
          ebullet[b].y = enemy[e].y + 7000 + ypart(ebullet[b].angle, 10000);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 30 + grand(10), CLOUD_COL_1, 950);
          ebullet[b].status = 0;//9 + grand (5);
         }
         b = create_ebullet(EBULLET_SHOT);
         if (b != -1)
         {
          ebullet[b].angle = ANGLE_2 - (enemy[e].angle1 + i * ANGLE_5);
          ebullet[b].x_speed = xpart(ebullet[b].angle, 5000 + arena.difficulty * 1000);
          ebullet[b].y_speed = ypart(ebullet[b].angle, 5000 + arena.difficulty * 1000);
          ebullet[b].x = enemy[e].x + 56300 + xpart(ebullet[b].angle, 10000);
          ebullet[b].y = enemy[e].y + 7000 + ypart(ebullet[b].angle, 10000);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 30 + grand(10), CLOUD_COL_1, 950);
          ebullet[b].status = 0;//9 + grand (5);
         }
        }

         boss.recycle = 5 DIFF 1;
         enemy[e].burst --;
      }

   break;
  case 3: // second attack - spirals of spinner3s
      if (boss.recycle <= 0 && enemy[e].x_speed == 0 && enemy[e].y_speed == 0)
      {
         play_effectwfvx(WAV_SHOT, 2500 + grand(100), 90, enemy[e].x);
        for (i = 0; i < 3; i ++)
        {
         b = create_ebullet(EBULLET_SPINNER3);
         if (b != -1)
         {
          ebullet[b].angle = enemy[e].angle1 + i * ANGLE_3;
          ebullet[b].x_speed = xpart(ebullet[b].angle, 2500);
          ebullet[b].y_speed = ypart(ebullet[b].angle, 2500);
          ebullet[b].x = enemy[e].x + 56300 + xpart(ebullet[b].angle, 10000);
          ebullet[b].y = enemy[e].y + 7000 + ypart(ebullet[b].angle, 10000);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_3;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 30 + grand(10), CLOUD_COL_3, 950);
          ebullet[b].status = -14 + enemy[e].burst * 2;
         }
         b = create_ebullet(EBULLET_SPINNER3);
         if (b != -1)
         {
          ebullet[b].angle = ANGLE_2 - (enemy[e].angle1 + i * ANGLE_3);
          ebullet[b].x_speed = xpart(ebullet[b].angle, 2500);
          ebullet[b].y_speed = ypart(ebullet[b].angle, 2500);
          ebullet[b].x = enemy[e].x - 56300 + xpart(ebullet[b].angle, 10000);
          ebullet[b].y = enemy[e].y + 7000 + ypart(ebullet[b].angle, 10000);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_3;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 30 + grand(10), CLOUD_COL_3, 950);
          ebullet[b].status = 14 - enemy[e].burst * 2;
         }
        }

         boss.recycle = 2;
         enemy[e].burst --;
         if (enemy[e].burst <= 0)
         {
          enemy[e].angle1 += ANGLE_8;
          enemy[e].burst = 4;
          boss.recycle = 15 DIFF 2;
         }
         enemy[e].angle1 += ANGLE_32;
      }

    break;
  case 4: // third attack - random blobs
      if (boss.recycle <= 0 && enemy[e].x_speed == 0 && enemy[e].y_speed == 0)
      {
         play_effectwfvx(WAV_SHOT, 2000 + grand(100), 90, enemy[e].x);
        for (i = 0; i < 3; i ++)
        {
         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].angle = grand(ANGLE_1);
          ebullet[b].status = 5 + grand(11);
          speed = 6000 - ebullet[b].status * 300;
          ebullet[b].x_speed = xpart(ebullet[b].angle, speed);
          ebullet[b].y_speed = ypart(ebullet[b].angle, speed);
          ebullet[b].x = enemy[e].x + 56300 + xpart(ebullet[b].angle, 10000);
          ebullet[b].y = enemy[e].y + 7000 + ypart(ebullet[b].angle, 10000);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_2;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 30 + grand(10), CLOUD_COL_2, 950);
         }
         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].angle = grand(ANGLE_1);
          ebullet[b].status = 5 + grand(11);
          speed = 6000 - ebullet[b].status * 300;
          ebullet[b].x_speed = xpart(ebullet[b].angle, speed);
          ebullet[b].y_speed = ypart(ebullet[b].angle, speed);
          ebullet[b].x = enemy[e].x - 56300 + xpart(ebullet[b].angle, 10000);
          ebullet[b].y = enemy[e].y + 7000 + ypart(ebullet[b].angle, 10000);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_2;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 30 + grand(10), CLOUD_COL_2, 950);
         }
        }

         boss.recycle = 8 DIFF 1;
      }

    break;
/*  case 1: // attack 1  - both red
   if (boss.recycle <= 0)
   {
    boss1_1_fire1(e, enemy[e].x - 76000, enemy[e].y - 11000, enemy[e].aim [0] + grand(24) - grand(24));
    boss.recycle = 4 + coin() DIFF 1;
   }
   if (boss.recycle2 <= 0)
   {
    boss1_1_fire1(e, enemy[e].x + 76000, enemy[e].y - 11000, enemy[e].aim [1] + grand(24) - grand(24));
    boss.recycle2 = 4 + coin() DIFF 1;
   }
   break;
  case 2: // attack 2 - both blue
   if (boss.recycle <= 0)
   {
    boss1_1_fire2(e, enemy[e].x - 76000, enemy[e].y - 11000, enemy[e].aim [0]);
    boss.recycle = 30 DIFF 7;
   }
   if (boss.recycle2 <= 0)
   {
    boss1_1_fire2(e, enemy[e].x + 76000, enemy[e].y - 11000, enemy[e].aim [1]);
    boss.recycle2 = 30 DIFF 7;
   }
   break;
  case 3: // attack 3 - red left, right blue
   if (boss.recycle <= 0)
   {
    boss1_1_fire1(e, enemy[e].x - 76000, enemy[e].y - 11000, enemy[e].aim [0] + grand(24) - grand(24));
    boss.recycle = 4 + coin() DIFF 1;
   }
   if (boss.recycle2 <= 0)
   {
    boss1_1_fire2(e, enemy[e].x + 76000, enemy[e].y - 11000, enemy[e].aim [1]);
    boss.recycle2 = 30 DIFF 7;
   }
   break;
  case 4: // attack 4 - reverse 3
   if (boss.recycle <= 0)
   {
    boss1_1_fire2(e, enemy[e].x - 76000, enemy[e].y - 11000, enemy[e].aim [0]);
    boss.recycle = 30 DIFF 7;
   }
   if (boss.recycle2 <= 0)
   {
    boss1_1_fire1(e, enemy[e].x + 76000, enemy[e].y - 11000, enemy[e].aim [1] + grand(24) - grand(24));
    boss.recycle2 = 4 + coin() DIFF 1;
   }
   break;
*/
 }

 switch(boss.phase2)
 {
  // 0 is nothing
  case 1: // attack 1
   if (boss.recycle2 <= 0)
   {
    play_effectwfvx(WAV_SHOT, 1000 + grand(100), 150, enemy[e].x);
    boss3_1_fire1(e, enemy[e].x - 108544, enemy[e].y + 66560, enemy[e].aim [0]);
    boss3_1_fire1(e, enemy[e].x + 108544, enemy[e].y + 66560, enemy[e].aim [1]);
    boss.recycle2 = 35 DIFF 7;
   }
   break;
  case 2: // attack 2
   if (boss.recycle2 <= 0)
   {
    play_effectwfvx(WAV_SHOT, 700 + grand(100), 150, enemy[e].x);
    boss3_1_fire2(e, enemy[e].x - 108544, enemy[e].y + 66560, enemy[e].aim [0]);
    boss3_1_fire2(e, enemy[e].x + 108544, enemy[e].y + 66560, enemy[e].aim [1]);
    boss.recycle2 = 55 DIFF 9;
   }
   break;
  case 3: // attack 3
   if (boss.recycle2 <= 0)
   {
    play_effectwfvx(WAV_SHOT, 1400 + grand(100), 150, enemy[e].x);
    boss3_1_fire3(e, enemy[e].x - 108544, enemy[e].y + 66560, enemy[e].aim [0]);
    boss3_1_fire3(e, enemy[e].x + 108544, enemy[e].y + 66560, enemy[e].aim [1]);
    boss.recycle2 = 11 DIFF 2;
   }
   break;

 }

 if (boss.phase > 1)
 {
   drag_enemy(e, 940);
   if (abs(enemy[e].x_speed) < 500 && abs(enemy[e].y_speed) < 500)
   {
    enemy[e].x_speed = 0;
    enemy[e].y_speed = 0;
   }
//    if (arena.counter
/*    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    enemy[e].x_speed += xpart(enemy[e].angle, 110);
    enemy[e].y_speed += ypart(enemy[e].angle, 110);
    if (enemy[e].x > enemy[e].x_target [0] - 20000
     && enemy[e].x < enemy[e].x_target [0] + 20000
     && enemy[e].y > enemy[e].y_target [0] - 20000
     && enemy[e].y < enemy[e].y_target [0] + 20000)
     {
      enemy[e].x_target [0] = RAND_X_IN;
      enemy[e].y_target [0] = 120000 + grand(180000);
      enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
//      enemy[e].x_speed = 0;
//      enemy[e].y_speed = 0;
     }*/
 }

    move_enemy(e);

}


void boss3_1_fire1(int e, int x, int y, int angle)
{


         int b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = x + xpart(angle, 9000);
          ebullet[b].y = y + ypart(angle, 9000);
          ebullet[b].x_speed = xpart(angle, 4000);
          ebullet[b].y_speed = ypart(angle, 4000);
          ebullet[b].angle = angle;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_1, 930);
          ebullet[b].status = 9;
         }

         int angle1 = angle - ANGLE_32;

         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = x + xpart(angle, 9000);
          ebullet[b].y = y + ypart(angle, 9000);
          ebullet[b].x_speed = xpart(angle1, 3000);
          ebullet[b].y_speed = ypart(angle1, 3000);
          ebullet[b].angle = angle1;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
//          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_1, 930);
          ebullet[b].status = 6;
         }

         angle1 += ANGLE_16;


         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = x + xpart(angle, 9000);
          ebullet[b].y = y + ypart(angle, 9000);
          ebullet[b].x_speed = xpart(angle1, 3000);
          ebullet[b].y_speed = ypart(angle1, 3000);
          ebullet[b].angle = angle1;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
//          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_1, 930);
          ebullet[b].status = 6;
         }


}



void boss3_1_fire2(int e, int x, int y, int angle)
{

 int i, b;

 for (i = 0; i < 7; i ++)
 {

         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = x + xpart(angle, 9000);
          ebullet[b].y = y + ypart(angle, 9000);
          ebullet[b].x_speed = xpart(angle, 5000 - i * 300);
          ebullet[b].y_speed = ypart(angle, 5000 - i * 300);
          ebullet[b].angle = angle;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_4;
          ebullet[b].status = 12 - i;
         }
 }

// b still valid
 if (b != -1)
  quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 40 + grand(20), CLOUD_COL_4, 930);

}

void boss3_1_fire3(int e, int x, int y, int angle)
{

 int b, size;

         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = x + xpart(angle, 9000);
          ebullet[b].y = y + ypart(angle, 9000);
          size = 5 + grand(11);
          ebullet[b].x_speed = xpart(angle, 6000 - size * 250);
          ebullet[b].y_speed = ypart(angle, 6000 - size * 250);
          ebullet[b].angle = angle;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_3;
          ebullet[b].status = size;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 40 + grand(20), CLOUD_COL_3, 930);
         }



}




/*

*************************************************************

Boss - stage 3 part 2

*************************************************************

*/



void run_boss3_2(int e)
{

    int i, speed, angle, b;

 boss.phase_count --;
 boss.recycle --;
 if (boss.recycle < 0)
  boss.recycle = 0;
 boss.recycle2 --;
 if (boss.recycle2 < 0)
  boss.recycle2 = 0;

 if (boss.phase_count <= 0)
 {
  switch(boss.phase)
  {
   case 0: // entering
    boss.phase = 4;
    boss.phase_count = 250 + grand(100);
    boss.phase2 = 0;
    enemy[e].x_target [0] = RAND_X_IN;
    enemy[e].y_target [0] = 120000 + grand(180000);
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    break;
   case 1: // moving
    boss.phase = 2;
    if (coin())
     boss.phase = 4;
    if (boss.hp < 50000)
     boss.phase = 3;
    boss.phase_count = 250 + grand(100);
    boss.phase2 = 1 + grand(3);
    break;
   case 3:
    if (enemy[e].burst > 0)
     break;
// otherwise fall through...
   case 2: // 2 etc are attack phases - boss sits still and fires
   case 4:
    if (grand(3) == 0)
    {
     boss.phase = 2 + grand(3);
     boss.phase_count = 250 + grand(100);
     boss.phase2 = 1 + grand(3);
     break;
    }
    boss.phase = 1;
    boss.phase_count = 150;// + grand(200);
    enemy[e].x_target [0] = RAND_X_IN;
    enemy[e].y_target [0] = 100000 + grand(120000);
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    enemy[e].burst = 4;

    break;
  }


 }

   aim_at_target(e, enemy[e].x - 79872, enemy[e].y + 8192, player.x, player.y, 8, 0);
   aim_at_target(e, enemy[e].x + 79872, enemy[e].y + 8192, player.x, player.y, 8, 1);


 switch(boss.phase)
 {
  //case 0: // entering;
//   break;
  case 1: // moving
    boss.phase2 = 0;
    if (enemy[e].x > enemy[e].x_target [0] - 20000
     && enemy[e].x < enemy[e].x_target [0] + 20000
     && enemy[e].y > enemy[e].y_target [0] - 20000
     && enemy[e].y < enemy[e].y_target [0] + 20000)
     {
      boss.phase_count = 0;
     }
     drag_enemy(e, 990);
     enemy[e].x_speed += xpart(enemy[e].angle, 130);
     enemy[e].y_speed += ypart(enemy[e].angle, 130);
     if (boss.recycle <= 0)
     {
              b = create_ebullet(EBULLET_STOPMINE);
              if (b != -1)
              {
               angle = grand(ANGLE_1);
               speed = 4000 + grand(15000);
               ebullet[b].x = enemy[e].x + xpart(angle, 10000);
               ebullet[b].y = enemy[e].y + ypart(angle, 10000);
               ebullet[b].x_speed = xpart(angle, speed);
               ebullet[b].y_speed = ypart(angle, speed);
               ebullet[b].timeout = 200;
               ebullet[b].colour = CLOUD_COL_3;
/*               if (b & 1)
                ebullet[b].status = -8;
                 else
                  ebullet[b].status = 8;*/
               ebullet[b].status = 5 + grand(10);
               if (coin())
                ebullet[b].status *= -1;
              }
            quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y, enemy[e].x_speed, enemy[e].y_speed, 30 + grand(16), CLOUD_COL_4, 0);
            boss.recycle = 5 DIFF 1;
     }
   break;
  case 2: // triple attack
      if (boss.recycle <= 0 && enemy[e].x_speed == 0 && enemy[e].y_speed == 0)
      {
         if (enemy[e].burst <= 0)
         {
          enemy[e].angle1 = enemy[e].angle1 + 30 + grand(25);
          enemy[e].burst = 4;
         }
         play_effectwfvx(WAV_SHOT, 2500 + grand(100), 90, enemy[e].x);
        for (i = 0; i < 9; i ++)
        {
         b = create_ebullet(EBULLET_BLOBSHOT);
         if (b != -1)
         {
          ebullet[b].angle = enemy[e].angle1 + i * ANGLE_9;
          ebullet[b].x_speed = xpart(ebullet[b].angle, 7000 + arena.difficulty * 500);
          ebullet[b].y_speed = ypart(ebullet[b].angle, 7000 + arena.difficulty * 500);
          ebullet[b].x = enemy[e].x + xpart(ebullet[b].angle, 10000);
          ebullet[b].y = enemy[e].y + ypart(ebullet[b].angle, 10000);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 30 + grand(10), CLOUD_COL_1, 950);
          ebullet[b].status = 9 + grand (5);
         }
        }

         boss.recycle = 5 DIFF 1;
         enemy[e].burst --;
      }

   break;
  case 3: // second attack - triple beam
      if (enemy[e].x_speed == 0 && enemy[e].y_speed == 0)
      {
       if (boss.recycle <= 0)
       {
           enemy[e].burst = 120;
           boss.recycle = 122;
           enemy[e].angle1 = grand(ANGLE_1);
           play_effectwfvx(WAV_BEAM3, 220, 130, enemy[e].x);
           if (boss.beam == 2)
            boss.beam = -2;
             else
              boss.beam = 2;
       }
       if (enemy[e].burst > 0)
       {
          for (i = 0; i < 3; i ++)
          {
             b = create_ebullet(EBULLET_BEAM);
             if (b != -1)
             {
              ebullet[b].angle = enemy[e].angle1 + i * ANGLE_3;
              ebullet[b].x = enemy[e].x + xpart(ebullet[b].angle, 10000);
              ebullet[b].y = enemy[e].y + ypart(ebullet[b].angle, 10000);
              ebullet[b].timeout = 2;
              ebullet[b].colour = 3;
              ebullet[b].status = enemy[e].burst;
             }
          }
          enemy[e].angle1 += boss.beam;
          enemy[e].burst --;
      }

      }
    break;
  case 4: // third attack - random blobs
      if (boss.recycle <= 0 && enemy[e].x_speed == 0 && enemy[e].y_speed == 0)
      {
         play_effectwfvx(WAV_SHOT, 2000 + grand(100), 90, enemy[e].x);
         int b2, b3;
        for (i = 0; i < 3; i ++)
        {
         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].angle = grand(ANGLE_1);
          ebullet[b].status = 5 + grand(7);
          speed = 6000 - ebullet[b].status * 270;
          ebullet[b].x_speed = xpart(ebullet[b].angle, speed);
          ebullet[b].y_speed = ypart(ebullet[b].angle, speed);
          ebullet[b].x = enemy[e].x + xpart(ebullet[b].angle, 10000);
          ebullet[b].y = enemy[e].y + ypart(ebullet[b].angle, 10000);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_2;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 30 + grand(10), CLOUD_COL_2, 950);
          b2 = create_ebullet(EBULLET_BLOB);
          if (b2 != -1)
          {
           ebullet[b2].angle = ebullet[b].angle;
           ebullet[b2].status = ebullet[b].status + 3;
           speed = 6500 - ebullet[b].status * 270;
           ebullet[b2].x_speed = xpart(ebullet[b2].angle, speed);
           ebullet[b2].y_speed = ypart(ebullet[b2].angle, speed);
           ebullet[b2].x = enemy[e].x + xpart(ebullet[b2].angle, 10000);
           ebullet[b2].y = enemy[e].y + ypart(ebullet[b2].angle, 10000);
           ebullet[b2].timeout = 4000;
           ebullet[b2].colour = CLOUD_COL_2;
           b3 = create_ebullet(EBULLET_BLOB);
           if (b3 != -1)
           {
            ebullet[b3].angle = ebullet[b2].angle;
            ebullet[b3].status = ebullet[b2].status + 3;
            speed = 8000 - ebullet[b2].status * 270;
            ebullet[b3].x_speed = xpart(ebullet[b3].angle, speed);
            ebullet[b3].y_speed = ypart(ebullet[b3].angle, speed);
            ebullet[b3].x = enemy[e].x + xpart(ebullet[b3].angle, 10000);
            ebullet[b3].y = enemy[e].y + ypart(ebullet[b3].angle, 10000);
            ebullet[b3].timeout = 4000;
            ebullet[b3].colour = CLOUD_COL_2;
           }
          }
         }
        }

         boss.recycle = 9 DIFF 1;
      }

    break;
 }

 switch(boss.phase2)
 {
  // 0 is nothing
  case 1: // attack 1
   if (boss.recycle2 <= 0)
   {
    play_effectwfvx(WAV_SHOT, 1000 + grand(100), 150, enemy[e].x);
    boss3_2_fire1(e, enemy[e].x - 79872, enemy[e].y - 8192, enemy[e].aim [0]);
    boss3_2_fire1(e, enemy[e].x + 79872, enemy[e].y - 8192, enemy[e].aim [1]);
    boss.recycle2 = 35 DIFF 7;
   }
   break;
  case 2: // attack 2
   if (boss.recycle2 <= 0)
   {
    play_effectwfvx(WAV_SHOT, 700 + grand(100), 150, enemy[e].x);
    boss3_2_fire2(e, enemy[e].x - 79872, enemy[e].y - 8192, enemy[e].aim [0]);
    boss3_2_fire2(e, enemy[e].x + 79872, enemy[e].y - 8192, enemy[e].aim [1]);
    boss.recycle2 = 64 DIFF 9;
   }
   break;
  case 3: // attack 3
   if (boss.recycle2 <= 0)
   {
    play_effectwfvx(WAV_SHOT, 1400 + grand(100), 150, enemy[e].x);
    boss3_2_fire3(e, enemy[e].x - 79872, enemy[e].y - 8192, enemy[e].aim [0]);
    boss3_2_fire3(e, enemy[e].x + 79872, enemy[e].y - 8192, enemy[e].aim [1]);
    boss.recycle2 = 11 DIFF 2;
   }
   break;

 }

 if (boss.phase > 1)
 {
   drag_enemy(e, 940);
   if (abs(enemy[e].x_speed) < 500 && abs(enemy[e].y_speed) < 500)
   {
    enemy[e].x_speed = 0;
    enemy[e].y_speed = 0;
   }
 }

    move_enemy(e);

}



void boss3_2_fire1(int e, int x, int y, int angle)
{


         int b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = x + xpart(angle, 9000);
          ebullet[b].y = y + ypart(angle, 9000);
          ebullet[b].x_speed = xpart(angle, 4000);
          ebullet[b].y_speed = ypart(angle, 4000);
          ebullet[b].angle = angle;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_1, 930);
          ebullet[b].status = 9;
         }

         int angle1 = angle - ANGLE_16;

         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = x + xpart(angle, 9000);
          ebullet[b].y = y + ypart(angle, 9000);
          ebullet[b].x_speed = xpart(angle1, 3000);
          ebullet[b].y_speed = ypart(angle1, 3000);
          ebullet[b].angle = angle1;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
//          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_1, 930);
          ebullet[b].status = 6;
         }

         angle1 += ANGLE_8;


         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = x + xpart(angle, 9000);
          ebullet[b].y = y + ypart(angle, 9000);
          ebullet[b].x_speed = xpart(angle1, 3000);
          ebullet[b].y_speed = ypart(angle1, 3000);
          ebullet[b].angle = angle1;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
//          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_1, 930);
          ebullet[b].status = 6;
         }


}



void boss3_2_fire2(int e, int x, int y, int angle)
{

 int i, b;

 for (i = 0; i < 7; i ++)
 {

         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = x + xpart(angle, 9000);
          ebullet[b].y = y + ypart(angle, 9000);
          ebullet[b].x_speed = xpart(angle, 5000 - i * 300);
          ebullet[b].y_speed = ypart(angle, 5000 - i * 300);
          ebullet[b].angle = angle;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_4;
          ebullet[b].status = 12 - i;
         }
 }

// b still valid
 if (b != -1)
  quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 40 + grand(20), CLOUD_COL_4, 930);

}

void boss3_2_fire3(int e, int x, int y, int angle)
{

 int b, size;

         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = x + xpart(angle, 9000);
          ebullet[b].y = y + ypart(angle, 9000);
          size = 5 + grand(11);
          angle += grand(ANGLE_32) - grand(ANGLE_32);
          ebullet[b].x_speed = xpart(angle, 6000 - size * 250);
          ebullet[b].y_speed = ypart(angle, 6000 - size * 250);
          ebullet[b].angle = angle;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_3;
          ebullet[b].status = size;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 40 + grand(20), CLOUD_COL_3, 930);
         }



}





/*

*************************************************************

Boss - stage 3 part 0

*************************************************************

*/



void run_boss3_0(int e)
{

    int i, angle, b;

 boss.phase_count --;
 boss.recycle --;
 if (boss.recycle < 0)
  boss.recycle = 0;
 boss.recycle2 --;
 if (boss.recycle2 < 0)
  boss.recycle2 = 0;

 if (boss.phase_count <= 0)
 {
  switch(boss.phase)
  {
   case 0: // entering
    boss.phase = 2;
    boss.phase_count = 350 + grand(100);
//    boss.phase2 = 0;
    enemy[e].x_target [0] = RAND_X_IN;
    enemy[e].y_target [0] = 120000 + grand(180000);
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    break;
   case 1: // moving
    boss.phase = 2 + grand(3);
    boss.phase_count = 350 + grand(100);
    boss.phase2 = 1 + grand(3);
    break;
   case 2: // 2 etc are attack phases - boss sits still and fires
   case 3:
   case 4:
    if (grand(3) == 0)
    {
     boss.phase = 2 + grand(3);
     boss.phase_count = 350 + grand(100);
     boss.phase2 = 1 + grand(3);
     break;
    }
    boss.phase = 1;
    boss.phase_count = 150;// + grand(200);
    enemy[e].x_target [0] = RAND_X_IN;
    enemy[e].y_target [0] = 100000 + grand(120000);
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    enemy[e].burst = 4;
//    enemy[e].x_speed = 0;
//    enemy[e].y_speed = 0;
    break;
  }


 }

//   aim_at_target(e, enemy[e].x - 76000, enemy[e].y - 11000, player.x, player.y, 5, 0);
//   aim_at_target(e, enemy[e].x + 76000, enemy[e].y - 11000, player.x, player.y, 5, 1);

//   quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 53000, enemy[e].y + 50000, enemy[e].x_speed, enemy[e].y_speed + 4000 + grand(4000), 15 + grand(10), CLOUD_COL_1, 980);
//   quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 53000, enemy[e].y + 50000, enemy[e].x_speed, enemy[e].y_speed + 4000 + grand(4000), 15 + grand(10), CLOUD_COL_1, 980);


//   aim_at_target(e, enemy[e].x - 106000, enemy[e].y + 65000, player.x, player.y, 8, 0);
//   aim_at_target(e, enemy[e].x + 106000, enemy[e].y + 65000, player.x, player.y, 8, 1);


 switch(boss.phase)
 {
  //case 0: // entering;
//   break;
  case 1: // moving
    boss.phase2 = 0;
    if (enemy[e].x > enemy[e].x_target [0] - 20000
     && enemy[e].x < enemy[e].x_target [0] + 20000
     && enemy[e].y > enemy[e].y_target [0] - 20000
     && enemy[e].y < enemy[e].y_target [0] + 20000)
     {
      boss.phase_count = 0;
     }
     drag_enemy(e, 990);
     enemy[e].x_speed += xpart(enemy[e].angle, 130);
     enemy[e].y_speed += ypart(enemy[e].angle, 130);
   break;
  case 2: // triple attack
   if (boss.recycle <= 0)
   {
   enemy[e].angle += xpart(enemy[e].angle1, 45) + 20;
   enemy[e].angle1 += 27 + grand(5);
   enemy[e].angle1 &= 1023;
   enemy[e].angle &= 1023;
   angle = enemy[e].angle;
   play_effectwfvx(WAV_CHIRP, 700 + grand(100), 130, enemy[e].x);
   for (i = 0; i < 3; i ++)
   {
         angle = enemy[e].angle + ANGLE_3 * i;

         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x - 71000 + xpart(angle, 9000);
          ebullet[b].y = enemy[e].y - 12000 + ypart(angle, 9000);
          ebullet[b].x_speed = xpart(angle, 3000);
          ebullet[b].y_speed = ypart(angle, 3000);
          ebullet[b].angle = angle;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_1, 930);
          ebullet[b].status = 9 + xpart(enemy[e].angle1, 4);
         }

         angle = ANGLE_1 - (enemy[e].angle + ANGLE_3 * i);

         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x + 71000 + xpart(angle, 9000);
          ebullet[b].y = enemy[e].y - 12000 + ypart(angle, 9000);
          ebullet[b].x_speed = xpart(angle, 3000);
          ebullet[b].y_speed = ypart(angle, 3000);
          ebullet[b].angle = angle;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_1, 930);
          ebullet[b].status = 9 + xpart(enemy[e].angle1, 4);
         }


   }
    boss.recycle = 6 DIFF 1;
   }
   break;
  case 3: // triple attack
   if (boss.recycle <= 0)
   {
   enemy[e].angle += xpart(enemy[e].angle1, 35) - 10;
   enemy[e].angle1 += 22 + grand(3);
   enemy[e].angle1 &= 1023;
   enemy[e].angle &= 1023;
   angle = enemy[e].angle;
   play_effectwfvx(WAV_CHIRP, 990 + grand(100), 130, enemy[e].x);
   for (i = 0; i < 5; i ++)
   {
         angle = enemy[e].angle + ANGLE_5 * i;

         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x - 71000 + xpart(angle, 9000);
          ebullet[b].y = enemy[e].y - 12000 + ypart(angle, 9000);
          ebullet[b].x_speed = xpart(angle, 3500);
          ebullet[b].y_speed = ypart(angle, 3500);
          ebullet[b].angle = angle;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_2;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_2, 930);
          ebullet[b].status = 9 + xpart(enemy[e].angle1, 4);
         }

         angle = ANGLE_1 - (enemy[e].angle + ANGLE_5 * i);

         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x + 71000 + xpart(angle, 9000);
          ebullet[b].y = enemy[e].y - 12000 + ypart(angle, 9000);
          ebullet[b].x_speed = xpart(angle, 3500);
          ebullet[b].y_speed = ypart(angle, 3500);
          ebullet[b].angle = angle;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_2;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_2, 930);
          ebullet[b].status = 9 + xpart(enemy[e].angle1, 4);
         }


   }
    boss.recycle = 7 DIFF 1;
   }
   break;
  case 4: // triple attack
   if (boss.recycle <= 0)
   {
   enemy[e].angle += xpart(enemy[e].angle1, 25) + 16;
   enemy[e].angle1 += 20 + grand(3);
   enemy[e].angle1 &= 1023;
   enemy[e].angle &= 1023;
   angle = enemy[e].angle;
   play_effectwfvx(WAV_CHIRP, 1200 + grand(100), 130, enemy[e].x);
   for (i = 0; i < 4; i ++)
   {
         angle = enemy[e].angle + ANGLE_4 * i;

         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x - 71000 + xpart(angle, 9000);
          ebullet[b].y = enemy[e].y - 12000 + ypart(angle, 9000);
          ebullet[b].x_speed = xpart(angle, 5000);
          ebullet[b].y_speed = ypart(angle, 5000);
          ebullet[b].angle = angle;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_3;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_3, 930);
          ebullet[b].status = 7 + xpart(enemy[e].angle1, 3);
         }

         angle = ANGLE_1 - (enemy[e].angle + ANGLE_4 * i);

         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x + 71000 + xpart(angle, 9000);
          ebullet[b].y = enemy[e].y - 12000 + ypart(angle, 9000);
          ebullet[b].x_speed = xpart(angle, 5000);
          ebullet[b].y_speed = ypart(angle, 5000);
          ebullet[b].angle = angle;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_3;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_3, 930);
          ebullet[b].status = 7 + xpart(enemy[e].angle1, 3);
         }


   }
    boss.recycle = 8 DIFF 1;
   }
   break;
 }

 if (boss.phase > 1)
 {
   drag_enemy(e, 940);
   if (abs(enemy[e].x_speed) < 500 && abs(enemy[e].y_speed) < 500)
   {
    enemy[e].x_speed = 0;
    enemy[e].y_speed = 0;
   }
 }

    move_enemy(e);

}


void boss3_0_fire1(int e, int x, int y, int angle)
{


         int b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = x + xpart(angle, 9000);
          ebullet[b].y = y + ypart(angle, 9000);
          ebullet[b].x_speed = xpart(angle, 4000);
          ebullet[b].y_speed = ypart(angle, 4000);
          ebullet[b].angle = angle;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_1, 930);
          ebullet[b].status = 9;
         }

         int angle1 = angle - ANGLE_32;

         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = x + xpart(angle, 9000);
          ebullet[b].y = y + ypart(angle, 9000);
          ebullet[b].x_speed = xpart(angle1, 3000);
          ebullet[b].y_speed = ypart(angle1, 3000);
          ebullet[b].angle = angle1;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
//          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_1, 930);
          ebullet[b].status = 6;
         }

         angle1 += ANGLE_16;


         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = x + xpart(angle, 9000);
          ebullet[b].y = y + ypart(angle, 9000);
          ebullet[b].x_speed = xpart(angle1, 3000);
          ebullet[b].y_speed = ypart(angle1, 3000);
          ebullet[b].angle = angle1;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
//          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(10), CLOUD_COL_1, 930);
          ebullet[b].status = 6;
         }


}



void boss3_0_fire2(int e, int x, int y, int angle)
{

 int i, b;

 for (i = 0; i < 7; i ++)
 {

         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = x + xpart(angle, 9000);
          ebullet[b].y = y + ypart(angle, 9000);
          ebullet[b].x_speed = xpart(angle, 5000 - i * 300);
          ebullet[b].y_speed = ypart(angle, 5000 - i * 300);
          ebullet[b].angle = angle;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_4;
          ebullet[b].status = 12 - i;
         }
 }

// b still valid
 if (b != -1)
  quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 40 + grand(20), CLOUD_COL_4, 930);

}

void boss3_0_fire3(int e, int x, int y, int angle)
{

 int b, size;

         b = create_ebullet(EBULLET_BLOB);
         if (b != -1)
         {
          ebullet[b].x = x + xpart(angle, 9000);
          ebullet[b].y = y + ypart(angle, 9000);
          size = 5 + grand(11);
          ebullet[b].x_speed = xpart(angle, 6000 - size * 250);
          ebullet[b].y_speed = ypart(angle, 6000 - size * 250);
          ebullet[b].angle = angle;
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_3;
          ebullet[b].status = size;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 40 + grand(20), CLOUD_COL_3, 930);
         }



}




/*

*************************************************************

END OF BOSSES

*************************************************************

*/





void move_dswooper(int e)
{

 enemy[e].recycle --;
 if (enemy[e].recycle == 0)
 {
  if (enemy[e].y < 470000)
  {
   enemy_fire(e);
  }
  enemy[e].recycle = 100;
 }

     switch(enemy[e].type)
     {
     }


 if (enemy [e].target_time > 0 && enemy [e].x_target [enemy [e].target] != -1)
  enemy [e].target_time --;
   else
   {
    if (enemy [e].x_target [enemy [e].target] != -1)
    {
     enemy [e].target_time = enemy [e].max_target_time;
     enemy [e].target ++;
     enemy[e].recycle = 30;
    }
   }


    if (enemy [e].x / 15000 == enemy [e].x_target [enemy [e].target] / 15000
      && enemy [e].y / 15000 == enemy [e].y_target [enemy [e].target] / 15000)
    {
     if (enemy [e].x_target [enemy [e].target] != -1)
     {
      enemy [e].target_time = enemy [e].max_target_time;
      enemy [e].target ++;
//      enemy[e].recycle = 30;
     }
    }

    if (enemy [e].x_target [enemy [e].target] != -1 && enemy [e].y < 470000)
    {
     track_target(e, enemy [e].x_target [enemy [e].target], enemy [e].y_target [enemy [e].target], enemy [e].angle1, eclass[enemy [e].type].speed2);
     enemy [e].angle1 &= 1023;
    }
    enemy [e].x_speed = xpart (enemy [e].angle1, eclass[enemy [e].type].speed1);
    enemy [e].y_speed = ypart (enemy [e].angle1, eclass[enemy [e].type].speed1);

    if (enemy[e].x_target [enemy[e].target] == -1)
     enemy[e].persist = 0;

    move_enemy(e);

}



void move_dancer(int e)
{

 char ready_to_fire = 0;
 int angle, b, speed, i;

 if (enemy[e].recycle > 0)
  enemy[e].recycle --;
 if (enemy[e].recycle <= 0)
 {
  if (enemy[e].y > 3000 && enemy[e].x > 3000 && enemy[e].x < 597000 && enemy[e].y < 590000)
  {
     ready_to_fire = 1;
  }
 }

     switch(enemy[e].type)
     {
       case ENEMY_FIGHTER4:
        if (enemy[e].recycle <= 45 && enemy[e].recycle > 0)// && ready_to_fire)
        {
         drag_enemy(e, 960);
         move_enemy(e);
         return;
        }
        if (enemy[e].recycle <= 0 && ready_to_fire == 1)
        {
         b = create_ebullet(EBULLET_SHOT2);
         play_effectwfvx(WAV_FIRE, 1200 + grand(300), 140, enemy[e].x);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x;
          ebullet[b].y = enemy[e].y + 4000;
          ebullet[b].angle = attack_angle(e);
          ebullet[b].x_speed = xpart(ebullet[b].angle, 7500);
          ebullet[b].y_speed = ypart(ebullet[b].angle, 7500);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_3;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, enemy[e].x_speed, enemy[e].y_speed, 30 + grand(10), CLOUD_COL_3, 975);
          for (i = 0; i < 4; i ++)
          {
           angle = ebullet[b].angle + ANGLE_4 + grand(100);
           speed = 3000 + grand(1000);
           quick_cloud(CLOUD_FCIRCLE4, ebullet[b].x, ebullet[b].y, enemy[e].x_speed + xpart(angle, speed), enemy[e].y_speed + ypart(angle, speed), 30 + grand(10), CLOUD_COL_3, 940);
           angle = ebullet[b].angle - ANGLE_4 - grand(100);
           speed = 3000 + grand(1000);
           quick_cloud(CLOUD_FCIRCLE4, ebullet[b].x, ebullet[b].y, enemy[e].x_speed + xpart(angle, speed), enemy[e].y_speed + ypart(angle, speed), 30 + grand(10), CLOUD_COL_3, 940);
          }
         }
         enemy[e].recycle = 90 DIFF 15;
//         enemy[e].status = 20 + grand(10);
        }
        break;

        case ENEMY_FIGHTER7:
         angle = attack_angle(e);
         if (enemy[e].recycle <= 0 && ready_to_fire == 1)
         {
              play_effectwfvx(WAV_CHIRP, 800 + grand(100), 190, enemy[e].x);
              b = create_ebullet(EBULLET_SPINNER3);
              if (b != -1)
              {
               ebullet[b].x = enemy[e].x - 10000;
               ebullet[b].y = enemy[e].y - 3000;
               speed = angle - 36;
               if (e & 1)
               {
                ebullet[b].status = 14;
                speed = angle + 36;
               }
                 else
                  ebullet[b].status = -14;
               ebullet[b].x_speed = xpart(speed, 3200);
               ebullet[b].y_speed = ypart(speed, 3200);
               ebullet[b].timeout = 4000;
               ebullet[b].colour = CLOUD_COL_4;
               quick_cloud(CLOUD_FCIRCLE1, ebullet[b].x, ebullet[b].y, enemy[e].x_speed, enemy[e].y_speed, 25 + grand(5), CLOUD_COL_3, 900);
              }
              b = create_ebullet(EBULLET_SPINNER3);
              if (b != -1)
              {
               ebullet[b].x = enemy[e].x + 10000;
               ebullet[b].y = enemy[e].y - 3000;
               speed = angle + 36;
               if (e & 1)
               {
                ebullet[b].status = -14;
                speed = angle - 36;
               }
                 else
                  ebullet[b].status = 14;
               ebullet[b].x_speed = xpart(speed, 3200);
               ebullet[b].y_speed = ypart(speed, 3200);
               ebullet[b].timeout = 4000;
               ebullet[b].colour = CLOUD_COL_4;
               quick_cloud(CLOUD_FCIRCLE1, ebullet[b].x, ebullet[b].y, enemy[e].x_speed, enemy[e].y_speed, 25 + grand(5), CLOUD_COL_3, 900);
              }
           enemy[e].recycle = 80 DIFF 15;
         }
         break;

       case ENEMY_FIGHTER8:
        angle = attack_angle(e);
        if (enemy[e].recycle <= 0 && ready_to_fire == 1)
        {
         b = create_ebullet(EBULLET_DRAGGER);
         play_effectwfvx(WAV_CHIRP, 1600 + grand(200), 120, enemy[e].x);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x;
          ebullet[b].y = enemy[e].y + 3000;
          ebullet[b].angle = angle + grand(36) - grand(36);
          ebullet[b].x_speed = enemy[e].x_speed + xpart(ebullet[b].angle, 9000);//xpart(ebullet[b].angle, 7000);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(ebullet[b].angle, 9000);//ypart(ebullet[b].angle, 7000);
          ebullet[b].angle = radians_to_angle(atan2(ebullet[b].y_speed, ebullet[b].x_speed));
          ebullet[b].timeout = 80;
          ebullet[b].colour = CLOUD_COL_1;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, enemy[e].x_speed, enemy[e].y_speed, 30 + grand(10), CLOUD_COL_1, 950);
         }
//         enemy[e].recycle = 20 DIFF 4;

         enemy[e].burst --;
         if (enemy[e].burst <= 0)
         {
          enemy[e].recycle = 55 DIFF 15;
          enemy[e].burst = 3 + arena.difficulty * 2;
         }
          else
           enemy[e].recycle = 6 + arena.difficulty * 2;

        }
        break;


        case ENEMY_FIGHTER6:
         if (enemy[e].recycle <= 0 && ready_to_fire == 1)
         {
           play_effectwfvx(WAV_INTER, 900 + grand(100), 190, enemy[e].x);
           b = create_ebullet(EBULLET_FLAKE3);
           if (b != -1)
           {
            ebullet[b].x = enemy[e].x;
            ebullet[b].y = enemy[e].y;
            angle = radians_to_angle(atan2(player.y - (enemy[e].y), player.x - (enemy[e].x)));
            quick_cloud(CLOUD_FCIRCLE1, enemy[e].x, enemy[e].y, enemy[e].x_speed, enemy[e].y_speed, 25 + grand(5), CLOUD_COL_3, 950);
            speed = 3200;
            ebullet[b].x_speed = xpart(angle, speed);// + enemy[e].x_speed;
            ebullet[b].y_speed = ypart(angle, speed);// + enemy[e].y_speed;
            ebullet[b].timeout = 4000; // important!
            ebullet[b].status = 1;
            ebullet[b].colour = CLOUD_COL_3;
            ebullet[b].angle2 = coin(); // rotation direction
           }
           enemy[e].recycle = 80 DIFF 15;
         }
         break;

       case ENEMY_FIGHTER:
        if (enemy[e].recycle == 15)
         enemy[e].frame = 1;
        if (enemy[e].recycle == 5)
         enemy[e].frame = 2;
        if (enemy[e].recycle == 55 DIFF 12)
         enemy[e].frame = 1;
        if (enemy[e].recycle == 45 DIFF 12)
         enemy[e].frame = 0;
        if (enemy[e].recycle <= 0 && ready_to_fire == 1)
        {
         play_effectwfvx(WAV_CHIRP, 1500 + grand(200), 120, enemy[e].x);
         b = create_ebullet(EBULLET_SHOT);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x;
          ebullet[b].y = enemy[e].y;
          ebullet[b].angle = attack_angle(e);
          ebullet[b].x_speed = xpart(ebullet[b].angle, 3000);
          ebullet[b].y_speed = ypart(ebullet[b].angle, 3000);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_2;
//          ebullet[b].drag = 0;
//          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, enemy[e].x_speed, enemy[e].y_speed, 30 + grand(10), CLOUD_COL_2, 950);
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 30 + grand(10), CLOUD_COL_2, 950);
          ebullet[b].status = 12 + grand (8);
         }
         enemy[e].recycle = 60 DIFF 12;
         enemy[e].status = 20 + grand(10);
        }
        if (enemy[e].status > 0)
        {
         if (enemy[e].status & 1)
         {
          quick_cloud(CLOUD_FCIRCLE4, enemy[e].x, enemy[e].y, enemy[e].x_speed, enemy[e].y_speed, 10 + grand(10) + enemy[e].status, CLOUD_COL_2, 992);
         }
         enemy[e].status --;
        }
        break;
       case ENEMY_FIGHTER2:
        if (enemy[e].recycle == 25)
         enemy[e].frame = 1;
        if (enemy[e].recycle == 15)
         enemy[e].frame = 2;
        if (enemy[e].recycle == 10)
         enemy[e].frame = 3;

        if (enemy[e].recycle == 170 DIFF 30)
         enemy[e].frame = 2;
        if (enemy[e].recycle == 155 DIFF 30)
         enemy[e].frame = 1;
        if (enemy[e].recycle == 140 DIFF 30)
         enemy[e].frame = 0;
        if (enemy[e].recycle <= 0 && ready_to_fire == 1)
        {
         play_effectwfvx(WAV_CHIRP, 2200 + grand(200), 90, enemy[e].x);
         b = create_ebullet(EBULLET_IBULLET);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x;
          ebullet[b].y = enemy[e].y;
          ebullet[b].angle = attack_angle(e) + grand(50) - grand(50);
          speed = 2500 + grand(1000);
          ebullet[b].x_speed = xpart(ebullet[b].angle, speed);
          ebullet[b].y_speed = ypart(ebullet[b].angle, speed);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_2;
          ebullet[b].status = 0;
          quick_cloud(CLOUD_FCIRCLE4, ebullet[b].x, ebullet[b].y, enemy[e].x_speed, enemy[e].y_speed, 40 + grand(10), CLOUD_COL_2, 950);
         }
         enemy[e].burst --;
         if (enemy[e].burst <= 0)
         {
          enemy[e].recycle = 135 DIFF 30;
          enemy[e].burst = 3 + arena.difficulty * 2;
         }
          else
           enemy[e].recycle = 6;
        }
        break;
       case ENEMY_FIGHTER3:
        if (enemy[e].recycle == 15)
         enemy[e].frame = 1;
        if (enemy[e].recycle == 5)
         enemy[e].frame = 2;
        if (enemy[e].recycle == 85 DIFF 15)
         enemy[e].frame = 1;
        if (enemy[e].recycle == 75 DIFF 15)
         enemy[e].frame = 0;
        angle = attack_angle(e);
        if (enemy[e].recycle <= 0 && ready_to_fire == 1)
        {
         b = create_ebullet(EBULLET_SHOT);
         play_effectwfvx(WAV_CHIRP, 1000 + grand(200), 120, enemy[e].x);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x;
          ebullet[b].y = enemy[e].y;
          ebullet[b].angle = angle;
          ebullet[b].x_speed = xpart(ebullet[b].angle, 4000);
          ebullet[b].y_speed = ypart(ebullet[b].angle, 4000);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_3;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 30 + grand(10), CLOUD_COL_3, 950);
          ebullet[b].status = 19 + grand (8);
         }
         b = create_ebullet(EBULLET_SHOT);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x;
          ebullet[b].y = enemy[e].y;
          ebullet[b].angle = angle + 15;
          ebullet[b].x_speed = xpart(ebullet[b].angle, 3300);
          ebullet[b].y_speed = ypart(ebullet[b].angle, 3300);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_2;
//          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 30 + grand(10), CLOUD_COL_2, 950);
          ebullet[b].status = 6 + grand (8);
         }
         b = create_ebullet(EBULLET_SHOT);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x;
          ebullet[b].y = enemy[e].y;
          ebullet[b].angle = angle - 15;
          ebullet[b].x_speed = xpart(ebullet[b].angle, 3300);
          ebullet[b].y_speed = ypart(ebullet[b].angle, 3300);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_2;
//          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 30 + grand(10), CLOUD_COL_2, 950);
          ebullet[b].status = 6 + grand (8);
         }
         enemy[e].recycle = 90 DIFF 15;
         enemy[e].status = 20 + grand(10);
        }
        if (enemy[e].status > 0)
        {
         if (enemy[e].status & 1)
         {
          quick_cloud(CLOUD_FCIRCLE4, enemy[e].x, enemy[e].y, enemy[e].x_speed, enemy[e].y_speed, 10 + grand(10) + enemy[e].status, CLOUD_COL_3, 992);
         }
         enemy[e].status --;
        }
        break;
     }

// enemy may have returned in switch above...

 if (enemy [e].target_time > 0 && enemy [e].x_target [enemy [e].target] != -1)
  enemy [e].target_time --;
   else
   {
    if (enemy [e].x_target [enemy [e].target] != -1)
    {
     enemy [e].target_time = enemy [e].max_target_time;
     enemy [e].target ++;
//     enemy[e].recycle = 30;
    }
   }


    if (enemy [e].x / 15000 == enemy [e].x_target [enemy [e].target] / 15000
      && enemy [e].y / 15000 == enemy [e].y_target [enemy [e].target] / 15000)
    {
     if (enemy [e].x_target [enemy [e].target] != -1)
     {
      enemy [e].target_time = enemy [e].max_target_time;
      enemy [e].target ++;
//      enemy[e].recycle = 30;
     }
    }

    if (enemy [e].x_target [enemy [e].target] != -1 && enemy [e].y < 470000)
    {
     track_target(e, enemy [e].x_target [enemy [e].target], enemy [e].y_target [enemy [e].target], enemy [e].angle1, eclass[enemy [e].type].speed2);
     enemy [e].angle1 %= ANGLE_1;
    }
    enemy [e].x_speed = xpart (enemy [e].angle1, eclass[enemy [e].type].speed1);
    enemy [e].y_speed = ypart (enemy [e].angle1, eclass[enemy [e].type].speed1);

    move_enemy(e);

    switch(enemy[e].type)
    {
     case ENEMY_FIGHTER:
//      quick_cloud(CLOUD_CIRCLE5, enemy[e].x, enemy[e].y + 12000, enemy[e].x_speed, enemy[e].y_speed + 2000 + grand(1000), 10 + grand(10), CLOUD_COL_1, 0);
      break;


    }

    if (enemy[e].x_target [enemy[e].target] == -1)
     enemy[e].persist = 0;


}





void move_stroller2(int e)
{

 char ready_to_fire = 0;
 int angle, b;

 if (enemy[e].recycle > 0)
  enemy[e].recycle --;
 if (enemy[e].recycle <= 0)
 {
  if (enemy[e].y > 3000 && enemy[e].x > 3000 && enemy[e].x < 597000 && enemy[e].y < 590000)
  {
     ready_to_fire = 1;
  }
 }

     switch(enemy[e].type)
     {
       case ENEMY_FIGHTER5:
        if (enemy[e].recycle == 15)
         enemy[e].frame = 1;
        if (enemy[e].recycle == 5)
         enemy[e].frame = 2;
        if (enemy[e].recycle == 85 DIFF 15)
         enemy[e].frame = 1;
        if (enemy[e].recycle == 70 DIFF 15)
         enemy[e].frame = 0;
        if (enemy[e].recycle <= 0 && ready_to_fire == 1)
        {
         play_effectwfvx(WAV_CHIRP, 1900 + grand(200), 120, enemy[e].x);
         b = create_ebullet(EBULLET_SHOT);
         angle = attack_angle(e) + grand(ANGLE_32) - grand(ANGLE_32);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x;
          ebullet[b].y = enemy[e].y - 5000;
          ebullet[b].angle = angle;
          ebullet[b].x_speed = xpart(ebullet[b].angle, 4000);
          ebullet[b].y_speed = ypart(ebullet[b].angle, 4000);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_3;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 30 + grand(10), CLOUD_COL_3, 950);
          ebullet[b].status = 9 + grand (5);
         }
         enemy[e].burst --;
         if (enemy[e].burst <= 0)
         {
          enemy[e].recycle = 90 DIFF 15;
          enemy[e].burst = 2 + arena.difficulty * 2;
         }
          else
           enemy[e].recycle = 4;
        }
        if (enemy[e].recycle <= 25 && enemy[e].recycle > 0)// && ready_to_fire)
        {
         drag_enemy(e, 960);
         move_enemy(e);
         return;
        }
        break;
     }

// enemy may have returned in switch above...

 if (enemy [e].target_time > 0 && enemy [e].x_target [enemy [e].target] != -1)
  enemy [e].target_time --;
   else
   {
    if (enemy [e].x_target [enemy [e].target] != -1)
    {
     enemy [e].target_time = enemy [e].max_target_time;
     enemy [e].target ++;
//     enemy[e].recycle = 30;
    }
   }


    if (enemy [e].x / 15000 == enemy [e].x_target [enemy [e].target] / 15000
      && enemy [e].y / 15000 == enemy [e].y_target [enemy [e].target] / 15000)
    {
     if (enemy [e].x_target [enemy [e].target] != -1)
     {
      enemy [e].target_time = enemy [e].max_target_time;
      enemy [e].target ++;
//      enemy[e].recycle = 30;
     }
    }

    if (enemy [e].x_target [enemy [e].target] != -1)// && enemy [e].y < 570000)
    {
     track_target(e, enemy [e].x_target [enemy [e].target], enemy [e].y_target [enemy [e].target], enemy [e].angle1, eclass[enemy [e].type].speed2);
     enemy [e].angle1 %= ANGLE_1;
    }
    enemy [e].x_speed += xpart (enemy [e].angle1, eclass[enemy [e].type].speed1);
    enemy [e].y_speed += ypart (enemy [e].angle1, eclass[enemy [e].type].speed1);
    drag_enemy(e, eclass[enemy [e].type].speed3);

    move_enemy(e);

    switch(enemy[e].type)
    {


    }

    if (enemy[e].x_target [enemy[e].target] == -1)
     enemy[e].persist = 0;


}



/*
void petal_turn(int e, int p, int target_angle, int accel, int max_speed)
{

 target_angle &= 1023;

 if ((enemy[e].petal_angle [p] < target_angle && target_angle > enemy[e].petal_angle [p] + ANGLE_2)
     || (enemy[e].petal_angle [p] > target_angle && target_angle > enemy[e].petal_angle [p] - ANGLE_2))
 {
  enemy[e].petal_angle_speed [p] -= accel;
  if (enemy[e].petal_angle_speed [p] < max_speed * -1)
   enemy[e].petal_angle_speed [p] = max_speed * -1;
  enemy[e].petal_angle [p] += enemy[e].petal_angle_speed [p];
  enemy[e].petal_angle [p] &= 1023;
  return;
 }

  enemy[e].petal_angle_speed [p] += accel;
  if (enemy[e].petal_angle_speed [p] > max_speed)
   enemy[e].petal_angle_speed [p] = max_speed;
  enemy[e].petal_angle [p] += enemy[e].petal_angle_speed [p];
  enemy[e].petal_angle [p] &= 1023;

}
*/


void move_darter(int e)
{
 int b, i;

 enemy[e].target_time --;
 if (enemy[e].recycle > 0)
  enemy[e].recycle --;

 if (enemy[e].target_time <= 0)
 {
  switch(enemy[e].phase)
  {
   case 0: // diving in first
    enemy[e].phase = 1; // opening
    enemy[e].target_time = 30; // also below
//    enemy[e].status = 0;
    break;
   case 1: // opening
    enemy[e].phase = 2; // shooting
    enemy[e].x_speed = 0;
    enemy[e].y_speed = 0;
    enemy[e].target_time = eclass[enemy[e].type].speed3;
    break;
   case 2: // shooting
    if (enemy[e].burst > 0)
     break;
    enemy[e].phase = 3; // closing
    enemy[e].target_time = 30;
    break;
   case 3: // closing
    if (enemy[e].phase2 >= 3)
    {
     enemy[e].phase = 5; // departing
     enemy[e].target_time = 1000; // important! see below
     enemy[e].persist = 0;
     break;
    }
    enemy[e].phase = 4; // moving
    enemy[e].phase2 ++;
    enemy[e].target_time = 1000; // important! see below
    enemy[e].x_target [0] = RAND_X_IN;
    enemy[e].y_target [0] = RAND_Y_IN;
    enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
    enemy[e].x_speed = 0;
    enemy[e].y_speed = 0;
    break;

  }
 }
//    track_target(e, player.x, player.y, enemy[e].angle1, 8);

  switch(enemy[e].phase)
  {
   case 2: // shooting
    switch(enemy[e].type)
    {
//     default:
     case ENEMY_MACHINE:
      if (enemy[e].recycle <= 0)
      {
         if (enemy[e].burst <= 0)
         {
          enemy[e].angle1 = attack_angle(e);
          enemy[e].burst = 7;
         }
         b = create_ebullet(EBULLET_SHOT);
         play_effectwfvx(WAV_SHOT, 1500 + grand(100), 120, enemy[e].x);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x;
          ebullet[b].y = enemy[e].y;
          ebullet[b].angle = enemy[e].angle1;
          ebullet[b].x_speed = xpart(ebullet[b].angle, 5000 + arena.difficulty * 1000);
          ebullet[b].y_speed = ypart(ebullet[b].angle, 5000 + arena.difficulty * 1000);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_1;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 30 + grand(10), CLOUD_COL_1, 950);
          ebullet[b].status = 0;//9 + grand (5);
         }

         enemy[e].recycle = 6;
         enemy[e].burst --;
         if (enemy[e].burst <= 0)
         {
          enemy[e].recycle = 50;
         }
      }
      break;
     case ENEMY_MACHINE2:
      if (enemy[e].recycle <= 0)
      {
         if (enemy[e].burst <= 0)
         {
          enemy[e].angle1 = grand(ANGLE_1);
          enemy[e].burst = 6 + arena.difficulty;
          if (enemy[e].status2 == 0)
           enemy[e].status2 = 1;
            else
             enemy[e].status2 = 0;
         }
         for (i = 0; i < 3; i ++)
         {
          b = create_ebullet(EBULLET_SHOT);
          play_effectwfvx(WAV_CHIRP, 1500 + grand(200), 120, enemy[e].x);
          if (b != -1)
          {
           ebullet[b].x = enemy[e].x;
           ebullet[b].y = enemy[e].y;
           ebullet[b].angle = enemy[e].angle1;
           ebullet[b].x_speed = xpart(ebullet[b].angle, 3000);
           ebullet[b].y_speed = ypart(ebullet[b].angle, 3000);
           ebullet[b].timeout = 4000;
           ebullet[b].colour = CLOUD_COL_3;
           quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 30 + grand(10), CLOUD_COL_3, 950);
           ebullet[b].status = 5 + grand (5);
          }
          if (enemy[e].status2 == 0)
           enemy[e].angle1 += ANGLE_3 - 16;
            else
             enemy[e].angle1 += ANGLE_3 + 16;

         }

         enemy[e].recycle = 6;
         enemy[e].burst --;
         if (enemy[e].burst <= 0)
         {
          enemy[e].recycle = 30 DIFF 5;
         }
      }
      break;
     case ENEMY_MACHINE3:
      if (enemy[e].recycle <= 0)
      {
         if (enemy[e].burst <= 0)
         {
          enemy[e].angle1 = attack_angle(e);
          enemy[e].burst = 7;
         }
         b = create_ebullet(EBULLET_SPINNER3);
         play_effectwfvx(WAV_SHOT, 1000 + grand(100), 120, enemy[e].x);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x;
          ebullet[b].y = enemy[e].y;
          ebullet[b].angle = enemy[e].angle1;
          ebullet[b].x_speed = xpart(ebullet[b].angle, 10000 + arena.difficulty * 1000 - enemy[e].burst * 1000);
          ebullet[b].y_speed = ypart(ebullet[b].angle, 10000 + arena.difficulty * 1000 - enemy[e].burst * 1000);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_2;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 40 + grand(16), CLOUD_COL_2, 950);
          if (e & 1)
           ebullet[b].status = 13 - enemy[e].burst;
            else ebullet[b].status = -13 + enemy[e].burst;
         }

         enemy[e].recycle = 9;
         enemy[e].burst --;
         if (enemy[e].burst <= 0)
         {
          enemy[e].recycle = 50;
         }
      }
      break;
    }
    break;
   case 1: // opening
    drag_enemy(e, 910);
    if (enemy[e].target_time % 4 == 0)
     enemy[e].frame ++;
    if (enemy[e].frame > 3)
     enemy[e].frame = 3;
     break;
   case 3: // closing
    drag_enemy(e, 910);
    if (enemy[e].target_time < 21 && enemy[e].target_time % 4 == 0)
     enemy[e].frame --;
    if (enemy[e].frame < 0)
     enemy[e].frame = 0;
    break;
   case 4: // moving
    if (enemy[e].target_time > 980)
    {
     enemy[e].x_speed += xpart(enemy[e].angle, eclass[enemy[e].type].speed2);
     enemy[e].y_speed += ypart(enemy[e].angle, eclass[enemy[e].type].speed2);
    }
    if (enemy[e].x > enemy[e].x_target [0] - 10000
     && enemy[e].x < enemy[e].x_target [0] + 10000
     && enemy[e].y > enemy[e].y_target [0] - 10000
     && enemy[e].y < enemy[e].y_target [0] + 10000)
     {
      enemy[e].phase = 1;
      enemy[e].target_time = 30; // also above
     }
    break;
   case 5: // departing
    if (enemy[e].target_time > 980)
     enemy[e].y_speed += eclass[enemy[e].type].speed2;
    break;

  }



 move_enemy(e);
}


void move_dart_stopper(int e)
{

 if (enemy[e].recycle > 0)
  enemy[e].recycle --;

  switch(enemy[e].phase)
  {
   case 0: // entry
    enemy[e].x_speed += xpart(enemy[e].angle, eclass[enemy[e].type].speed1);
    enemy[e].y_speed += ypart(enemy[e].angle, eclass[enemy[e].type].speed1);
    drag_enemy(e, 920);
    if (enemy[e].x > enemy[e].x_target [0] - 40000
        && enemy[e].x < enemy[e].x_target [0] + 40000
        && enemy[e].y > enemy[e].y_target [0] - 40000
        && enemy[e].y < enemy[e].y_target [0] + 40000)
        {
         enemy[e].angle = get_e_angle(e, enemy[e].x_target [1], enemy[e].y_target [1]);
         enemy[e].phase = 1;
         enemy[e].persist = 0;
        }
    break;
   case 1: // then go to 2nd target
    enemy[e].x_speed += xpart(enemy[e].angle, eclass[enemy[e].type].speed1);
    enemy[e].y_speed += ypart(enemy[e].angle, eclass[enemy[e].type].speed1);
    if (arena.counter % 5 == 0)
     enemy[e].angle = get_e_angle(e, enemy[e].x_target [1], enemy[e].y_target [1]);
    drag_enemy(e, 920);
    if (enemy[e].x > enemy[e].x_target [1] - 40000
        && enemy[e].x < enemy[e].x_target [1] + 40000
        && enemy[e].y > enemy[e].y_target [1] - 40000
        && enemy[e].y < enemy[e].y_target [1] + 40000)
        {
         enemy[e].phase = 2;
         enemy[e].target_time = eclass[enemy[e].type].speed3;
        }
    break;
   case 2: // firing!
    switch(enemy[e].type)
    {
//     case ENEMY_LARGER:
     default:
     if (enemy[e].recycle == 0)
     {
/*           fire_wing_diamond(enemy[e].x, enemy[e].y, grand(ANGLE_1), EBCOL_B2, 20, 15 + arena.difficulty * 5);
           if (arena.difficulty > 0)
            fire_wing_diamond(enemy[e].x, enemy[e].y, grand(ANGLE_1), EBCOL_B2, 20, 15 + arena.difficulty * 5);
           if (arena.difficulty > 1)
            fire_wing_diamond(enemy[e].x, enemy[e].y, grand(ANGLE_1), EBCOL_B2, 20, 15 + arena.difficulty * 5);
*/
           enemy[e].recycle = 20 - arena.difficulty * 5;
        }
     }
/*     for (i = 0; i < 4; i ++)
     {
       tangle = enemy[e].angle + i * ANGLE_4;
       tangle &= 1023;
       if ((enemy[e].petal_angle [i] > tangle - 20
         && enemy[e].petal_angle [i] < tangle + 20)
        || (tangle < 20 && enemy[e].petal_angle [i] > ANGLE_1 - tangle)
        || (tangle > ANGLE_1 - 20 && enemy[e].petal_angle [i] < ANGLE_1 - tangle))
        {
         enemy[e].petal_angle [i] = tangle;
         enemy[e].petal_angle_speed [i] = enemy[e].status;
        }
         else
          petal_turn(e, i, tangle, 4, 24);
     }
     enemy[e].angle += enemy[e].status;
     enemy[e].angle &= 1023;

     drag_enemy(e, 910);
     enemy[e].target_time --;
     if (enemy[e].target_time <= 0)
     {
      enemy[e].phase = 4;
      enemy[e].persist = 0;
      if (enemy[e].x < 320000)
       enemy[e].angle = grand(ANGLE_8) - grand(ANGLE_8);
        else
         enemy[e].angle = ANGLE_2 + grand(ANGLE_8) - grand(ANGLE_8);
     }
     */
//     break;
//    }
    break;
   case 4: // departing
    enemy[e].x_speed += xpart(enemy[e].angle, eclass[enemy[e].type].speed1);
    enemy[e].y_speed += ypart(enemy[e].angle, eclass[enemy[e].type].speed1);
    drag_enemy(e, 950);
    break;

  }

 switch(enemy[e].type)
 {
     default:
//   case ENEMY_LARGER:
/*       enemy_pulse(e, EVENT_PULSE2);
       if (enemy[e].phase == 2)
        break;
       petal_turn(e, 0, enemy[e].angle + ANGLE_2 - ANGLE_8, 2, 8);
       petal_turn(e, 1, enemy[e].angle + ANGLE_2 - ANGLE_16, 2, 8);
       petal_turn(e, 2, enemy[e].angle + ANGLE_2 + ANGLE_16, 2, 8);
       petal_turn(e, 3, enemy[e].angle + ANGLE_2 + ANGLE_8, 2, 8);*/
       break;


 }

 move_enemy(e);
}










void move_drifter(int e)
{

 enemy[e].target_time --;
 if (enemy[e].recycle > 0)
  enemy[e].recycle --;

 if (enemy[e].target_time <= 0)
 {
  enemy[e].phase = 1;
  enemy[e].persist = 0;
  enemy[e].target_time = 5000;
  switch(grand(4))
  {
   case 0: enemy[e].x_target [0] = -100000; break;
   case 1: enemy[e].x_target [0] = 900000; break;
   case 2: enemy[e].y_target [0] = -100000; break;
   case 3: enemy[e].y_target [0] = 900000; break;
  }
  enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
 }

 switch(enemy[e].phase)
 {
   case 0:
   if ((enemy[e].x > enemy[e].x_target [0] - 40000
     && enemy[e].x < enemy[e].x_target [0] + 40000
     && enemy[e].y > enemy[e].y_target [0] - 40000
     && enemy[e].y < enemy[e].y_target [0] + 40000)
      || (enemy[e].escorting != -1 && (enemy[e].target_time & 16) == 1))
     {
      if (enemy[e].escorting != -1)
      {
        enemy[e].x_target [0] = enemy[enemy[e].escorting].x + grand(60000) - grand(60000) + enemy[e].pulse1;
        enemy[e].y_target [0] = enemy[enemy[e].escorting].y + grand(60000) - grand(60000) + enemy[e].pulse2;
        enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
      }
       else
       {
        enemy[e].x_target [0] = RAND_X_IN;
        enemy[e].y_target [0] = RAND_Y_IN;
        enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);
       }
     }
    break;

 }

 if (enemy[e].target_time % 10 == 0)
  enemy[e].angle = get_e_angle(e, enemy[e].x_target [0], enemy[e].y_target [0]);

 enemy[e].x_speed += xpart(enemy[e].angle, eclass[enemy[e].type].speed1);
 enemy[e].y_speed += ypart(enemy[e].angle, eclass[enemy[e].type].speed1);

 drag_enemy(e, eclass[enemy[e].type].speed2);

 move_enemy(e);

  int angle;

  switch(enemy[e].type)
  {
//   case ENEMY_DRIFTER1:
//   case ENEMY_ESCORT:
   default:
    if (enemy[e].status > 0)
     enemy[e].status --;
    if (enemy[e].recycle <= 0
        && enemy[e].phase == 0
        && enemy[e].y < 440000)
    {
           angle = attack_angle(e);
//           fire_dot(enemy[e].x, enemy[e].y, angle, EBCOL_B3, 5, 4000 + arena.difficulty * 1000, 7);
           enemy[e].recycle = 65 - arena.difficulty * 10;
           enemy[e].status = 16;
    }
//      quick_cloud(CLOUD_CIRCLE2, enemy[e].x, enemy[e].y + 15000 + (enemy[e].type == ENEMY_DRIFTER1) * 4000, enemy[e].x_speed + grand(300) - grand(300), enemy[e].y_speed + 1500 + grand(2500), 6 + grand(4), TCOL_SH2, 0);
    break;
  }


}




void move_diver(int e)
{
 enemy[e].target_time --;
 if (enemy[e].recycle > 0)
  enemy[e].recycle --;

  int angle, i;

 if (enemy[e].target_time <= 0)
 {
  switch(enemy[e].phase)
  {
   case 0: // diving in first
    enemy[e].phase = 1; // opening
    enemy[e].target_time = 20; // also below
    break;
   case 1: // opening
    enemy[e].phase = 2; // shooting
//    enemy[e].x_speed = 0;
//    enemy[e].y_speed = 0;
    enemy[e].target_time = 54 + arena.difficulty * 10;
    break;
   case 2: // shooting
    enemy[e].phase = 3; // closing
    enemy[e].target_time = 20;
    break;
   case 3: // closing
     enemy[e].phase = 5; // departing
     enemy[e].target_time = 1000; // important! see below
     enemy[e].persist = 0;
    break;

  }
 }
    track_target(e, player.x, player.y, enemy[e].angle1, 8);

  switch(enemy[e].phase)
  {
   case 2: // shooting
    drag_enemy_y(e, 950);
    if (enemy[e].recycle <= 0)
    {
         angle = grand(ANGLE_1);
         for (i = 0; i < 3; i ++)
         {
//          fire_dart(enemy[e].x, enemy[e].y, angle, EBCOL_B2, 50, 80);
          angle += ANGLE_3;
          angle &= 1023;
         }
         enemy[e].recycle = 14 - arena.difficulty * 4;
    }
    break;
   case 1: // opening
   case 3: // closing
    drag_enemy_y(e, 950);
//    if (arena.counter % 10 != 0)
//     enemy[e].target_time ++;
    break;
//   case 4: // moving
//    break;
   case 5: // departing
    if (enemy[e].target_time > 960)
     enemy[e].y_speed -= 170;
    break;

  }



 move_enemy(e);
}


void move_cruiser(int e)
{

 if (enemy[e].recycle > 0 && enemy[e].y < 460000)
  enemy[e].recycle --;

 switch(enemy[e].type)
 {
/*  case ENEMY_SPINNER:
      if (enemy[e].recycle <= 0)
      {
       if (synch_ready [EVENT_FIRE4_1])
       {
        synch_done [DONE_FIRE4_1] |= FIRE4_SPINNER;
        enemy[e].recycle = 50 - arena.difficulty * 5;
        if (enemy [e].y > 360000)
         enemy[e].recycle = 2000;
        angle = radians_to_angle(atan2(player.y - enemy[e].y - 32000, player.x - enemy[e].x));
        fire_ddart(enemy[e].x, enemy[e].y + 32000, angle, EBCOL_B5, 40, 90 + arena.difficulty * 10);
        angle = radians_to_angle(atan2(player.y - enemy[e].y + 32000, player.x - enemy[e].x));
        fire_ddart(enemy[e].x, enemy[e].y - 32000, angle, EBCOL_B5, 40, 90 + arena.difficulty * 10);
       }
      }
   enemy_pulse_slow(e, EVENT_PULSE);
   if (enemy[e].y > 1000 && enemy[e].y < 200000)
   {
    enemy[e].status += 3;
    if (enemy[e].status > 150)
     enemy[e].status = 150;
   }
   if (enemy[e].y > 430000)
   {
    enemy[e].status -= 3;
    if (enemy[e].status < 5)
     enemy[e].status = 5;
   }
   if (enemy[e].status > 0)
   {
    b = create_ebullet(EBULLET_SPIN);
    if (b != -1)
    {
     ebullet[b].x = enemy[e].x;
     ebullet[b].y = enemy[e].y;
     ebullet[b].x_speed = enemy[e].status;
     if (ebullet[b].x_speed > 12)
      ebullet[b].x_speed = 12;
     ebullet[b].y_speed = 5;
     ebullet[b].status = enemy[e].status;
     ebullet[b].angle = enemy[e].angle1;
     ebullet[b].timeout = 2;
     ebullet[b].colour = TCOL_B4;
     ebullet[b].accel = TCOL_B2;
     ebullet[b].cord_colour = TRANS_B2_OUT;
    }
   }
   enemy[e].angle1 += enemy[e].angle2;
   enemy[e].angle1 &= 1023;
   break;

*/


//  case ENEMY_CRUISER4:
/*  if ((arena.counter & 1) == 0)
  {
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 14000, enemy[e].y + 49000, grand(300) - grand(300), 3500 + grand(2500), 6 + grand(6), TCOL_ORANGE, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 14000, enemy[e].y + 49000, grand(300) - grand(300), 3500 + grand(2500), 6 + grand(6), TCOL_ORANGE, 0);
  }
   else
   {
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 14000, enemy[e].y + 49000, grand(300) - grand(300), 3500 + grand(2500), 12 + grand(12), TCOL_SH2, 0);
    quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 14000, enemy[e].y + 49000, grand(300) - grand(300), 3500 + grand(2500), 12 + grand(12), TCOL_SH2, 0);
   }*/
   default:
      if (enemy[e].status == 0)
       track_target(e, player.x, player.y, enemy[e].angle1, 5 + arena.difficulty * 3);
      if (enemy[e].status > 0)
      {
//       fire_beam(e, 25000);
       enemy[e].status --;
       if (enemy[e].status == 100)
       {
         enemy[e].status = 35;
       }
      }
      if (enemy[e].recycle <= 0)
      {
        enemy[e].recycle = 90 - arena.difficulty * 5;
        enemy[e].status = 120; // change in fire_beam as well!
        if (enemy [e].y > 360000)
         enemy[e].recycle = 2000;
      }
   break;
 }


 if (enemy[e].y > 200000)
  enemy[e].persist = 0;

 move_enemy(e);

}


void move_stopper(int e)
{

 int i, angle, angle_inc, b, speed, j, angle2, xa;

 enemy[e].target_time --;
 if (enemy[e].recycle > 0)
  enemy[e].recycle --;

 if (enemy[e].target_time <= 0)
 {
  switch(enemy[e].phase)
  {
   case 0: // diving in first
    enemy[e].phase = 1; // opening
    enemy[e].target_time = eclass[enemy[e].type].speed3; // also below
    break;
   case 1: // opening
    enemy[e].phase = 2; // shooting
    enemy[e].x_speed = 0;
    enemy[e].y_speed = 0;
    enemy[e].target_time = eclass[enemy[e].type].speed2;
    break;
   case 2: // shooting
    if ((enemy[e].type == ENEMY_MEDIUM
     || enemy[e].type == ENEMY_RISER)
     && enemy[e].burst > 0)
    {
     enemy[e].target_time ++;
     break;
    }
    enemy[e].phase = 3; // closing
    enemy[e].target_time = eclass[enemy[e].type].speed3;
    switch(enemy[e].type)
    {
        case ENEMY_DOME:
         enemy[e].recycle = 40;
         break;
    }
    break;
   case 3: // closing
//    if (enemy[e].phase2 >= 3)
    {
     enemy[e].phase = 4; // departing
     enemy[e].target_time = 1000; // important! see below
     enemy[e].persist = 0;
     enemy[e].x_speed = 0;
     break;
    }
    break;
  }

 }

    if (enemy[e].x < 50000)
     enemy[e].x_speed += 15;
    if (enemy[e].x > 614400 - 50000)
     enemy[e].x_speed -= 15;

  switch(enemy[e].phase)
  {
   case 0: // entering
    switch(enemy[e].type)
    {
        case ENEMY_DOUBLE:
        case ENEMY_BIG4:
         enemy[e].x_speed += xpart(arena.counter * 8, 30);
         enemy[e].y_speed += ypart(arena.counter * 8, 30);
         break;
        case ENEMY_DOUBLE2:
        case ENEMY_DOUBLE3:
        case ENEMY_DOUBLE4:
         enemy[e].x_speed += xpart(arena.counter * 8, 20);
         enemy[e].y_speed += ypart(arena.counter * 8, 20);
         break;
        case ENEMY_BIG2:
        case ENEMY_BIG3:
        case ENEMY_RISER:
        case ENEMY_DOME2:
        case ENEMY_DOME3:
        case ENEMY_WIDE:
         enemy[e].x_speed += xpart(arena.counter * 8, 10);
         enemy[e].y_speed += ypart(arena.counter * 8, 10);
         break;
    }
    break;
   case 1: // stopping
   drag_enemy(e, 960);
    switch(enemy[e].type)
    {
        case ENEMY_MEDIUM:
         if (enemy[e].target_time == 20)
          enemy[e].frame = 1;
         if (enemy[e].target_time == 15)
          enemy[e].frame = 2;
         if (enemy[e].target_time == 10)
          enemy[e].frame = 3;
         break;
        case ENEMY_WIDE:
         if (enemy[e].target_time == 20)
          enemy[e].frame = 1;
         if (enemy[e].target_time == 15)
          enemy[e].frame = 2;
         if (enemy[e].target_time == 10)
          enemy[e].frame = 3;
         break;
        case ENEMY_RISER:
         if (enemy[e].target_time == 20)
          enemy[e].frame = 1;
         if (enemy[e].target_time == 15)
          enemy[e].frame = 2;
         if (enemy[e].target_time == 10)
          enemy[e].frame = 3;
         if (enemy[e].target_time == 5)
          enemy[e].frame = 4;
         break;
    }
    break;

   case 2: // shooting
    switch(enemy[e].type)
    {
        case ENEMY_BIG4:
         enemy[e].x_speed += xpart(arena.counter * 8, 30);
         enemy[e].y_speed += ypart(arena.counter * 8, 30);
         if (enemy[e].x < 100000)
          enemy[e].x_speed += 15;
         if (enemy[e].x > 500000)
          enemy[e].x_speed -= 15;
         break;
        case ENEMY_DOUBLE:
         enemy[e].x_speed += xpart(arena.counter * 8, 30);
         enemy[e].y_speed += ypart(arena.counter * 8, 30);
         if (enemy[e].y < 100000)
          enemy[e].y_speed += 15;
         break;
        case ENEMY_DOUBLE2:
        case ENEMY_DOUBLE3:
        case ENEMY_DOUBLE4:
         enemy[e].x_speed += xpart(arena.counter * 8, 20);
         enemy[e].y_speed += ypart(arena.counter * 8, 20);
         if (enemy[e].y < 100000)
          enemy[e].y_speed += 15;
         break;
        case ENEMY_BIG2:
        case ENEMY_BIG3:
        case ENEMY_WIDE:
         enemy[e].x_speed += xpart(arena.counter * 8, 10);
         enemy[e].y_speed += ypart(arena.counter * 8, 10);
         if (enemy[e].y < 100000)
          enemy[e].y_speed += 15;
         break;
        case ENEMY_RISER:
        case ENEMY_DOME2:
        case ENEMY_DOME3:
         enemy[e].x_speed += xpart(arena.counter * 8, 10);
         enemy[e].y_speed += ypart(arena.counter * 8, 10);
         break;
        case ENEMY_MEDIUM:
   if (enemy[e].recycle <= 0)
   {
       enemy[e].burst = 120;
       enemy[e].recycle = 122;
       enemy[e].angle = attack_angle(e);
       play_effectwfvx(WAV_BEAM1, 320, 110, enemy[e].x);
   }
   if (enemy[e].burst > 0)
   {
         b = create_ebullet(EBULLET_BEAM2);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x;
          ebullet[b].y = enemy[e].y;
          ebullet[b].angle = enemy[e].angle;
          ebullet[b].timeout = 2;
          ebullet[b].colour = 2;
          ebullet[b].status = enemy[e].burst;
         }
         if (enemy[e].burst == 85)
          play_effectwfvx(WAV_BEAM2, 900, 110, enemy[e].x);
         enemy[e].burst --;
   }
   break;

    }
    break;
   case 3: // closing
    drag_enemy(e, 960);
//    if (arena.counter % 10 != 0)
//     enemy[e].target_time ++;
    switch(enemy[e].type)
    {
        case ENEMY_MEDIUM:
         if (enemy[e].target_time == 20)
          enemy[e].frame = 2;
         if (enemy[e].target_time == 15)
          enemy[e].frame = 1;
         if (enemy[e].target_time == 10)
          enemy[e].frame = 0;
         break;
        case ENEMY_WIDE:
         if (enemy[e].target_time == 20)
          enemy[e].frame = 2;
         if (enemy[e].target_time == 15)
          enemy[e].frame = 1;
         if (enemy[e].target_time == 10)
          enemy[e].frame = 0;
         break;
        case ENEMY_RISER:
         if (enemy[e].target_time == 20)
          enemy[e].frame = 3;
         if (enemy[e].target_time == 15)
          enemy[e].frame = 2;
         if (enemy[e].target_time == 10)
          enemy[e].frame = 1;
         if (enemy[e].target_time == 5)
          enemy[e].frame = 0;
         break;
     }
    break;
   case 4: // departing
    if (enemy[e].target_time > 960)
     enemy[e].y_speed += eclass[enemy[e].type].speed1 / 37;
    switch(enemy[e].type)
    {
        case ENEMY_DOUBLE:
        case ENEMY_BIG4:
         enemy[e].x_speed += xpart(arena.counter * 8, 20);
         break;
        case ENEMY_DOUBLE2:
        case ENEMY_DOUBLE3:
        case ENEMY_DOUBLE4:
        case ENEMY_DOME2:
        case ENEMY_DOME3:
         enemy[e].x_speed += xpart(arena.counter * 8, 15);
         break;
        case ENEMY_BIG2:
        case ENEMY_BIG3:
        case ENEMY_RISER:
        case ENEMY_WIDE:
         enemy[e].x_speed += xpart(arena.counter * 8, 7);
         break;
    }
    break;
  }


 // persistent

 move_enemy(e);

 switch(enemy[e].type)
 {
  case ENEMY_DOME3:
   if (e & 1)
    enemy[e].angle += 4;
     else
      enemy[e].angle -= 4;
   enemy[e].angle &= 1023;
   if (enemy[e].y < 500000 && enemy[e].recycle <= 0)
   {
    xa = 0;//grand(10);
    angle = enemy[e].angle + xa * ANGLE_10;
    play_effectwfvx(WAV_CHIRP, 2000 + grand(200), 220, enemy[e].x);
    for (i = -2; i < 3; i ++)
    {
           b = create_ebullet(EBULLET_SHOT);
           if (b != -1)
           {
            ebullet[b].x = enemy[e].x + xpart(enemy[e].angle + (xa + i) * ANGLE_10, 19000);
            ebullet[b].y = enemy[e].y + ypart(enemy[e].angle + (xa + i) * ANGLE_10, 19000) - 19000;
            ebullet[b].angle = angle;
            speed = 4000 - abs(i * 200);
            ebullet[b].x_speed = enemy[e].x_speed + xpart(ebullet[b].angle, speed);
            ebullet[b].y_speed = enemy[e].y_speed + ypart(ebullet[b].angle, speed);
            ebullet[b].timeout = 4000;
            ebullet[b].colour = CLOUD_COL_2;
            quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 20 + grand(10), CLOUD_COL_2, 950);
            ebullet[b].status = 0;//12 + grand (8);
           }

           b = create_ebullet(EBULLET_SHOT);
           if (b != -1)
           {
            ebullet[b].x = enemy[e].x - xpart(enemy[e].angle + (xa + i) * ANGLE_10, 19000);
            ebullet[b].y = enemy[e].y - ypart(enemy[e].angle + (xa + i) * ANGLE_10, 19000) - 19000;
            ebullet[b].angle = angle + ANGLE_2;
            speed = 4000 - abs(i * 200);
            ebullet[b].x_speed = enemy[e].x_speed + xpart(ebullet[b].angle, speed);
            ebullet[b].y_speed = enemy[e].y_speed + ypart(ebullet[b].angle, speed);
            ebullet[b].timeout = 4000;
            ebullet[b].colour = CLOUD_COL_2;
            quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 20 + grand(10), CLOUD_COL_2, 950);
            ebullet[b].status = 0;//12 + grand (8);
           }


    }
     enemy[e].recycle = 20 DIFF 4;//30 DIFF 7;
   }
   break;

  case ENEMY_RISER:
      if (enemy[e].phase != 2)
       enemy[e].recycle ++;
      if (enemy[e].recycle <= 0)
      {
         if (enemy[e].burst <= 0)
         {
          enemy[e].burst = 7;
          enemy[e].angle = grand(ANGLE_1);
         }
          enemy[e].angle += enemy[e].status;
          b = create_ebullet(EBULLET_BLOB);
          if (b != -1)
          {
           ebullet[b].x = enemy[e].x;
           ebullet[b].y = enemy[e].y;
           ebullet[b].angle = enemy[e].angle;
           ebullet[b].x_speed = xpart(ebullet[b].angle, 2500);
           ebullet[b].y_speed = ypart(ebullet[b].angle, 2500);
           ebullet[b].timeout = 4000;
           ebullet[b].colour = CLOUD_COL_3;
           quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 30 + grand(10), CLOUD_COL_3, 950);
           if (enemy[e].burst == 7 || enemy[e].burst == 1)
            ebullet[b].status = 4;
           if (enemy[e].burst == 6 || enemy[e].burst == 2)
            ebullet[b].status = 6;
           if (enemy[e].burst == 5 || enemy[e].burst == 3)
            ebullet[b].status = 8;
           if (enemy[e].burst == 4)
            ebullet[b].status = 10;
          }
          b = create_ebullet(EBULLET_BLOB);
          if (b != -1)
          {
           ebullet[b].x = enemy[e].x;
           ebullet[b].y = enemy[e].y;
           ebullet[b].angle = enemy[e].angle;
           ebullet[b].x_speed = xpart(ebullet[b].angle + ANGLE_2, 2500);
           ebullet[b].y_speed = ypart(ebullet[b].angle + ANGLE_2, 2500);
           ebullet[b].timeout = 4000;
           ebullet[b].colour = CLOUD_COL_3;
           quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 30 + grand(10), CLOUD_COL_3, 950);
           if (enemy[e].burst == 7 || enemy[e].burst == 1)
            ebullet[b].status = 4;
           if (enemy[e].burst == 6 || enemy[e].burst == 2)
            ebullet[b].status = 6;
           if (enemy[e].burst == 5 || enemy[e].burst == 3)
            ebullet[b].status = 8;
           if (enemy[e].burst == 4)
            ebullet[b].status = 10;
          }


         enemy[e].recycle = 8;
         enemy[e].burst --;
         if (enemy[e].burst <= 0)
         {
          enemy[e].recycle = 80 DIFF 15;
         }
      }
   break;
  case ENEMY_BIG3:
   quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 77000, enemy[e].y + 16000, enemy[e].x_speed, enemy[e].y_speed + 4000 + grand(4000), 5 + grand(4), CLOUD_COL_1, 980);
   quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 77000, enemy[e].y + 16000, enemy[e].x_speed, enemy[e].y_speed + 4000 + grand(4000), 5 + grand(4), CLOUD_COL_1, 980);
   if (enemy[e].recycle <= 0)
   {
    angle = grand(ANGLE_1);
    angle2 = angle;
    play_effectwfvx(WAV_DOUBLE, 400 + grand(100), 190, enemy[e].x);
    for (j = 0; j < 3; j ++)
    {
          angle += ANGLE_6;
     for (i = 0; i < 12; i ++)
     {
         angle_inc = angle + (i * ANGLE_12);
         angle_inc &= 1023;
         angle2 = angle - angle_inc;
         angle2 &= 1023;
         b = create_ebullet(EBULLET_IBULLET);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x;
          ebullet[b].y = enemy[e].y - 26000;
//          ebullet[b].angle = angle_inc;
          speed = 2500;
          ebullet[b].x_speed = enemy[e].x_speed + xpart(angle, speed) + xpart(angle_inc, 1200 + xpart(angle2 * 2, 200));
          ebullet[b].y_speed = enemy[e].y_speed + ypart(angle, speed) + ypart(angle_inc, 1200 + xpart(angle2 * 2, 200));

//          ebullet[b].x_speed = enemy[e].x_speed + xpart(angle, speed) + xpart(angle_inc, 1200 + xpart(angle2 * 4, 400));
//          ebullet[b].y_speed = enemy[e].y_speed + ypart(angle, speed) + ypart(angle_inc, 1200 + xpart(angle2 * 4, 400));
//          ebullet[b].x_speed += xpart(i * 128, 1200);
//          ebullet[b].y_speed += ypart(i * 128, 1200);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_3;
          ebullet[b].status = 0;
         }
     }

     angle += ANGLE_6;
     for (i = 0; i < 9; i ++)
     {
         angle_inc = angle + (i * ANGLE_9);
         angle_inc &= 1023;
         angle2 = angle - angle_inc;
         angle2 &= 1023;
         b = create_ebullet(EBULLET_IBULLET_2);
         if (b != -1)
         {
          ebullet[b].x = enemy[e].x;
          ebullet[b].y = enemy[e].y - 26000;
          speed = 2500;
          ebullet[b].x_speed = enemy[e].x_speed + xpart(angle, speed) + xpart(angle_inc, 800 + xpart(angle2 * 2, 100));
          ebullet[b].y_speed = enemy[e].y_speed + ypart(angle, speed) + ypart(angle_inc, 800 + xpart(angle2 * 2, 100));
          ebullet[b].timeout = 4000;
          ebullet[b].colour = 1;
          ebullet[b].status = 3;
         }

     }
     }

//    angle += ANGLE_3;
/*    for (i = 0; i < 32; i ++)
    {
         b = create_ebullet(EBULLET_IBULLET_2);
         if (b != -1)
         {
          angle += ANGLE_32;
          ebullet[b].x = enemy[e].x;
          ebullet[b].y = enemy[e].y - 25000;
          ebullet[b].angle = angle;
          speed = 1200;
          ebullet[b].x_speed = enemy[e].x_speed + xpart(ebullet[b].angle, speed);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(ebullet[b].angle, speed);
          ebullet[b].x_speed += xpart(i * 128, 600);
          ebullet[b].y_speed += ypart(i * 128, 600);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_2;
          ebullet[b].status = 0;
         }
     }*/

    quick_cloud(CLOUD_FCIRCLE2, enemy[e].x, enemy[e].y - 25000, enemy[e].x_speed, enemy[e].y_speed, 40 + grand(10), CLOUD_COL_3, 950);
    enemy[e].recycle = 150 DIFF 20;
   }
   break;
  case ENEMY_MEDIUM:
   break;
  case ENEMY_DOUBLE3:
   if (enemy[e].status == 1 || enemy[e].burst <= 0)
    aim_at_target(e, enemy[e].x - 19000, enemy[e].y - 10000, player.x, player.y, 7, 0);
   if (enemy[e].status == 0 || enemy[e].burst <= 0)
    aim_at_target(e, enemy[e].x + 19000, enemy[e].y - 10000, player.x, player.y, 7, 1);
   if (enemy[e].recycle <= 0 && enemy[e].y < 500000)
   {
       play_effectwfvx(WAV_BEAM1, 300, 220, enemy[e].x);
       enemy[e].burst = 120;
       enemy[e].recycle = 102;
       if (enemy[e].status == 0)
       {
        enemy[e].status = 1;
        enemy[e].angle1 = 1;
//        if (arena.difficulty == 2)
//         enemy[e].angle1 = 2;
       }
         else
         {
          enemy[e].status = 0;
          enemy[e].angle1 = -1;
//          if (arena.difficulty == 2)
//           enemy[e].angle1 = -2;
         }
//       enemy[e].angle1 = delta_turn_towards_xy(enemy[e].x, enemy[e].y, player.x, player.y, enemy[e].aim [enemy[e].status], 1);
        // inexact but doesn't really matter
   }
   if (enemy[e].burst > 0)
   {
         b = create_ebullet(EBULLET_BEAM2);
         if (b != -1)
         {
          angle += ANGLE_32;
          if (enemy[e].status == 1)
          {
           ebullet[b].x = enemy[e].x + 18000 + xpart(enemy[e].aim [1], 7000);
           ebullet[b].y = enemy[e].y - 17000 + ypart(enemy[e].aim [1], 7000);
//           if ((arena.counter & 1) == 1)
//            enemy[e].aim [1] += enemy[e].angle1;
          }
           else
           {
            ebullet[b].x = enemy[e].x - 18000 + xpart(enemy[e].aim [0], 7000);
            ebullet[b].y = enemy[e].y - 17000 + ypart(enemy[e].aim [0], 7000);
//            if ((arena.counter & 1) == 1)
//             enemy[e].aim [0] += enemy[e].angle1;
           }
          ebullet[b].angle = enemy[e].aim [enemy[e].status];
          ebullet[b].timeout = 2;
          ebullet[b].colour = 0;
          ebullet[b].status = enemy[e].burst;
          if (enemy[e].burst == 85)
           play_effectwfvx(WAV_BEAM2, 700, 230, enemy[e].x);
         }
         enemy[e].burst --;
         if (enemy[e].burst > 100)
          enemy[e].burst --;
   }
   break;

  case ENEMY_DOUBLE2:
   if (enemy[e].status == 1 || enemy[e].burst <= 0)
    aim_at_target(e, enemy[e].x - 19000, enemy[e].y - 10000, player.x, player.y, 5, 0);
   if (enemy[e].status == 0 || enemy[e].burst <= 0)
    aim_at_target(e, enemy[e].x + 19000, enemy[e].y - 10000, player.x, player.y, 5, 1);
   if (enemy[e].recycle <= 0 && enemy[e].y < 500000)
   {
//       play_effectwfvx(WAV_BEAM1, 100, 220, enemy[e].x);
       play_effectwfvx(WAV_BEAM3, 300, 220, enemy[e].x);
       enemy[e].burst = 120;
       enemy[e].recycle = 122;
       if (enemy[e].status == 0)
       {
        enemy[e].status = 1;
        enemy[e].angle1 = 1;
//        if (arena.difficulty == 2)
//         enemy[e].angle1 = 2;
       }
         else
         {
          enemy[e].status = 0;
          enemy[e].angle1 = -1;
//          if (arena.difficulty == 2)
//           enemy[e].angle1 = -2;
         }
//       enemy[e].angle1 = delta_turn_towards_xy(enemy[e].x, enemy[e].y, player.x, player.y, enemy[e].aim [enemy[e].status], 1);
        // inexact but doesn't really matter
   }
   if (enemy[e].burst > 0)
   {
         b = create_ebullet(EBULLET_BEAM);
         if (b != -1)
         {
          angle += ANGLE_32;
          if (enemy[e].status == 1)
          {
           ebullet[b].x = enemy[e].x + 19000 + xpart(enemy[e].aim [1], 6000);
           ebullet[b].y = enemy[e].y - 10000 + ypart(enemy[e].aim [1], 6000);
           if ((arena.counter & 1) == 1)
            enemy[e].aim [1] += enemy[e].angle1;
          }
           else
           {
            ebullet[b].x = enemy[e].x - 19000 + xpart(enemy[e].aim [0], 6000);
            ebullet[b].y = enemy[e].y - 10000 + ypart(enemy[e].aim [0], 6000);
            if ((arena.counter & 1) == 1)
             enemy[e].aim [0] += enemy[e].angle1;
           }
          ebullet[b].angle = enemy[e].aim [enemy[e].status];
          ebullet[b].timeout = 2;
          ebullet[b].colour = 0;
          ebullet[b].status = enemy[e].burst;
//          if (enemy[e].burst == 75)
//           play_effectwfvx(WAV_BEAM3, 500, 220, enemy[e].x);
         }
         enemy[e].burst --;
   }
   break;


  case ENEMY_DOUBLE4:
   if (enemy[e].burst <= 0)
    aim_at_target(e, enemy[e].x - 69000, enemy[e].y + 19000, player.x, player.y, 9, 0);
   if (enemy[e].burst <= 0)
    aim_at_target(e, enemy[e].x + 69000, enemy[e].y + 19000, player.x, player.y, 9, 1);
   if (enemy[e].recycle <= 0 && enemy[e].y < 500000)
   {
       play_effectwfvx(WAV_BEAM3, 250, 220, enemy[e].x);
       enemy[e].burst = 120;
       enemy[e].recycle = 142;
       if (enemy[e].status == 0)
       {
        enemy[e].status = 1;
        enemy[e].angle1 = 1;
       }
         else
         {
          enemy[e].status = 0;
          enemy[e].angle1 = -1;
         }
   }
   if (enemy[e].burst > 0)
   {
         b = create_ebullet(EBULLET_BEAM);
         if (b != -1)
         {
//          angle += ANGLE_32;
           ebullet[b].x = enemy[e].x - 69000 + xpart(enemy[e].aim [0], 6000);
           ebullet[b].y = enemy[e].y + 19000 + ypart(enemy[e].aim [0], 6000);
           if ((arena.counter & 1) == 1)
            enemy[e].aim [0] += enemy[e].angle1;
          ebullet[b].angle = enemy[e].aim [0];
          ebullet[b].timeout = 2;
          ebullet[b].colour = 3;
          ebullet[b].status = enemy[e].burst;
         }

         b = create_ebullet(EBULLET_BEAM);
         if (b != -1)
         {
//          angle += ANGLE_32;
           ebullet[b].x = enemy[e].x + 69000 + xpart(enemy[e].aim [1], 6000);
           ebullet[b].y = enemy[e].y + 19000 + ypart(enemy[e].aim [1], 6000);
           if ((arena.counter & 1) == 1)
            enemy[e].aim [1] -= enemy[e].angle1;
          ebullet[b].angle = enemy[e].aim [1];
          ebullet[b].timeout = 2;
          ebullet[b].colour = 3;
          ebullet[b].status = enemy[e].burst;
         }
         enemy[e].burst --;

   }
   break;



  case ENEMY_BIG2:
   quick_cloud(CLOUD_CIRCLE2, enemy[e].x, enemy[e].y + 95000, enemy[e].x_speed, enemy[e].y_speed + 4000 + grand(4000), 15 + grand(10), CLOUD_COL_1, 980);
   angle = 64 * grand(16); //grand(ANGLE_1);
   if (enemy[e].recycle <= 0)
   {
    play_effectwfvx(WAV_DOUBLE, 300 + grand(10), 190, enemy[e].x);
    for (i = 0; i < 32; i ++)
    {
         b = create_ebullet(EBULLET_IBULLET);
         if (b != -1)
         {
          angle += ANGLE_32;
          ebullet[b].x = enemy[e].x;
          ebullet[b].y = enemy[e].y - 25000;
          ebullet[b].angle = angle;
          speed = 2500;
          ebullet[b].x_speed = enemy[e].x_speed + xpart(ebullet[b].angle, speed);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(ebullet[b].angle, speed);
          ebullet[b].x_speed += xpart(i * 128, 1200);
          ebullet[b].y_speed += ypart(i * 128, 1200);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_3;
          ebullet[b].status = 0;
         }
    }
//    angle += ANGLE_3;
    for (i = 0; i < 32; i ++)
    {
         b = create_ebullet(EBULLET_IBULLET_2);
         if (b != -1)
         {
          angle += ANGLE_32;
          ebullet[b].x = enemy[e].x;
          ebullet[b].y = enemy[e].y - 25000;
          ebullet[b].angle = angle;
          speed = 1200;
          ebullet[b].x_speed = enemy[e].x_speed + xpart(ebullet[b].angle, speed);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(ebullet[b].angle, speed);
          ebullet[b].x_speed += xpart(i * 128, 600);
          ebullet[b].y_speed += ypart(i * 128, 600);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_2;
          ebullet[b].status = 0;
         }
    }
    quick_cloud(CLOUD_FCIRCLE2, enemy[e].x, enemy[e].y - 25000, enemy[e].x_speed, enemy[e].y_speed, 40 + grand(10), CLOUD_COL_3, 950);
    enemy[e].recycle = 150 DIFF 20;
   }
   break;
  case ENEMY_DOUBLE:
   aim_at_target(e, enemy[e].x - 21000, enemy[e].y + 10000, player.x, player.y, 4, 0);
   aim_at_target(e, enemy[e].x + 21000, enemy[e].y + 10000, player.x, player.y, 4, 1);
        if (enemy[e].recycle <= 0)
        {
         if (enemy[e].status == 0)
          enemy[e].status = 1;
           else
            enemy[e].status = 0;
         play_effectwfvx(WAV_FIRE, 1800 + grand(200), 140, enemy[e].x);
         b = create_ebullet(EBULLET_SHOT2);
         if (b != -1)
         {
          if (enemy[e].status == 0)
          {
           ebullet[b].x = enemy[e].x - 21504 + xpart(enemy[e].aim [enemy[e].status], 5000);
           ebullet[b].y = enemy[e].y + 10240 + ypart(enemy[e].aim [enemy[e].status], 5000);
          }
           else
           {
            ebullet[b].x = enemy[e].x + 21504 + xpart(enemy[e].aim [enemy[e].status], 5000);
            ebullet[b].y = enemy[e].y + 10240 + ypart(enemy[e].aim [enemy[e].status], 5000);
           }
          ebullet[b].angle = enemy[e].aim [enemy[e].status];
          ebullet[b].x_speed = enemy[e].x_speed + xpart(ebullet[b].angle, 6000);
          ebullet[b].y_speed = enemy[e].y_speed + ypart(ebullet[b].angle, 6000);
          ebullet[b].timeout = 4000;
          ebullet[b].colour = CLOUD_COL_2;
          quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, enemy[e].x_speed, enemy[e].y_speed, 30 + grand(10), CLOUD_COL_2, 975);
          for (i = 0; i < 4; i ++)
          {
           angle = ebullet[b].angle + ANGLE_4 + grand(100);
           speed = 3000 + grand(1000);
           quick_cloud(CLOUD_FCIRCLE4, ebullet[b].x, ebullet[b].y, enemy[e].x_speed + xpart(angle, speed), enemy[e].y_speed + ypart(angle, speed), 30 + grand(10), CLOUD_COL_2, 940);
           angle = ebullet[b].angle - ANGLE_4 - grand(100);
           speed = 3000 + grand(1000);
           quick_cloud(CLOUD_FCIRCLE4, ebullet[b].x, ebullet[b].y, enemy[e].x_speed + xpart(angle, speed), enemy[e].y_speed + ypart(angle, speed), 30 + grand(10), CLOUD_COL_2, 940);
          }
//          ebullet[b].status = 25 + grand (8);
         }
         enemy[e].recycle = 40 DIFF 10;
        }
   break;
  case ENEMY_WIDE:
   if (enemy[e].phase != 2)
    break;
        if (enemy[e].recycle <= 0)
        {
            play_effectwfvx(WAV_DOUBLE, 2900 + grand(200), 190, enemy[e].x);
            angle = ANGLE_4 + grand(24) - grand(24);
              b = create_ebullet(EBULLET_BLOB);
              if (b != -1)
              {
               ebullet[b].x = enemy[e].x;
               ebullet[b].y = enemy[e].y;
               speed = 2500 + grand(2000);
               ebullet[b].x_speed = enemy[e].x_speed + xpart(angle, speed);
               ebullet[b].y_speed = enemy[e].y_speed + ypart(angle, speed);
               ebullet[b].timeout = 4000;
               ebullet[b].colour = CLOUD_COL_3;
               ebullet[b].status = 5 + grand(7);
              }
              angle += ANGLE_2;
              b = create_ebullet(EBULLET_BLOB);
              if (b != -1)
              {
               ebullet[b].x = enemy[e].x;
               ebullet[b].y = enemy[e].y;
               speed = 2500 + grand(2000);
               ebullet[b].x_speed = enemy[e].x_speed + xpart(angle, speed);
               ebullet[b].y_speed = enemy[e].y_speed + ypart(angle, speed);
               ebullet[b].timeout = 4000;
               ebullet[b].colour = CLOUD_COL_3;
               ebullet[b].status = 5 + grand(7);
              }
            quick_cloud(CLOUD_CIRCLE1_2, enemy[e].x, enemy[e].y, enemy[e].x_speed, enemy[e].y_speed, 10 + grand(10), CLOUD_COL_4, 0);
            enemy[e].recycle = 5 DIFF 1;
        }
       break;

  case ENEMY_BIG4:
        if (enemy[e].recycle <= 0 && enemy[e].y < 500000)
        {
            play_effectwfvx(WAV_DOUBLE, 1000 + grand(100), 190, enemy[e].x);
            angle = grand(ANGLE_1);
            for (i = 0; i < 9; i ++)
            {
              angle += ANGLE_9 / 2;
              b = create_ebullet(EBULLET_SPINNER3);
              if (b != -1)
              {
               ebullet[b].x = enemy[e].x;
               ebullet[b].y = enemy[e].y - 8000;
               ebullet[b].x_speed = xpart(angle, 2200);
               ebullet[b].y_speed = ypart(angle, 2200);
               ebullet[b].timeout = 4000;
               ebullet[b].colour = CLOUD_COL_1;
               if (e & 1)
                ebullet[b].status = 13;
                 else
                  ebullet[b].status = -13;
              }
              angle += ANGLE_9 / 2;
              b = create_ebullet(EBULLET_STOPMINE);
              if (b != -1)
              {
               ebullet[b].x = enemy[e].x;
               ebullet[b].y = enemy[e].y - 8000;
               ebullet[b].x_speed = xpart(angle, 6400);
               ebullet[b].y_speed = ypart(angle, 6400);
               ebullet[b].timeout = 200;
               ebullet[b].colour = CLOUD_COL_3;
               if (e & 1)
                ebullet[b].status = -8;
                 else
                  ebullet[b].status = 8;
              }
            }
            quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y, enemy[e].x_speed, enemy[e].y_speed, 30 + grand(16), CLOUD_COL_4, 0);
            enemy[e].recycle = 150 DIFF 15;
        }
       break;


  case ENEMY_DOME2:
        if (enemy[e].recycle <= 0)
        {
            play_effectwfvx(WAV_DOUBLE, 1000 + grand(100), 190, enemy[e].x);
            angle = grand(ANGLE_1);
            for (i = 0; i < 7; i ++)
            {
              angle += ANGLE_7;
              b = create_ebullet(EBULLET_BLOB);
              if (b != -1)
              {
               ebullet[b].x = enemy[e].x;
               ebullet[b].y = enemy[e].y;
               ebullet[b].x_speed = enemy[e].x_speed + xpart(angle, 3500);
               ebullet[b].y_speed = enemy[e].y_speed + ypart(angle, 3500);
               ebullet[b].timeout = 4000;
               ebullet[b].colour = CLOUD_COL_3;
               ebullet[b].status = 12;
              }
              b = create_ebullet(EBULLET_BLOB);
              if (b != -1)
              {
               ebullet[b].x = enemy[e].x;
               ebullet[b].y = enemy[e].y;
               ebullet[b].x_speed = enemy[e].x_speed + xpart(angle, 2500);
               ebullet[b].y_speed = enemy[e].y_speed + ypart(angle, 2500);
               ebullet[b].timeout = 4000;
               ebullet[b].colour = CLOUD_COL_1;
               ebullet[b].status = 6;
              }
              b = create_ebullet(EBULLET_BLOB);
              if (b != -1)
              {
               ebullet[b].x = enemy[e].x;
               ebullet[b].y = enemy[e].y;
               ebullet[b].x_speed = enemy[e].x_speed + xpart(angle, 4500);
               ebullet[b].y_speed = enemy[e].y_speed + ypart(angle, 4500);
               ebullet[b].timeout = 4000;
               ebullet[b].colour = CLOUD_COL_1;
               ebullet[b].status = 6;
              }
              b = create_ebullet(EBULLET_BLOB);
              if (b != -1)
              {
               ebullet[b].x = enemy[e].x;
               ebullet[b].y = enemy[e].y;
               ebullet[b].x_speed = enemy[e].x_speed + xpart(angle + ANGLE_14, 3500);
               ebullet[b].y_speed = enemy[e].y_speed + ypart(angle + ANGLE_14, 3500);
               ebullet[b].timeout = 4000;
               ebullet[b].colour = CLOUD_COL_1;
               ebullet[b].status = 6;
              }
            }
            quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y, enemy[e].x_speed, enemy[e].y_speed, 30 + grand(16), CLOUD_COL_4, 0);
            enemy[e].recycle = 70 DIFF 10;
        }
       break;
  case ENEMY_DOME:
        if (enemy[e].recycle <= 0)
        {
         if (enemy[e].phase == 4)
         {
          quick_cloud(CLOUD_SMALL_SHOCK, enemy[e].x, enemy[e].y, 0, enemy[e].y_speed, 30, CLOUD_COL_3, 0);
          quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y, 0, enemy[e].y_speed, 30 + grand(16), CLOUD_COL_3, 0);
          angle = grand(ANGLE_1);
          for (i = 0; i < 5; i ++)
          {
           angle += ANGLE_5;
           play_effectwfvx(WAV_INTER, 500 + grand(100), 190, enemy[e].x);
           b = create_ebullet(EBULLET_FLAKE);
           if (b != -1)
           {
            ebullet[b].x = enemy[e].x;
            ebullet[b].y = enemy[e].y;
            ebullet[b].angle = angle;
            speed = 3000;// + grand(1500);
            ebullet[b].x_speed = enemy[e].x_speed + xpart(angle, speed);
            ebullet[b].y_speed = enemy[e].y_speed + ypart(angle, speed);
//            if (abs(ebullet[b].y_speed) + abs(ebullet[b].y_speed) < 100)
//             ebullet[b].y_speed = 500;
            ebullet[b].timeout = 4000; // important!
            ebullet[b].status = 1;
            ebullet[b].colour = CLOUD_COL_3;
            ebullet[b].angle2 = coin(); // rotation direction
           }
          }
          enemy[e].recycle = 55 DIFF 12;
         }
          else
          {
           b = create_ebullet(EBULLET_SHOT);
           play_effectwfvx(WAV_CHIRP, 2800 + grand(200), 120, enemy[e].x);
           if (b != -1)
           {
            ebullet[b].x = enemy[e].x;
            ebullet[b].y = enemy[e].y;
            ebullet[b].angle = grand(ANGLE_1);
            ebullet[b].x_speed = enemy[e].x_speed + xpart(ebullet[b].angle, 4000);
            ebullet[b].y_speed = enemy[e].y_speed + ypart(ebullet[b].angle, 4000);
            ebullet[b].timeout = 4000;
            ebullet[b].colour = CLOUD_COL_1;
            quick_cloud(CLOUD_FCIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 10 + grand(10), CLOUD_COL_1, 950);
            ebullet[b].status = 12 + grand (8);
           }
           enemy[e].recycle = 6 DIFF 1;
          }

/*         angle_inc = ANGLE_1 / 11;
         speed = grand(ANGLE_1);
//       speed = angle;
         for (i = 0; i < 11; i ++)
         {
          angle += angle_inc;
          angle &= 1023;
          b = create_ebullet(EBULLET_WIGGLY);
          if (b != -1)
          {
           ebullet[b].x = enemy[e].x;
           ebullet[b].y = enemy[e].y;
           ebullet[b].angle = angle;
           ebullet[b].x_speed = xpart(angle, 4000);
           ebullet[b].y_speed = ypart(angle, 4000);
           ebullet[b].timeout = 4000;
           ebullet[b].status = 8;
           ebullet[b].colour = CLOUD_COL_2;
          }

         }

          angle = grand(ANGLE_1);
          angle_inc = ANGLE_1 / 21;
          speed = grand(ANGLE_1);
         for (i = 0; i < 21; i ++)
         {
          b = create_ebullet(EBULLET_SHOT);
          if (b != -1)
          {
           speed += angle_inc;
           angle += angle_inc;
           ebullet[b].x = enemy[e].x;
           ebullet[b].y = enemy[e].y;
           ebullet[b].angle = angle;
//           ebullet[b].x_speed = xpart(angle, 3000) + xpart(angle + 30, xpart(speed, 1500));
//           ebullet[b].y_speed = ypart(angle, 3000) + ypart(angle + 30, xpart(speed, 1500));
           ebullet[b].x_speed = xpart(angle, 1000 + abs(ypart(speed, 3500)));
           ebullet[b].y_speed = ypart(angle, 1000 + abs(ypart(speed, 3500)));
           ebullet[b].timeout = 4000;
           ebullet[b].status = 5;
           ebullet[b].colour = CLOUD_COL_3;
          }

         }*/
        }
   break;

 }

}

void launch_part(int pe, int index, int se, int xs, int ys)
{
 enemy[se].x_speed = xs;
 enemy[se].y_speed = ys;
 enemy[se].parent = -1;

 enemy[pe].part [index] = -1;

}

void move_enemy(int e)
{
     enemy[e].x += enemy[e].x_speed;
/*
     if (enemy[e].x > X_MAX
         || enemy[e].x < X_MIN)
      enemy[e].x_speed *= -1; // bounce*/

     enemy[e].y += enemy[e].y_speed;
/*
     if (enemy[e].y > Y_MAX
         || enemy[e].y < Y_MIN)
      enemy[e].y_speed *= -1;
*/
// the edge collision should take account of enemy size, but it doesn't for now.

}
/*
int fire_dart(int x, int y, int angle, int colour, int wait, int accel)
{
 int b = create_ebullet(EBULLET_SHOT);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = 0;
  ebullet[b].y_speed = 0;
  ebullet[b].timeout = 500;
  ebullet[b].accel = accel;
  return b;
 }
 return -1;
}

int fire_sdart(int x, int y, int angle, int colour, int wait, int accel)
{
 int b = create_ebullet(EBULLET_SDART);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = 0;
  ebullet[b].y_speed = 0;
  ebullet[b].timeout = 500;
  ebullet[b].accel = accel;
  return b;
 }
 return -1;
}

int fire_ddart(int x, int y, int angle, int colour, int wait, int accel)
{
 int b = create_ebullet(EBULLET_DDART);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = 0;
  ebullet[b].y_speed = 0;
  ebullet[b].timeout = 500;
  ebullet[b].accel = accel;
  return b;
 }
 return -1;

}

int fire_diamond(int x, int y, int angle, int colour, int speed, int rot, int wait)
{
 int b = create_ebullet(EBULLET_SHOT2);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = xpart(angle, speed);
  ebullet[b].y_speed = ypart(angle, speed);
  ebullet[b].timeout = 500;
  ebullet[b].angle2 = rot;
  return b;
 }
 return -1;

}

int fire_curve(int x, int y, int angle, int colour, int speed, int rot, int wait)
{
 int b = create_ebullet(EBULLET_CURVE);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = speed;//xpart(angle, speed);
//  ebullet[b].y_speed = pos_or_neg(2);//ypart(angle, speed);
  ebullet[b].y_speed = 2 + grand(4);
  if (rot < 0)
   ebullet[b].y_speed = -2 - grand(4);
  ebullet[b].status = ebullet[b].angle;
  ebullet[b].timeout = 600;
  ebullet[b].angle2 = rot;
  return b;
 }
 return -1;

}


int fire_wing_diamond(int x, int y, int angle, int colour, int speed, int wait)
{
 int b = create_ebullet(EBULLET_WING_DIAMOND);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = xpart(angle, speed);
  ebullet[b].y_speed = ypart(angle, speed);
  ebullet[b].timeout = 500;
  ebullet[b].status = pos_or_neg(8);
  ebullet[b].accel = 50 + arena.difficulty * 15;
  return b;
 }
 return -1;

}


int fire_seed(int x, int y, int angle, int colour, int wait, int accel)
{
 int b = create_ebullet(EBULLET_SEED);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].colour = grand(4);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = xpart(ebullet[b].angle, accel);
  ebullet[b].y_speed = ypart(ebullet[b].angle, accel);
  ebullet[b].timeout = 500;
  ebullet[b].accel = accel;
  return b;
 }
 return -1;

}



int fire_seed2(int x, int y, int angle, int colour, int wait, int accel)
{
 int b = create_ebullet(EBULLET_LSEED);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].colour = grand(4);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = xpart(ebullet[b].angle, accel);
  ebullet[b].y_speed = ypart(ebullet[b].angle, accel);
  ebullet[b].timeout = 500;
  ebullet[b].accel = accel;
  return b;
 }
 return -1;

}

int fire_burst(int x, int y, int angle, int colour, int speed, int wait)
{
 int b = create_ebullet(EBULLET_BURST);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = xpart(angle, speed);
  ebullet[b].y_speed = ypart(angle, speed);
  ebullet[b].timeout = 60 + grand(20);
  ebullet[b].status = pos_or_neg(8);
  ebullet[b].accel = 50;
  return b;
 }
 return -1;

}


int fire_tracker(int x, int y, int angle, int colour, int size, int speed)
{
 int b = create_ebullet(EBULLET_TRACKER);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].x_speed = xpart(angle, speed);
  ebullet[b].y_speed = ypart(angle, speed);
  ebullet[b].timeout = 500;
  ebullet[b].status = size;
  ebullet[b].cord_wait = -1;
  quick_cloud(CLOUD_CIRCLE5, ebullet[b].x, ebullet[b].y, 0, 0, 30, TCOL_B2, 0);
//  ebullet[b].accel = accel;
  return b;
 }
 return -1;
}


int fire_stream(int x, int y, int angle, int colour, int speed, int wait)
{
 int b = create_ebullet(EBULLET_STREAM);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = xpart(angle, speed);
  ebullet[b].y_speed = ypart(angle, speed);
  ebullet[b].timeout = 500;
//  ebullet[b].status = pos_or_neg(8);
  ebullet[b].accel = 50;
  return b;
 }
 return -1;

}

int fire_dot(int x, int y, int angle, int colour, int size, int speed, int wait)
{
 int b = create_ebullet(EBULLET_DOT);
 if (b != -1)
 {
  angle &= 1023;
  ebullet[b].x = x;
  ebullet[b].y = y;
  ebullet[b].angle = angle;
  ebullet[b].colour = colour;
  set_cord_colour(b);
  ebullet[b].cord_x = x;
  ebullet[b].cord_y = y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = wait;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = xpart(angle, speed);
  ebullet[b].y_speed = ypart(angle, speed);
  ebullet[b].timeout = 500;
  ebullet[b].status = size;
//  ebullet[b].accel = accel;
  return b;
 }
 return -1;
}

int fire_beam(int e, int dist)
{
 int length = 1;
 int x = enemy[e].x + xpart(enemy[e].angle1, dist);
 int y = enemy[e].y + ypart(enemy[e].angle1, dist);

 do
 {
  length ++;
  x += xpart(enemy[e].angle1, 45000);
  y += ypart(enemy[e].angle1, 45000);
  if (x > 690000
   || x < -50000
   || y > 530000
   || y < - 50000)
    break;
 } while(TRUE);
 int b = create_ebullet(EBULLET_BEAM);
 if (b != -1)
 {
  enemy[e].angle1 &= 1023;
  if (enemy[e].status > 100 || enemy[e].status < 20)
  {
    ebullet[b].x = enemy[e].x + xpart(enemy[e].angle1, dist + 4000);
    ebullet[b].y = enemy[e].y + ypart(enemy[e].angle1, dist + 4000);
  }
   else
   {
    ebullet[b].x = enemy[e].x + xpart(enemy[e].angle1, dist + 4000 - (enemy[e].status % 6) * GRAIN * 4);
    ebullet[b].y = enemy[e].y + ypart(enemy[e].angle1, dist + 4000 - (enemy[e].status % 6) * GRAIN * 4);
   }
  ebullet[b].status = length;
  ebullet[b].angle = enemy[e].angle1;
  ebullet[b].sprite_angle = enemy[e].angle1 / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = enemy[e].x + xpart(enemy[e].angle1, dist);
  ebullet[b].y_speed = enemy[e].y + ypart(enemy[e].angle1, dist);
  ebullet[b].timeout = 2;
  ebullet[b].angle2 = enemy[e].status;
  if (enemy[e].status > 100)
   ebullet[b].angle2 = 120 - enemy[e].status;
  return b;
 }
 return -1;
}

int fire_beam2(int e, int angle, int dist, int time)
{
 int length = 1;
 int x = enemy[e].x + xpart(angle, dist);
 int y = enemy[e].y + ypart(angle, dist);

 do
 {
  length ++;
  x += xpart(enemy[e].angle1, 45000);
  y += ypart(enemy[e].angle1, 45000);
  if (x > 690000
   || x < -50000
   || y > 530000
   || y < - 50000)
    break;
 } while(TRUE);
 if (length > (100 - enemy[e].status + 7) / 6)
  length = (100 - enemy[e].status + 7) / 6;
 int b = create_ebullet(EBULLET_BEAM2);
 if (b != -1)
 {
  angle &= 1023;
  if (enemy[e].status > 100 || enemy[e].status < 20)
  {
    ebullet[b].x = enemy[e].x + xpart(enemy[e].angle1, dist + 4000);
    ebullet[b].y = enemy[e].y + ypart(enemy[e].angle1, dist + 4000);
  }
   else
   {
    ebullet[b].x = enemy[e].x + xpart(enemy[e].angle1, dist + 4000 - ((enemy[e].status * 2) % 12) * GRAIN * 4);
    ebullet[b].y = enemy[e].y + ypart(enemy[e].angle1, dist + 4000 - ((enemy[e].status * 2) % 12) * GRAIN * 4);
   }
  ebullet[b].status = length;
  ebullet[b].angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = enemy[e].x + xpart(angle, dist);
  ebullet[b].y_speed = enemy[e].y + ypart(angle, dist);
  ebullet[b].timeout = 2;
  ebullet[b].angle2 = enemy[e].status;
//  if (enemy[e].status > 100)
//   ebullet[b].angle2 = 120 - enemy[e].status;
  return b;
 }
 return -1;
}

*/

void enemy_fire(int e)
{
 int btype = EBULLET_SHOT + grand(2);
 btype = EBULLET_SHOT;
 int b = create_ebullet(btype);
 if (b != -1)
 {
  switch(btype)
  {
/*  case EBULLET_SHOT:
  ebullet[b].x = enemy[e].x + xpart(angle, 10000);
  ebullet[b].y = enemy[e].y + ypart(angle, 10000);
  ebullet[b].angle = angle;
  ebullet[b].colour = EBCOL_B1;//grand(5);
  set_cord_colour(b);
//ebullet[b].cord_colour = TRANS_ORANGE_OUT;
  ebullet[b].cord_x = ebullet[b].x;
  ebullet[b].cord_y = ebullet[b].y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = 50;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = 0;//xpart(angle, 3000);
  ebullet[b].y_speed = 0;//ypart(angle, 3000);
  ebullet[b].timeout = 500;
//  play_effectwfvx(WAV_FIRE, 500 + grand(1000), 200, ebullet[b].x);
  enemy[e].x_speed -= xpart(angle, 400);
  enemy[e].y_speed -= ypart(angle, 400); // a little bit of recoil
   enemy[e].recycle = 50;
  ebullet[b].accel = 60;
  break;
  case EBULLET_SHOT2:
  ebullet[b].x = enemy[e].x + xpart(angle, 10000);
  ebullet[b].y = enemy[e].y + ypart(angle, 10000);
  ebullet[b].angle = angle;
  ebullet[b].colour = EBCOL_B1;//grand(3);
  if (grand(5) == 0)
   ebullet[b].colour = grand(5);
  set_cord_colour(b);
  ebullet[b].cord_x = ebullet[b].x;
  ebullet[b].cord_y = ebullet[b].y;
  ebullet[b].cord_x_speed = 0;
  ebullet[b].cord_y_speed = 0;
  ebullet[b].cord_wait = 50;
  ebullet[b].cord_angle = angle;
  ebullet[b].sprite_angle = angle / (ANGLE_1 / SMALL_ROTATIONS);
  ebullet[b].x_speed = xpart(angle, 2000);
  ebullet[b].y_speed = ypart(angle, 2000);
  ebullet[b].timeout = 500;
  play_effectwfvx(WAV_FIRE, 500 + grand(1000), 200, ebullet[b].x);
  enemy[e].x_speed -= xpart(angle, 400);
  enemy[e].y_speed -= ypart(angle, 400); // a little bit of recoil
  ebullet[b].angle2 = -10 + grand(11);
//  if (grand(2) == 0)
   //ebullet[b].angle2 = -10;
   enemy[e].recycle = 50;
   break;*/
  }
 }
//      play_effectwfvx(w, NWAV_ZAP, 1800 + grand(50), 30, enemy[e].x);
}


// atan2 is slow, so don't use it too much.
// see stuff.c for radians_to_angle etc.
int attack_angle(int e)
{
 return radians_to_angle(atan2(player.y - enemy[e].y, player.x - enemy[e].x));
}

int get_e_angle(int e, int x, int y)
{
 return radians_to_angle(atan2(y - enemy[e].y, x - enemy[e].x));
}


void aim_at_target(int e, int x, int y, int tx, int ty, int turn, int aiming)
{


      int angle_move = delta_turn_towards_xy(x, y,
          tx, ty, enemy[e].aim [aiming], 1);

      if (angle_move != enemy[e].aim_turning [aiming])
      {
       if (enemy[e].aim_turning_count [aiming] <= 0)
       {
        enemy[e].aim_turning_count [aiming] = 15;
        enemy[e].aim_turning [aiming] = angle_move;
        return;

       }
       enemy[e].aim_turning_count [aiming] --;
       return;
      }

      enemy[e].aim_turning_count [aiming] --;
      enemy[e].aim [aiming] += turn * enemy[e].aim_turning [aiming];

      enemy[e].aim [aiming] &= 1023;

}


void track_target(int e, int x, int y, int angle, int turn)
{


      int angle_move = delta_turn_towards_xy(enemy[e].x, enemy[e].y,
          x, y, angle, 1);

//      textprintf_ex(screen, font, 5, 5, COLOUR_11, COLOUR_2, "%i %i %i %i %i %i    ", e, enemy[w] [e].type, enemy[w] [e].x, enemy[w] [e].y, turn, angle_move);


      if (angle_move != enemy[e].angle1_turning)
      {
       if (enemy[e].angle1_turning_count <= 0)
       {
        enemy[e].angle1_turning_count = 15;
        enemy[e].angle1_turning = angle_move;
/*        if (angle_move < 0)
         enemy[e].angle1_turning = -1;
          else
           enemy[e].angle1_turning = 1;*/
        return;

       }
       enemy[e].angle1_turning_count --;
       return;
      }

      enemy[e].angle1_turning_count --;
      enemy[e].angle1 += turn * enemy[e].angle1_turning;

      enemy[e].angle1 &= 1023;

}

void track_target_zero(int e, int x, int y, int angle, int turn)
{


      int angle_move = delta_turn_towards_xy(enemy[e].x, enemy[e].y,
          x, y, angle, 1);

//      textprintf_ex(screen, font, 5, 5, COLOUR_11, COLOUR_2, "%i %i %i %i %i %i    ", e, enemy[w] [e].type, enemy[w] [e].x, enemy[w] [e].y, turn, angle_move);


      if (angle_move != enemy[e].angle_turning)
      {
       if (enemy[e].angle_turning_count <= 0)
       {
        enemy[e].angle_turning_count = 15;
        enemy[e].angle_turning = angle_move;
/*        if (angle_move < 0)
         enemy[e].angle1_turning = -1;
          else
           enemy[e].angle1_turning = 1;*/
        return;

       }
       enemy[e].angle_turning_count --;
       return;
      }

      enemy[e].angle_turning_count --;
      enemy[e].angle += turn * enemy[e].angle_turning;

      enemy[e].angle &= 1023;

}

void drag_enemy(int e, int drag)
{
 enemy[e].x_speed *= drag;
 enemy[e].x_speed >>= 10;
 enemy[e].y_speed *= drag;
 enemy[e].y_speed >>= 10;

}

void drag_enemy_y(int e, int drag)
{
 enemy[e].y_speed *= drag;
 enemy[e].y_speed >>= 10;

}


int pbullet_hits_enemy(int b, int e)
{
/*
 if (pbullet[b].type == PBULLET_BLADE)
 {
  int rel_x = abs(pbullet[b].x_speed - enemy[e].x_speed);
  //if (rel_x < 2000)
   //rel_x = 2000;
  int rel_y = abs(pbullet[b].y_speed - enemy[e].y_speed);
//  if (rel_y < 2000)
//   rel_y = 2000;
  int dam = (pbullet[b].dam + (pbullet[b].dam * hypot(rel_y, rel_x) / 200)) / 160;
  hurt_enemy(e, dam, pbullet[b].source);
//  player.score = dam;
  return;
 }*/

 return hurt_enemy(e, pbullet[b].dam, pbullet[b].source);
}
// note explosion in pbullet.c calls hurt_enemy directly

int hurt_enemy(int e, int dam, int source)
{

 if (eclass[enemy[e].type].ai == AI_BOSS)
 {
  boss.hp -= dam;
  if (boss.hp <= 0)
  {
   boss.hp = 0;
   enemy_explode(e);
   boss.fight = 0;
   if (boss.end_of_stage == 1 && arena.game_over == 0)
   {
    arena.level_finished = 400;
    stage_finished();
   }
   return 0;
  }
  return 1;
 }


 enemy[e].hp -= dam;

 if (enemy[e].hp <= 0)
 {
  enemy[e].whurt [source] += enemy[e].hp + dam;
  enemy_explode(e);
  return 0;
 }

 enemy[e].whurt [source] += dam;

 return 1;
// deal with source/xp here

}


void enemy_explode(int e)
{

 int angle, angle2;
 int i, j;
 int c;
 int xa;

 switch(enemy[e].type)
 {
    case ENEMY_DOUBLE:
    play_effectwfvx(WAV_BANG4, 1800 + grand(200), 120, enemy[e].x);
      quick_cloud(CLOUD_LARGE_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y - 8000, 0, 0, 50 + grand(26), CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 25000, enemy[e].y + 25000, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 25000, enemy[e].y + 25000, 0, 0, 40, CLOUD_COL_1, 0);

      for (i = 0; i < 10; i ++)
      {

       angle = grand(ANGLE_1);
       xa = 15000 + grand(5400);
       quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), xpart(angle, xa / 15), ypart(angle, xa / 15), 80 + grand(26), CLOUD_COL_1, 960);

       //angle = grand(ANGLE_1);
       //xa = 20000 + grand(10000);
       //quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), 0, 0, 40 + grand(30), CLOUD_COL_1, 0);

       angle = grand(ANGLE_1);
       xa = 4000 + grand(5400);
       quick_cloud(CLOUD_FCIRCLE2, enemy[e].x + xpart(angle, 5000), enemy[e].y + ypart(angle, 5000), xpart(angle, xa), ypart(angle, xa), 40 + grand(10), CLOUD_COL_1, 980);
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x + xpart(angle, 5000), enemy[e].y + ypart(angle, 5000), xpart(angle, xa - 1000), ypart(angle, xa - 1000), 35 + grand(10), CLOUD_COL_1, 980);
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x + xpart(angle, 5000), enemy[e].y + ypart(angle, 5000), xpart(angle, xa - 2000), ypart(angle, xa - 2000), 30 + grand(10), CLOUD_COL_1, 980);
      }

     break;

    case ENEMY_BOSS3_1:
    play_effectwfvx(WAV_BANG4, 300 + grand(100), 240, enemy[e].x);
//    play_effectwfvx(WAV_BANG2, 500, 200, enemy[e].x);
      quick_cloud(CLOUD_LARGE_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 50, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y, 0, 0, 70, CLOUD_COL_4, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 106000, enemy[e].y + 65000, 0, 0, 70, CLOUD_COL_4, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 106000, enemy[e].y + 65000, 0, 0, 70, CLOUD_COL_4, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 55000, enemy[e].y + 7000, 0, 0, 70, CLOUD_COL_4, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 55000, enemy[e].y + 7000, 0, 0, 70, CLOUD_COL_4, 0);

      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 45000, enemy[e].y - 40000, 0, 0, 60, CLOUD_COL_4, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 45000, enemy[e].y - 40000, 0, 0, 60, CLOUD_COL_4, 0);

      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 100000, enemy[e].y + 5000, 0, 0, 60, CLOUD_COL_4, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 100000, enemy[e].y + 5000, 0, 0, 60, CLOUD_COL_4, 0);

      for (i = 0; i < 15; i ++)
      {

       angle = grand(ANGLE_1);
       xa = 25000 + grand(5400);
       quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa * 2), enemy[e].y + ypart(angle, xa), xpart(angle, xa / 10), ypart(angle, xa / 10), 80 + grand(26), CLOUD_COL_4, 960);

       //angle = grand(ANGLE_1);
       //xa = 20000 + grand(10000);
       //quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), 0, 0, 40 + grand(30), CLOUD_COL_1, 0);

       angle = grand(ANGLE_1);
       xa = 5000 + grand(5400);
       quick_cloud(CLOUD_FCIRCLE2, enemy[e].x + xpart(angle, 38000), enemy[e].y + ypart(angle, 25000), xpart(angle, xa), ypart(angle, xa), 60 + grand(30), CLOUD_COL_4, 980);
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x + xpart(angle, 38000), enemy[e].y + ypart(angle, 25000), xpart(angle, xa - 1000), ypart(angle, xa - 1000), 50 + grand(30), CLOUD_COL_4, 980);
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x + xpart(angle, 38000), enemy[e].y + ypart(angle, 25000), xpart(angle, xa - 2000), ypart(angle, xa - 2000), 40 + grand(30), CLOUD_COL_4, 980);
      }

     break;


    case ENEMY_BOSS3_2:
    case ENEMY_BOSS3_0:
    play_effectwfvx(WAV_BANG4, 300 + grand(100), 240, enemy[e].x);
//    play_effectwfvx(WAV_BANG2, 500, 200, enemy[e].x);
      quick_cloud(CLOUD_LARGE_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 50, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y, 0, 0, 70, CLOUD_COL_4, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 90000, enemy[e].y - 46000, 0, 0, 70, CLOUD_COL_4, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 90000, enemy[e].y - 46000, 0, 0, 70, CLOUD_COL_4, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 47000, enemy[e].y - 86000, 0, 0, 70, CLOUD_COL_4, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 47000, enemy[e].y - 86000, 0, 0, 70, CLOUD_COL_4, 0);

      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 80000, enemy[e].y + 51000, 0, 0, 60, CLOUD_COL_4, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 80000, enemy[e].y + 51000, 0, 0, 60, CLOUD_COL_4, 0);

      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 30000, enemy[e].y + 69000, 0, 0, 60, CLOUD_COL_4, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 30000, enemy[e].y + 69000, 0, 0, 60, CLOUD_COL_4, 0);

      for (i = 0; i < 15; i ++)
      {

       angle = grand(ANGLE_1);
       xa = 25000 + grand(5400);
       quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa * 2), enemy[e].y + ypart(angle, xa), xpart(angle, xa / 10), ypart(angle, xa / 10), 80 + grand(26), CLOUD_COL_4, 960);

       //angle = grand(ANGLE_1);
       //xa = 20000 + grand(10000);
       //quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), 0, 0, 40 + grand(30), CLOUD_COL_1, 0);

       angle = grand(ANGLE_1);
       xa = 5000 + grand(5400);
       quick_cloud(CLOUD_FCIRCLE2, enemy[e].x + xpart(angle, 38000), enemy[e].y + ypart(angle, 25000), xpart(angle, xa), ypart(angle, xa), 60 + grand(30), CLOUD_COL_4, 980);
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x + xpart(angle, 38000), enemy[e].y + ypart(angle, 25000), xpart(angle, xa - 1000), ypart(angle, xa - 1000), 50 + grand(30), CLOUD_COL_4, 980);
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x + xpart(angle, 38000), enemy[e].y + ypart(angle, 25000), xpart(angle, xa - 2000), ypart(angle, xa - 2000), 40 + grand(30), CLOUD_COL_4, 980);
      }

     break;

    case ENEMY_DOUBLE4:
     play_effectwfvx(WAV_BANG3, 450 + grand(100), 250, enemy[e].x);
      quick_cloud(CLOUD_HUGE_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y, 0, 0, 80 + grand(16), CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 85000, enemy[e].y + 15000, 2000, 0, 50 + grand(16), CLOUD_COL_1, 960);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 85000, enemy[e].y + 15000, -2000, 0, 50 + grand(16), CLOUD_COL_1, 960);

      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 25000, enemy[e].y - 45000, 1000, 1000, 50 + grand(16), CLOUD_COL_1, 950);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 25000, enemy[e].y - 45000, -1000, 1000, 50 + grand(16), CLOUD_COL_1, 950);

//      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y + 45000, 0, 0, 50 + grand(16), CLOUD_COL_1, 950);

      for (i = 0; i < 9; i ++)
      {
       angle2 = grand(ANGLE_1);
       j = 2900 + grand(2000);
       xa = 30000 + grand(30000);
       c = quick_cloud(CLOUD_WAITCIRCLE4, enemy[e].x + xpart(angle2, xa * 2), enemy[e].y + ypart(angle2, xa), xpart(angle2, j), ypart(angle2, j), 36 + grand(50), CLOUD_COL_1, 980);
       if (c != -1)
        cloud[c].status = 20 + grand(15);
       j = 2900 + grand(2000);
       xa = 30000 + grand(30000);
       angle2 += ANGLE_2;
       c = quick_cloud(CLOUD_WAITCIRCLE4, enemy[e].x + xpart(angle2, xa * 2), enemy[e].y + ypart(angle2, xa), xpart(angle2, j), ypart(angle2, j), 36 + grand(50), CLOUD_COL_1, 980);
       if (c != -1)
        cloud[c].status = 20 + grand(15);
      }
     break;

    case ENEMY_BIG4:
    play_effectwfvx(WAV_BANG3, 500 + grand(200), 240, enemy[e].x);
      quick_cloud(CLOUD_HUGE_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y, 0, 0, 80 + grand(16), CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 45000, enemy[e].y - 25000, 0, 0, 50 + grand(16), CLOUD_COL_1, 950);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 45000, enemy[e].y - 25000, 0, 0, 50 + grand(16), CLOUD_COL_1, 950);

      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 35000, enemy[e].y + 35000, 0, 0, 50 + grand(16), CLOUD_COL_1, 950);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 35000, enemy[e].y + 35000, 0, 0, 50 + grand(16), CLOUD_COL_1, 950);

      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y + 45000, 0, 0, 50 + grand(16), CLOUD_COL_1, 950);

      for (i = 0; i < 9; i ++)
      {
       angle2 = grand(ANGLE_1);
       j = 2900 + grand(2000);
       xa = 30000 + grand(30000);
       c = quick_cloud(CLOUD_WAITCIRCLE4, enemy[e].x + xpart(angle2, xa), enemy[e].y + ypart(angle2, xa), xpart(angle2, j), ypart(angle2, j), 36 + grand(50), CLOUD_COL_1, 980);
       if (c != -1)
        cloud[c].status = 20 + grand(15);
       j = 2900 + grand(2000);
       xa = 30000 + grand(30000);
       angle2 += ANGLE_2;
       c = quick_cloud(CLOUD_WAITCIRCLE4, enemy[e].x + xpart(angle2, xa), enemy[e].y + ypart(angle2, xa), xpart(angle2, j), ypart(angle2, j), 36 + grand(50), CLOUD_COL_1, 980);
       if (c != -1)
        cloud[c].status = 20 + grand(15);
      }
     break;

    case ENEMY_DOME3:
    play_effectwfvx(WAV_BANG3, 900 + grand(200), 240, enemy[e].x);
      quick_cloud(CLOUD_HUGE_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y, 0, 0, 80 + grand(16), CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 22000, enemy[e].y + 42000, 0, 0, 50 + grand(16), CLOUD_COL_1, 950);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 22000, enemy[e].y + 42000, 0, 0, 50 + grand(16), CLOUD_COL_1, 950);

      for (i = 0; i < 7; i ++)
      {
       angle2 = grand(ANGLE_1);
       j = 200 + grand(3500);
       xa = 15000 + grand(20000);
       c = quick_cloud(CLOUD_WAITCIRCLE4, enemy[e].x + xpart(angle2, xa), enemy[e].y + ypart(angle2, xa), xpart(angle2, j), ypart(angle2, j), 30 + grand(40), CLOUD_COL_1, 980);
       if (c != -1)
        cloud[c].status = 20 + grand(15);
       j = 200 + grand(3500);
       xa = 15000 + grand(20000);
       angle2 += ANGLE_2;
       c = quick_cloud(CLOUD_WAITCIRCLE4, enemy[e].x + xpart(angle2, xa), enemy[e].y + ypart(angle2, xa), xpart(angle2, j), ypart(angle2, j), 30 + grand(40), CLOUD_COL_1, 980);
       if (c != -1)
        cloud[c].status = 15 + grand(11);
      }
     break;


    case ENEMY_WIDE:
    play_effectwfvx(WAV_BANG3, 600 + grand(200), 240, enemy[e].x);
      quick_cloud(CLOUD_LARGE_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y, 0, 0, 50 + grand(16), CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 25000, enemy[e].y, 700, 0, 50 + grand(16), CLOUD_COL_1, 950);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 50000, enemy[e].y, 1400, 0, 50 + grand(16), CLOUD_COL_1, 950);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 75000, enemy[e].y, 2100, 0, 50 + grand(16), CLOUD_COL_1, 950);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 25000, enemy[e].y, -700, 0, 50 + grand(16), CLOUD_COL_1, 950);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 50000, enemy[e].y, -1400, 0, 50 + grand(16), CLOUD_COL_1, 950);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 75000, enemy[e].y, -2100, 0, 50 + grand(16), CLOUD_COL_1, 950);
      for (i = 0; i < 7; i ++)
      {
       angle2 = grand(ANGLE_1);
       j = 2500 + grand(2000);
       xa = 19000 + grand(30000);
       c = quick_cloud(CLOUD_WAITCIRCLE4, enemy[e].x + xpart(angle2, xa), enemy[e].y + ypart(angle2, xa), xpart(angle2, j), ypart(angle2, j), 36 + grand(50), CLOUD_COL_1, 980);
       if (c != -1)
        cloud[c].status = 10 + grand(15);
       j = 2500 + grand(2000);
       xa = 19000 + grand(30000);
       angle2 += ANGLE_2;
       c = quick_cloud(CLOUD_WAITCIRCLE4, enemy[e].x + xpart(angle2, xa), enemy[e].y + ypart(angle2, xa), xpart(angle2, j), ypart(angle2, j), 36 + grand(50), CLOUD_COL_1, 980);
       if (c != -1)
        cloud[c].status = 10 + grand(15);
      }
     break;

    case ENEMY_FIGHTER7:
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 27000, enemy[e].y, 1500, 0, 25 + grand(16), CLOUD_COL_1, 990);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 27000, enemy[e].y, -1500, 0, 25 + grand(16), CLOUD_COL_1, 990);
// fall through...
    case ENEMY_FIGHTER8:
     play_effectwfvx(WAV_BANG1, 500 + grand(200), 240, enemy[e].x);
      quick_cloud(CLOUD_SMALL_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 30, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y, 0, 0, 40 + grand(16), CLOUD_COL_1, 0);
      for (i = 0; i < 5; i ++)
      {
       angle2 = grand(ANGLE_1);
       j = 1000 + grand(1500);
       xa = 10000 + grand(30000);
       c = quick_cloud(CLOUD_WAITCIRCLE4, enemy[e].x + xpart(angle2, xa), enemy[e].y + ypart(angle2, xa), xpart(angle2, j), ypart(angle2, j), 20 + grand(40), CLOUD_COL_1, 980);
       if (c != -1)
        cloud[c].status = 7 + grand(5);
       j = 1000 + grand(1500);
       xa = 10000 + grand(30000);
       angle2 += ANGLE_2;
       c = quick_cloud(CLOUD_WAITCIRCLE4, enemy[e].x + xpart(angle2, xa), enemy[e].y + ypart(angle2, xa), xpart(angle2, j), ypart(angle2, j), 20 + grand(40), CLOUD_COL_1, 980);
       if (c != -1)
        cloud[c].status = 7 + grand(5);
      }
     break;

    case ENEMY_MACHINE3:
     play_effectwfvx(WAV_BANG2, 700 + grand(100), 240, enemy[e].x);
      quick_cloud(CLOUD_LARGE_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y, 0, 0, 50 + grand(16), CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 25000, enemy[e].y + 19000, 0, 0, 50 + grand(16), CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 25000, enemy[e].y + 19000, 0, 0, 50 + grand(16), CLOUD_COL_1, 0);
      for (i = 0; i < 7; i ++)
      {
       angle2 = grand(ANGLE_1);
       j = 1000 + grand(2000);
       xa = 10000 + grand(30000);
       c = quick_cloud(CLOUD_WAITCIRCLE4, enemy[e].x + xpart(angle2, xa), enemy[e].y + ypart(angle2, xa), xpart(angle2, j), ypart(angle2, j), 26 + grand(50), CLOUD_COL_1, 980);
       if (c != -1)
        cloud[c].status = 10 + grand(5);
       j = 1000 + grand(2000);
       xa = 10000 + grand(30000);
       angle2 += ANGLE_2;
       c = quick_cloud(CLOUD_WAITCIRCLE4, enemy[e].x + xpart(angle2, xa), enemy[e].y + ypart(angle2, xa), xpart(angle2, j), ypart(angle2, j), 26 + grand(50), CLOUD_COL_1, 980);
       if (c != -1)
        cloud[c].status = 10 + grand(5);
      }
/*      angle = grand(ANGLE_1);
      for (i = 0; i < 4; i ++)
      {
       angle2 = angle;
       xa = 15000 + grand(12000);
       for (j = 0; j < 5; j ++)
       {
        c = quick_cloud(CLOUD_WAITCIRCLE4, enemy[e].x + xpart(angle2, xa), enemy[e].y + ypart(angle2, xa), xpart(angle2, 2000), ypart(angle2, 2000), 66 + grand(20) - j * 8, CLOUD_COL_1, 800);
        if (c != -1)
         cloud[c].status = 10 + grand(5) + j * 2;
        xa += 15000 + grand(4000);
        angle2 += grand(36) - grand(36);
        angle2 &= 1023;
       }
       angle += ANGLE_4;
      }*/

     break;
    case ENEMY_MACHINE2:
    case ENEMY_RISER:
      quick_cloud(CLOUD_CIRCLE2, enemy[e].x - 40000, enemy[e].y + 15000, 0, 0, 50 + grand(16), CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE2, enemy[e].x + 40000, enemy[e].y + 15000, 0, 0, 50 + grand(16), CLOUD_COL_1, 0);
// fall through...
    case ENEMY_MACHINE:
    play_effectwfvx(WAV_BANG2, 700 + grand(100), 240, enemy[e].x);
      for (i = 0; i < 5; i ++)
      {
       angle = grand(ANGLE_1);
       xa = 20000 + grand(4400);
       quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), xpart(angle, xa / 15), ypart(angle, xa / 15), 50 + grand(26), CLOUD_COL_1, 960);
      }
      quick_cloud(CLOUD_LARGE_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 40, CLOUD_COL_1, 0);
//      quick_cloud(CLOUD_SMALL_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 30, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y, 0, 0, 50 + grand(16), CLOUD_COL_1, 0);
      for (i = 0; i < 15; i ++)
      {
       angle = grand(ANGLE_1);
       xa = 4000 + grand(5400);
       quick_cloud(CLOUD_FCIRCLE2, enemy[e].x, enemy[e].y, xpart(angle, xa), ypart(angle, xa), 30 + grand(10), CLOUD_COL_1, 970 + grand(5));
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x, enemy[e].y, xpart(angle, xa - 1000), ypart(angle, xa - 1000), 30 + grand(10), CLOUD_COL_1, 975 + grand(5));
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x, enemy[e].y, xpart(angle, xa - 2000), ypart(angle, xa - 2000), 25 + grand(10), CLOUD_COL_1, 977 + grand(5));
      }
      break;
    case ENEMY_MEDIUM:
    play_effectwfvx(WAV_BANG3, 1500 + grand(200), 240, enemy[e].x);
      for (i = 0; i < 5; i ++)
      {
       angle = grand(ANGLE_1);
       xa = 20000 + grand(4400);
       quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), xpart(angle, xa / 15), ypart(angle, xa / 15), 40 + grand(26), CLOUD_COL_1, 960);
      }
      quick_cloud(CLOUD_SMALL_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 30, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y, 0, 0, 40 + grand(16), CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 10000, enemy[e].y - 25000, 0, 0, 30 + grand(10), CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 10000, enemy[e].y - 25000, 0, 0, 30 + grand(10), CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 19000, enemy[e].y + 15000, 0, 0, 30 + grand(10), CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 19000, enemy[e].y + 15000, 0, 0, 30 + grand(10), CLOUD_COL_1, 0);
      for (i = 0; i < 10; i ++)
      {
       angle = grand(ANGLE_1);
       xa = 2000 + grand(5400);
       quick_cloud(CLOUD_FCIRCLE2, enemy[e].x, enemy[e].y, xpart(angle, xa), ypart(angle, xa), 30 + grand(10), CLOUD_COL_1, 980);
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x, enemy[e].y, xpart(angle, xa - 1000), ypart(angle, xa - 1000), 30 + grand(10), CLOUD_COL_1, 980);
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x, enemy[e].y, xpart(angle, xa - 2000), ypart(angle, xa - 2000), 25 + grand(10), CLOUD_COL_1, 980);
      }
      break;
    case ENEMY_FIGHTER3:
    case ENEMY_FIGHTER5:
    case ENEMY_FIGHTER6:
    play_effectwfvx(WAV_BANG1, 700 + grand(200), 240, enemy[e].x);
      for (i = 0; i < 5; i ++)
      {
       angle = grand(ANGLE_1);
       xa = 20000 + grand(4400);
       quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), xpart(angle, xa / 15), ypart(angle, xa / 15), 40 + grand(26), CLOUD_COL_1, 960);
      }
      quick_cloud(CLOUD_SMALL_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 30, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y, 0, 0, 40 + grand(16), CLOUD_COL_1, 0);
      for (i = 0; i < 10; i ++)
      {
       angle = grand(ANGLE_1);
       xa = 2000 + grand(5400);
       quick_cloud(CLOUD_FCIRCLE2, enemy[e].x, enemy[e].y, xpart(angle, xa), ypart(angle, xa), 30 + grand(10), CLOUD_COL_1, 980);
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x, enemy[e].y, xpart(angle, xa - 1000), ypart(angle, xa - 1000), 30 + grand(10), CLOUD_COL_1, 980);
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x, enemy[e].y, xpart(angle, xa - 2000), ypart(angle, xa - 2000), 25 + grand(10), CLOUD_COL_1, 980);
      }
      break;
    case ENEMY_FIGHTER2:
      for (i = 0; i < 5; i ++)
      {
       angle = grand(ANGLE_1);
       xa = 10000 + grand(4400);
       quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), xpart(angle, xa / 15), ypart(angle, xa / 15), 40 + grand(26), CLOUD_COL_1, 960);
      }
// fall through...
    case ENEMY_FIGHTER:
    case ENEMY_FIGHTER4:
    play_effectwfvx(WAV_BANG1, 900 + grand(200), 240, enemy[e].x);
      quick_cloud(CLOUD_SMALL_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 30, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y, 0, 0, 30 + grand(16), CLOUD_COL_1, 0);
      for (i = 0; i < 10; i ++)
      {
       //angle = grand(ANGLE_1);
       //xa = 20000 + grand(10000);
       //quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), 0, 0, 40 + grand(30), CLOUD_COL_1, 0);

       angle = grand(ANGLE_1);
       xa = 2000 + grand(5400);
       quick_cloud(CLOUD_FCIRCLE2, enemy[e].x, enemy[e].y, xpart(angle, xa), ypart(angle, xa), 30 + grand(10), CLOUD_COL_1, 980);
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x, enemy[e].y, xpart(angle, xa - 1000), ypart(angle, xa - 1000), 30 + grand(10), CLOUD_COL_1, 980);
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x, enemy[e].y, xpart(angle, xa - 2000), ypart(angle, xa - 2000), 25 + grand(10), CLOUD_COL_1, 980);
      }
/*      quick_cloud(CLOUD_SMALL_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 30, CLOUD_COL_3, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y, 0, 0, 30 + grand(16), CLOUD_COL_3, 0);
      for (i = 0; i < 10; i ++)
      {
       //angle = grand(ANGLE_1);
       //xa = 20000 + grand(10000);
       //quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), 0, 0, 40 + grand(30), CLOUD_COL_1, 0);

       angle = grand(ANGLE_1);
       xa = 2000 + grand(5400);
       quick_cloud(CLOUD_FCIRCLE2, enemy[e].x, enemy[e].y, xpart(angle, xa), ypart(angle, xa), 30 + grand(10), CLOUD_COL_3, 980);
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x, enemy[e].y, xpart(angle, xa - 1000), ypart(angle, xa - 1000), 30 + grand(10), CLOUD_COL_3, 980);
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x, enemy[e].y, xpart(angle, xa - 2000), ypart(angle, xa - 2000), 25 + grand(10), CLOUD_COL_3, 980);
      }*/
      break;

    case ENEMY_DOUBLE3:
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 71000, enemy[e].y + 22000, 0, 0, 40 + grand(10), CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 71000, enemy[e].y + 22000, 0, 0, 40 + grand(10), CLOUD_COL_1, 0);
// fall through...
    case ENEMY_DOUBLE2:
      play_effectwfvx(WAV_BANG3, 800 + grand(200), 220, enemy[e].x);
      quick_cloud(CLOUD_HUGE_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y - 8000, 0, 0, 50 + grand(26), CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 25000, enemy[e].y + 25000, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 25000, enemy[e].y + 25000, 0, 0, 40, CLOUD_COL_1, 0);

      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 35000, enemy[e].y - 5000, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 35000, enemy[e].y - 5000, 0, 0, 40, CLOUD_COL_1, 0);

      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 18000, enemy[e].y + 57000, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 18000, enemy[e].y + 57000, 0, 0, 40, CLOUD_COL_1, 0);

      for (i = 0; i < 10; i ++)
      {

       angle = grand(ANGLE_1);
       xa = 15000 + grand(5400);
       quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa * 2), enemy[e].y + ypart(angle, xa), xpart(angle, xa / 8), ypart(angle, xa / 19), 80 + grand(26), CLOUD_COL_1, 960);

       //angle = grand(ANGLE_1);
       //xa = 20000 + grand(10000);
       //quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), 0, 0, 40 + grand(30), CLOUD_COL_1, 0);

       angle = grand(ANGLE_1);
       xa = 4000 + grand(5400);
       quick_cloud(CLOUD_FCIRCLE2, enemy[e].x + xpart(angle, 9000), enemy[e].y + ypart(angle, 4000), xpart(angle, (xa * 2)), ypart(angle, xa), 40 + grand(10), CLOUD_COL_1, 950 + grand(5));
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x + xpart(angle, 9000), enemy[e].y + ypart(angle, 4000), xpart(angle, (xa * 2) - 1000), ypart(angle, xa - 1000), 35 + grand(10), CLOUD_COL_1, 956 + grand(5));
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x + xpart(angle, 9000), enemy[e].y + ypart(angle, 4000), xpart(angle, (xa * 2) - 2000), ypart(angle, xa - 2000), 30 + grand(10), CLOUD_COL_1, 961 + grand(5));
      }

     break;

    case ENEMY_DOME2:
      play_effectwfvx(WAV_BANG4, 1000 + grand(200), 220, enemy[e].x);
      quick_cloud(CLOUD_HUGE_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y - 8000, 0, 0, 50 + grand(26), CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 25000, enemy[e].y, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 25000, enemy[e].y, 0, 0, 40, CLOUD_COL_1, 0);

      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y - 65000, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y + 15000, 0, 0, 40, CLOUD_COL_1, 0);

      for (i = 0; i < 10; i ++)
      {

       angle = grand(ANGLE_1);
       xa = 15000 + grand(5400);
       quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), xpart(angle, xa / 15), ypart(angle, xa / 15), 80 + grand(26), CLOUD_COL_1, 960);

       //angle = grand(ANGLE_1);
       //xa = 20000 + grand(10000);
       //quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), 0, 0, 40 + grand(30), CLOUD_COL_1, 0);

       angle = grand(ANGLE_1);
       xa = 4000 + grand(5400);
       quick_cloud(CLOUD_FCIRCLE2, enemy[e].x + xpart(angle, 5000), enemy[e].y + ypart(angle, 5000), xpart(angle, xa), ypart(angle, xa), 40 + grand(10), CLOUD_COL_1, 980);
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x + xpart(angle, 5000), enemy[e].y + ypart(angle, 5000), xpart(angle, xa - 1000), ypart(angle, xa - 1000), 35 + grand(10), CLOUD_COL_1, 980);
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x + xpart(angle, 5000), enemy[e].y + ypart(angle, 5000), xpart(angle, xa - 2000), ypart(angle, xa - 2000), 30 + grand(10), CLOUD_COL_1, 980);
      }

     break;
    case ENEMY_DOME:
    case ENEMY_BIG:
    play_effectwfvx(WAV_BANG3, 500 + grand(200), 240, enemy[e].x);
      quick_cloud(CLOUD_HUGE_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y - 8000, 0, 0, 50 + grand(26), CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 25000, enemy[e].y + 25000, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 25000, enemy[e].y + 25000, 0, 0, 40, CLOUD_COL_1, 0);

      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 35000, enemy[e].y + 55000, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 35000, enemy[e].y + 55000, 0, 0, 40, CLOUD_COL_1, 0);

      for (i = 0; i < 10; i ++)
      {

       angle = grand(ANGLE_1);
       xa = 15000 + grand(5400);
       quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), xpart(angle, xa / 15), ypart(angle, xa / 15), 80 + grand(26), CLOUD_COL_1, 960);

       //angle = grand(ANGLE_1);
       //xa = 20000 + grand(10000);
       //quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), 0, 0, 40 + grand(30), CLOUD_COL_1, 0);

       angle = grand(ANGLE_1);
       xa = 4000 + grand(5400);
       quick_cloud(CLOUD_FCIRCLE2, enemy[e].x + xpart(angle, 5000), enemy[e].y + ypart(angle, 5000), xpart(angle, xa), ypart(angle, xa), 40 + grand(10), CLOUD_COL_1, 980);
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x + xpart(angle, 5000), enemy[e].y + ypart(angle, 5000), xpart(angle, xa - 1000), ypart(angle, xa - 1000), 35 + grand(10), CLOUD_COL_1, 980);
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x + xpart(angle, 5000), enemy[e].y + ypart(angle, 5000), xpart(angle, xa - 2000), ypart(angle, xa - 2000), 30 + grand(10), CLOUD_COL_1, 980);
      }

     break;
    case ENEMY_BIG3:
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 81000, enemy[e].y - 8000, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 81000, enemy[e].y - 8000, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_SMALL_SHOCK, enemy[e].x - 81000, enemy[e].y - 8000, 0, 0, 30, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_SMALL_SHOCK, enemy[e].x + 81000, enemy[e].y - 8000, 0, 0, 30, CLOUD_COL_1, 0);
    case ENEMY_BIG2:
    play_effectwfvx(WAV_BANG4, 750 + grand(80), 150, enemy[e].x);
      quick_cloud(CLOUD_HUGE_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y - 8000, 0, 0, 60 + grand(26), CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 25000, enemy[e].y + 25000, 0, 0, 40, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 25000, enemy[e].y + 25000, 0, 0, 40, CLOUD_COL_1, 0);

      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y + 55000, 0, 0, 50 + grand(10), CLOUD_COL_1, 0);

      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 65000, enemy[e].y + 70000, 0, 0, 50 + grand(10), CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 65000, enemy[e].y + 70000, 0, 0, 50 + grand(10), CLOUD_COL_1, 0);

      for (i = 0; i < 20; i ++)
      {

       angle = grand(ANGLE_1);
       xa = 35000 + grand(8400);
       quick_cloud(CLOUD_CIRCLE2, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), xpart(angle, xa / 15), ypart(angle, xa / 15), 80 + grand(26), CLOUD_COL_1, 960);

       //angle = grand(ANGLE_1);
       //xa = 20000 + grand(10000);
       //quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), 0, 0, 40 + grand(30), CLOUD_COL_1, 0);

       angle = grand(ANGLE_1);
       xa = 4000 + grand(5400);
       quick_cloud(CLOUD_FCIRCLE2, enemy[e].x + xpart(angle, 45000), enemy[e].y + ypart(angle, 45000), xpart(angle, xa), ypart(angle, xa), 40 + grand(10), CLOUD_COL_1, 970 + grand(5));
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x + xpart(angle, 45000), enemy[e].y + ypart(angle, 45000), xpart(angle, xa - 1000), ypart(angle, xa - 1000), 35 + grand(10), CLOUD_COL_1, 975 + grand(5));
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x + xpart(angle, 45000), enemy[e].y + ypart(angle, 45000), xpart(angle, xa - 2000), ypart(angle, xa - 2000), 30 + grand(10), CLOUD_COL_1, 980 + grand(5));
      }

     break;

    case ENEMY_BOSS1_1:
    case ENEMY_BOSS1_2:
    case ENEMY_BOSS1_3:
    play_effectwfvx(WAV_BANG4, 350 + grand(80), 150, enemy[e].x);
//      quick_cloud(CLOUD_HUGE_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 40, CLOUD_COL_2, 0);
      quick_cloud(CLOUD_HUGE_SHOCK, enemy[e].x - 76000, enemy[e].y - 11000, 0, 0, 40, CLOUD_COL_2, 0);
      quick_cloud(CLOUD_HUGE_SHOCK, enemy[e].x + 76000, enemy[e].y - 11000, 0, 0, 40, CLOUD_COL_2, 0);

      c = quick_cloud(CLOUD_RUMBLE, enemy[e].x, enemy[e].y, 0, 0, 75, CLOUD_COL_2, 0);
      if (c != -1)
      {
       cloud[c].status = 31;
       cloud[c].status2 = 90000;
      }

      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y - 8000, 0, 0, 60 + grand(26), CLOUD_COL_2, 0);

      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 25000, enemy[e].y + 25000, 0, 0, 40, CLOUD_COL_2, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 25000, enemy[e].y + 25000, 0, 0, 40, CLOUD_COL_2, 0);

      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 125000, enemy[e].y - 11000, 0, 0, 40, CLOUD_COL_2, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 125000, enemy[e].y - 11000, 0, 0, 40, CLOUD_COL_2, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 105000, enemy[e].y - 7000, 0, 0, 40, CLOUD_COL_2, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 105000, enemy[e].y - 7000, 0, 0, 40, CLOUD_COL_2, 0);

      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y + 55000, 0, 0, 50 + grand(10), CLOUD_COL_2, 0);

      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 65000, enemy[e].y + 70000, 0, 0, 50 + grand(10), CLOUD_COL_2, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 65000, enemy[e].y + 70000, 0, 0, 50 + grand(10), CLOUD_COL_2, 0);

      for (i = 0; i < 20; i ++)
      {

       angle = grand(ANGLE_1);
       xa = 35000 + grand(8400);
       quick_cloud(CLOUD_CIRCLE2, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), xpart(angle, xa / 7), ypart(angle, xa / 15), 80 + grand(26), CLOUD_COL_2, 960);

       //angle = grand(ANGLE_1);
       //xa = 20000 + grand(10000);
       //quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), 0, 0, 40 + grand(30), CLOUD_COL_1, 0);

       angle = grand(ANGLE_1);
       xa = 4000 + grand(5400);
       quick_cloud(CLOUD_FCIRCLE2, enemy[e].x + xpart(angle, 45000), enemy[e].y + ypart(angle, 45000), xpart(angle, xa * 2), ypart(angle, xa), 40 + grand(10), CLOUD_COL_2, 970 + grand(5));
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x + xpart(angle, 45000), enemy[e].y + ypart(angle, 45000), xpart(angle, xa * 2 - 1000), ypart(angle, xa - 1000), 35 + grand(10), CLOUD_COL_2, 975 + grand(5));
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x + xpart(angle, 45000), enemy[e].y + ypart(angle, 45000), xpart(angle, xa * 2 - 2000), ypart(angle, xa - 2000), 30 + grand(10), CLOUD_COL_2, 980 + grand(5));
      }

     break;


    case ENEMY_BOSS2_1:
    play_effectwfvx(WAV_BANG4, 350 + grand(80), 150, enemy[e].x);
     quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 90000, enemy[e].y + 27000, 0, 0, 40 + grand(15), CLOUD_COL_3, 0);
     quick_cloud(CLOUD_LARGE_SHOCK, enemy[e].x + 90000, enemy[e].y + 27000, 0, 0, 40, CLOUD_COL_3, 0);
     quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 90000, enemy[e].y + 27000, 0, 0, 40 + grand(15), CLOUD_COL_3, 0);
     quick_cloud(CLOUD_LARGE_SHOCK, enemy[e].x - 90000, enemy[e].y + 27000, 0, 0, 40, CLOUD_COL_3, 0);

     quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 50000, enemy[e].y - 13000, 0, 0, 40 + grand(15), CLOUD_COL_3, 0);
     quick_cloud(CLOUD_LARGE_SHOCK, enemy[e].x + 50000, enemy[e].y - 13000, 0, 0, 40, CLOUD_COL_3, 0);
     quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 50000, enemy[e].y - 13000, 0, 0, 40 + grand(15), CLOUD_COL_3, 0);
     quick_cloud(CLOUD_LARGE_SHOCK, enemy[e].x - 50000, enemy[e].y - 13000, 0, 0, 40, CLOUD_COL_3, 0);

     xa = create_enemy(ENEMY_BOSS2_1_DEAD);
     if (xa != -1)
     {
      enemy[xa].x = enemy[e].x;
      enemy[xa].y = enemy[e].y;
      enemy[xa].x_speed = 0;
      enemy[xa].y_speed = 0;
      enemy[xa].status = 0;
      enemy[xa].burst = 0;
      enemy[xa].phase = 50;
      enemy[xa].persist = 1;
     }
     break;

    case ENEMY_BOSS2_2:
    play_effectwfvx(WAV_BANG4, 350 + grand(80), 150, enemy[e].x);
      quick_cloud(CLOUD_HUGE_SHOCK, enemy[e].x, enemy[e].y, 0, 0, 40, CLOUD_COL_3, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x, enemy[e].y - 8000, 0, 0, 60 + grand(26), CLOUD_COL_3, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x - 25000, enemy[e].y + 25000, 0, 0, 40, CLOUD_COL_3, 0);
      quick_cloud(CLOUD_CIRCLE1, enemy[e].x + 25000, enemy[e].y + 25000, 0, 0, 40, CLOUD_COL_3, 0);

      c = quick_cloud(CLOUD_RUMBLE, enemy[e].x, enemy[e].y, 0, 0, 75, CLOUD_COL_3, 0);
      if (c != -1)
      {
       cloud[c].status = 31;
       cloud[c].status2 = 60000;
      }

      for (i = 0; i < 20; i ++)
      {

       angle = grand(ANGLE_1);
       xa = 35000 + grand(8400);
       quick_cloud(CLOUD_CIRCLE2, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), xpart(angle, xa / 15), ypart(angle, xa / 15), 80 + grand(26), CLOUD_COL_3, 960);

       //angle = grand(ANGLE_1);
       //xa = 20000 + grand(10000);
       //quick_cloud(CLOUD_CIRCLE4, enemy[e].x + xpart(angle, xa), enemy[e].y + ypart(angle, xa), 0, 0, 40 + grand(30), CLOUD_COL_1, 0);

       angle = grand(ANGLE_1);
       xa = 4000 + grand(5400);
       quick_cloud(CLOUD_FCIRCLE2, enemy[e].x + xpart(angle, 45000), enemy[e].y + ypart(angle, 45000), xpart(angle, xa), ypart(angle, xa), 40 + grand(10), CLOUD_COL_3, 970 + grand(5));
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x + xpart(angle, 45000), enemy[e].y + ypart(angle, 45000), xpart(angle, xa - 1000), ypart(angle, xa - 1000), 35 + grand(10), CLOUD_COL_3, 975 + grand(5));
       quick_cloud(CLOUD_FCIRCLE4, enemy[e].x + xpart(angle, 45000), enemy[e].y + ypart(angle, 45000), xpart(angle, xa - 2000), ypart(angle, xa - 2000), 30 + grand(10), CLOUD_COL_3, 980 + grand(5));
      }

     break;


 }

// remember part_explode!!

 allocate_xp(e);
 register_destroyed(e);
 destroy_enemy(e);

}
/*
void delay_cloud(int x, int y, int col, int delay, int size)
{

   int c = quick_cloud(CLOUD_DELAY1, x, y, 0, 0, 3000, col, 0);
   if (c != 0)
   {
    cloud[c].angle = delay;
    cloud[c].y2 = size;
   }

}*/

// this is ONLY for parts which explode because the parent has exploded. If the part is destroyed itself,
//  don't call this!
void part_explode(int e)
{

// int x = enemy[e].x;
// int y = enemy[e].y;
 //int angle;
 //int i;

 switch(enemy[e].type)
 {

 }

 parts_must_explode(e);

// allocate_xp(e);
 register_destroyed(e);
 destroy_enemy(e);

}

void parts_must_explode(int e)
{

// int angle;
// int i;

 switch(enemy[e].type)
 {
// just explosion effect - nothing else
 }

}

#define LEVEL_CALC (3500 + (1100 * player.wlevel [i]))
// duplicated in menu.c and level.c


//#define LEVEL_CALC (3000 + (1400 * player.wlevel [i]))

//#define LEVEL_CALC (1000 + (400 * player.wlevel [i]))

void allocate_xp(int e)
{

 if (eclass[enemy[e].type].ai == AI_BOSS)
 {
  gain_score(eclass[enemy[e].type].score);
  return;
 }

// for mini enemies in boss fights - no xp
// if (boss.fight && (enemy[e].type == ENEMY_SMALL || enemy[e].type == ENEMY_SMALL2))
  //return;

 int xp = (eclass[enemy[e].type].score * 1000);// * 12;
 int mhp = eclass[enemy[e].type].max_hp;

 int i;
 for (i = 0; i < NO_PARTS; i ++)
 {
   if (enemy[e].part [i] != -1)
   {
     xp += (eclass[enemy[enemy[e].part [i]].type].score * 1000);
     gain_score(eclass[enemy[enemy[e].part [i]].type].score);
   }
 }
// xp for parts allocated according to damage to main. Any damage to parts is ignored (but becomes relevant
//  if the part separates later).

 float xp_per_dam = (float) xp / (float) mhp;

 gain_score(eclass[enemy[e].type].score);
 arena.stage_score += eclass[enemy[e].type].score;

 for (i = 0; i < 3; i ++)
 {
  if (player.wlevel [i] > 8)
   continue;
  player.wxp [i] += (enemy[e].whurt [i] * xp_per_dam) / 100;
  if (enemy[e].whurt [i] > 0)
   player.wflash [i] = 10;
  if (player.wxp [i] >= LEVEL_CALC)
  {
   player.wxp [i] -= LEVEL_CALC;
   player.wlevel [i] ++;
   player.wgained [i] = 20;
   player.bflash [i] = 25;
//   play_effectwfvx(WAV_LEVEL, tone[NOTE_1G + player.wlevel [i]], 200, player.x);
//   play_effectwfvx(WAV_LEVEL, tone[NOTE_2G], 250, player.x);

  }
  set_wpixels();

 }

 int old_weapon_level = player.weapon_level;
 player.weapon_level = 100;
 for (i = 0; i < 3; i ++)
 {
  if (player.wlevel [i] < player.weapon_level)
   player.weapon_level = player.wlevel [i];
 }
 if (player.weapon_level > old_weapon_level)
 {
  player.overbflash = 25;
  play_effectwfvx(WAV_LEVEL, 700, 240, player.x);

#define XAA grand(1000) - grand(1000)

  switch(player.type)
  {
   case PTYPE_BASIC:

    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, 0, 2000, 15 + grand(5), CLOUD_COL_1, 800);

    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, 0, 12000, 25 + grand(5), CLOUD_COL_1, 900);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, 0, 15000, 18 + grand(5), CLOUD_COL_1, 900);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, 0, 10000, 18 + grand(5), CLOUD_COL_1, 900);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, XAA, 9000, 15 + grand(5), CLOUD_COL_1, 900);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, XAA, 8000, 12 + grand(5), CLOUD_COL_1, 900);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, XAA, 7000, 11 + grand(4), CLOUD_COL_1, 890);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, XAA, 6000, 10 + grand(4), CLOUD_COL_1, 880);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, XAA, 5000, 9 + grand(4), CLOUD_COL_1, 870);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, XAA, 4000, 8 + grand(4), CLOUD_COL_1, 860);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, XAA, 3000, 7 + grand(4), CLOUD_COL_1, 850);
    break;
   case PTYPE_ANENOME:

    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 11000, 0, 2000, 15 + grand(5), CLOUD_COL_1, 800);

    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 11000, 0, 12000, 25 + grand(5), CLOUD_COL_1, 900);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 11000, 0, 15000, 18 + grand(5), CLOUD_COL_1, 900);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 11000, 0, 10000, 18 + grand(5), CLOUD_COL_1, 900);

    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 11000, 6000, 9000, 15 + grand(2), CLOUD_COL_1, 900);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 11000, 3500, 8000, 12 + grand(2), CLOUD_COL_1, 900);

    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 11000, - 6000, 9000, 15 + grand(2), CLOUD_COL_1, 900);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 11000, - 3500, 8000, 12 + grand(2), CLOUD_COL_1, 900);

    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 11000, XAA, 7000, 11 + grand(4), CLOUD_COL_1, 890);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 11000, XAA, 6000, 10 + grand(4), CLOUD_COL_1, 880);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 11000, XAA, 5000, 9 + grand(4), CLOUD_COL_1, 870);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 11000, XAA, 4000, 8 + grand(4), CLOUD_COL_1, 860);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 11000, XAA, 3000, 7 + grand(4), CLOUD_COL_1, 850);
    break;

   case PTYPE_BOMBER:

    quick_cloud(CLOUD_CIRCLE1, player.x - 18000, player.y + 7000, 0, 15000, 20 + grand(5), CLOUD_COL_1, 840);
    quick_cloud(CLOUD_CIRCLE1, player.x - 18000, player.y + 7000, 0, 18000, 13 + grand(5), CLOUD_COL_1, 840);
    quick_cloud(CLOUD_CIRCLE1, player.x - 18000, player.y + 7000, 0, 12000, 13 + grand(5), CLOUD_COL_1, 840);

    quick_cloud(CLOUD_CIRCLE1, player.x - 18000, player.y + 7000, XAA, 9000, 9 + grand(4), CLOUD_COL_1, 840);
    quick_cloud(CLOUD_CIRCLE1, player.x - 18000, player.y + 7000, XAA, 8000, 8 + grand(4), CLOUD_COL_1, 830);
    quick_cloud(CLOUD_CIRCLE1, player.x - 18000, player.y + 7000, XAA, 7000, 7 + grand(4), CLOUD_COL_1, 820);

    quick_cloud(CLOUD_CIRCLE1, player.x + 18000, player.y + 7000, 0, 15000, 20 + grand(5), CLOUD_COL_1, 840);
    quick_cloud(CLOUD_CIRCLE1, player.x + 18000, player.y + 7000, 0, 18000, 13 + grand(5), CLOUD_COL_1, 840);
    quick_cloud(CLOUD_CIRCLE1, player.x + 18000, player.y + 7000, 0, 12000, 13 + grand(5), CLOUD_COL_1, 840);

    quick_cloud(CLOUD_CIRCLE1, player.x + 18000, player.y + 7000, XAA, 9000, 9 + grand(4), CLOUD_COL_1, 840);
    quick_cloud(CLOUD_CIRCLE1, player.x + 18000, player.y + 7000, XAA, 8000, 8 + grand(4), CLOUD_COL_1, 830);
    quick_cloud(CLOUD_CIRCLE1, player.x + 18000, player.y + 7000, XAA, 7000, 7 + grand(4), CLOUD_COL_1, 820);
    break;

   case PTYPE_HAND:
    quick_cloud(CLOUD_CIRCLE1, player.x - 16000, player.y + 7000, -6000 + XAA, 10000, 20 + grand(5), CLOUD_COL_1, 840);
    quick_cloud(CLOUD_CIRCLE1, player.x - 16000, player.y + 7000, -8000 + XAA, 12000, 13 + grand(5), CLOUD_COL_1, 840);
    quick_cloud(CLOUD_CIRCLE1, player.x - 16000, player.y + 7000, -5000 + XAA, 8000, 13 + grand(5), CLOUD_COL_1, 840);

    quick_cloud(CLOUD_CIRCLE1, player.x - 16000, player.y + 7000, -4000 + XAA, 6000, 9 + grand(4), CLOUD_COL_1, 840);
    quick_cloud(CLOUD_CIRCLE1, player.x - 16000, player.y + 7000, -3500 + XAA, 5000, 8 + grand(4), CLOUD_COL_1, 830);
    quick_cloud(CLOUD_CIRCLE1, player.x - 16000, player.y + 7000, -3000 + XAA, 4000, 7 + grand(4), CLOUD_COL_1, 820);

    quick_cloud(CLOUD_CIRCLE1, player.x + 16000, player.y + 7000, 6000 + XAA, 10000, 20 + grand(5), CLOUD_COL_1, 840);
    quick_cloud(CLOUD_CIRCLE1, player.x + 16000, player.y + 7000, 8000 + XAA, 12000, 13 + grand(5), CLOUD_COL_1, 840);
    quick_cloud(CLOUD_CIRCLE1, player.x + 16000, player.y + 7000, 5000 + XAA, 8000, 13 + grand(5), CLOUD_COL_1, 840);

    quick_cloud(CLOUD_CIRCLE1, player.x + 16000, player.y + 7000, 4000 + XAA, 6000, 9 + grand(4), CLOUD_COL_1, 840);
    quick_cloud(CLOUD_CIRCLE1, player.x + 16000, player.y + 7000, 3500 + XAA, 5000, 8 + grand(4), CLOUD_COL_1, 830);
    quick_cloud(CLOUD_CIRCLE1, player.x + 16000, player.y + 7000, 3000 + XAA, 4000, 7 + grand(4), CLOUD_COL_1, 820);
    break;

   case PTYPE_SQUID:

    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, 0, 2000, 15 + grand(5), CLOUD_COL_1, 800);

    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, 0, 12000, 25 + grand(5), CLOUD_COL_1, 900);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, 0, 15000, 18 + grand(5), CLOUD_COL_1, 900);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, 0, 10000, 18 + grand(5), CLOUD_COL_1, 900);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, XAA, 9000, 15 + grand(5), CLOUD_COL_1, 900);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, XAA, 8000, 12 + grand(5), CLOUD_COL_1, 900);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, XAA, 7000, 11 + grand(4), CLOUD_COL_1, 890);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, XAA, 6000, 10 + grand(4), CLOUD_COL_1, 880);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, XAA, 5000, 9 + grand(4), CLOUD_COL_1, 870);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, XAA, 4000, 8 + grand(4), CLOUD_COL_1, 860);
    quick_cloud(CLOUD_CIRCLE1, player.x, player.y + 8000, XAA, 3000, 7 + grand(4), CLOUD_COL_1, 850);


    quick_cloud(CLOUD_CIRCLE1, player.x - 16000, player.y - 5000, -1000, 0, 10 + grand(3), CLOUD_COL_3, 800);

    quick_cloud(CLOUD_CIRCLE1, player.x - 16000, player.y - 5000, -10000, 0, 15 + grand(5), CLOUD_COL_3, 800);
    quick_cloud(CLOUD_CIRCLE1, player.x - 16000, player.y - 5000, -8000, 0, 13 + grand(5), CLOUD_COL_3, 770);
    quick_cloud(CLOUD_CIRCLE1, player.x - 16000, player.y - 5000, -6000, 0, 11 + grand(5), CLOUD_COL_3, 750);
    quick_cloud(CLOUD_CIRCLE1, player.x - 16000, player.y - 5000, -5000, 0, 9 + grand(5), CLOUD_COL_3, 720);
    quick_cloud(CLOUD_CIRCLE1, player.x - 16000, player.y - 5000, -4000, 0, 7 + grand(5), CLOUD_COL_3, 700);

    quick_cloud(CLOUD_CIRCLE1, player.x + 16000, player.y - 5000, 1000, 0, 10 + grand(3), CLOUD_COL_3, 800);

    quick_cloud(CLOUD_CIRCLE1, player.x + 16000, player.y - 5000, 10000, 0, 15 + grand(5), CLOUD_COL_3, 800);
    quick_cloud(CLOUD_CIRCLE1, player.x + 16000, player.y - 5000, 8000, 0, 13 + grand(5), CLOUD_COL_3, 770);
    quick_cloud(CLOUD_CIRCLE1, player.x + 16000, player.y - 5000, 6000, 0, 11 + grand(5), CLOUD_COL_3, 750);
    quick_cloud(CLOUD_CIRCLE1, player.x + 16000, player.y - 5000, 5000, 0, 9 + grand(5), CLOUD_COL_3, 720);
    quick_cloud(CLOUD_CIRCLE1, player.x + 16000, player.y - 5000, 4000, 0, 7 + grand(5), CLOUD_COL_3, 700);


    break;

  }

 }

}

void gain_score(int s)
{
 if ((player.score / 2000) != (player.score + s) / 2000)
 {
  player.lives ++;
//   play_effectwfvx(WAV_LEVEL, tone[NOTE_1G], 250, player.x);
 }


 player.score += s;

}

void set_wpixels(void)
{
 int i;

 for (i = 0; i < 3; i ++)
 {
  player.wpixels2 [i] = 20;//LEVEL_CALC / 150;
  player.wpixels [i] = ((float) (player.wxp [i]) / LEVEL_CALC) * 20;
  if (player.wpixels [i] >= player.wpixels2 [i])
   player.wpixels [i] = player.wpixels2 [i] - 1;
 }

}


void register_destroyed(int e)
{
 int b;
 int i;


/* if (player.weapon [0] == WPN_SEEKER
     || player.weapon [1] == WPN_SEEKER
     || player.weapon [2] == WPN_SEEKER)*/
 if (player.type == PTYPE_BASIC)
 {
  for (b = 0; b < NO_PBULLETS; b ++)
  {
      if (pbullet[b].type == PBULLET_SEEKER1)
      {
       if (pbullet[b].target == e)
        pbullet[b].target = -1;
      }
  }
 }

 if (player.type == PTYPE_HAND)
 {
  for (i = 0; i < 6; i ++)
  {
      if (player.lock [i] == e)
      {
          player.lock [i] = -1;
      }
  }
 }

/*
 if (enemy[e].parent != -1)
 {
  int pe = enemy[e].parent;
  enemy[pe].part [enemy[e].part_index] = -1;
 }
  else
  {
   for (i = 0; i < NO_PARTS; i ++)
   {
     if (enemy[e].part [i] != -1)
     {
       part_explode(enemy[e].part [i]);
     }
   }
  }

 for (i = 0; i < NO_ENEMIES; i ++)
 {
  if (enemy[i].escorting == e)
   enemy[i].escorting = -1;
 }
*/
}

void destroy_enemy(int e)
{

 enemy[e].type = ENEMY_NONE;

}
