
#include "config.h"

#include "allegro.h"

#include <math.h>

#include "globvars.h"
#include "pbullet.h"
#include "cloud.h"
#include "sound.h"
#include "enemy.h"

#include "stuff.h"

#define TARGET_ARRAY_SIZE 25

#include "palette.h"

#define SCREENSHOT

#ifdef SCREENSHOT
#include <string.h>
extern RGB palet [256];
#endif

void multi_fire(int m);
void mflash(int b, int w, int m);
int find_enemies(int targets [TARGET_ARRAY_SIZE], int x, int y, int number, int start_number, int force);
void fire_seekers(int m, int num);

void check_firing();
void acquire_lock(void);
void pack_locks(void);
void run_hand(void);
void drag_hand(int drag);
void create_hand_bullet(void);

void run_orb(void);
void drag_orb(int drag);
void create_orb_bullet(void);

void create_orb_field(void);

//#define SHOW_DAMAGE

#ifdef SHOW_DAMAGE
void calc_damage(int dam, int speed);
extern int damage_per_time;
#endif


enum
{
MOVE_NO,
MOVE_L,
MOVE_R,
MOVE_U,
MOVE_D,
MOVE_UL,
MOVE_DL,
MOVE_UR,
MOVE_DR
};

void get_input(void)
{


#ifdef SCREENSHOT

static int scrs = 0;
static int sshot_counter = 0;


 char sfile [20];
 char istr [20];

 if (sshot_counter > 0)
  sshot_counter --;

 if (key [KEY_F1] && sshot_counter <= 0)
 {
  BITMAP *scrshot_bmp;
   scrshot_bmp = create_bitmap(600, 600);
   blit(screen, scrshot_bmp, 100,0,0,0,600,600);

  strcpy(sfile, "sshot");
  strcat(sfile, itoa(scrs, istr, 10));
  strcat(sfile, ".bmp");
  save_bitmap(sfile, scrshot_bmp, palet);
  rectfill(screen, 100, 0, 700, 0, COL_WHITE);
//  clear_to_color(screen, COL_WHITE);
  scrs ++;
  sshot_counter = 15;
  destroy_bitmap(scrshot_bmp);
 }

#endif

#ifdef DEBUGGING

 if (arena.counter % 20 == 0)
 {
  if (key [KEY_1] && player.wlevel [0] < 12)
  {
   player.wlevel [0] ++;
   player.wxp [0] = 0;
   set_wpixels();
//   play_effectwfvx(WAV_LEVEL, tone[NOTE_2G], 250, player.x);
   player.wgained [0] = 20;
   player.overbflash = 25;

  }
  if (key [KEY_2] && player.wlevel [1] < 12)
  {
   player.wlevel [1] ++;
   player.wxp [1] = 0;
   set_wpixels();
  }
  if (key [KEY_3] && player.wlevel [2] < 12)
  {
   player.wlevel [2] ++;
   player.wxp [2] = 0;
   set_wpixels();
  }
  if (key [KEY_4] && player.weapon_level < 12)
  {
   player.wlevel [0] ++;
   player.wxp [0] = 0;
   player.wlevel [1] ++;
   player.wxp [1] = 0;
   player.wlevel [2] ++;
   player.wxp [2] = 0;
   player.weapon_level ++;
   set_wpixels();
  }
  if (key [KEY_5])
  {
   player.lives ++;
//   play_effectwfvx(WAV_EXTRA, tone [NOTE_3C], 100, 320);
   //play_effectwfvx(WAV_LEVEL, tone[NOTE_1G], 250, player.x);
  }
/*  if (key [KEY_5] && arena.shift_message == 0)
  {
   arena.shift_message = 300;
   arena.phase ++;
  }*/
  if (key [KEY_6])
   arena.level_finished = 50;

 }

#endif


    if (player.respawning > 0)
    {
     player.y -= 4000;
     player.respawning --;
     return;
    }

    int xc = 0, yc = 0;

//   int move_which = 0;

  int i;

  for (i = 0; i < 3; i ++)
  {

    if (player.beam [i] == -1 && player.beam_flicker [i] == 0)
     player.beam_counter [i] = 0;
      else
       player.beam [i] = -1;
  }

// check firing was here!

/*   if (key [player.key [CKEY_FIRE1]])
   {
    multi_fire(0);
   }
     else
     {
      if (key [player.key [CKEY_FIRE2]])
       multi_fire(1);
        else
        {
         if (key [player.key [CKEY_FIRE3]])
          multi_fire(2);
        }
     }*/

 if (options.key_or_joy == 1)
 {
   poll_joystick();
/*
   if (joy[0].button[options.joy_button [0]].b)
    multi_fire(0);
   if (joy[0].button[options.joy_button [1]].b)
    multi_fire(1);
   if (joy[0].button[options.joy_button [2]].b)
    multi_fire(2);*/

//   if (player.swing_hold == 1 && joy[0].button [options.joy_button[player.blade_index]].b == 0)
//    player.swing_hold = 0;

/*   if (player.ring_hold == 1 && joy[0].button [options.joy_button[player.ring_index]].b == 0)
    player.ring_hold = 0;*/

/*   if (joy[0].button[options.joy_button [0]].b
    || joy[0].button[options.joy_button [1]].b
    || joy[0].button[options.joy_button [2]].b)
     player.firing = 1;
*/
//   int moving = MOVE_NO;

   xc = joy[0].stick[options.joy_stick].axis[0].pos * options.joy_sensitivity;// / 5120 * 6000;
   yc = joy[0].stick[options.joy_stick].axis[1].pos * options.joy_sensitivity;// / 5120 * 6000;

   int jangle = radians_to_angle(atan2(yc, xc));
   jangle &= 1023;
   if (xc == 0 && yc == 0)
    jangle = ANGLE_4;
   int dist = hypot(yc, xc);

   if (dist > 6000)
    dist = 6000;

   if (joy[0].button[options.joy_button [2]].b)
    dist /= 2;

   xc = xpart(jangle, dist);
   yc = ypart(jangle, dist);

  if (xc < -6000)
   xc = -6000;
  if (yc < -6000)
   yc = -6000;
  if (xc > 6000)
   xc = 6000;
  if (yc > 6000)
   yc = 6000;

// NO!!! Need to calc angle then move in that angle
/*
   player.wings1 += xc / 1000;

   if (player.wings1 > 20)
    player.wings1 = 20;
   if (player.wings1 < -20)
    player.wings1 = -20;

   player.wings2 += yc / 1000;

   if (player.wings2 > 20)
    player.wings2 = 20;
   if (player.wings2 < -20)
    player.wings2 = -20;

   if (xc == 0)
   {
    if (player.wings1 > 0)
     player.wings1 -= 2;
    if (player.wings1 < 0)
     player.wings1 += 2;
   }

   if (yc == 0)
   {
    if (player.wings2 > 0)
     player.wings2 -= 2;
    if (player.wings2 < 0)
     player.wings2 += 2;

   }
*/
     player.x += xc;
     player.y += yc;
     if (player.x < X_MIN)
       player.x = X_MIN;
     if (player.y < Y_MIN)
       player.y = Y_MIN;
     if (player.x > X_MAX)
       player.x = X_MAX;
     if (player.y > Y_MAX)
       player.y = Y_MAX;


/*     if (player.ring_index != -1)
     {
      player.move_x = 0;
      player.move_y = 0;
      if (jangle > ANGLE_16)// && jangle <= ANGLE_8 + ANGLE_16)
      {
       player.move_x = 1;
       player.move_y = 1;
      }
      if (jangle > ANGLE_8 + ANGLE_16)// && jangle <= ANGLE_8 + ANGLE_16)
      {
       player.move_x = 0;
       player.move_y = 1;
      }
      if (jangle > ANGLE_4 + ANGLE_16)// && jangle <= ANGLE_8 + ANGLE_16)
      {
       player.move_x = -1;
       player.move_y = 1;
      }
      if (jangle > ANGLE_4 + ANGLE_8 + ANGLE_16)// && jangle <= ANGLE_8 + ANGLE_16)
      {
       player.move_x = -1;
       player.move_y = 0;
      }
      if (jangle > ANGLE_2 + ANGLE_16)// && jangle <= ANGLE_8 + ANGLE_16)
      {
       player.move_x = -1;
       player.move_y = -1;
      }
      if (jangle > ANGLE_2 + ANGLE_8 + ANGLE_16)// && jangle <= ANGLE_8 + ANGLE_16)
      {
       player.move_x = 0;
       player.move_y = -1;
      }
      if (jangle > ANGLE_2 + ANGLE_4 + ANGLE_16)// && jangle <= ANGLE_8 + ANGLE_16)
      {
       player.move_x = 1;
       player.move_y = -1;
      }
      if (jangle > ANGLE_1-ANGLE_16 || jangle < ANGLE_16)// && jangle <= ANGLE_16)
      {
       player.move_x = 1;
       player.move_y = 0;
      }
     }*/


 }
  else
  {

/* if (key [player.key [CKEY_FIRE1]])
    multi_fire(0);

 if (key [player.key [CKEY_FIRE2]])
    multi_fire(1);

 if (key [player.key [CKEY_FIRE3]])
    multi_fire(2);

   if (player.swing_hold == 1 && !key[player.key [CKEY_FIRE1 + player.blade_index]])
    player.swing_hold = 0;

   if (player.ring_hold == 1 && !key[player.key [CKEY_FIRE1 + player.ring_index]])
    player.ring_hold = 0;

   if (key [player.key [CKEY_FIRE1]]
    || key [player.key [CKEY_FIRE2]]
    || key [player.key [CKEY_FIRE3]])
     player.firing = 1;
*/
   int moving = MOVE_NO;

   char key_left = key [player.key [CKEY_LEFT]];
   char key_right = key [player.key [CKEY_RIGHT]];
   char key_up = key [player.key [CKEY_UP]];
   char key_down = key [player.key [CKEY_DOWN]];

   if (key_left)
   {
    if (key_right || player.x <= X_MIN)
     xc = 0;
      else xc = -2000;
   }
    else
    {
     if (key_right)
     {
      if (player.x < X_MAX)
       xc = 2000;
     }
    }

   if (key_up)
   {
    if (key_down || player.y <= Y_MIN)
     yc = 0;
      else yc = -2000;
   }
    else
     if (key_down)
     {
      if (player.y < Y_MAX - 4000)
       yc = 2000;
     }

   if (xc > 0 && yc > 0)
   {
    xc = 1414;
    yc = 1414;
    moving = MOVE_DR;
   }
   if (xc < 0 && yc > 0)
   {
    xc = -1414;
    yc = 1414;
    moving = MOVE_DL;
   }
   if (xc < 0 && yc < 0)
   {
    xc = -1414;
    yc = -1414;
    moving = MOVE_UL;
   }
   if (xc > 0 && yc < 0)
   {
    xc = 1414;
    yc = -1414;
    moving = MOVE_UR;
   }

 xc *= 3;
 yc *= 3;

/*
   player.wings1 += xc / 1000;

   if (player.wings1 > 20)
    player.wings1 = 20;
   if (player.wings1 < -20)
    player.wings1 = -20;

   player.wings2 += yc / 1000;

   if (player.wings2 > 20)
    player.wings2 = 20;
   if (player.wings2 < -20)
    player.wings2 = -20;

   if (xc == 0)
   {
    if (player.wings1 > 0)
     player.wings1 -= 2;
    if (player.wings1 < 0)
     player.wings1 += 2;
   }

   if (yc == 0)
   {
    if (player.wings2 > 0)
     player.wings2 -= 2;
    if (player.wings2 < 0)
     player.wings2 += 2;

   }
*/
 if (key [player.key [CKEY_SLOW]])
 {
  xc /= 2;
  yc /= 2;
 }

     player.x += xc;
     player.y += yc;
     if (player.x < X_MIN)
       player.x = X_MIN;
     if (player.y < Y_MIN)
       player.y = Y_MIN;
     if (player.x > X_MAX)
       player.x = X_MAX;
     if (player.y > Y_MAX)
       player.y = Y_MAX;


/*     if (player.ring_index != -1)
     {
      player.move_x = 0;
      player.move_y = 0;
      if (key_right)
       player.move_x = 1;
      if (key_left)
       player.move_x = -1;
      if (key_down)
       player.move_y = 1;
      if (key_up)
       player.move_y = -1;
     }*/


  } // end keyboard control

  check_firing();

}

