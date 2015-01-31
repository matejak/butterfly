#include "config.h"

#include <math.h>

#include "allegro.h"

#include "globvars.h"

#include "stuff.h"
#include "enemy.h"
#include "sound.h"
#include "cloud.h"

#include "palette.h"

void run_ebullets(void);
void destroy_ebullet(int b);
//int check_ebullet_collision(int b);
void run_standard_cord(int b);
void drag_ebullet(int b, int drag);

/*
Initialises the enemy bullets. Must be called at start of each game/level.
*/
void init_ebullets(void)
{

 int b;

 for (b = 0; b < NO_EBULLETS; b ++)
 {
  ebullet[b].type = EBULLET_NONE;
 }

}

/*
Creates an enemy bullet of type type and returns the index in the ebullet struct array.
*/
int create_ebullet(int type)
{
  int b;

  for (b = 0; b < NO_EBULLETS; b ++)
  {
      if (ebullet[b].type == EBULLET_NONE)
       break;
      if (b == NO_EBULLETS - 1)
       return -1;

  }

 ebullet[b].type = type;
 ebullet[b].time = 0;
 ebullet[b].new_ebullet = 1;
 return b;

}

/*
Call this from the game loop to make the ebullets work.
*/
void run_ebullets(void)
{

  int b, angle, k;
  char no_more_bullet = 0;

  for (b = 0; b < NO_EBULLETS; b ++)
  {
    if (ebullet[b].type == EBULLET_NONE)
     continue;
    no_more_bullet = 0;
    ebullet[b].time ++;
    switch(ebullet[b].type)
    {
/*        case EBULLET_TRACKER:
            ebullet[b].x_speed += xpart(ebullet[b].angle, 200);
            ebullet[b].y_speed += ypart(ebullet[b].angle, 200);
            ebullet[b].x += ebullet[b].x_speed;
            ebullet[b].y += ebullet[b].y_speed;

            c = create_cloud(CLOUD_ESLINE);
            if (c != -1)
            {
             cloud[c].x2 = ebullet[b].x;
             cloud[c].y2 = ebullet[b].y;
             cloud[c].x = ebullet[b].x - ebullet[b].x_speed;
             cloud[c].y = ebullet[b].y - ebullet[b].y_speed;
             cloud[c].timeout = 9;
             cloud[c].colour = TRANS_B1_OUT;
            }


            ebullet[b].x_speed *= 98;
            ebullet[b].x_speed /= 100;
            ebullet[b].y_speed *= 98;
            ebullet[b].y_speed /= 100;
            if (ebullet[b].timeout > 430 - (arena.difficulty * 40) && ebullet[b].timeout < 490)
             ebullet[b].angle = turn_towards_xy(ebullet[b].x, ebullet[b].y, player.x, player.y, ebullet[b].angle, 10 + arena.difficulty * 4);

            break;*/
//        default:
     case EBULLET_BEAM:
         ebullet[b].timeout --;
         if (ebullet[b].timeout <= 0)
          destroy_ebullet(b);

          no_more_bullet = 1; // don't want to destroy the beam just because it starts outside the screen border.
          // here no_more_bullet just tells it not to run the destruction checks, whether or not it still exists.
          break;


        case EBULLET_BLOB:
            ebullet[b].x += ebullet[b].x_speed;
            ebullet[b].y += ebullet[b].y_speed;
            break;
        case EBULLET_BLOBSHOT:
            ebullet[b].x += ebullet[b].x_speed;
            ebullet[b].y += ebullet[b].y_speed;
            if (ebullet[b].status > 0)
             ebullet[b].status --;
            break;
        case EBULLET_STOPMINE:
            ebullet[b].x += ebullet[b].x_speed;
            ebullet[b].y += ebullet[b].y_speed;
            drag_ebullet(b, 985);
            ebullet[b].angle += ebullet[b].status;
            ebullet[b].angle &= 1023;
            if (ebullet[b].timeout <= 5)
            {
             quick_cloud(CLOUD_CIRCLE2, ebullet[b].x, ebullet[b].y, 0, 0, 50, ebullet[b].colour, 0);
             no_more_bullet = 1;
             destroy_ebullet(b);
            }
            break;
        case EBULLET_DRAGGER:
            ebullet[b].x += ebullet[b].x_speed;
            ebullet[b].y += ebullet[b].y_speed;
            drag_ebullet(b, 995);
            if (ebullet[b].timeout <= 5)
            {
             quick_cloud(CLOUD_3FCIRCLE1, ebullet[b].x, ebullet[b].y, 0, 0, 15, ebullet[b].colour, 0);
             no_more_bullet = 1;
             destroy_ebullet(b);
            }
            break;
        case EBULLET_SHOT:
            ebullet[b].x += ebullet[b].x_speed;
            ebullet[b].y += ebullet[b].y_speed;
            if (ebullet[b].status > 0)
            {
             if (ebullet[b].status & 1)
             {
              quick_cloud(CLOUD_FCIRCLE4, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed + grand(1000) - grand(1000), ebullet[b].y_speed + grand(1000) - grand(1000), 10 + grand(10) + ebullet[b].status, ebullet[b].colour, 900);
             }
             ebullet[b].status --;
            }
         break;
        case EBULLET_SHOT2:
            ebullet[b].x += ebullet[b].x_speed;
            ebullet[b].y += ebullet[b].y_speed;
//            if (ebullet[b].timeout & 1)
//            {
             quick_cloud(CLOUD_3FCIRCLE8, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 20 + grand(30), ebullet[b].colour, 920);
//             quick_cloud(CLOUD_FCIRCLE8, ebullet[b].x, ebullet[b].y, 0, 0, 20 + grand(30), ebullet[b].colour, 0);
//            }
         break;
        case EBULLET_B21SHOT:
            ebullet[b].x_speed += xpart(ebullet[b].angle, 150);
            ebullet[b].y_speed += ypart(ebullet[b].angle, 150);
            ebullet[b].x += ebullet[b].x_speed;
            ebullet[b].y += ebullet[b].y_speed;
            drag_ebullet(b, 1010);
            quick_cloud(CLOUD_3FCIRCLE8, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed, ebullet[b].y_speed, 15 + grand(20), ebullet[b].colour, 940);
         break;
        case EBULLET_WIGGLY:
            ebullet[b].x += ebullet[b].x_speed;
            ebullet[b].y += ebullet[b].y_speed;
            angle = ebullet[b].angle + ANGLE_4;
            k = xpart(ebullet[b].timeout * 45, 2000);
//            quick_cloud(CLOUD_3PCIRCLE2, ebullet[b].x, ebullet[b].y, ebullet[b].x_speed + xpart(angle, k), ebullet[b].y_speed + ypart(angle, k), 20, ebullet[b].colour, 980);
            quick_cloud(CLOUD_3PCIRCLE2, ebullet[b].x, ebullet[b].y, xpart(angle, k), ypart(angle, k), 12, ebullet[b].colour, 940);
         break;
        case EBULLET_FLAKE:
        case EBULLET_FLAKE2:
        case EBULLET_FLAKE3:
            ebullet[b].x += ebullet[b].x_speed;
            ebullet[b].y += ebullet[b].y_speed;
            if (ebullet[b].timeout & 1)
            ebullet[b].status --;
//            ebullet[b].status %= 7;
            if (ebullet[b].status < 0)
             ebullet[b].status = 5;
            if (ebullet[b].timeout < 3300)
             ebullet[b].y_speed += 20;
            break;
        case EBULLET_IBULLET:
            ebullet[b].x += ebullet[b].x_speed;
            ebullet[b].y += ebullet[b].y_speed;
            if (ebullet[b].timeout & 1)
            ebullet[b].status --;
            if (ebullet[b].status < 0)
             ebullet[b].status = 5;
            if (ebullet[b].timeout < 3300)
             ebullet[b].y_speed += 20;
         break;
        case EBULLET_IBULLET_2:
            ebullet[b].x += ebullet[b].x_speed;
            ebullet[b].y += ebullet[b].y_speed;
//            if (ebullet[b].timeout % 21 == 0)
            if (ebullet[b].timeout & 1)
            ebullet[b].status --;
            if (ebullet[b].status < 0)
             ebullet[b].status = 11;
            if (ebullet[b].timeout < 3300)
             ebullet[b].y_speed += 20;
         break;
        case EBULLET_B13SHOT:
            ebullet[b].x += ebullet[b].x_speed;
            ebullet[b].y += ebullet[b].y_speed;
            ebullet[b].angle += ebullet[b].status;
            ebullet[b].angle &= 1023;
            quick_cloud(CLOUD_3FCIRCLE2, ebullet[b].x + grand(8000) - grand(8000), ebullet[b].y + grand(8000) - grand(8000), ebullet[b].x_speed, ebullet[b].y_speed, 15 + grand(10), ebullet[b].colour, 960);
         break;
        case EBULLET_SPINNER3:
            ebullet[b].x += ebullet[b].x_speed;
            ebullet[b].y += ebullet[b].y_speed;
            ebullet[b].angle += ebullet[b].status;
            ebullet[b].angle &= 1023;
            break;
// don't use default! It wrecks things like beams
    }

//    if (check_ebullet_collision(b) == 1)
//     continue; // ebullet no longer exists

    if (no_more_bullet == 1)
     continue;

    ebullet[b].timeout --;

    if (ebullet[b].x <= -50000 || ebullet[b].y <= -50000
          || ebullet[b].x >= 650000 || ebullet[b].y >= 650000)
      destroy_ebullet(b);
        else
        {
         if (ebullet[b].timeout <= 0)
          destroy_ebullet(b);
        }

  }

}


void drag_ebullet(int b, int drag)
{
 ebullet[b].x_speed *= drag;
 ebullet[b].x_speed >>= 10;
 ebullet[b].y_speed *= drag;
 ebullet[b].y_speed >>= 10;

}



void destroy_ebullet(int b)
{
  ebullet[b].type = EBULLET_NONE;
}

