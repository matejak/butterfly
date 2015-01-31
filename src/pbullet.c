
#include "config.h"

#include <math.h>

#include "allegro.h"

#include "globvars.h"

#include "stuff.h"
#include "enemy.h"
#include "sound.h"
#include "cloud.h"


#include "palette.h"

void run_pbullets(void);
void destroy_pbullet(int b);
int check_pbullet_collision(int b, int survive);
void pbullet_explode(int b, int hit);
void run_seeker(int b);
void find_new_seeker_target(int b);
void explosion(int x, int y, int r, int d, int b);
void orb_beams(void);
void drag_pbullet(int b, int drag);
void run_orbseeker(int b);


void init_pbullets(void)
{

 int b;

 for (b = 0; b < NO_PBULLETS; b ++)
 {
  pbullet[b].type = PBULLET_NONE;
 }

}

int create_pbullet(int type)
{
  int b;

  for (b = 0; b < NO_PBULLETS; b ++)
  {
      if (pbullet[b].type == PBULLET_NONE)
       break;
      if (b == NO_PBULLETS - 1)
       return -1;

  }

 pbullet[b].type = type;
 pbullet[b].persist = 0;
 pbullet[b].time = 0;
 pbullet[b].timeout = -1;
 return b;

}

void run_pbullets(void)
{

  int b, i, angle, speed;

  for (b = 0; b < NO_PBULLETS; b ++)
  {
    if (pbullet[b].type == PBULLET_NONE)
     continue;
    pbullet[b].time ++;
    switch(pbullet[b].type)
    {
/*        case PBULLET_BLADE:
         check_pbullet_collision(b, 1);
         destroy_pbullet(b);
         continue;
        case PBULLET_SEEKER:
         run_seeker(b);
         check_pbullet_collision(b, 0);
         break;
        case PBULLET_CANNON:
            pbullet[b].y += pbullet[b].y_speed;
            c = create_cloud(CLOUD_CIRCLE5);
            if (c != -1)
            {
             cloud[c].x = pbullet[b].x;
             cloud[c].y = pbullet[b].y;
//             cloud[c].x_speed = xpart((arena.counter * 64) % ANGLE_1, 700 + pbullet[b].level * 70);
             cloud[c].x_speed = xpart(arena.counter * 64, 700 + pbullet[b].level * 70);
             cloud[c].y_speed = -900 - grand(500);
             cloud[c].colour = TCOL_ORANGE;
             cloud[c].timeout = 30 + grand(20) + pbullet[b].level;
            }
            check_pbullet_collision(b, 0);
            break;*/
        default:
            pbullet[b].x += pbullet[b].x_speed;
            pbullet[b].y += pbullet[b].y_speed;
            check_pbullet_collision(b, 0);
         break;
        case PBULLET_CHARGE1:
            pbullet[b].x += pbullet[b].x_speed;
            pbullet[b].y += pbullet[b].y_speed;
            check_pbullet_collision(b, 2);
//            if (arena.counter & 1)
             quick_cloud(CLOUD_3FCIRCLE2, pbullet[b].x, pbullet[b].y, pbullet[b].x_speed + grand(1000) - grand(1000), -4000, 1 + grand(10) + pbullet[b].dam / 300, 0, 950);
//            quick_cloud(CLOUD_3FCIRCLE2, pbullet[b].x, pbullet[b].y, grand(500) - grand(500), 0, 1 + grand(20) + pbullet[b].dam / 1000, 0, 960);
         break;
        case PBULLET_SEEKER1:
         run_seeker(b);
         check_pbullet_collision(b, 0);
         break;
        case PBULLET_ORBSEEKER:
         run_orbseeker(b);
         check_pbullet_collision(b, 0);
         break;
        case PBULLET_ASHOT:
            pbullet[b].x += pbullet[b].x_speed;
            pbullet[b].y += pbullet[b].y_speed;
             quick_cloud(CLOUD_3FCIRCLE2, pbullet[b].x, pbullet[b].y, pbullet[b].x_speed + grand(500) - grand(500), -3000, 3 + grand(7) + grand(pbullet[b].level * 2), 0, 950);
         check_pbullet_collision(b, 0);
             break;
        case PBULLET_BLADE:
         check_pbullet_collision(b, 1);
         break;
        case PBULLET_HAND:
        case PBULLET_ORB:
        case PBULLET_ORBFIELD:
         check_pbullet_collision(b, 4);
         break;
/*        case PBULLET_BLADE:
         check_pbullet_collision(b, 1);
         break;*/
        case PBULLET_BOMB:
            pbullet[b].x += pbullet[b].x_speed;
            pbullet[b].y += pbullet[b].y_speed;
            angle = grand(ANGLE_1);
            speed = 2000 + grand(2000);
            quick_cloud(CLOUD_3FCIRCLE1, pbullet[b].x, pbullet[b].y, xpart(angle, speed), ypart(angle, speed), 10 + grand(5), 0, 910 + grand(40));
            drag_pbullet(b, 960);
            if (pbullet[b].timeout == 2)
            {
             play_effectwfvx(WAV_BOMB, 800 - pbullet[b].level * 10, 150, pbullet[b].x);
             quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, 0, 0, 40 + grand(10) + pbullet[b].level, 0, 950);
             quick_cloud(CLOUD_HUGE_SHOCK_FAST, pbullet[b].x, pbullet[b].y, 0, 0, 50, CLOUD_COL_1, 0);
             for (i = 0; i < 9; i ++)
             {
              angle = grand(ANGLE_1);
              speed = 2000 + grand(9000) + pbullet[b].level * 200;
              quick_cloud(CLOUD_CIRCLE2, pbullet[b].x + xpart(angle, 30000), pbullet[b].y + ypart(angle, 30000), xpart(angle, speed), ypart(angle, speed), 20 + grand(40) + pbullet[b].level, 0, 910 + grand(40));
              speed -= 1000 + grand(1000);
              quick_cloud(CLOUD_CIRCLE2, pbullet[b].x + xpart(angle, 30000), pbullet[b].y + ypart(angle, 30000), xpart(angle, speed), ypart(angle, speed), 20 + grand(40) + pbullet[b].level, 0, 910 + grand(40));
              angle += ANGLE_2;
              speed = 2000 + grand(9000) + pbullet[b].level * 200;
              quick_cloud(CLOUD_CIRCLE2, pbullet[b].x + xpart(angle, 30000), pbullet[b].y + ypart(angle, 30000), xpart(angle, speed), ypart(angle, speed), 20 + grand(40) + pbullet[b].level, 0, 910 + grand(40));
              speed -= 1000 + grand(1000);
              quick_cloud(CLOUD_CIRCLE2, pbullet[b].x + xpart(angle, 30000), pbullet[b].y + ypart(angle, 30000), xpart(angle, speed), ypart(angle, speed), 20 + grand(40) + pbullet[b].level, 0, 910 + grand(40));
             }
             explosion(pbullet[b].x, pbullet[b].y, 80000, 4500 + pbullet[b].level * 450, b);
             destroy_pbullet(b);
             continue;
            }
         break;
    }

    if (pbullet[b].type == PBULLET_NONE)
     continue;

    if (pbullet[b].timeout > -1)
    {
     pbullet[b].timeout --;
     if (pbullet[b].timeout <= 0)
     {
      pbullet_explode(b, 0);
      destroy_pbullet(b);
      continue;
     }
    }
    if (pbullet[b].persist == 0)
    {
     if (pbullet[b].x <= X_MIN - 30000
      || pbullet[b].x >= X_MAX + 30000
      || pbullet[b].y <= -20000
      || pbullet[b].y >= Y_MAX + 30000)
      {
       destroy_pbullet(b);
       continue;
      }
    }
    }

}