void check_firing(void)
{

 int b;
 int f, i, x, y, angle;

 int key1 = key [player.key [CKEY_FIRE1]];
 int key2 = key [player.key [CKEY_FIRE2]];
 int key3 = key [player.key [CKEY_FIRE3]];
// remember joystick!

 if (options.key_or_joy == 1)
 {
   poll_joystick();
   key1 = joy[0].button[options.joy_button [0]].b;
   key2 = joy[0].button[options.joy_button [1]].b;
   key3 = joy[0].button[options.joy_button [3]].b;
 }

/* if (key1)
 {
   if (player.hold_key1 == 0)
    player.hold_key1 = 1;
     else
      player.hold_key1 = 2;
 }
  else
  {
  if (player.hold_key1)
   player.hold_key1 = 0;
  }

 if (key1)
 {
   if (player.hold_key1 == 0)
    player.hold_key1 = 1;
     else
      player.hold_key1 = 2;
 }
  else
  {
  if (player.hold_key1)
   player.hold_key1 = 0;
  }*/

 switch(player.type)
 {
  case PTYPE_BASIC:
   if (key1 && player.charge <= 0)
   {
    if (player.recycle1 <= 0)
    {
     play_effectwfvx(WAV_SHOT2, 1000 - player.weapon_level * 15, 80, player.x);
     player.recoil = 9;
     player.recoiling = 1;
     player.recycle1 = 1000;
     player.recycle_speed1 = 120;
     f = 0;
     for (f = 0; f < 2; f ++)
     {
      b = create_pbullet(PBULLET_SHOT);
      if (b != -1)
      {
       pbullet[b].x = player.x - 8000;
       if (f == 1)
        pbullet[b].x = player.x + 8000;
       pbullet[b].y = player.y - 22000;
       pbullet[b].x_speed = 0;
       pbullet[b].y_speed = - 11500;
       pbullet[b].xsize = 2000 + player.weapon_level * 660;//9000 + player.wlevel [m] * 500;
       pbullet[b].ysize = 2000 + player.weapon_level * 660;//9000 + player.wlevel [m] * 500;
       pbullet[b].level = player.weapon_level;
       pbullet[b].dam = 350 + player.weapon_level * 30;
       pbullet[b].source = 0;
      }
     }
#ifdef SHOW_PLAYER_DAMAGE
// don't bother testing for b here.
             player.wstrength [0] = ((pbullet[b].dam * 100) / (player.recycle1 / player.recycle_speed1)) * 2;
#endif

     quick_cloud(CLOUD_CIRCLE1, player.x - 8000, player.y - 22000, 0, -200, 10 + grand(10), CLOUD_COL_1, 990);
     quick_cloud(CLOUD_CIRCLE1, player.x + 8000, player.y - 22000, 0, -200, 10 + grand(10), CLOUD_COL_1, 990);

     quick_cloud(CLOUD_CIRCLE4, player.x - 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x - 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x - 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x - 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x - 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x - 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);

     quick_cloud(CLOUD_CIRCLE4, player.x + 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x + 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x + 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x + 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x + 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x + 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);

    }
   }
   if (key2 || key3 || (!key2 && !key3 && player.key_holding == 3))
   {
    player.recycle1 = 1000;
    if ((key1 || (!key2 && !key3 && player.key_holding == 3))
     && player.charge >= 6000)
    {
//             fire_seekers(2, player.charge / 3000 - 1);
             play_effectwfvx(WAV_ROBOT, 200 + player.charge / 80, 180, player.x);
             fire_seekers(2, player.charge / 3000 - 1);
#ifdef SHOW_PLAYER_DAMAGE
             player.wstrength [2] = (800 * (player.charge / 3000 - 1) * 50) / (player.charge / 200);
#endif
             quick_cloud(CLOUD_CIRCLE2, player.x, player.y - 16000, 0, 0, 15 + grand(20) + player.charge / 500, CLOUD_COL_1, 0);
             if (player.charge >= 20000)
              quick_cloud(CLOUD_SMALL_SHOCK, player.x, player.y - 16000, 0, 0, 30, CLOUD_COL_1, 0);
             player.charge = 0;
             player.key_holding = 0;
    }
     else
     {
                    player.charge += 250;
     // note next line - change the 200 there if changing here!
     // and also in the wstrength set below!!
/*      if (player.charge >= 6000 && player.charge - 250 < 6000)
      {

      }*/
      if (player.charge >= 19750 && player.charge < 20000)
        play_effectwfvx(WAV_BLIP, 800, 150, player.x);

      if (player.charge > 20000)
       player.charge = 20000;
      if (key2)
       player.key_holding = 2;
      if (key3)
       player.key_holding = 3;
     }
   }
    else
    {
     player.key_holding = 0;
     if (player.charge > 0)
     {
       if (player.charge > 6000)
       {
        b = create_pbullet(PBULLET_CHARGE1);
        if (b != -1)
        {
         pbullet[b].x = player.x;
         pbullet[b].y = player.y - 12000;
         pbullet[b].x_speed = 0;
         pbullet[b].y_speed = - 8000;
         pbullet[b].xsize = 8000 + player.charge / 2;
         pbullet[b].ysize = 8000 + player.charge / 2;
         pbullet[b].level = player.weapon_level;
//         pbullet[b].dam = (player.charge / 2.5) - 1000; // if this changed, change in bullet explosion function also
         pbullet[b].dam = (player.charge * (10 + player.weapon_level)) / 29; // if this changed, change in bullet explosion function also
         pbullet[b].dam -= 1000; // there is some overhead for beginning the charge
         play_effectwfvx(WAV_CHARGE, 1500 - (player.charge) / 40, 140, player.x);

#ifdef SHOW_PLAYER_DAMAGE
         player.wstrength [1] = (pbullet[b].dam * 100) / (player.charge / 200);
         //     player.charge / 200 is the number of ticks spent charging.
#endif
         pbullet[b].source = 1;
         quick_cloud(CLOUD_CIRCLE2, player.x, player.y - 16000, 0, 0, 20 + grand(20) + pbullet[b].dam / 200, CLOUD_COL_1, 0);
        }

       }

     }
     player.charge = 0;
    }
   break;


  case PTYPE_ANENOME:
    if (key2 || key3)
    {
#ifdef SHOW_DAMAGE
   calc_damage(100 + player.wlevel [player.blade_index] * 22, 1000);
#endif

   if (key1 || key3)
   {
     player.blade_redness += 2;
     if (player.blade_redness > 25)
      player.blade_redness = 25;
   }

   if (player.blade_pos == BLADE_LEFT)
   {
    player.blade_pos = BLADE_CW_SLASH;
    player.swing_hold = 1;
    player.blade_distance = 20000;
    player.blade_size = 2000;
    player.blade_x = player.x;
    player.blade_y = player.y;
    play_effectwfvx(WAV_SWEEP, 1000 - player.weapon_level * 15, 180, player.x);

//    play_effectwfvx(WAV_WHOOSH, 600, 120, player.x);
    return;
   }
   if (player.blade_pos == BLADE_RIGHT)
   {
    player.blade_pos = BLADE_ACW_SLASH;
    player.swing_hold = 1;
    player.blade_distance = 20000;
    player.blade_size = 2000;
    player.blade_x = player.x;
    player.blade_y = player.y;
//    play_effectwfvx(WAV_WHOOSH, 600, 120, player.x);
    play_effectwfvx(WAV_SWEEP, 1000 - player.weapon_level * 15, 180, player.x);
    return;
   }

   return;

   }
    else
    {
     player.swing_hold = 0;
    }

   if (key1)
   {
    if (player.recycle1 <= 0)
    {
     player.recoil = 9;
     player.recoiling = 1;
     player.recycle1 = 1000;
     player.recycle_speed1 = 50;
     play_effectwfvx(WAV_SHOT2, 400 - player.weapon_level * 5, 100, player.x);
      b = create_pbullet(PBULLET_ASHOT);
      if (b != -1)
      {
       pbullet[b].x = player.x;
       pbullet[b].y = player.y - 12000;
       pbullet[b].x_speed = 0;
       pbullet[b].y_speed = -7500;
       pbullet[b].xsize = 7000;//9000 + player.wlevel [m] * 500;
       pbullet[b].ysize = 7000;//9000 + player.wlevel [m] * 500;
       pbullet[b].level = player.weapon_level;
       pbullet[b].dam = 1700 + player.weapon_level * 130;
       pbullet[b].source = 0;
      }
#ifdef SHOW_PLAYER_DAMAGE
// don't bother testing for b here.
             player.wstrength [0] = (pbullet[b].dam * 25) / (player.recycle1 / player.recycle_speed1);
#endif

     quick_cloud(CLOUD_CIRCLE1, player.x, player.y - 12000, 0, -200, 20 + grand(10), CLOUD_COL_1, 990);

     quick_cloud(CLOUD_CIRCLE4, player.x, player.y - 12000, -1000 + grand(2000), -1000 - grand(2000), 20 + grand(25), CLOUD_COL_1, 960);
     quick_cloud(CLOUD_CIRCLE4, player.x, player.y - 12000, -1000 + grand(2000), -1000 - grand(2000), 20 + grand(25), CLOUD_COL_1, 960);
     quick_cloud(CLOUD_CIRCLE4, player.x, player.y - 12000, -1000 + grand(2000), -1000 - grand(2000), 20 + grand(25), CLOUD_COL_1, 960);
     quick_cloud(CLOUD_CIRCLE4, player.x, player.y - 12000, -1000 + grand(2000), -1000 - grand(2000), 20 + grand(25), CLOUD_COL_1, 960);
     quick_cloud(CLOUD_CIRCLE4, player.x, player.y - 12000, -1000 + grand(2000), -1000 - grand(2000), 20 + grand(25), CLOUD_COL_1, 960);
     quick_cloud(CLOUD_CIRCLE4, player.x, player.y - 12000, -1000 + grand(2000), -1000 - grand(2000), 20 + grand(25), CLOUD_COL_1, 960);


    }
    return;
   }
   break;
  case PTYPE_BOMBER:
   if (!key1 || key2 || key3)
   {
    if (player.beam_firing != 0)
    {
     for (i = 0; i < 3; i ++)
     {
       if (player.beam_y [i] == -100000)
        continue;
       x = player.x + xpart(player.ring_angle + (ANGLE_3 * i), player.ring_size << 10);
       y = player.y + ypart(player.ring_angle + (ANGLE_3 * i), player.ring_size << 10);
       quick_cloud(CLOUD_SHRINKING_BEAM, x, y, 0, 0, 1000, player.beam_y [i], 0);
     }
    }
   }

   if (key1 && !key2 && !key3)
   {
    orb_beams();
    for (i = 0; i < 3; i ++)
    {
     player.beam_y [i] -= 4000;
     if (player.beam_y [i] < -5000)
      player.beam_y [i] = -5000;
    }
    player.beam_firing = 2;
#ifdef SHOW_PLAYER_DAMAGE
// don't bother testing for b here.
//             player.wstrength [0] = (pbullet[b].dam * 25) / (player.recycle1 / player.recycle_speed1);
#endif
   }
    else
    {
     player.beam_y [0] = -100000;
     player.beam_y [1] = -100000;
     player.beam_y [2] = -100000;
    }

   if (!key1 && key2 && !key3)
   {
    x = -900000;
    if (player.bomb_recycle1 == 0 && player.recycle2 == 0)
    {
     player.bomb_recycle1 = 1000;
     player.recycle2 = 50;
     x = player.x - 9000;
    }

    if (player.bomb_recycle2 == 0 && player.recycle2 == 0)
    {
     player.bomb_recycle2 = 1000;
     player.recycle2 = 50;
     x = player.x + 9000;
    }

    if (x != -900000)
    {
      play_effectwfvx(WAV_SHOT2, 250, 200, player.x);
      b = create_pbullet(PBULLET_BOMB);
      if (b != -1)
      {
       pbullet[b].x = x;
       pbullet[b].y = player.y - 9000;
       pbullet[b].x_speed = 0;
       pbullet[b].y_speed = -9500;
       pbullet[b].xsize = 0;//9000 + player.wlevel [m] * 500;
       pbullet[b].ysize = 0;//9000 + player.wlevel [m] * 500;
       pbullet[b].level = player.weapon_level;
       pbullet[b].dam = 0;
       pbullet[b].source = 2;
       pbullet[b].timeout = 45;
      }
     quick_cloud(CLOUD_CIRCLE1, x, player.y - 9000, 0, -300, 30 + grand(10), CLOUD_COL_1, 990);

     quick_cloud(CLOUD_CIRCLE2, x, player.y - 9000, - 3000 + grand(500), 3000 + grand(500), 30 + grand(25), CLOUD_COL_1, 940);
     quick_cloud(CLOUD_CIRCLE2, x, player.y - 9000, - 3000 + grand(500), 3000 + grand(500), 30 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE2, x, player.y - 9000, - 3000 + grand(500), 3000 + grand(500), 30 + grand(25), CLOUD_COL_1, 960);
     quick_cloud(CLOUD_CIRCLE2, x, player.y - 9000,  3000 - grand(500), 3000 + grand(500), 30 + grand(25), CLOUD_COL_1, 940);
     quick_cloud(CLOUD_CIRCLE2, x, player.y - 9000,  3000 - grand(500), 3000 + grand(500), 30 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE2, x, player.y - 9000,  3000 - grand(500), 3000 + grand(500), 30 + grand(25), CLOUD_COL_1, 960);

    }

   }

   if ((key1 && key2) || key3)
   {
    player.ring_speed -= 4;
    if (player.ring_speed < -12)
     player.ring_speed = -12;
    player.ring_size += 2;
    if (player.ring_size > 48)
     player.ring_size = 48;

    if (player.recycle1 <= 0)
    {
     player.recycle1 = 1000;
     player.recycle_speed1 = 250 + grand(25);
     player.orb_firing ++;
     if (player.orb_firing > 2)
      player.orb_firing = 0;

      play_effectwfvx(WAV_SHOT2, 2250, 100, player.x);
      b = create_pbullet(PBULLET_RINGSHOT);
      if (b != -1)
      {
       pbullet[b].x = player.x + xpart(player.ring_angle + (ANGLE_3 * player.orb_firing), player.ring_size << 10);
       pbullet[b].y = player.y + ypart(player.ring_angle + (ANGLE_3 * player.orb_firing), player.ring_size << 10);
       pbullet[b].x_speed = xpart(-ANGLE_4 + xpart(player.ring_angle + (ANGLE_3 * player.orb_firing), 36), 9000);
       pbullet[b].y_speed = ypart(-ANGLE_4 + xpart(player.ring_angle + (ANGLE_3 * player.orb_firing), 36), 9000);
       pbullet[b].xsize = 7000;//9000 + player.wlevel [m] * 500;
       pbullet[b].ysize = 7000;//9000 + player.wlevel [m] * 500;
       pbullet[b].angle = -ANGLE_4 + xpart(player.ring_angle + (ANGLE_3 * player.orb_firing), 36);
       pbullet[b].level = player.weapon_level;
       pbullet[b].dam = 300 + player.weapon_level * 25;
       pbullet[b].source = 1;
      }
     quick_cloud(CLOUD_CIRCLE2, pbullet[b].x, pbullet[b].y, pbullet[b].x_speed, pbullet[b].y_speed, 20 + grand(10) + player.weapon_level, CLOUD_COL_1, 900);
#ifdef SHOW_PLAYER_DAMAGE
// don't bother testing for b here.
             player.wstrength [0] = (pbullet[b].dam * 25) / (player.recycle1 / player.recycle_speed1);
#endif
    }
   }
   break;
  case PTYPE_HAND:
   if (key3)
   {
    key1 = 1;
    key2 = 1;
   }
   if (key1 && !key2)
   {
    if (player.recycle1 <= 0)
    {
//     player.recoil = 9;
//     player.recoiling = 1;
     player.recycle1 = 1000;
     player.recycle_speed1 = 100;
     f = 0;
     angle = 0;
     switch(player.arc_firing)
     {
      default:
      case 0:
       player.arc_firing = 1;
       break;
      case 1:
       angle = ANGLE_32;
       player.arc_firing = 2;
       break;
      case 2:
       angle = ANGLE_16;
       player.arc_firing = 0;
       break;
     }
      player.recoil = 18;
     play_effectwfvx(WAV_SHOT2, 1350 - player.weapon_level * 10, 70, player.x);
     for (f = 0; f < 2; f ++)
     {
      b = create_pbullet(PBULLET_HSHOT);
      if (b != -1)
      {
       pbullet[b].x = player.x - 8000;
       if (f == 1)
       {
        pbullet[b].x = player.x + 8000;
        pbullet[b].x_speed = xpart(-ANGLE_4 + angle, 6500);
        pbullet[b].y_speed = ypart(-ANGLE_4 + angle, 6500);
        pbullet[b].angle = -ANGLE_4 + angle;
       }
        else
        {
          pbullet[b].x = player.x - 8000;
          pbullet[b].x_speed = xpart(-ANGLE_4 - angle, 6500);
          pbullet[b].y_speed = ypart(-ANGLE_4 - angle, 6500);
          pbullet[b].angle = -ANGLE_4 - angle;
        }
       pbullet[b].y = player.y - 8000;
       pbullet[b].xsize = 2000;//9000 + player.wlevel [m] * 500;
       pbullet[b].ysize = 2000;//9000 + player.wlevel [m] * 500;
       pbullet[b].level = player.weapon_level;
       pbullet[b].dam = 300 + player.weapon_level * 30;
       pbullet[b].source = 0;

       quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, pbullet[b].x_speed, pbullet[b].y_speed, 10 + grand(10), CLOUD_COL_1, 0);
       quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, pbullet[b].x_speed, pbullet[b].y_speed, 10 + grand(10), CLOUD_COL_1, 600);

      }
     }
#ifdef SHOW_PLAYER_DAMAGE
// don't bother testing for b here.
             player.wstrength [0] = (pbullet[b].dam * 25) / (player.recycle1 / player.recycle_speed1);
#endif

//     quick_cloud(CLOUD_CIRCLE1, player.x - 8000, player.y - 22000, 0, -200, 10 + grand(10), CLOUD_COL_1, 990);
//     quick_cloud(CLOUD_CIRCLE1, player.x + 8000, player.y - 22000, 0, -200, 10 + grand(10), CLOUD_COL_1, 990);

/*     quick_cloud(CLOUD_CIRCLE4, player.x - 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x - 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x - 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x - 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x - 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x - 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);

     quick_cloud(CLOUD_CIRCLE4, player.x + 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x + 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x + 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x + 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x + 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
     quick_cloud(CLOUD_CIRCLE4, player.x + 8000, player.y - 22000, -1000 + grand(2000), -1000 - grand(2000), 10 + grand(25), CLOUD_COL_1, 950);
*/
    }
   }
