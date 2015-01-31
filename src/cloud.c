

#include "config.h"

#include <math.h>

#include "allegro.h"

#include "globvars.h"

#include "stuff.h"


#include "palette.h"


void run_clouds(void);
void destroy_cloud(int b);
int quick_cloud(int type, int x, int y, int x_speed, int y_speed, int timeout, int colour, int drag);


void init_clouds(void)
{

 int c;

 for (c = 0; c < NO_CLOUDS; c ++)
 {
  cloud[c].type = CLOUD_NONE;
 }

}

int create_cloud(int type)
{
  int c;

  for (c = 0; c < NO_CLOUDS; c ++)
  {
      if (cloud[c].type == CLOUD_NONE)
       break;
      if (c == NO_CLOUDS - 1)
       return -1;

  }

 cloud[c].type = type;
 cloud[c].timeout = 0;
 cloud[c].x_speed = 0;
 cloud[c].y_speed = 0;
 cloud[c].drag = 0;
 return c;

}

void run_clouds(void)
{

  int c, xa, angle, ya;

  for (c = 0; c < NO_CLOUDS; c ++)
  {
    if (cloud[c].type == CLOUD_NONE)
     continue;
//    if (arena.counter % 8 == 0)
     cloud[c].timeout --;
    if (cloud[c].timeout <= 0)
    {
     destroy_cloud(c);
     continue;
    }
    if (cloud[c].drag != 0)
    {
     cloud[c].x_speed *= cloud[c].drag;
     cloud[c].x_speed >>= 10;
     cloud[c].y_speed *= cloud[c].drag;
     cloud[c].y_speed >>= 10;
    }
    cloud[c].x += cloud[c].x_speed;
    cloud[c].y += cloud[c].y_speed;
    switch(cloud[c].type)
    {
        default:
         break;
        case CLOUD_WAITCIRCLE1:
        case CLOUD_WAITCIRCLE2:
        case CLOUD_WAITCIRCLE4:
         if (cloud[c].status > 0)
         {
          cloud[c].timeout ++;
          cloud[c].status --;
         }
         break;
        case CLOUD_LARGE_SHOCK:
//         cloud[c].timeout --;
         break;
        case CLOUD_RUMBLE:
         if (cloud[c].timeout & 1)
          break;
         xa = cloud[c].timeout;
         if (xa > cloud[c].status)
          xa = cloud[c].status;
         angle = grand(ANGLE_1);
         ya = grand(cloud[c].status2);
         quick_cloud(CLOUD_FCIRCLE1, cloud[c].x + xpart(angle, ya), cloud[c].y + ypart(angle, ya), 0, 0, xa + grand(xa), cloud[c].colour, 0);
         break;
        case CLOUD_HUGE_SHOCK_FAST:
        case CLOUD_SHRINKING_LOCK:
         cloud[c].timeout --;
         break;
        case CLOUD_SHRINKING_BEAM:
         cloud[c].y -= 10000;
         cloud[c].colour += 4000;
         if (cloud[c].y < cloud[c].colour)
          destroy_cloud(c);
         break;
    } // cloud may be destroyed here
    }

}



int quick_cloud(int type, int x, int y, int x_speed, int y_speed, int timeout, int colour, int drag)
{

  int c;

  c = create_cloud(type);

  if (c != -1)
  {
   cloud[c].x = x;
   cloud[c].y = y;
   cloud[c].timeout = timeout;
   cloud[c].colour = colour;
   cloud[c].drag = drag;
   cloud[c].x_speed = x_speed;
   cloud[c].y_speed = y_speed;
  }

  return c;

}




void destroy_cloud(int c)
{
 cloud[c].type = CLOUD_NONE;

}