void drag_pbullet(int b, int drag)
{
 pbullet[b].x_speed *= drag;
 pbullet[b].x_speed >>= 10;
 pbullet[b].y_speed *= drag;
 pbullet[b].y_speed >>= 10;

}



void run_seeker(int b)
{

    int old_x = pbullet[b].x;
    int old_y = pbullet[b].y;
    int target_x = -1, target_y = -1;

    if (pbullet[b].target == -1 && pbullet[b].timeout % 5 == 0)
     find_new_seeker_target(b);


    if (pbullet[b].target != -1)
    {
     target_x = enemy[pbullet[b].target].x;
     target_y = enemy[pbullet[b].target].y;
    }
     else
     {
      target_x = player.x;
      target_y = player.y;
     }


//    pbullet[b].acceleration += 80;
/*    pbullet[b].acceleration += 300;
    if (pbullet[b].acceleration > 1900)
     pbullet[b].acceleration = 1900;*/

    pbullet[b].x_speed += xpart(pbullet[b].angle, 1000);
    pbullet[b].y_speed += ypart(pbullet[b].angle, 1000);
    pbullet[b].x += pbullet[b].x_speed;
    pbullet[b].y += pbullet[b].y_speed;

/*    int c = create_cloud(CLOUD_CIRCLE1);
    if (c != -1)
    {
     cloud[c].x2 = pbullet[b].x;
     cloud[c].y2 = pbullet[b].y;
     cloud[c].x = pbullet[b].x - pbullet[b].x_speed;
     cloud[c].y = pbullet[b].y - pbullet[b].y_speed;
     cloud[c].timeout = 5 + pbullet[b].level / 3;
//     cloud[c].colour = TRANS_ORANGE_OUT;
    }*/

/*    pbullet[b].drag -= 50;
    if (pbullet[b].drag < 900)
     pbullet[b].drag = 900;*/

    pbullet[b].x_speed *= 93;
    pbullet[b].x_speed /= 100;
    pbullet[b].y_speed *= 93;
    pbullet[b].y_speed /= 100;

    int c = create_cloud(CLOUD_SEEKER_TRAIL); //, 0, pbullet[b].x, pbullet[b].y, 0, 0, 11);
    if (c != -1)
    {
     cloud[c].x = pbullet[b].x;
     cloud[c].y = pbullet[b].y;
     cloud[c].x2 = old_x;
     cloud[c].y2 = old_y;
     cloud[c].angle = pbullet[b].angle;
     cloud[c].timeout = 8 + pbullet[b].level;
    }

//    pbullet[b].turn += 185;
//    if (pbullet[b].turn > 1300)
//     pbullet[b].turn = 1300;
/* if (pbullet[b].timeout < 130)
 {
    if (target_x != -1 || target_y != -1)
     pbullet[b].angle = radians_to_angle(atan2((target_x - pbullet[b].y), (target_y - pbullet[b].x))) % ANGLE_1;

 }
  else*/
  {
    if (target_x != -1 || target_y != -1)
     pbullet[b].angle = turn_towards_xy(pbullet[b].x, pbullet[b].y, target_x, target_y, pbullet[b].angle, 28); // 22
  }
//     seeker[s].angle = turn_towards_xy(seeker[s].x, seeker[s].y, target_x, target_y, seeker[s].angle, 32);

    pbullet[b].timeout --;

//    if (seeker_collision(w, s))
//     return;

    if (pbullet[b].timeout <= 0)
    {
//     create_cloud(w, CLOUD_DRAG_EXPLODE, 0, pbullet[b].x, pbullet[b].y, 0, 0, 30 + grand(5)); // also in collision
     pbullet_explode(b, 0);
     destroy_pbullet(b);
    }

}