//    else
//     player.arc_firing = 0;

/*   if (key2 && !key1)
   {
     if (player.hand_acquire == 1)
      player.hand_acquire = -1;
     if (player.hand_acquire == 0)
      player.hand_acquire = 2;
   }
    else
    {
     switch(player.hand_acquire)
     {
      case 2: player.hand_acquire = 1; break;
      case -1: player.hand_acquire = 0; break;
     }
    }*/

    // settings:
    //  1 - acquire on
    //  0 - off
    //  2 - on and player still holding key down
    // -1 - off and etc

//   if (player.hand_acquire > 0)
   {
    acquire_lock();
//    player.recycle1 = 1000;
   }

   if (key2)
   {
//    if (player.hand_phase != HAND_DOCK)
    {
        player.hand_phase = HAND_STOP;

    }

   }
    else
    {
     if (player.hand_phase == HAND_STOP)
      player.hand_phase = HAND_SEEK;
    }

   run_hand();

   break;
  case PTYPE_SQUID:
   if (key3)
   {
    key1 = 1;
    key2 = 1;
   }
   if (key1)
   {
    if (player.orb_phase != ORB_DOCK)
    {
    if (player.recycle1 <= 0)
    {
     play_effectwfvx(WAV_SHOT2, 600 - player.weapon_level * 15, 90, player.x);
     player.recycle1 = 1000;
     player.recycle_speed1 = 160;
     f = 0;
     if (player.firing == 0)
     {
      f = 1;
      player.firing = 1;
     }
      else
       player.firing = 0;
      b = create_pbullet(PBULLET_ORBSEEKER);
      if (b != -1)
      {
       pbullet[b].x = player.x - 16000;
       pbullet[b].y = player.y - 4000;
       pbullet[b].x_speed = -2000 - grand(8000);
       pbullet[b].angle = ANGLE_2;
       if (f == 1)
       {
        pbullet[b].x_speed = 2000 + grand(8000);
        pbullet[b].x = player.x + 16000;
        pbullet[b].angle = 0;
        quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, 1000, 0, 7 + grand(5), CLOUD_COL_3, 900);
        quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, 3000, 0, 10 + grand(5), CLOUD_COL_3, 900);
        quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, 5000, 0, 15 + grand(5), CLOUD_COL_3, 900);
       }
        else
        {
         quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, -1000, 0, 7 + grand(5), CLOUD_COL_3, 900);
         quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, -3000, 0, 10 + grand(5), CLOUD_COL_3, 900);
         quick_cloud(CLOUD_CIRCLE1, pbullet[b].x, pbullet[b].y, -5000, 0, 15 + grand(5), CLOUD_COL_3, 900);
        }
       pbullet[b].y_speed = 0;
       pbullet[b].xsize = 4000;// + player.weapon_level * 660;//9000 + player.wlevel [m] * 500;
       pbullet[b].ysize = 4000;// + player.weapon_level * 660;//9000 + player.wlevel [m] * 500;
       pbullet[b].level = player.weapon_level;
       pbullet[b].dam = 600 + player.weapon_level * 60;
       pbullet[b].source = 1;
       pbullet[b].timeout = 250;
       pbullet[b].persist = 1;
      }
     }
    }
     else // i.e. orb is docked
     if (player.recycle1 <= 0)
     {

      player.recycle1 = 1000;
      player.recycle_speed1 = 220;

      play_effectwfvx(WAV_SHOT2, 2000, 100, player.x);
      b = create_pbullet(PBULLET_ORBSHOT);
      if (b != -1)
      {
       angle = grand(32) + grand(32) - grand(32) - grand(32) - ANGLE_4;
       pbullet[b].x = player.x + xpart(angle, 18000);
       pbullet[b].y = player.y + ypart(angle, 18000);
       pbullet[b].x_speed = xpart(angle, 8000);
       pbullet[b].y_speed = ypart(angle, 8000);
       pbullet[b].xsize = 7000;
       pbullet[b].ysize = 7000;
       pbullet[b].angle = angle;
       pbullet[b].level = player.weapon_level;
       pbullet[b].dam = 300 + player.weapon_level * 25;
       pbullet[b].source = 0;
      }
     quick_cloud(CLOUD_CIRCLE4, pbullet[b].x, pbullet[b].y, pbullet[b].x_speed, pbullet[b].y_speed, 20 + grand(10) + player.weapon_level, CLOUD_COL_3, 900);
    }
   }
   if (key2 || key3)
   {
    switch(player.orb_phase)
    {
     case ORB_RETURN:
       if (key1 || key3)
        player.orb_phase = ORB_AWAY;
      break;
     case ORB_AWAY:
       if (!key1 && !key3)
        player.orb_phase = ORB_RETURN;
      break;
     case ORB_FLOAT:
      player.orb_phase = ORB_RETURN;
      if (key1 || key3)
       player.orb_phase = ORB_AWAY;
      player.orb_hold_key2 = 1;
      break;
     case ORB_FIRED:
      if (player.orb_hold_key2 == 0)
      {
       player.orb_phase = ORB_RETURN;
       if (key1 || key3)
        player.orb_phase = ORB_AWAY;
      }
      player.orb_hold_key2 = 1;
      break;
     case ORB_DOCK:
      if (player.orb_hold_key2 == 0)
      {
       player.orb_phase = ORB_FIRED;
       player.orb_y_speed = -10000;
       player.orb_x_speed = 0;
      }
      player.orb_hold_key2 = 1;
      break;

    }

   }
    else
    {
     player.orb_hold_key2 = 0;
     if (player.orb_phase == ORB_RETURN || player.orb_phase == ORB_AWAY)
      player.orb_phase = ORB_FLOAT;
    }
   run_orb();
   break;


 }


}


