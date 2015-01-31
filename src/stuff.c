/*

This file contains some maths and utility functions. The only ones used at present are:

xpart/ypart - call them with angle x and distance d; they'll give you the x component and y component
              of a line in the angle x and the distance d

angle_to_radians - I use a 1024-degree circle in a format called 'angle'. It lets me use lookup tables
radians_to_angle   without interpolation for simple trig, and minimises slow floating-point operations.
                   angle_to_radians takes 'angle' values and turns them into radians; radians_to_angle etc.

grand(n) - random number from 1 to n


*/


#include "config.h"

#include "allegro.h"

#include <math.h>

int turn_towards_angle(int angle, int tangle, int turning);
int delta_turn_towards_angle(int angle, int tangle, int turning);

float lcos(int angle);
float lsin(int angle);
float angle_to_radians(int angle);

// I have no idea why, but the first few elements of cos_table always get corrupted
//  unless I put a big fat decoy array just above. A similar thing happens to the
//  palette arrays; allegro seems to have a problem with global arrays like these.
float decoy_table [ANGLE_1]; // not used
float cos_table [ANGLE_1];
float sin_table [ANGLE_1];

inline int xpart(int angle, int length);

void init_trig(void)
{
 int i;

 for (i = 0; i < ANGLE_1; i ++)
 {
   cos_table [i] = cos(angle_to_radians(i));// * ANGLE_1;
   sin_table [i] = sin(angle_to_radians(i));// * ANGLE_1;
 }


}

inline int xpart(int angle, int length)
{
 return (cos_table [angle & 1023] * length);
}

inline int ypart(int angle, int length)
{
 return (sin_table [angle & 1023] * length);
}

float lcos(int angle)
{
 return cos_table [angle & 1023];

}

float lsin(int angle)
{
 return sin_table [angle & 1023];

}


float angle_to_radians(int angle)
{
 return ((float) angle * PI * 2) / ANGLE_1;
}

int radians_to_angle(float angle)
{
 if (angle < 0)
  angle += PI * 2;
 return (int) ((angle * ANGLE_1) / (PI * 2));
}

fixed angle_to_fixed(int angle)
{
 return itofix(angle / ANGLE_TO_FIXED);
}

int grand(int number)
{
 if (number == 0)
  return 0;
 return ((rand() + (rand() << 16)) & 0x7fffffff) % number;
}

// returns the new angle
int turn_towards_angle(int angle, int tangle, int turning)
{

 if ((angle < tangle && tangle > angle + ANGLE_2)
     || (angle > tangle && tangle > angle - ANGLE_2))
 {
  angle -= turning;
  if (angle < 0)
   angle += ANGLE_1;
 }
  else
  {
   angle += turning;
   if (angle > ANGLE_1)
    angle -= ANGLE_1;
  }

 return angle;

}


int turn_towards_xy(int x1, int y1, int x2, int y2, int angle, int turning)
{

  int tangle =
    radians_to_angle(atan2((y2 - y1), (x2 - x1))) & 1023;

  return turn_towards_angle(angle, tangle, turning);

}

// delta version just returns the change needed
int delta_turn_towards_angle(int angle, int tangle, int turning)
{

// if ((angle < tangle && tangle > angle + ANGLE_2)
 if ((angle < tangle && tangle > angle + ANGLE_2)
     || (angle > tangle && tangle > angle - ANGLE_2))
 {
  return turning * -1;
 }

 return turning;

}

int odd_even(int v)
{
 if ((v | 1) == 1)
  return 1;

  return -1;

}

int delta_turn_towards_xy(int x1, int y1, int x2, int y2, int angle, int turning)
{

  int tangle =
    radians_to_angle(atan2((y2 - y1), (x2 - x1)));
   if (tangle < 0)
    tangle += ANGLE_1;
   if (tangle > ANGLE_1)
    tangle -= ANGLE_1;

  return delta_turn_towards_angle(angle, tangle, turning);

}



// speed must be at least 4, and a factor of 1024
int pulsate(int speed, int amount, int county)
{
 return xpart((county * speed) & 1023, amount);
}

void error_message_out(const char *errm)
{
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      allegro_message(errm);
      exit(1);
}


int angle_difference(int a1, int a2)
{
 int d1, d2;

 d1 = (a1 - a2 + ANGLE_1) % ANGLE_1;
 d2 = (a2 - a1 + ANGLE_1) % ANGLE_1;

 if (d1 < d2)
  return abs(d1);

 return abs(d2);
}

char coin(void)
{
 return rand() & 1;
}

int pos_or_neg(int a)
{
 if (coin == 0)
  return a;

 return a * -1;

}