void find_new_seeker_target(int b)
{

 int enemies_found = 0;


 int e;
 int x = pbullet[b].x;
 int y = pbullet[b].y;

 for (e = 0; e < NO_ENEMIES; e ++)
 {
  if (enemy[e].type == ENEMY_NONE)
  {
   enemy[e].distance = 900000;
   continue;
  }

  enemy[e].distance = hypot(enemy[e].y - y, enemy[e].x - x) / 100;
  enemies_found ++;

 }

 if (enemies_found == 0)
  return;

 int closest = -1;
 int smallest_distance = 900000;

  for (e = 0; e < NO_ENEMIES; e ++)
  {
   if (enemy[e].distance < smallest_distance)
   {
/*    for (j = 0; j < NO_SEEKERS; j ++)
    {
     if (seeker[w][j].target == e) // should allow multiple seekers to lock on to big targets
      continue;
    }*/
    closest = e;
    smallest_distance = enemy[e].distance;
   }
  }

  if (smallest_distance == 900000 || closest == -1)
  {
   return;
  }

  pbullet[b].target = closest;

}



void run_orbseeker(int b)
{

    int old_x = pbullet[b].x;
    int old_y = pbullet[b].y;



    if (pbullet[b].timeout > 200)
    {

     pbullet[b].angle = turn_towards_xy(pbullet[b].x, pbullet[b].y, player.orb_x, player.orb_y, pbullet[b].angle, pbullet[b].timeout - 180); // 22

     pbullet[b].x_speed += xpart(pbullet[b].angle, 2000);
     pbullet[b].y_speed += ypart(pbullet[b].angle, 2000);

     drag_pbullet(b, 900);

    }

     pbullet[b].x += pbullet[b].x_speed;
     pbullet[b].y += pbullet[b].y_speed;

    int c = create_cloud(CLOUD_ORBSEEKER_TRAIL);
    if (c != -1)
    {
     cloud[c].x = pbullet[b].x;
     cloud[c].y = pbullet[b].y;
     cloud[c].x2 = old_x;
     cloud[c].y2 = old_y;
     cloud[c].angle = pbullet[b].angle;
     cloud[c].timeout = 8 + pbullet[b].level;
    }

    pbullet[b].timeout --;

    if (pbullet[b].timeout <= 0)
    {
     pbullet_explode(b, 0);
     destroy_pbullet(b);
    }


}