void run_hand(void)
{
 int i;
 int lowest = 0;
 int angle;

 for (i = 0; i < 6; i ++)
 {
  if (player.lock [i] != -1)
  {
   if (enemy[player.lock [i]].type == ENEMY_NONE)
   {
    player.lock [i] = -1;
    continue;
   }
//   if (i < lowest)
//    lowest = i;
   player.lock_size [i] += 2;
   if (player.lock_size [i] > eclass[enemy[player.lock [i]].type].xsize >> 9)
    player.lock_size [i] = eclass[enemy[player.lock [i]].type].xsize >> 9;
  }

 }


 if (player.hand_phase == HAND_SEEK)
 {
  if (player.lock [0] == -1)
  {
   pack_locks();
  }
  if (player.lock [0] == -1)
  {
           player.hand_phase = HAND_RETURN;
  }
 }

   int size = player.hand_unfold >> 2;
   if (size > 4) size = 4;
//   if ((arena.counter & 1) && size > 0)
   if (player.hand_phase != HAND_DOCK && size > 0)
   {
    for (i = 0; i < 3; i ++)
    {
//      quick_cloud(CLOUD_3FCIRCLE2, player.hand_x + xpart(player.hand_angle + ANGLE_3 * i, (((player.hand_unfold >> 1) + 2) << 10)), player.hand_y + ypart(player.hand_angle + ANGLE_3 * i, (((player.hand_unfold >> 1) + 2) << 10)), 0, 0, 10 + grand(5) + grand(player.weapon_level), CLOUD_COL_3, 0);
      quick_cloud(CLOUD_3FCIRCLE2, player.hand_x + xpart(player.hand_angle + ANGLE_3 * i, (((player.hand_unfold >> 1) + 2) << 10)), player.hand_y + ypart(player.hand_angle + ANGLE_3 * i, (((player.hand_unfold >> 1) + 2) << 10)), grand(500) - grand(500), grand(500) - grand(500), 10 + grand(5) + grand(player.weapon_level), CLOUD_COL_3, 0);
    }
   }// remember to change also in draw_hand!

   if (player.hand_phase != HAND_DOCK && arena.counter & 1)
    quick_cloud(CLOUD_3FCIRCLE2, player.hand_x, player.hand_y, player.hand_x_speed + grand(2000) - grand(2000), player.hand_y_speed + grand(2000) - grand(2000), 12 + grand(5) + (player.weapon_level), CLOUD_COL_1, 940);

   if (player.hand_unfold > 30 + player.weapon_level)
   {
    int size = player.hand_unfold >> 3;
    if (size > 4) size = 4;

    if (size > 0)
    {
     angle = ANGLE_1 - player.hand_angle;
     for (i = 0; i < 3; i ++)
     {
       quick_cloud(CLOUD_3FCIRCLE2, player.hand_x + xpart(angle + ANGLE_3 * i, (((player.hand_unfold >> 2) + 3) << 10)), player.hand_y + ypart(angle + ANGLE_3 * i, (((player.hand_unfold >> 2) + 3) << 10)), 0, 0, 7 + grand(3) + grand(player.weapon_level), CLOUD_COL_4, 0);
     }
     if (arena.counter % 8 == 0)
      play_effectwfvx(WAV_ROBOT, 1000 + player.hand_unfold * 8, 20 + player.hand_unfold, player.hand_x);

    } // remember to change also in draw_hand!


   }

       player.grip_frame -= 2;
       if (player.grip_frame < 0)
        player.grip_frame = 0;

       player.recoil -= 2;
       if (player.recoil < 0)
        player.recoil = 0;

/*
   if (player.hand_phase != HAND_DOCK)
   {
       player.grip_frame -= 2;
       if (player.grip_frame < 0)
        player.grip_frame = 0;
   }
    else
    {
       player.grip_frame += 2;
       if (player.grip_frame > 20)
        player.grip_frame = 20;
    }
*/
     switch(player.hand_phase)
     {
      case HAND_DOCK:
       player.hand_x = player.x;
       player.hand_y = player.y + 12000;
       player.hand_x_speed = 0;
       player.hand_y_speed = 0;
       // also in input.c
/*       player.hand_unfold -= 4;
       if (player.hand_unfold < 0)
        player.hand_unfold = 0;*/

       player.grip_frame += 4;
       if (player.grip_frame > 20)
        player.grip_frame = 20;

       player.hand_unfold = 0;

//       player.hand_angle += (player.hand_x_speed >> 8) + (player.hand_y_speed >> 8);
//       player.hand_angle &= 1023;
       break;
      case HAND_SEEK:
       create_hand_bullet();
       if (lowest == 10)
       {
           player.hand_phase = HAND_RETURN;
           break;
       }
       angle = radians_to_angle(atan2(enemy[player.lock [lowest]].y - player.hand_y, enemy[player.lock [lowest]].x - player.hand_x));
       player.hand_x_speed += xpart(angle, 500);
       player.hand_y_speed += ypart(angle, 500);
       drag_hand(975);
       player.hand_x += player.hand_x_speed;
       player.hand_y += player.hand_y_speed;
       player.hand_unfold ++;
       if (player.hand_unfold > 30 + player.weapon_level)
        player.hand_unfold -= 3;
       player.hand_angle += (player.hand_x_speed >> 8) + (player.hand_y_speed >> 8);
       player.hand_angle &= 1023;
       break;
      case HAND_RETURN:
       create_hand_bullet();
       angle = radians_to_angle(atan2((player.y + 12000) - player.hand_y, player.x - player.hand_x));
       player.hand_x_speed += xpart(angle, 400);
       player.hand_y_speed += ypart(angle, 400);
       drag_hand(960);
       player.hand_x += player.hand_x_speed;
       player.hand_y += player.hand_y_speed;
       if (player.hand_x > player.x - 9000
        && player.hand_x < player.x + 9000
        && player.hand_y > player.y + 5000
        && player.hand_y < player.y + 16000)
        {
         player.hand_phase = HAND_DOCK;
        }
        player.hand_unfold --;
        if (player.hand_unfold < 0)
         player.hand_unfold = 0;
        player.hand_angle += (player.hand_x_speed >> 8) + (player.hand_y_speed >> 8);
        player.hand_angle &= 1023;
     break;
      case HAND_STOP:
       drag_hand(800);
       player.hand_x += player.hand_x_speed;
       player.hand_y += player.hand_y_speed;
       player.hand_unfold ++;
       if (player.hand_unfold > 60 + player.weapon_level * 2)
        player.hand_unfold = 60 + player.weapon_level * 2;
       player.hand_angle += (player.hand_x_speed >> 8) + (player.hand_y_speed >> 8);
       if (player.hand_x < 307200)
        player.hand_angle += 24;
         else
          player.hand_angle -= 24;
       player.hand_angle &= 1023;
       create_hand_bullet();
       break;
     }

//     player.hand_angle += xpart(player.hand_x_speed >> 4, 12) + xpart((player.hand_y_speed + ANGLE_4) >> 4, 12);
//     player.hand_angle += (player.hand_x_speed >> 8) + (player.hand_y_speed >> 8);
     //player.hand_angle += arena.counter * 4;
//     player.hand_angle += xpart(player.hand_x_speed, 6);
//     player.hand_angle &= 1023;

}

void create_hand_bullet(void)
{

 int b = create_pbullet(PBULLET_HAND);
 if (b != -1)
 {
  pbullet[b].x = player.hand_x;
  pbullet[b].y = player.hand_y;
  pbullet[b].x_speed = 0;
  pbullet[b].y_speed = 0;
  pbullet[b].xsize = 3000 + player.hand_unfold * 500;
  pbullet[b].ysize = 3000 + player.hand_unfold * 500;
  pbullet[b].dam = 90 + player.hand_unfold / 3;
  pbullet[b].source = 1;
  pbullet[b].level = player.weapon_level;
  if (player.hand_phase == HAND_STOP)
   pbullet[b].source = 2;
  pbullet[b].timeout = 2;
 }

}

void drag_hand(int drag)
{
 player.hand_x_speed *= drag;
 player.hand_x_speed >>= 10;
 player.hand_y_speed *= drag;
 player.hand_y_speed >>= 10;

}
// see also pbullet explode

void acquire_lock(void)
{
    int i, e;

    for (i = 0; i < 6; i ++)
    {
     if (player.lock [i] == -1)
      break;
     if (i == 5)
      return; // full
    }

    int x = player.x;
    int y = player.y - 140000;
    int etype;
    char not_this_one = 0;


 for (e = 0; e < NO_ENEMIES; e ++)
 {
  etype = enemy[e].type;
  if (etype <= ENEMY_NONE)
   continue;

  if (x >= enemy[e].x - eclass[etype].xsize + eclass[etype].xoffset - 40000
      && x <= enemy[e].x + eclass[etype].xsize + eclass[etype].xoffset + 40000
      && y >= enemy[e].y - eclass[etype].ysize + eclass[etype].yoffset - 40000
      && y <= enemy[e].y + eclass[etype].ysize + eclass[etype].yoffset + 40000)
    {
      pack_locks();
      for (i = 0; i < 6; i ++)
      {
          if (player.lock [i] == e)
           not_this_one = 1;
      }
      if (not_this_one == 1)
      {
       not_this_one = 0;
       continue;
      }
      for (i = 0; i < 6; i ++)
      {
       if (player.lock [i] == -1)
       {
        player.lock [i] = e;
        player.lock_size [i] = 1;
        play_effectwfvx(WAV_BLIP, 1000, 50, player.x);
        if (player.hand_phase == HAND_DOCK || player.hand_phase == HAND_RETURN)
         player.hand_phase = HAND_SEEK;
        return;
       }
      }

    }
 }

}