void orb_beams(void)
{


 int e;

 int x, y;
 int min_y = 0, hitting = -1;
 int i;
 int j;


 for (i = 0; i < 3; i ++)
 {

  x = player.x + xpart(player.ring_angle + (ANGLE_3 * i), player.ring_size << 10);
  y = player.y + ypart(player.ring_angle + (ANGLE_3 * i), player.ring_size << 10);
  hitting = -1;
  if (player.beam_y [i] == -100000)
   player.beam_y [i] = y;
  if (player.beam_y [i] > y)
   player.beam_y [i] = y;
  min_y = player.beam_y [i];

  for (e = 0; e < NO_ENEMIES; e ++)
  {
   if (enemy[e].type == ENEMY_NONE)
    continue;
   if (x >= enemy[e].x - eclass[enemy[e].type].xsize + eclass[enemy[e].type].xoffset
       && x <= enemy[e].x + eclass[enemy[e].type].xsize + eclass[enemy[e].type].xoffset
       && y >= enemy[e].y - eclass[enemy[e].type].ysize + eclass[enemy[e].type].yoffset
       && min_y <= enemy[e].y + eclass[enemy[e].type].ysize + eclass[enemy[e].type].yoffset)
       {
//        max_y = enemy[e].y + eclass[enemy[e].type].ysize + eclass[enemy[e].type].yoffset;
//        if (max_y > y)
//         max_y = y;
//         max_y -= grand(16000);
//         max_y -= grand(8000);

        if (eclass[enemy[e].type].cbox [0] [0] == -1)
        {
           hitting = e;
           min_y = enemy[e].y + eclass[enemy[e].type].ysize + eclass[enemy[e].type].yoffset;
           if (min_y > y)
            min_y = y;
           continue;
        }
         else
          {
//           hit = 0;
           for (j = 0; j < 7; j ++)
           {
            if (eclass[enemy[e].type].cbox [j] [0] == -1)
            {
                break;
            }
            if (x >= enemy[e].x + ((eclass [enemy[e].type].cbox [j] [0]) << 10)
             && y >= enemy[e].y + ((eclass [enemy[e].type].cbox [j] [1]) << 10)
             && x <= enemy[e].x + ((eclass [enemy[e].type].cbox [j] [2]) << 10)
             && min_y < enemy[e].y + ((eclass [enemy[e].type].cbox [j] [3]) << 10))
            {
//             hit = 1;
               hitting = e;
               min_y = enemy[e].y + ((eclass [enemy[e].type].cbox [j] [3]) << 10);
               if (min_y > y)
                min_y = y;
               continue;
            }
           }
          }



       }
  }
           player.beam_y [i] = min_y;
           if (hitting != -1)
           {
            quick_cloud(CLOUD_3FCIRCLE2, x, min_y, grand(300) - grand(300), -500 - grand(500), 10 + grand(10) + (player.weapon_level), CLOUD_COL_1, 970);
            hurt_enemy(hitting, 45 + player.weapon_level * 5, 0);
           }
           player.wstrength [i] = hitting;


         // remember, enemy might be destroyed here!!
         // so we can't assume its values are still good (actually we prob can, but let's not)

 }



}

/*
void swbeam_collision(int m)
{


 int e;

 int x, y, dam;
 int max_y, angle, speed;
 int i;

 dam = 1400 + player.swbeam_level [m] * 240;


  x = player.wx [m];

  y = player.wy [m] - 3000;

  max_y = y - 90000;

  if (player.swbeam_level [m] > 2)
  {
   while (max_y > -90000)
   {
    quick_cloud(CLOUD_CIRCLE5, x, max_y, grand(500) - grand(500), 3000 + grand(1000), 20 + grand(8) + player.swbeam_level [m] * 2, TCOL_ORANGE, 970);
    max_y -= 20000 + grand(15000) - player.swbeam_level [m] * 1000;

   };
  }

  for (e = 0; e < NO_ENEMIES; e ++)
  {
   if (enemy[e].type == ENEMY_NONE)
    continue;
   if (x >= enemy[e].x - eclass[enemy[e].type].xsize + eclass[enemy[e].type].xoffset - 5000 - player.swbeam_level [m] * 1500
       && x <= enemy[e].x + eclass[enemy[e].type].xsize + eclass[enemy[e].type].xoffset + 5000 + player.swbeam_level [m] * 1500
       && y >= enemy[e].y - eclass[enemy[e].type].ysize + eclass[enemy[e].type].yoffset)
       {
        max_y = enemy[e].y + eclass[enemy[e].type].ysize + eclass[enemy[e].type].yoffset;
        if (max_y > y)
         max_y = y;
         max_y -= grand(16000);
         angle = grand(ANGLE_8);
         for (i = 0; i < 8 + player.swbeam_level [m]; i ++)
         {
           angle = (i * ANGLE_8) + grand(ANGLE_8);
           speed = grand(3000) + 1000 + player.swbeam_level [m] * 200;
           quick_cloud(CLOUD_CIRCLE5, x, max_y, xpart(angle, speed), ypart(angle, speed) - grand(2000), 45 + grand(28) + player.swbeam_level [m] * 2, TCOL_ORANGE, 940);
         }
         quick_cloud(CLOUD_CIRCLE2, x, max_y, 0, 0 - grand(1000), 25 + grand(8) + player.swbeam_level [m], TCOL_YELLOW, 950);
         max_y -= grand(8000);
         quick_cloud(CLOUD_CIRCLE2, x, max_y, 0, 0 - grand(1000), 45 + grand(15) + player.swbeam_level [m] * 2, TCOL_ORANGE, 950);
         if (hurt_enemy(e, dam, m) == 1)
          quick_cloud(CLOUD_MSHOCK, x, max_y, 0, 0, 20, 1, 0);
         // remember, enemy might be destroyed here!!
         // so we can't assume its values are still good (actually we prob can, but let's not)
       }
  }




}
*/