void pack_locks(void)
{
 int i, j;

 for (i = 0; i < 6; i ++)
 {
   if (player.lock [i] == -1)
   {
    j = i;
    do
    {
     if (player.lock [j] != -1)
     {
      player.lock [i] = player.lock [j];
      player.lock_size [i] = player.lock_size [j];
      player.lock [j] = -1;
      break;
     }
     j ++;
    } while (j < 6);
   }
 }

  // should never fail as acquire_lock checks for lock fullness
}



void run_orb(void)
{


 int angle;


   int size = player.orb_unfold >> 2;
   if (size > 4) size = 4;

       player.orb_frame -= 2;
       if (player.orb_frame < 0)
        player.orb_frame = 0;

     if (player.orb_phase == ORB_DOCK)
     {
      player.field_angle += player.field_angle_inc;
      player.field_angle &= 1023;
      player.field_size ++;
      if (player.field_size > 20)
       player.field_size = 20;
      player.field_radius ++;
      if (player.field_radius > 30)
       player.field_radius = 30;
     }
      else
      {
       player.field_angle += player.field_angle_inc;
       player.field_size --;
       if (player.field_size < 0)
        player.field_size = 0;
       player.field_radius --;
       if (player.field_radius < 0)
        player.field_radius = 0;
      }

     create_orb_field();

     switch(player.orb_phase)
     {

      case ORB_DOCK:
       player.orb_x = player.x;
       player.orb_y = player.y - 18000;
       player.orb_x_speed = 0;
       player.orb_y_speed = 0;

        player.orb_unfold --;
        if (player.orb_unfold < 0)
         player.orb_unfold = 0;

       break;
      case ORB_RETURN:
//       create_hand_bullet();
       angle = radians_to_angle(atan2((player.y - 18000) - player.orb_y, player.x - player.orb_x));
       player.orb_x_speed += xpart(angle, 400);
       player.orb_y_speed += ypart(angle, 400);
       drag_orb(970);
       player.orb_x += player.orb_x_speed;
       player.orb_y += player.orb_y_speed;
       if (player.orb_x > player.x - 12000
        && player.orb_x < player.x + 12000
        && player.orb_y < player.y - 12000
        && player.orb_y > player.y - 25000)
        {
         player.orb_phase = ORB_DOCK;
         player.field_angle_inc *= -1;
        }
       player.orb_unfold += 4;
       if (player.orb_unfold > 40)
        player.orb_unfold = 40;
     break;
      case ORB_AWAY:
//       create_hand_bullet();
       angle = radians_to_angle(atan2((player.y - 18000) - player.orb_y, player.x - player.orb_x));
       player.orb_x_speed -= xpart(angle, 400);
       player.orb_y_speed -= ypart(angle, 400);
       drag_orb(970);
       player.orb_x += player.orb_x_speed;
       player.orb_y += player.orb_y_speed;
       if (player.orb_x > player.x - 12000
        && player.orb_x < player.x + 12000
        && player.orb_y < player.y - 12000
        && player.orb_y > player.y - 25000)
        {
         player.orb_phase = ORB_DOCK;
         player.field_angle_inc *= -1;
        }
        player.orb_unfold --;
        if (player.orb_unfold < 0)
         player.orb_unfold = 0;
       if (player.orb_x < X_MIN)
       {
        player.orb_x_speed += 1000;
//        drag_pbullet(b, 800);
       }
       if (player.orb_y < Y_MIN)
       {
        player.orb_y_speed += 1000;
//        drag_pbullet(800);
       }
       if (player.orb_x > X_MAX)
       {
        player.orb_x_speed -= 1000;
//        drag_pbullet(800);
       }
       if (player.orb_y > Y_MAX)
       {
        player.orb_y_speed -= 1000;
//        drag_pbullet(800);
       }
        player.orb_x_speed += xpart(arena.counter * 16, 90);
        player.orb_y_speed += ypart(arena.counter * 16, 90);
     break;
/*      case ORB_LOCKED:
       if (player.orb_lock == -1)
       {
        player.orb_phase = ORB_FLOAT;
        break;
       }
       player.orb_unfold ++;
       if (player.orb_unfold > 60 + player.weapon_level * 2)
        player.orb_unfold = 60 + player.weapon_level * 2;
       break;*/
      case ORB_FLOAT:
       drag_orb(900);
       player.orb_x += player.orb_x_speed;
       player.orb_y += player.orb_y_speed;
       if (player.orb_x < 1000)
        player.orb_x_speed += 100;
       if (player.orb_y < 1000)
        player.orb_y_speed += 100;
       if (player.orb_x > 600000)
        player.orb_x_speed -= 100;
       if (player.orb_y > 600000)
        player.orb_y_speed -= 100;
       if (player.orb_x > player.x - 12000
        && player.orb_x < player.x + 12000
        && player.orb_y < player.y - 12000
        && player.orb_y > player.y - 25000)
        {
         player.orb_phase = ORB_DOCK;
         player.field_angle_inc *= -1;
        }
        player.orb_x_speed += xpart(arena.counter * 16, 90);
        player.orb_y_speed += ypart(arena.counter * 16, 90);
        player.orb_unfold ++;
        if (player.orb_unfold > 40)
         player.orb_unfold = 40;
       break;
      case ORB_FIRED:
       player.orb_x += player.orb_x_speed;
       player.orb_y += player.orb_y_speed;
       if (player.orb_y < 120000)
        player.orb_phase = ORB_FLOAT;
        player.orb_unfold ++;
        if (player.orb_unfold > 40)
         player.orb_unfold = 40;
       break;
     }


       create_orb_bullet();

}


void drag_orb(int drag)
{
 player.orb_x_speed *= drag;
 player.orb_x_speed >>= 10;
 player.orb_y_speed *= drag;
 player.orb_y_speed >>= 10;

}

void create_orb_bullet(void)
{

 int b = create_pbullet(PBULLET_ORB);
 if (b != -1)
 {
  pbullet[b].x = player.orb_x;
  pbullet[b].y = player.orb_y;
  pbullet[b].x_speed = 0;
  pbullet[b].y_speed = 0;
  pbullet[b].xsize = 4000 + player.orb_unfold * 1000;
  pbullet[b].ysize = 4000 + player.orb_unfold * 1000;
  pbullet[b].dam = 20 + player.orb_unfold / 10 + player.weapon_level * 6;
  pbullet[b].source = 2;
  pbullet[b].level = player.weapon_level;
  pbullet[b].timeout = 2;
 }

}