int check_pbullet_collision(int b, int survive)
{

{
 int e, i, j, etype, hit, enemy_survived = 0;
// int destroy_type = 0;

 for (e = 0; e < NO_ENEMIES; e ++)
 {
  if (enemy[e].type == ENEMY_NONE)
   continue;
   etype = enemy[e].type;
  if (pbullet[b].x >= enemy[e].x - eclass[etype].xsize + eclass[etype].xoffset - pbullet[b].xsize
      && pbullet[b].x <= enemy[e].x + eclass[etype].xsize + eclass[etype].xoffset + pbullet[b].xsize
      && pbullet[b].y >= enemy[e].y - eclass[etype].ysize + eclass[etype].yoffset - pbullet[b].ysize
      && pbullet[b].y <= enemy[e].y + eclass[etype].ysize + eclass[etype].yoffset + pbullet[b].ysize)
      {
       if (eclass[etype].cbox [0] [0] != -1)
       {
        hit = 0;
        for (i = 0; i < 7; i ++)
        {
         if (eclass[etype].cbox [i] [0] == -1)
         {
             break;
         }
         if (pbullet[b].x >= enemy[e].x + ((eclass [etype].cbox [i] [0]) << 10) - pbullet[b].xsize
          && pbullet[b].y >= enemy[e].y + ((eclass [etype].cbox [i] [1]) << 10) - pbullet[b].ysize
          && pbullet[b].x <= enemy[e].x + ((eclass [etype].cbox [i] [2]) << 10) + pbullet[b].xsize
          && pbullet[b].y <= enemy[e].y + ((eclass [etype].cbox [i] [3]) << 10) + pbullet[b].ysize)
         {
          hit = 1;
          break;
         }
        }

         if (hit == 0)
          continue;
       }

        enemy_survived = pbullet_hits_enemy(b, e);
//        if (survive == 2)
//         return 1; // green2_circle
        switch(survive)
        {
         case 0: // doesn't survive hitting enemy
          pbullet_explode(b, 1);
          destroy_pbullet(b);
          return 1; // bullet destroyed
         case 1: // does survive
          pbullet_explode(b, 1);
          break;
         case 2: // survives if enemy destroyed; otherwise destroyed. Doesn't explode
          if (enemy_survived == 1)
          {
           pbullet_explode(b, 1);
           destroy_pbullet(b);
           return 1;
          }
          break;
         case 3: // survives if enemy destroyed; otherwise destroyed. Explodes
          if (enemy_survived == 1)
          {
           pbullet_explode(b, 1);
           destroy_pbullet(b);
           return 1;
          }
          pbullet_explode(b, 1);
          break;
         case 4: // hand of xom
          if (enemy_survived == 1)
          {
            for (j = 0; j < 6; j ++)
            {
             if (player.lock [j] == e)
             {
              player.lock [j] = -1;
//              if (e != -1 && enemy[e].type != ENEMY_NONE)
              quick_cloud(CLOUD_SHRINKING_LOCK, enemy[e].x, enemy[e].y, 0, 0, player.lock_size [j], 0, 0);
              break;
             }
            }
          }
          pbullet_explode(b, 1);
          break;
        }
      }

 }
 return 0; // bullet not destroyed
}


}

void explosion(int x, int y, int r, int d, int b)
{
 int e;
// int destroy_type = 0;

 for (e = 0; e < NO_ENEMIES; e ++)
 {
  if (enemy[e].type == ENEMY_NONE)
   continue;
  if (x >= enemy[e].x - eclass[enemy[e].type].xsize + eclass[enemy[e].type].xoffset - r
      && x <= enemy[e].x + eclass[enemy[e].type].xsize + eclass[enemy[e].type].xoffset + r
      && y >= enemy[e].y - eclass[enemy[e].type].ysize + eclass[enemy[e].type].yoffset - r
      && y <= enemy[e].y + eclass[enemy[e].type].ysize + eclass[enemy[e].type].yoffset + r)
      {
        hurt_enemy(e, d, pbullet[b].source);
      }
 }

}