void create_orb_field(void)
{

  if (player.field_size < 1)
   return;

  int b, angle, dist, i;

  int dam = (player.field_size >> 1) + player.weapon_level;

     for (i = 0; i < 7; i ++)
     {
      angle = player.field_angle + i * ANGLE_7;
      dist = player.field_radius + 30;
      b = create_pbullet(PBULLET_ORBFIELD);
      if (b != -1)
      {
        pbullet[b].x = player.x + xpart(angle, dist << 10);
        pbullet[b].y = player.y - 18000 + ypart(angle, dist << 10);
        pbullet[b].x_speed = 0;
        pbullet[b].y_speed = 0;
        pbullet[b].xsize = 4000 + (player.field_size << 10);
        pbullet[b].ysize = pbullet[b].xsize;
        if (arena.counter & 1)
         pbullet[b].dam = dam;
        pbullet[b].source = 2;
        pbullet[b].level = player.weapon_level;
        pbullet[b].timeout = 2;
        pbullet[b].target = CLOUD_COL_3;
        pbullet[b].status = player.field_size / 3;
      }
      if (player.field_size > 7)
      {
       b = create_pbullet(PBULLET_ORBFIELD);
       if (b != -1)
       {
        pbullet[b].x = player.x + xpart(angle, (dist << 10) - 3000);
        pbullet[b].y = player.y - 18000 + ypart(angle, (dist << 10) - 3000);
        pbullet[b].x_speed = 0;
        pbullet[b].y_speed = 0;
        pbullet[b].xsize = 4000 + ((player.field_size - 10) << 10);
        pbullet[b].ysize = pbullet[b].xsize;
        pbullet[b].dam = 0;//dam >> 1;
        pbullet[b].source = 2;
        pbullet[b].level = player.weapon_level;
        pbullet[b].timeout = 2;
        pbullet[b].target = CLOUD_COL_3;
        pbullet[b].status = (player.field_size - 7) / 3;
       }
      }

      if (player.field_size > 10)
      {
       b = create_pbullet(PBULLET_ORBFIELD);
       if (b != -1)
       {
        pbullet[b].x = player.x + xpart(angle + 16, (dist << 10));
        pbullet[b].y = player.y - 18000 + ypart(angle + 16, (dist << 10));
        pbullet[b].x_speed = 0;
        pbullet[b].y_speed = 0;
        pbullet[b].xsize = 4000 + ((player.field_size - 10) << 10);
        pbullet[b].ysize = pbullet[b].xsize;
        pbullet[b].dam = 0;
        pbullet[b].source = 2;
        pbullet[b].level = player.weapon_level;
        pbullet[b].timeout = 2;
        pbullet[b].target = CLOUD_COL_3;
        pbullet[b].status = (player.field_size - 10) / 3;
       }

       b = create_pbullet(PBULLET_ORBFIELD);
       if (b != -1)
       {
        pbullet[b].x = player.x + xpart(angle - 16, (dist << 10));
        pbullet[b].y = player.y - 18000 + ypart(angle - 16, (dist << 10));
        pbullet[b].x_speed = 0;
        pbullet[b].y_speed = 0;
        pbullet[b].xsize = 4000 + ((player.field_size - 10) << 10);
        pbullet[b].ysize = pbullet[b].xsize;
        pbullet[b].dam = 0;
        pbullet[b].source = 2;
        pbullet[b].level = player.weapon_level;
        pbullet[b].timeout = 2;
        pbullet[b].target = CLOUD_COL_3;
        pbullet[b].status = (player.field_size - 10) / 3;
       }

      }


     }




     for (i = 0; i < 5; i ++)
     {
      angle = ANGLE_1 - (player.field_angle + i * ANGLE_5);
      dist = player.field_radius;
      if (player.field_size > 7)
      {
       b = create_pbullet(PBULLET_ORBFIELD);
       if (b != -1)
       {
        pbullet[b].x = player.x + xpart(angle, (dist << 10));
        pbullet[b].y = player.y - 18000 + ypart(angle, (dist << 10));
        pbullet[b].x_speed = 0;
        pbullet[b].y_speed = 0;
        pbullet[b].xsize = 4000 + ((player.field_size - 10) << 10);
        pbullet[b].ysize = pbullet[b].xsize;
        if (arena.counter & 3)
         pbullet[b].dam = dam;
        pbullet[b].source = 2;
        pbullet[b].level = player.weapon_level;
        pbullet[b].timeout = 2;
        pbullet[b].target = CLOUD_COL_4;
        pbullet[b].status = (player.field_size - 7) / 2;
       }
      if (player.field_size > 7)
      {
       b = create_pbullet(PBULLET_ORBFIELD);
       if (b != -1)
       {
        pbullet[b].x = player.x + xpart(angle, (dist << 10) + 3000);
        pbullet[b].y = player.y - 18000 + ypart(angle, (dist << 10) + 3000);
        pbullet[b].x_speed = 0;
        pbullet[b].y_speed = 0;
        pbullet[b].xsize = 4000 + ((player.field_size - 10) << 10);
        pbullet[b].ysize = pbullet[b].xsize;
        pbullet[b].dam = 0;//dam >> 1;
        pbullet[b].source = 2;
        pbullet[b].level = player.weapon_level;
        pbullet[b].timeout = 2;
        pbullet[b].target = CLOUD_COL_4;
        pbullet[b].status = (player.field_size - 7) / 3;
       }
      }


     }
     }



}


void multi_fire(int m)
{
/*
 if (player.weapon [m] == WPN_BLADE)
 {
#ifdef SHOW_DAMAGE
   calc_damage(100 + player.wlevel [player.blade_index] * 22, 1000);
#endif

   if (player.swing_state == BLADE_LEFT)
   {
//    if (player.recycle > 0)
//     return;
    player.swing_state = BLADE_CW_SLASH;
    player.swing_hold = 1;
    play_effectwfvx(WAV_WHOOSH, 600, 120, player.x);
    return;
   }
   if (player.swing_state == BLADE_RIGHT)
   {
//    if (player.recycle > 0)
//     return;
    player.swing_state = BLADE_ACW_SLASH;
    player.swing_hold = 1;
    play_effectwfvx(WAV_WHOOSH, 600, 120, player.x);
    return;
   }

  return;
 }

 if (player.recycle [m] > 0)
  return;

 int angle, b;

 switch(player.weapon [m])
 {
  case WPN_RECT:
   b = create_pbullet(PBULLET_RECT);
   if (b != -1)
   {
    pbullet[b].x = player.wx [m];
    pbullet[b].y = player.wy [m] - 4000;
    pbullet[b].x_speed = 0;
    pbullet[b].y_speed = - 5500;
    pbullet[b].xsize = 9000 + player.wlevel [m] * 500;
    pbullet[b].ysize = 9000 + player.wlevel [m] * 500;
    pbullet[b].level = player.wlevel [m];
    pbullet[b].dam = 10;
    pbullet[b].source = m;
    mflash(b, player.weapon [m], m);
   }
   player.recycle [m] = 1000;
   player.recycle_speed [m] = 15;
#ifdef SHOW_DAMAGE
   calc_damage(pbullet[b].dam, player.recycle_speed [m]);
#endif
   player.wfired = m;
   play_effectwfvx(WAV_RECT2, 800, 100, player.x);
   break;
  case WPN_CANNON:
   b = create_pbullet(PBULLET_CANNON);
   if (b != -1)
   {
    pbullet[b].x = player.wx [m];
    pbullet[b].y = player.wy [m] - 4000;
    pbullet[b].x_speed = 0;
    pbullet[b].y_speed = - 7000;
    pbullet[b].xsize = 9000 + player.wlevel [m] * 500;
    pbullet[b].ysize = 9000 + player.wlevel [m] * 500;
    pbullet[b].level = player.wlevel [m];
    pbullet[b].dam = 2000 + player.wlevel [m] * 360;
    pbullet[b].source = m;
    mflash(b, player.weapon [m], m);
   }
   player.recycle [m] = 1000;
   player.recycle_speed [m] = 15;
#ifdef SHOW_DAMAGE
   calc_damage(pbullet[b].dam, player.recycle_speed [m]);
#endif
   player.wfired = m;
//   player.wframe_speed = 10;
//   player.wframe_max = 39;
   play_effectwfvx(WAV_CANNON, 1300, 250, player.x);
   break;
  case WPN_EIGHT:
   if (player.ring_hold == 0)
   {
    if (player.move_x == -1)
     player.ring_angle = 0;
    if (player.move_x == 1)
     player.ring_angle = ANGLE_2;
    if (player.move_y == -1)
     player.ring_angle = ANGLE_4;
    if (player.move_y == 1)
     player.ring_angle = ANGLE_2 + ANGLE_4;
    if (player.move_x == -1 && player.move_y == -1)
     player.ring_angle = ANGLE_8;
    if (player.move_x == 1 && player.move_y == -1)
     player.ring_angle = ANGLE_4 + ANGLE_8;
    if (player.move_x == -1 && player.move_y == 1)
     player.ring_angle = ANGLE_2 + ANGLE_4 + ANGLE_8;
    if (player.move_x == 1 && player.move_y == 1)
     player.ring_angle = ANGLE_2 + ANGLE_8;
   }
   b = create_pbullet(PBULLET_RING);
   if (b != -1)
   {
    pbullet[b].x_speed = xpart(player.ring_angle, 6000);
    pbullet[b].y_speed = ypart(player.ring_angle, 6000);
    pbullet[b].x = player.wx [m] - pbullet[b].x_speed;
    pbullet[b].y = player.wy [m] - pbullet[b].y_speed;
    pbullet[b].xsize = 10000; // need to set this better!
    pbullet[b].ysize = 10000;
    pbullet[b].status = 45;
    pbullet[b].level = player.wlevel [m];
    pbullet[b].dam = 120 + player.wlevel [m] * 11;
    pbullet[b].source = m;
    mflash(b, player.weapon [m], m);
   }
   player.recycle [m] = 1000;
   player.recycle_speed [m] = 70 + player.wlevel [m] * 6;
#ifdef SHOW_DAMAGE
   calc_damage(pbullet[b].dam, player.recycle_speed [m]);
#endif
   player.wfired = m;
   player.ring_hold = 1;
   play_effectwfvx(WAV_MOUTH, 1250 - player.wlevel [m] * 70, 90, player.x);
   break;
  case WPN_MGUN:
   b = create_pbullet(PBULLET_MGUN);
   if (b != -1)
   {
    pbullet[b].x = player.wx [m] - 4000;
    pbullet[b].y = player.wy [m] - 13000 - player.wlevel [m] * 1000;
    pbullet[b].x_speed = 0;
    pbullet[b].y_speed = - 14000;
    pbullet[b].xsize = 2000;
    pbullet[b].ysize = 15000;
    pbullet[b].dam = 70 + player.wlevel [m] * 5;
    pbullet[b].source = m;
    pbullet[b].level = player.wlevel [m];
    mflash(b, WPN_MGUN, m);
   }

   b = create_pbullet(PBULLET_MGUN);
   if (b != -1)
   {
    pbullet[b].x = player.wx [m] + 4000;
    pbullet[b].y = player.wy [m] - 13000 - player.wlevel [m] * 1000;
    pbullet[b].x_speed = 0;
    pbullet[b].y_speed = - 14000;
    pbullet[b].xsize = 2000;
    pbullet[b].ysize = 15000;
    pbullet[b].dam = 70 + player.wlevel [m] * 5;
    pbullet[b].source = m;
    pbullet[b].level = player.wlevel [m];
    mflash(b, WPN_MGUN, m);
   }
   player.recycle [m] = 1000;
   player.recycle_speed [m] = 70 + player.wlevel [m] * 7;
#ifdef SHOW_DAMAGE
   calc_damage(pbullet[b].dam * 2, player.recycle_speed [m]);
#endif
   player.wfired = m;
   play_effectwfvx(WAV_MGUN, 800, 60, player.x);
   break;
  case WPN_LWBEAM:
   mflash(b, player.weapon [m], m);
   player.beam_fade [m] = 10;
   player.beam_counter [m] ++;
   player.beam_open [m] ++;
   if (player.beam_open [m] > 8)
    player.beam_open [m] = 8;
   if (player.beam_counter [m] % 15 == 1)
    play_effectwfvx(WAV_LWBEAM, 1000, 150, player.x);
   if (player.wstrength [m] == 2 && arena.counter % 2 == 0)
   {
    player.beam_counter [m] --;
    player.beam_flicker [m] = 2;
    break;
   }
   if (player.wstrength [m] == 3 && arena.counter % 3 != 0)
   {
    player.beam_flicker [m] = 2;
    player.beam_counter [m] --;
    break;
   }
//   if (arena.counter % 6 != 0)
//    break;
   player.beam_flicker [m] = 0;
   player.beam [m] = m;
   beam_collision(m);
   player.wfired = m;
#ifdef SHOW_DAMAGE
   calc_damage(60 + player.wlevel [m] * 15, 100);
// see lwbeam function in pbullet.c for damage
#endif
   break;
  case WPN_SWBEAM:
   player.swbeam [m] = 10 + player.wlevel [m];
   player.swbeam_level [m] = player.wlevel [m];
   player.swbeam_x1 [m] = player.wx [m];
   player.swbeam_y1 [m] = player.wy [m];
   mflash(b, player.weapon [m], m);
   swbeam_collision(m);
   player.recycle [m] = 1000;
   player.recycle_speed [m] = 15;
   player.swbeam_counter [m] = 50;
   player.wfired = m;
   play_effectwfvx(WAV_SWBEAM, 1000, 200, player.x);
#ifdef SHOW_DAMAGE
   calc_damage(1400 + player.wlevel [m] * 240, player.recycle_speed [m]);
// see swbeam function in pbullet.c for damage
#endif
   break;
  case WPN_SEEKER:
   fire_seekers(m);
   player.recycle [m] = 1000;
   player.recycle_speed [m] = 17;
   player.wfired = m;
#ifdef SHOW_DAMAGE
   calc_damage(pbullet[0].dam * (3 + player.wlevel [m]), player.recycle_speed [m]);
// see lwbeam function in pbullet.c for damage
#endif
   play_effectwfvx(WAV_WORMS, 2500 - player.wlevel [m] * 100, 90, player.x);
   break;
  case WPN_SCATTER:
   b = create_pbullet(PBULLET_SCATTER);
   if (b != -1)
   {
    pbullet[b].x = player.wx [m];
    pbullet[b].y = player.wy [m] - 4000;
    angle = -ANGLE_4 + grand(ANGLE_16) - grand(ANGLE_16);
    pbullet[b].angle = angle;
    pbullet[b].x_speed = xpart(angle, 9000);
    pbullet[b].y_speed = ypart(angle, 9000);
    pbullet[b].xsize = 1000;
    pbullet[b].ysize = 1000;
    pbullet[b].level = player.wlevel [m];
    pbullet[b].dam = 50 + player.wlevel [m] * 6;
    pbullet[b].source = m;
    mflash(b, player.weapon [m], m);
   }
   player.recycle [m] = 1000;
   player.recycle_speed [m] = 220 + player.wlevel [m] * 16;
#ifdef SHOW_DAMAGE
   calc_damage(pbullet[b].dam, player.recycle_speed [m]);
#endif
   player.wfired = m;
   play_effectwfvx(WAV_SCATTER, 800, 50, player.x);
   break;
  case WPN_BLADE:
   break;

 }

*/
}

#ifdef SHOW_DAMAGE
void calc_damage(int dam, int speed)
{
 damage_per_time = (dam * speed) / 100;

}
#endif

void fire_seekers(int m, int num)
{

//   if (num == 0)
//    return;
     int i;

 int angles [13] = {ANGLE_2, ANGLE_2 + ANGLE_16, ANGLE_2 - ANGLE_16, ANGLE_2 + ANGLE_8, ANGLE_2 - ANGLE_8,
               ANGLE_2 + ANGLE_8 + ANGLE_16, ANGLE_2 - ANGLE_8 - ANGLE_16,
               ANGLE_4, -ANGLE_4, ANGLE_4 - ANGLE_8, -ANGLE_4 + ANGLE_8, ANGLE_4 - ANGLE_16, -ANGLE_4 + ANGLE_16};

 int targets [TARGET_ARRAY_SIZE];

 int t;

 for (t = 0; t < TARGET_ARRAY_SIZE; t ++)
 {
  targets [t] = -1;
 }

 int number = num;// + player.wlevel [m];//player.charge / 100;
 if (number > 12)
  number = 12;

 int which_angle = 0;
  if (number % 2 == 0)
   which_angle = 1;

 find_enemies(targets, player.x, player.y, number, 0, 0);

 int b = -1, angle;

 for (i = 0; i < number; i ++)
 {
  b = create_pbullet(PBULLET_SEEKER1);
  if (b != -1)
  {
   pbullet[b].target = targets [i];
   angle = angles [which_angle] - ANGLE_4;
   pbullet[b].x = player.x + xpart(angle, 5000);
   pbullet[b].y = player.y - 15000 + ypart(angle, 5000);
//   quick_cloud(CLOUD_CIRCLE3, pbullet[b].x, pbullet[b].y, 0, 0, 25, TCOL_ORANGE, 0);
   pbullet[b].x_speed = xpart(angle, 5000);
   pbullet[b].y_speed = ypart(angle, 5000);
   pbullet[b].xsize = 2000;
   pbullet[b].ysize = 2000;
   pbullet[b].angle = angle;

   pbullet[b].dam = 500 + player.weapon_level * 40;// + player.wlevel [m] * 5;
       // if changing here, change in calculate damage bit as well!!

   pbullet[b].persist = 1;
   pbullet[b].timeout = 160;//220 + player.wlevel [m] * 7;
   pbullet[b].source = m;
   pbullet[b].level = player.weapon_level;

  }

  which_angle ++;


 }



}



int find_enemies(int targets [TARGET_ARRAY_SIZE], int x, int y, int number, int start_number, int force)
{

 int t;
 int targets_found = 0;
 int enemies_found = 0;

 int e;

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
  return 0;

 int closest = -1;
 int smallest_distance = 900000;

 for (t = start_number; t < TARGET_ARRAY_SIZE; t ++)
 {
  for (e = 0; e < NO_ENEMIES; e ++)
  {
   if (enemy[e].distance < smallest_distance)
   {
    closest = e;
    smallest_distance = enemy[e].distance;
   }
  }
  if (smallest_distance == 900000)
  {
   return targets_found;
  }
  targets [t] = closest;
  enemy[closest].distance = 900000;
  targets_found ++;
  closest = -1;
  smallest_distance = 900000;
 }

 return targets_found;
}


void mflash(int b, int w, int m)
{


 switch(w)
 {
/*   case WPN_EIGHT:
    xa = pbullet[b].x;
    ya = pbullet[b].y;
    quick_cloud(CLOUD_PRING, xa, ya, 0, 0, 135, 0, 0);
      break;*/

 }

}





/*

****************************************************

Joystick routines

****************************************************

*/

void init_joystick(void)
{

   options.joystick = 0;

   if (install_joystick(JOY_TYPE_AUTODETECT) != 0)
      return;

   if (num_joysticks == 0)
    return;

//   if (options.joy_analogue)
//    analogmode = TRUE;
//     else


   options.joystick = 1;

}


void jstick_calibrate(void)
{

   AL_CONST char *msg;

   if (num_joysticks == 0)
      return;


   while (joy[0].flags & JOYFLAG_CALIBRATE)
   {
      vsync();
      clear_bitmap(screen);

      msg = calibrate_joystick_name(0);

      textprintf_centre_ex(screen, font, 320, 170, -1, -1, msg);
      textprintf_centre_ex(screen, font, 320, 200, -1, -1, "AND PRESS SPACE");

      do
      {} while (key [KEY_SPACE] == 0);

      if (calibrate_joystick(0) != 0)
	   return;

   }

 return;

}