// this function does not destroy the bullet!
void pbullet_explode(int b, int hit)
{
 int xa, ya, xc, i, angle, speed;

 switch(pbullet[b].type)
 {
  case PBULLET_SEEKER1:
   quick_cloud(CLOUD_CIRCLE2, pbullet[b].x, pbullet[b].y, 0, 0, 30 + grand(15), CLOUD_COL_1, 990);
   if (hit == 0)
    break;
   xa = pbullet[b].x_speed / -3;
   ya = pbullet[b].y_speed / -3;
   quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, xa + grand(2000) - grand(2000), ya + grand(2000) - grand(2000), 20 + grand(10), CLOUD_COL_1, 980);
   quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, xa + grand(2000) - grand(2000), ya + grand(2000) - grand(2000), 15 + grand(7), CLOUD_COL_1, 950);
   quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, xa + grand(2000) - grand(2000), ya + grand(2000) - grand(2000), 10 + grand(5), CLOUD_COL_1, 920);
   break;
  case PBULLET_SHOT:
//   quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, 0, -500, 20 + grand(10) + pbullet[b].level, CLOUD_COL_1, 990);
   quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, 0, -500, 15 + grand(10) + pbullet[b].level, CLOUD_COL_1, 990);
   for (i = 0; i < 7 + pbullet[b].level; i ++)
   {
    speed = 2000 + grand(1500);
    angle = grand(ANGLE_1);
    quick_cloud(CLOUD_CIRCLE2, pbullet[b].x + xpart(angle, 2000), pbullet[b].y + ypart(angle, 2000), xpart(angle, speed), ypart(angle, speed) - 500, 10 + grand(5) + pbullet[b].level / 3, CLOUD_COL_1, 900);
   }
   break;
  case PBULLET_ORBSEEKER:
   quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, 0, 0, 15 + grand(10) + pbullet[b].level, CLOUD_COL_3, 990);
   for (i = 0; i < 7 + pbullet[b].level; i ++)
   {
    speed = 3000 + grand(2500);
    angle = grand(ANGLE_1);
    quick_cloud(CLOUD_CIRCLE2, pbullet[b].x + xpart(angle, 2000), pbullet[b].y + ypart(angle, 2000), xpart(angle, speed), ypart(angle, speed), 20 + grand(5) + pbullet[b].level / 2, CLOUD_COL_3, 930);
   }
   break;
  case PBULLET_RINGSHOT:
  case PBULLET_HSHOT:
   quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, 0, 0, 15 + grand(7), CLOUD_COL_1, 990);
   break;
  case PBULLET_ORBSHOT:
   quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, 0, 0, 15 + grand(7), CLOUD_COL_3, 990);
   break;
  case PBULLET_BLADE:
   if (hit == 1)
   {
    if (player.blade_redness > 15)
     xa = CLOUD_COL_1;
      else
       xa = CLOUD_COL_3;
     quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, 0, 0, 10 + grand(5), xa, 990);
//     for (i = 0; i < 3 + pbullet[b].level / 2; i ++)
//     {
      speed = 2000 + grand(1500);
      angle = grand(ANGLE_1);
      quick_cloud(CLOUD_CIRCLE1, pbullet[b].x + xpart(angle, 2000), pbullet[b].y + ypart(angle, 2000), xpart(angle, speed), ypart(angle, speed), 10 + grand(5), xa, 900);
//     }
   }
   break;
  case PBULLET_HAND:
   if (hit == 1 && (arena.counter & 1))
   {
     angle = grand(ANGLE_1);
     speed = 2000 + grand(3000);
     quick_cloud(CLOUD_CIRCLE2, pbullet[b].x, pbullet[b].y, xpart(angle, speed), ypart(angle, speed), 30 + grand(10), CLOUD_COL_1, 970);
   }
   if (hit == 1)
   {
    player.hand_x_speed *= 950;
    player.hand_x_speed >>= 10;
    player.hand_y_speed *= 950;
    player.hand_y_speed >>= 10;
    player.hand_angle += (player.hand_x_speed >> 7) + (player.hand_y_speed >> 7);
    player.hand_angle &= 1023;
   }
   break;
  case PBULLET_ORB:
   if (hit == 1 && (arena.counter & 1))
   {
     angle = grand(ANGLE_1);
     speed = 1500 + grand(2000);
     quick_cloud(CLOUD_CIRCLE4, pbullet[b].x, pbullet[b].y, xpart(angle, speed), ypart(angle, speed), 30 + grand(10), CLOUD_COL_3, 900);
   }
   break;
  case PBULLET_ORBFIELD:
   if (pbullet[b].dam > 0)
   {
    if (hit == 1)// && (arena.counter & 1))
     quick_cloud(CLOUD_CIRCLE2, pbullet[b].x, pbullet[b].y, 0, 0, 5 + grand(5), pbullet[b].target, 0);
   }
   break;
  case PBULLET_ASHOT:
   quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, 0, -300 + grand(1000), 30 + grand(10), CLOUD_COL_1, 980);
   quick_cloud(CLOUD_CIRCLE2, pbullet[b].x, pbullet[b].y, -2000, -300 + grand(1000), 40 + grand(10), CLOUD_COL_1, 920);
   quick_cloud(CLOUD_CIRCLE2, pbullet[b].x, pbullet[b].y, -4000, -300 + grand(1000), 30 + grand(10), CLOUD_COL_1, 920);
   quick_cloud(CLOUD_CIRCLE2, pbullet[b].x, pbullet[b].y, 2000, -300 + grand(1000), 40 + grand(10), CLOUD_COL_1, 920);
   quick_cloud(CLOUD_CIRCLE2, pbullet[b].x, pbullet[b].y, 4000, -300 + grand(1000), 30 + grand(10), CLOUD_COL_1, 920);
   break;
  case PBULLET_CHARGE1:
//   quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, 0, -500, 20 + grand(10), CLOUD_COL_1, 990);
   quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, 0, 0, 10 + grand(15) + pbullet[b].dam / 200, 0, 0);
   xc = 3 + (pbullet[b].dam / 2000);
   for (i = 0; i < xc; i ++)
   {
    quick_cloud(CLOUD_CIRCLE2, pbullet[b].x, pbullet[b].y, (pbullet[b].dam / -12) - grand(3000), grand(800) - grand(800), 10 + grand(15) + pbullet[b].dam / 200, 0, 970);
    quick_cloud(CLOUD_CIRCLE2, pbullet[b].x, pbullet[b].y, (pbullet[b].dam / 12) + grand(3000), grand(800) - grand(800), 10 + grand(15) + pbullet[b].dam / 200, 0, 970);
//    quick_cloud(CLOUD_CIRCLE2, pbullet[b].x, pbullet[b].y, (pbullet[b].dam / -8) - grand(2000), grand(500) - grand(500), 10 + grand(15) + pbullet[b].dam / 200, 0, 970);
//    quick_cloud(CLOUD_CIRCLE2, pbullet[b].x, pbullet[b].y, (pbullet[b].dam / 8) + grand(2000), grand(500) - grand(500), 10 + grand(15) + pbullet[b].dam / 200, 0, 970);
   }
//   if (pbullet[b].dam >= 6000)
//      quick_cloud(CLOUD_SMALL_SHOCK, pbullet[b].x, pbullet[b].y, 0, 0, 30, CLOUD_COL_1, 0);
   break;
/*  case PBULLET_BLADE:
   quick_cloud(CLOUD_CIRCLE2, pbullet[b].x + grand(5000) - grand(5000), pbullet[b].y + grand(5000) - grand(5000), 0, 0, 12, TCOL_YELLOW, 0);
   quick_cloud(CLOUD_CIRCLE1, pbullet[b].x + grand(5000) - grand(5000), pbullet[b].y + grand(5000) - grand(5000), 0, 0, 12, TCOL_ORANGE, 0);
   angle = grand(ANGLE_1);
   quick_cloud(CLOUD_CIRCLE2, pbullet[b].x + xpart(angle, 9000), pbullet[b].y + ypart(angle, 9000), xpart(angle, 3000), ypart(angle, 3000), 20, TCOL_ORANGE, 970);
   quick_cloud(CLOUD_CIRCLE2, pbullet[b].x - xpart(angle, 9000), pbullet[b].y - ypart(angle, 9000), xpart(angle, -3000), ypart(angle, -3000), 20, TCOL_ORANGE, 970);
   angle = grand(ANGLE_1);
   quick_cloud(CLOUD_CIRCLE2, pbullet[b].x + xpart(angle, 9000), pbullet[b].y + ypart(angle, 9000), xpart(angle, 3000), ypart(angle, 3000), 20, TCOL_ORANGE, 970);
   quick_cloud(CLOUD_CIRCLE2, pbullet[b].x - xpart(angle, 9000), pbullet[b].y - ypart(angle, 9000), xpart(angle, -3000), ypart(angle, -3000), 20, TCOL_ORANGE, 970);
   break;
  case PBULLET_SCATTER:
   xa = pbullet[b].level;
   ya = 9000 + pbullet[b].level * 500;
   quick_cloud(CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, 0, 0, 30 + xa, TCOL_YELLOW, 0);
   quick_cloud(CLOUD_CIRCLE2, pbullet[b].x, pbullet[b].y, 0, 0, 30 + xa, TCOL_ORANGE, 0);
   angle = grand(ANGLE_1);
   quick_cloud(CLOUD_CIRCLE2, pbullet[b].x + xpart(angle, ya), pbullet[b].y + ypart(angle, ya), 0, 0, 20 + xa, TCOL_ORANGE, 0);
   quick_cloud(CLOUD_CIRCLE2, pbullet[b].x - xpart(angle, ya), pbullet[b].y - ypart(angle, ya), 0, 0, 20 + xa, TCOL_ORANGE, 0);
   break;
  case PBULLET_RECT:
   ya = pbullet[b].y - grand(14000);
   quick_cloud(CLOUD_RSHOCK1 + pbullet[b].level / 2, pbullet[b].x, ya, 0, 0, 20, 0, 0);
   quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, ya, 0, 0, 20 + pbullet[b].level, TCOL_YELLOW, 0);
   quick_cloud(CLOUD_CIRCLE2, pbullet[b].x, ya, 0, 0, 50 + pbullet[b].level * 2, TCOL_ORANGE, 0);

   angle = grand(ANGLE_1);
   pieces = 3 + pbullet[b].level;
   pieces2 = ANGLE_1 / pieces;
   xc = 40 + grand(5);
   for (i = 0; i < pieces; i ++)
   {
    xb = pbullet[b].x + xpart(angle, 25000);
    yb = ya + ypart(angle, 25000);
    quick_cloud(CLOUD_CIRCLE1, xb, yb, xpart(angle, 1000 + pbullet[b].level * 40), ypart(angle, 1000 + pbullet[b].level * 40), 20 + pbullet[b].level, TCOL_YELLOW, 0);
    quick_cloud(CLOUD_CIRCLE2, xb, yb, xpart(angle, 1000 + pbullet[b].level * 40), ypart(angle, 1000 + pbullet[b].level * 40), 50 + pbullet[b].level * 2, TCOL_ORANGE, 0);
    angle += pieces2;
   }
   explosion(pbullet[b].x, pbullet[b].y, 60000 + pbullet[b].level * 5000, 1400 + pbullet[b].level * 255, b);
   play_effectwfvx(WAV_RECT, 250, 120, pbullet[b].x);
   break;
  case PBULLET_SEEKER:
//   quick_cloud(CLOUD_MGSHOCK, pbullet[b].x, pbullet[b].y, 0, 0, 11, 0, 0);
   quick_cloud(CLOUD_CIRCLE5, pbullet[b].x, pbullet[b].y, 0, 0, 30, TCOL_YELLOW, 0);
   quick_cloud(CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, 0, 0, 30, TCOL_ORANGE, 0);
   if (hit == 1)
   {
    xa = xpart(pbullet[b].angle - ANGLE_16 + ANGLE_2, 500);
    ya = ypart(pbullet[b].angle - ANGLE_16 + ANGLE_2, 500);
    quick_cloud(CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, xa, ya, 30, TCOL_ORANGE, 980);
    quick_cloud(CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, xa * 2, ya * 2, 35, TCOL_ORANGE, 980);
    quick_cloud(CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, xa * 3, ya * 3, 40, TCOL_ORANGE, 980);
    xa = xpart(pbullet[b].angle + ANGLE_16 + ANGLE_2, 500);
    ya = ypart(pbullet[b].angle + ANGLE_16 + ANGLE_2, 500);
    quick_cloud(CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, xa, ya, 30, TCOL_ORANGE, 980);
    quick_cloud(CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, xa * 2, ya * 2, 35, TCOL_ORANGE, 980);
    quick_cloud(CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, xa * 3, ya * 3, 40, TCOL_ORANGE, 980);
   }
    else
    {
     angle = pbullet[b].angle;
     for (i = 0; i < 3; i ++)
     {
      quick_cloud(CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, xpart(angle, 800), ypart(angle, 800), 40, TCOL_ORANGE, 980);
      angle += ANGLE_3;
     }
     angle += ANGLE_6;
     for (i = 0; i < 3; i ++)
     {
      quick_cloud(CLOUD_CIRCLE5, pbullet[b].x, pbullet[b].y, xpart(angle, 900), ypart(angle, 900), 40, TCOL_ORANGE, 990);
      angle += ANGLE_3;
     }
    }
   break;*/

 }
// destroy_pbullet(b);

}


void destroy_pbullet(int b)
{

 pbullet[b].type = PBULLET_NONE;

}


