#include "config.h"

#include "allegro.h"

#include <math.h>

#include "globvars.h"

#include "stuff.h"

#include "input.h"
#include "ebullet.h"
#include "pbullet.h"
#include "enemy.h"
#include "level.h"
#include "cloud.h"

#include "palette.h"
#include "display.h"

#include "sound.h"

void begin_game(void);

void run_player(void);
void init_player(void);
void init_player_on_respawn(void);
void clear_ship_data(void);
void new_level(void);
void make_blocks(void);
void run_blocks(void);
void run_blade(void);
void drag_blade(void);

extern volatile unsigned char ticked;
extern volatile int framecounter;
extern int slacktime;
extern volatile int long_slacktime_store;
extern volatile int long_slacktime;

unsigned char counter;

void run_stage3(void);
void make_part_row(int row);

//void make_platform(void);
//void run_platform(void);
void run_background(void);

void init_background(void);
//void init_background1(void);

void new_bcircle(int i, int j);
void new_panel(int i, int j);
void new_l3panel(int i, int j);

//void new_bar(int i, in

/*void fix_platform(int y1, int y2);
void fix_island1(int y1, int y2);
int check_tile_flow(int t1, int t2);
int get_tile(int x, int y);
void platform_hline(int x1, int y, int x2);
void platform_vline(int x, int y1, int y2);

void make_islands(void);
void run_islands(void);
int get_island1_tile(int x, int y);
void make_stars(void);
void run_stars(void);

void make_bones(void);
void run_bones(void);

void init_stage_3(void);
*/
char run_pause(void);
/*
extern int platform_tile [9] [PLAT_Y]; // in display
extern int platform_position; // in display
extern int platform_step; // in display

extern int island1_tile [9] [ISLAND1_Y]; // in display
extern int island1_position; // in display
extern int island1_step; // in display

// all stars in display
extern int star_x [NO_STARS] [3];
extern int star_y [NO_STARS] [3];
extern int star_y_speed [NO_STARS] [3];
//extern int star_col [NO_STARS] [3];
*/


extern int bcircle_x [3] [NO_BCIRCLES];
extern int bcircle_y [3] [NO_BCIRCLES];
extern int bcircle_size [3] [NO_BCIRCLES];
extern int bcircle_delta_size [3] [NO_BCIRCLES];
// defined in display.c

extern int panel_x [3] [NO_BCIRCLES];
extern int panel_y [3] [NO_BCIRCLES];
extern int panel_x_size [3] [NO_BCIRCLES];
extern int panel_y_size [3] [NO_BCIRCLES];

// only l3:
extern int panel_growth [3] [NO_BCIRCLES];

extern int river_x;
extern int river_x_speed;
// defined in display.c

void new_game(void)
{


 arena.game_over = 0;
 arena.level_finished = 0;

 arena.target_palette = PAL_BASE;
 arena.shift_palette = 2;
// arena.shift_palette = 0;

 init_player();

 init_beat();

// arena.underlay_position = 0; // need to put in 'new level' function

// arena.change_palette = BASECOL_L1_SPRING;
/* arena.shift_palette = 0;
 arena.shift_message = 300;
 arena.phase = 0;
 arena.phase_bonus = 0;*/
 arena.just_got_highscore = 0;

// arena.target_palette = BASECOL_L1_SPRING;

 init_pbullets();
 init_ebullets();
 init_enemies();
 init_clouds();

 new_level();

// set_base_palette();

 return;

}

void game_loop(void)
{
 int playing = 1;

 counter = 0;

 do
 {

 if (arena.game_over > 0)
 {
  arena.game_over --;
//  arena.target_palette = PAL_BASE;
  //arena.shift_palette = 2;
  if (arena.game_over == 1)
  {
   playing = 0;
   break;
  }
 }

 if (arena.level_finished > 0)
 {
  arena.level_finished --;
  if (arena.level_finished == 1)
  {
   if (arena.level == 3)
   {
    playing = 0;
    arena.level = 4;
    break;
   }
   arena.level ++;
   new_level();
//   clear_bitmap(screen);
//   rest(500);
//   ticked = 0;
  }
 }

 if (arena.new_level_sign > 0)
  arena.new_level_sign --;
   else
    run_beat();


 if (player.in_play == 1)
  get_input();
 run_clouds();
 if (player.in_play == 1)
  run_player();
 run_level();
 run_enemies();
 run_ebullets();
 run_pbullets();
 run_background();
 if (arena.shift_palette > 0 && arena.counter % 8 == 0)
 {
  run_palette_shift();
  build_new_palette(0, 0, 0, 0);
 }
// run_underlay();


  if (ticked == 0)
  {
   run_display(1);
   framecounter++;
  }
   else
    run_display(0); // just does what is needed for collision detection



  slacktime = 0;

  do
  {
   rest(1);
   slacktime ++;
  } while(ticked == 0);

  ticked --;

  long_slacktime += slacktime;

  counter ++;
  arena.counter ++;

  if (key [KEY_ESC])
  {
      if (run_pause() == 1)
       playing = 0;
  }

 } while(playing == 1);


 if (player.score > options.highscore [arena.difficulty])
 {
  arena.just_got_highscore = arena.difficulty + 1;
  options.highscore [arena.difficulty] = player.score;
  switch(arena.difficulty)
  {
   case 0:
    set_config_int("Scores", "Easy", player.score); break;
   case 1:
    set_config_int("Scores", "Normal", player.score); break;
   case 2:
    set_config_int("Scores", "Hard", player.score); break;

  }
 }

}

char run_pause(void)
{
 int pc = 0;

  display_pause(pc);

 do
 {
  pc ++;
  if (pc > 256)
   pc = 0;


   do
   {
       rest(1);
   } while(ticked == 0);
   ticked --;

  if (key [KEY_Y])
  {
//   clear_bitmap(screen);
//   vsync();
   return 1;
  }

  if (key [KEY_N])
   return 0;
 }
  while (TRUE);

 return 0;

}

void run_player(void)
{
    int i;

    if (player.grace > 0)
     player.grace --;

  if (player.respawning == 0)
  {
   for (i = 0; i < 3; i ++)
   {
     if (player.wx [i] > 600000)
      player.wx [i] = 600000;
     if (player.wy [i] > 600000)
      player.wy [i] = 600000;
     if (player.wx [i] < 0)
      player.wx [i] = 0;
     if (player.wy [i] < 0)
      player.wy [i] = 0;
     if (player.wflash [i] > 0)
      player.wflash [i] --;
     if (player.bflash [i] > 0)
      player.bflash [i] --;
   }
  }

  if (player.overbflash > 0)
   player.overbflash --;
 switch(player.type)
 {
    case PTYPE_BASIC:
     if (player.recycle1 > 0)
     {
      player.recycle1 -= player.recycle_speed1;
     }
      else
       player.recycle1 = 0;
     if (player.recoiling > 0)
     {
         player.recoil += 10;
         if (player.recoil > 20)
          player.recoiling = 0;
     }
      else
      {
          if (player.recoil > 0)
           player.recoil -= 7;
      }
     break;
    case PTYPE_ANENOME:
     player.blade_redness --;
     if (player.blade_redness < 0)
      player.blade_redness = 0;

     run_blade();
     if (player.recycle1 > 0)
     {
      player.recycle1 -= player.recycle_speed1;
     }
      else
       player.recycle1 = 0;

     if (player.recoiling > 0)
     {
         player.recoil += 10;
         if (player.recoil > 20)
          player.recoiling = 0;
     }
      else
      {
          if (player.recoil > 0)
           player.recoil -= 7;
      }
     break;
    case PTYPE_BOMBER:
     player.ring_angle += player.ring_speed;
     player.ring_angle &= 1023;
     player.ring_speed += 2;
     if (player.ring_speed > 8)
      player.ring_speed = 8;
     player.ring_size -= 1;
     if (player.ring_size < 27)
      player.ring_size = 27;
     if (player.recycle1 > 0)
     {
      player.recycle1 -= player.recycle_speed1;
     }
      else
       player.recycle1 = 0;
     if (player.beam_firing > 0)
      player.beam_firing --;
     if ((player.bomb_recycle1 > 5 && player.bomb_recycle1 <= 8)
      || (player.bomb_recycle2 > 5 && player.bomb_recycle2 <= 8))
        play_effectwfvx(WAV_BLIP, 600, 150, player.x);

     player.bomb_recycle1 -= 3; // and below
     if (player.bomb_recycle1 < 0)
     {
      player.bomb_recycle1 = 0;
      if (player.bomb_recycle2 < 5)
       player.bomb_recycle2 = 5;
     }
     player.bomb_recycle2 -= 3; // and above
     if (player.bomb_recycle2 < 0)
     {
      player.bomb_recycle2 = 0;
      if (player.bomb_recycle1 < 5)
       player.bomb_recycle1 = 5;
     }
     player.recycle2 --;
     if (player.recycle2 < 0)
      player.recycle2 = 0;
     break;
    case PTYPE_HAND:
     if (player.recycle1 > 0)
     {
      player.recycle1 -= player.recycle_speed1;
     }
      else
       player.recycle1 = 0;
    if (player.hand_phase == HAND_DOCK)
    {
       player.hand_x = player.x;
       player.hand_y = player.y + 12000;
    }
/*     switch(player.hand_phase)
     {
      case HAND_DOCK:
       player.hand_x = player.x;
       player.hand_y = player.y + 12000;
       break;

     }*/
     // hand is mostly run in input.c
     break;
    case PTYPE_SQUID:
     if (player.recycle1 > 0)
     {
      player.recycle1 -= player.recycle_speed1;
     }
      else
       player.recycle1 = 0;
    if (player.orb_phase == ORB_DOCK)
    {
       player.orb_x = player.x;
       player.orb_y = player.y - 16000;
       player.grip_frame -= 4;
       if (player.grip_frame < 0)
        player.grip_frame = 0;
    }
     else
     {
       player.grip_frame += 4;
       if (player.grip_frame > 25)
        player.grip_frame = 25;
     }
     break;
 }


}

#define BLADE_DISTANCE 45000
//#define BLADE_SPEED 48

#define BLADE_RETURN_SHRINK 5000
#define BLADE_GROW 3000
#define BLADE_DISTANCE_GROW 3000

void run_blade(void)
{
       int goal_angle;
       int old_x = player.blade_x;
       int old_y = player.blade_y;
       int xa = 0;
       int blade_speed = 48 - player.blade_redness;
       int max_size = 30000 + player.blade_redness * 1000;
//       int old_x = player.swing_x [player.swing_pos];
//       int old_y = player.swing_y [player.swing_pos];
//player.blade_distance = 30000;
//player.blade_size = 20000;

        int max_distance = (BLADE_DISTANCE * (20 + player.blade_redness)) / 20;

       switch(player.blade_pos)
       {
        case BLADE_LEFT:
         player.blade_angle = ANGLE_2 - ANGLE_16 - 8;
         player.blade_x = player.x + xpart(player.blade_angle, BLADE_DISTANCE);
         player.blade_y = player.y + ypart(player.blade_angle, BLADE_DISTANCE);
         break;
        case BLADE_RIGHT:
         goal_angle = ANGLE_16;
         player.blade_angle = ANGLE_16 + 8;
         player.blade_x = player.x + xpart(player.blade_angle, BLADE_DISTANCE);
         player.blade_y = player.y + ypart(player.blade_angle, BLADE_DISTANCE);
         break;

        case BLADE_RETURN_CW_RIGHT:
//         player.old_blade_length --;
         player.blade_angle += blade_speed;
         player.blade_angle &= 1023;
         player.blade_size -= BLADE_RETURN_SHRINK;
         if (player.blade_size <= 2000)
          player.blade_size = 2000;
         if (player.blade_angle >= ANGLE_8 && player.blade_angle < ANGLE_4)
         {
          player.blade_pos = BLADE_RIGHT;
         }
         break;

        case BLADE_RETURN_CW_LEFT:
//         player.old_blade_length --;
         player.blade_angle += blade_speed;
         player.blade_angle &= 1023;
         player.blade_size -= BLADE_RETURN_SHRINK;
         if (player.blade_size <= 2000)
          player.blade_size = 2000;
         if (player.blade_angle >= ANGLE_2 - ANGLE_16)
         {
          player.blade_pos = BLADE_LEFT;
         }
         break;

        case BLADE_RETURN_ACW_LEFT:
//         player.old_blade_length --;
         player.blade_angle -= blade_speed;
         player.blade_angle &= 1023;
         player.blade_size -= BLADE_RETURN_SHRINK;
         if (player.blade_size <= 2000)
          player.blade_size = 2000;
//         if (player.blade_angle <= ANGLE_2 - ANGLE_8 && player.blade_angle > ANGLE_4)
         if (player.blade_angle <= ANGLE_2 - ANGLE_8 && player.blade_angle > ANGLE_4)
         {
          player.blade_pos = BLADE_LEFT;
         }
         player.blade_x = player.x + xpart(player.blade_angle, BLADE_DISTANCE);
         player.blade_y = player.y + ypart(player.blade_angle, BLADE_DISTANCE);
         break;

        case BLADE_RETURN_ACW_RIGHT:
//         player.old_blade_length --;
         player.blade_angle -= blade_speed;
         player.blade_angle &= 1023;
         player.blade_size -= BLADE_RETURN_SHRINK;
         if (player.blade_size <= 2000)
          player.blade_size = 2000;
         if (player.blade_angle < ANGLE_16 || player.blade_angle > ANGLE_1 - ANGLE_8)
         {
          player.blade_pos = BLADE_RIGHT;
         }
         player.blade_x = player.x + xpart(player.blade_angle, BLADE_DISTANCE);
         player.blade_y = player.y + ypart(player.blade_angle, BLADE_DISTANCE);
         break;



        case BLADE_CW_SLASH:
/*         player.swing_length += 2;
         if (player.swing_length >= 9)
          player.swing_length = 9;*/
         player.blade_angle += blade_speed;
         player.blade_angle &= 1023;
         player.blade_size += BLADE_GROW;
         if (player.blade_size > max_size)
          player.blade_size = max_size;
         player.blade_distance += BLADE_DISTANCE_GROW;
         if (player.blade_distance > max_distance)
          player.blade_distance = max_distance;

         if (player.blade_angle >= ANGLE_1-ANGLE_8 || (player.blade_angle > 0 && player.blade_angle < ANGLE_4))
         {
            if (player.swing_hold == 1)
            {
             player.blade_pos = BLADE_CW_SWING;
            }
             else
             {
              player.blade_pos = BLADE_RETURN_CW_RIGHT;
             }
         }
         player.blade_x = player.x + xpart(player.blade_angle, BLADE_DISTANCE);
         player.blade_y = player.y + ypart(player.blade_angle, BLADE_DISTANCE);
         break;

        case BLADE_ACW_SLASH:
/*         player.swing_length += 2;
         if (player.swing_length >= 9)
          player.swing_length = 9;*/
         player.blade_angle -= blade_speed;
         player.blade_angle &= 1023;
         player.blade_size += BLADE_GROW;
         if (player.blade_size > max_size)
          player.blade_size = max_size;
         player.blade_distance += BLADE_DISTANCE_GROW;
         if (player.blade_distance > max_distance)
          player.blade_distance = max_distance;

         if (player.blade_angle <= ANGLE_2 + ANGLE_8 && player.blade_angle > ANGLE_4)
         {
            if (player.swing_hold == 1)
            {
             player.blade_pos = BLADE_ACW_SWING;
            }
             else
             {
              player.blade_pos = BLADE_RETURN_ACW_LEFT;
             }
         }
         player.blade_x = player.x + xpart(player.blade_angle, BLADE_DISTANCE);
         player.blade_y = player.y + ypart(player.blade_angle, BLADE_DISTANCE);
         break;

        case BLADE_CW_SWING:
/*         player.swing_length += 2;
         if (player.swing_length >= 9)
          player.swing_length = 9;*/
         player.blade_angle += blade_speed;
         player.blade_angle &= 1023;
         if (player.blade_angle >= ANGLE_2 - ANGLE_4 + 1 && player.blade_angle < ANGLE_1 - ANGLE_4)
         {
           if (player.swing_hold)
           {
            player.blade_pos = BLADE_CW_SLASH;
            play_effectwfvx(WAV_SWEEP, 1000 - player.blade_redness * 20, 170, player.x);
           }
             else
              player.blade_pos = BLADE_RETURN_CW_LEFT;
         }
         player.blade_x = player.x + xpart(player.blade_angle, BLADE_DISTANCE);
         player.blade_y = player.y + ypart(player.blade_angle, BLADE_DISTANCE);
         break;

        case BLADE_ACW_SWING:
/*         player.swing_length += 2;
         if (player.swing_length >= 9)
          player.swing_length = 9;*/
         player.blade_angle -= blade_speed;
         player.blade_angle &= 1023;
/*         if (player.blade_angle <= ANGLE_16 && player.blade_angle > 0)
         {
              player.blade_pos = BLADE_RIGHT;
         }*/
         if (player.blade_angle <= ANGLE_4 - 1 && player.blade_angle > 0)
         {
           if (player.swing_hold)
           {
            player.blade_pos = BLADE_ACW_SLASH;
            play_effectwfvx(WAV_SWEEP, 1000 - player.blade_redness * 20, 170, player.x);
           }
             else
              player.blade_pos = BLADE_RETURN_ACW_RIGHT;
         }
         player.blade_x = player.x + xpart(player.blade_angle, BLADE_DISTANCE);
         player.blade_y = player.y + ypart(player.blade_angle, BLADE_DISTANCE);
         break;


       }


         player.blade_x = player.x + xpart(player.blade_angle, player.blade_distance);
         player.blade_y = player.y + ypart(player.blade_angle, player.blade_distance);

//       player.swing_sprite_angle = radians_to_angle(atan2(player.wy [m] - player.y, player.wx [m] - player.x));
//       player.swing_sprite_angle /= (ANGLE_1 / SMALL_ROTATIONS);

        if (player.old_blade_length > 0)
         player.old_blade_length --;

        player.old_blade_pos ++;
        if (player.old_blade_pos == 10)
         player.old_blade_pos = 0;

//        player.old_blade_strength [player.old_blade_pos] = 5;

//        player.old_blade_x [player.old_blade_pos] = player.blade_x - xpart(player.blade_angle, (player.blade_size >> 2));
//        player.old_blade_y [player.old_blade_pos] = player.blade_y - ypart(player.blade_angle, (player.blade_size >> 2));

//        xa = player.blade_size >> 1; //grand(player.blade_size / 2);

        player.old_blade_x [player.old_blade_pos] = player.blade_x - xpart(player.blade_angle, xa);
        player.old_blade_y [player.old_blade_pos] = player.blade_y - ypart(player.blade_angle, xa);


 if (player.blade_pos == BLADE_LEFT)
  {
   player.old_blade_x [player.old_blade_pos] = player.x - 16000;
   player.old_blade_y [player.old_blade_pos] = player.y + 19000;
   return;
  }

 if (player.blade_pos == BLADE_RIGHT)
 {
   player.old_blade_x [player.old_blade_pos] = player.x + 16000;
   player.old_blade_y [player.old_blade_pos] = player.y + 19000;
   return;
 }

// int s = player.blade

 //quick_cloud(CLOUD_3FCIRCLE4, player.blade_x - xpart(player.blade_angle, xa), player.blade_y - ypart(player.blade_angle, xa), 0, 0, 10 + grand(20), 0, 950);

//  quick_cloud(CLOUD_3FCIRCLE4, player.blade_x - xpart(player.blade_angle, xa), player.blade_y - ypart(player.blade_angle, xa), old_x - player.blade_x, old_y - player.blade_y, 10 + grand(20), 0, 900);

  int cloud_col = 2;

  if (grand(15) < player.blade_redness)
   cloud_col = 0;

  if (player.blade_redness > 10 - grand(4) && player.blade_redness < 10 + grand(9) && grand(3) == 0)
   cloud_col = 3;

  xa = player.blade_size / 2;
  quick_cloud(CLOUD_3FCIRCLE2, player.blade_x + xpart(player.blade_angle, xa), player.blade_y + ypart(player.blade_angle, xa), player.blade_x - old_x, player.blade_y - old_y, 10 + grand(10 + player.weapon_level), cloud_col, 820);

  xa = grand(player.blade_size);
  quick_cloud(CLOUD_3FCIRCLE4, player.blade_x + xpart(player.blade_angle, xa), player.blade_y + ypart(player.blade_angle, xa), player.blade_x - old_x + grand(2000) - grand(2000), player.blade_y - old_y + grand(2000) - grand(2000), 10 + grand(20 + player.weapon_level), cloud_col, 820);
  xa = grand(player.blade_size);
  quick_cloud(CLOUD_3FCIRCLE4, player.blade_x + xpart(player.blade_angle, xa), player.blade_y + ypart(player.blade_angle, xa), player.blade_x - old_x + grand(2000) - grand(2000), player.blade_y - old_y + grand(2000) - grand(2000), 10 + grand(20 + player.weapon_level), cloud_col, 720);
  xa = grand(player.blade_size);
  quick_cloud(CLOUD_3FCIRCLE4, player.blade_x + xpart(player.blade_angle, xa), player.blade_y + ypart(player.blade_angle, xa), player.blade_x - old_x + grand(2000) - grand(2000), player.blade_y - old_y + grand(2000) - grand(2000), 10 + grand(20 + player.weapon_level), cloud_col, 620);

/*  xa = player.blade_size / 2;
  quick_cloud(CLOUD_3FCIRCLE2, player.blade_x - xpart(player.blade_angle, xa), player.blade_y - ypart(player.blade_angle, xa), player.blade_x - old_x, player.blade_y - old_y, 10 + grand(20), cloud_col, 820);

  xa = grand(player.blade_size);
  quick_cloud(CLOUD_3FCIRCLE4, player.blade_x - xpart(player.blade_angle, xa), player.blade_y - ypart(player.blade_angle, xa), player.blade_x - old_x + grand(2000) - grand(2000), player.blade_y - old_y + grand(2000) - grand(2000), 10 + grand(20), cloud_col, 820);
  xa = grand(player.blade_size);
  quick_cloud(CLOUD_3FCIRCLE4, player.blade_x - xpart(player.blade_angle, xa), player.blade_y - ypart(player.blade_angle, xa), player.blade_x - old_x + grand(2000) - grand(2000), player.blade_y - old_y + grand(2000) - grand(2000), 10 + grand(20), cloud_col, 720);
  xa = grand(player.blade_size);
  quick_cloud(CLOUD_3FCIRCLE4, player.blade_x - xpart(player.blade_angle, xa), player.blade_y - ypart(player.blade_angle, xa), player.blade_x - old_x + grand(2000) - grand(2000), player.blade_y - old_y + grand(2000) - grand(2000), 10 + grand(20), cloud_col, 620);
*/
/* int c = quick_cloud(CLOUD_BLADE_TRAIL, player.blade_x, player.blade_y, 0, 0, 15, 0, 0);
 if (c != -1)
 {
  cloud[c].x2 = old_x;
  cloud[c].y2 = old_y;
 }*/

        player.old_blade_length += 2;
        if (player.old_blade_length >= 9)
         player.old_blade_length = 9;

 int b = create_pbullet(PBULLET_BLADE);
 if (b != -1)
 {
  pbullet[b].x = player.blade_x + xpart(player.blade_angle, player.blade_size >> 1);// [player.swing_pos];
  pbullet[b].y = player.blade_y + ypart(player.blade_angle, player.blade_size >> 1);// [player.swing_pos];
//  pbullet[b].x_speed = player.swing_x [player.swing_pos] - old_x;
  //pbullet[b].y_speed = player.swing_y [player.swing_pos] - old_y;
  pbullet[b].xsize = player.blade_size;
  pbullet[b].ysize = player.blade_size;
//  pbullet[b].dam = ((300 - player.blade_redness * 9) * (10 + player.weapon_level)) / 10;// + player.wlevel [player.blade_index] * 19; // was 100/22 or 60/14
  pbullet[b].dam = ((175 - player.blade_redness * 5) * (10 + player.weapon_level)) / 10;// + player.wlevel [player.blade_index] * 19; // was 100/22 or 60/14
  if (player.blade_redness > 15)
   pbullet[b].source = 2;
    else
     pbullet[b].source = 1;
  pbullet[b].timeout = 2;
 }


/* if (player.wlevel [player.blade_index] > 6)
 {
  int bangle = radians_to_angle(atan2(old_y - player.swing_y [player.swing_pos], old_x - player.swing_x [player.swing_pos]));
  bangle -= ANGLE_4;
  quick_cloud(CLOUD_CIRCLE2, old_x + xpart(bangle, 11000), old_y + ypart(bangle, 11000), 0, 0, player.wlevel [player.blade_index] - 2, TCOL_ORANGE, 0);
  bangle += ANGLE_2;
  quick_cloud(CLOUD_CIRCLE2, old_x + xpart(bangle, 11000), old_y + ypart(bangle, 11000), 0, 0, player.wlevel [player.blade_index] - 2, TCOL_ORANGE, 0);
 }*/


}

void player_hit(void)
{

 int x = player.x;
 int y = player.y;


 play_effectwfvx(WAV_BANG2, tone [NOTE_0G], 250, x);
 play_effectwfvx(WAV_BANG3, tone [NOTE_2G], 250, x);
// play_effectwfvx(WAV_P_BANG, 800, 250, x);

 int i, angle, xa;

      for (i = 0; i < 5; i ++)
      {
       angle = grand(ANGLE_1);
       xa = 20000 + grand(4400);
       quick_cloud(CLOUD_CIRCLE4, x + xpart(angle, xa), y + ypart(angle, xa), xpart(angle, xa / 15), ypart(angle, xa / 15), 40 + grand(26), CLOUD_COL_1, 960);
      }
      quick_cloud(CLOUD_SMALL_SHOCK, x, y, 0, 0, 30, CLOUD_COL_1, 0);
      quick_cloud(CLOUD_CIRCLE1, x, y, 0, 0, 40 + grand(16), CLOUD_COL_1, 0);
      for (i = 0; i < 10; i ++)
      {
       angle = grand(ANGLE_1);
       xa = 2000 + grand(5400);
       quick_cloud(CLOUD_FCIRCLE2, x, y, xpart(angle, xa), ypart(angle, xa), 30 + grand(10), CLOUD_COL_1, 980);
       quick_cloud(CLOUD_FCIRCLE4, x, y, xpart(angle, xa - 1000), ypart(angle, xa - 1000), 30 + grand(10), CLOUD_COL_1, 980);
       quick_cloud(CLOUD_FCIRCLE4, x, y, xpart(angle, xa - 2000), ypart(angle, xa - 2000), 25 + grand(10), CLOUD_COL_1, 980);

       xa = 2000 + grand(10000);
       quick_cloud(CLOUD_CIRCLE4, x + xpart(angle, xa), y + ypart(angle, xa), 0, 0, 60 + grand(26), CLOUD_COL_1, 0);

      }


 if (player.lives == 0)
 {
  player.in_play = 0;
  arena.game_over = 200;
  arena.level_finished = 0;
  clear_ship_data();
 }
  else
  {
    player.lives --;
    if (player.lives > -1)
    {
        init_player_on_respawn();
    }
  }

}


void init_player(void)
{

 int i;

 init_player_on_respawn();

 player.lives = 2;//12;
 player.score = 0;
 player.grace = 0;
 player.in_play = 1;
 player.overbflash = 0;

 int weapon_bar = 0;

 if (arena.level == 2)
 {
  weapon_bar = options.stage_power_bar [player.type - 1] [0] [arena.difficulty];
 }

 if (arena.level == 3)
 {
  weapon_bar = options.stage_power_bar [player.type - 1] [1] [arena.difficulty];
 }

 for (i = 0; i < 3; i ++)
 {
  player.wxp [i] = weapon_bar;
  player.wlevel [i] = 0;
  player.wflash [i] = 0;
  player.bflash [i] = 0;
  player.wstatus [i] = 0;
  player.wpixels [i] = 0;
  player.wgained [i] = 0;
  player.recycle [i] = 0;
//  player.recycle_speed [i] = 0;
  player.wstrength [i] = 0;
  player.rpixels [i] = 50;

 }

 player.weapon_level = 0;

 if (arena.level == 2)
 {
  player.wlevel [0] = options.stage_power [player.type - 1] [0] [arena.difficulty];
  player.wlevel [1] = options.stage_power [player.type - 1] [0] [arena.difficulty];
  player.wlevel [2] = options.stage_power [player.type - 1] [0] [arena.difficulty];
  player.weapon_level = options.stage_power [player.type - 1] [0] [arena.difficulty];
 }

 if (arena.level == 3)
 {
  player.wlevel [0] = options.stage_power [player.type - 1] [1] [arena.difficulty];
  player.wlevel [1] = options.stage_power [player.type - 1] [1] [arena.difficulty];
  player.wlevel [2] = options.stage_power [player.type - 1] [1] [arena.difficulty];
  player.weapon_level = options.stage_power [player.type - 1] [1] [arena.difficulty];
 }

 set_wpixels();



}

void init_player_on_respawn(void)
{



 player.respawning = 50;
 player.grace = 180;

 player.x = 300000;
 player.y = 650000;

 player.firing = 0;
// player.swing_state = BLADE_LEFT;
// player.swing_pos = 0;
// player.swing_length = 0;

 player.wfired = -1;
 player.wframe = 0;
 player.wframe_speed = 0;
 player.wframe_max = 0;

/* player.ring_angle = -ANGLE_4;
 player.ring_hold = 0;*/
 player.move_x = 0;
 player.move_y = 0;

 player.recycle1 = 1000;
 player.recycle2 = 1000;
 player.recycle3 = 1000;

 player.recycle_speed1 = 100;
 player.recycle_speed2 = 100;
 player.recycle_speed3 = 100;

 player.hold_key1 = 0;
 player.hold_key2 = 0;

 clear_ship_data();

}

void clear_ship_data(void)
{

 int i;

// ship 1
 player.recoil = 0;
 player.recoiling = 0;
 player.charge = 0;

// anenome
 player.blade_pos = BLADE_LEFT;
 player.blade_angle = 0;
 player.blade_distance = 0;
 player.blade_size = 0;
 player.old_blade_pos = 0;
 player.old_blade_length = 0;
 player.blade_redness = 0;

// bomber
 player.ring_angle = 0;
 player.ring_speed = 0;
 player.orb_firing = 0;
 player.beam_y [0] = -100000;
 player.beam_y [1] = -100000;
 player.beam_y [2] = -100000;
 player.bomb_recycle1 = 0;
 player.bomb_recycle2 = 50;
 if (player.type == PTYPE_BOMBER)
  player.recycle2 = 0;

// hand
 player.hand_x = 0;
 player.hand_y = 0;
 player.hand_x_speed = 0;
 player.hand_y_speed = 0;
 for (i = 0; i < 6; i ++)
 {
  player.lock [i] = -1;
  player.lock_size [i] = 0;
 }
 player.hand_stop = 0;
 player.hand_phase = HAND_DOCK;
 player.arc_firing = 0;
 player.hand_unfold = 0;
 player.hand_angle = 0;
 player.hand_acquire = 1;
 player.grip_frame = 20;
 player.recoil = 20;

// squid
  player.orb_hold_key2 = 1;
  player.orb_phase = ORB_DOCK;
  player.field_angle = 0;
  player.field_angle_inc = 8;
  player.field_size = 0;
  player.field_radius = 0;
  player.orb_unfold = 0;

}



/*
#define BLADE_DISTANCE 45000
#define BLADE_SPEED 48

void run_blade(void)
{


       int goal_angle;
       int old_x = player.swing_x [player.swing_pos];
       int old_y = player.swing_y [player.swing_pos];

       switch(player.blade_pos)
       {
        case BLADE_LEFT:
         player.blade_angle = ANGLE_2 - ANGLE_16 - 8;
         player.wx [m] = player.x + xpart(player.swing_angle, BLADE_DISTANCE);
         player.wy [m] = player.y + ypart(player.swing_angle, BLADE_DISTANCE);
         break;
        case BLADE_RIGHT:
         goal_angle = ANGLE_16;
         player.wx [m] = player.x + xpart(player.swing_angle, BLADE_DISTANCE);
         player.wy [m] = player.y + ypart(player.swing_angle, BLADE_DISTANCE);
         break;

        case BLADE_CW_SLASH:
         player.swing_length += 2;
         if (player.swing_length >= 9)
          player.swing_length = 9;
         player.swing_angle += BLADE_SPEED;
         player.swing_angle &= 1023;
         if (player.swing_angle >= ANGLE_16 && player.swing_angle < ANGLE_4)
         {
            if (player.swing_hold == 1)
            {
             player.swing_state = BLADE_CW_SWING;
            }
             else
             {
              player.swing_state = BLADE_RIGHT;
             }
         }
         player.wx [m] = player.x + xpart(player.swing_angle, BLADE_DISTANCE);
         player.wy [m] = player.y + ypart(player.swing_angle, BLADE_DISTANCE);
         break;

        case BLADE_ACW_SLASH:
         player.swing_length += 2;
         if (player.swing_length >= 9)
          player.swing_length = 9;
         player.swing_angle -= BLADE_SPEED;
         player.swing_angle &= 1023;
         if (player.swing_angle <= ANGLE_2 - ANGLE_16 + 48 && player.swing_angle > ANGLE_4)
         {
            if (player.swing_hold == 1)
            {
             player.swing_state = BLADE_ACW_SWING;
            }
             else
             {
              player.swing_state = BLADE_LEFT;
             }
         }
         player.wx [m] = player.x + xpart(player.swing_angle, BLADE_DISTANCE);
         player.wy [m] = player.y + ypart(player.swing_angle, BLADE_DISTANCE);
         break;

        case BLADE_CW_SWING:
         player.swing_length += 2;
         if (player.swing_length >= 9)
          player.swing_length = 9;
         player.swing_angle += BLADE_SPEED;
         player.swing_angle &= 1023;
         if (player.swing_angle >= ANGLE_2 - ANGLE_16 - 48 && player.swing_angle < ANGLE_1 - ANGLE_4)
         {
              player.swing_state = BLADE_LEFT;
         }
         player.wx [m] = player.x + xpart(player.swing_angle, BLADE_DISTANCE);
         player.wy [m] = player.y + ypart(player.swing_angle, BLADE_DISTANCE);
         break;

        case BLADE_ACW_SWING:
         player.swing_length += 2;
         if (player.swing_length >= 9)
          player.swing_length = 9;
         player.swing_angle -= BLADE_SPEED;
         player.swing_angle &= 1023;
         if (player.swing_angle <= ANGLE_16 && player.swing_angle > 0)
         {
              player.swing_state = BLADE_RIGHT;
         }
         player.wx [m] = player.x + xpart(player.swing_angle, BLADE_DISTANCE);
         player.wy [m] = player.y + ypart(player.swing_angle, BLADE_DISTANCE);
         break;


       }

       player.swing_sprite_angle = radians_to_angle(atan2(player.wy [m] - player.y, player.wx [m] - player.x));
       player.swing_sprite_angle /= (ANGLE_1 / SMALL_ROTATIONS);

        if (player.swing_length > 0)
         player.swing_length --;
        player.swing_pos ++;
        if (player.swing_pos == 10)
         player.swing_pos = 0;

        player.swing_x [player.swing_pos] = player.wx [m] + xpart(player.swing_angle, 12000);
        player.swing_y [player.swing_pos] = player.wy [m] + ypart(player.swing_angle, 12000);
        player.swing_x2 [player.swing_pos] = player.wx [m] + xpart(player.swing_angle, 25000);
        player.swing_y2 [player.swing_pos] = player.wy [m] + ypart(player.swing_angle, 25000);

        player.swing_strength [player.swing_pos] = player.wstrength [player.blade_index];

 if (player.swing_state == BLADE_LEFT
  || player.swing_state == BLADE_RIGHT)
   return;

 int b = create_pbullet(PBULLET_BLADE);
 if (b != -1)
 {
  pbullet[b].x = player.swing_x [player.swing_pos];
  pbullet[b].y = player.swing_y [player.swing_pos];
  pbullet[b].x_speed = player.swing_x [player.swing_pos] - old_x;
  pbullet[b].y_speed = player.swing_y [player.swing_pos] - old_y;
  pbullet[b].xsize = 13000;
  pbullet[b].ysize = 13000;
  pbullet[b].dam = 80 + player.wlevel [player.blade_index] * 19; // was 100/22 or 60/14
  pbullet[b].dam /= player.wstrength [player.blade_index];
  pbullet[b].source = player.blade_index;
 }
 if (player.wlevel [player.blade_index] > 6)
 {
  int bangle = radians_to_angle(atan2(old_y - player.swing_y [player.swing_pos], old_x - player.swing_x [player.swing_pos]));
  bangle -= ANGLE_4;
  quick_cloud(CLOUD_CIRCLE2, old_x + xpart(bangle, 11000), old_y + ypart(bangle, 11000), 0, 0, player.wlevel [player.blade_index] - 2, TCOL_ORANGE, 0);
  bangle += ANGLE_2;
  quick_cloud(CLOUD_CIRCLE2, old_x + xpart(bangle, 11000), old_y + ypart(bangle, 11000), 0, 0, player.wlevel [player.blade_index] - 2, TCOL_ORANGE, 0);
 }


}

*/

void new_level(void)
{

 arena.phase = 0;
 arena.just_got_highscore = 0;
 arena.stage_score = 0;
 arena.max_stage_score = 0;//get_max_stage_score();

 init_background();
/*
 switch(arena.level)
 {
     case 1:
      init_background();
      break;
     case 1:
      init_background();
      break;
 }*/

// set_base_palette();



 init_level();
 init_beat_new_level();

 ticked = 0;

 boss.fight = 0;

 arena.new_level_sign = 120;

 arena.number = 0;

}

/*
void make_blocks(void)
{

 int i;

 for (i = 0; i < 20; i ++)
 {
  arena.block1_x [i] = grand(490) + 50;
  arena.block1_y [i] = 2400 * i; //grand(480) * 100;
  arena.block1_size [i] = 10 + grand(40);
  arena.block1_y_speed [i] = 180 + grand(20) + grand(20);

  arena.block2_x [i] = grand(490) + 50;
  arena.block2_y [i] = 2400 * i; //grand(480) * 100;
  arena.block2_y_speed [i] = 90 + grand(10) + grand(10);
  arena.block2_size [i] = 7 + grand(28);
 }

}

void run_blocks(void)
{
    int i;

    for (i = 0; i < 20; i ++)
    {
     arena.block1_y [i] += arena.block1_y_speed [i];
     arena.block2_y [i] += arena.block2_y_speed [i];

     if (arena.block1_y [i] > 48000 + (arena.block1_size [i] * 100))
     {
       arena.block1_y [i] = -10000 - grand(10000);
       arena.block1_x [i] = grand(490) + 50;
       arena.block1_size [i] = 10 + grand(40);
       arena.block1_y_speed [i] = 180 + grand(20) + grand(20);
     }
     if (arena.block2_y [i] > 48000 + (arena.block2_size [i] * 100))
     {
      arena.block2_y [i] = -10000 - grand(10000);
      arena.block2_y_speed [i] = 90 + grand(10) + grand(10);
      arena.block2_x [i] = grand(490) + 50;
      arena.block2_size [i] = 10 + grand(40);
     }

    }
}
*/

void run_background(void)
{
 int j, number, speed;

 switch(arena.level)
 {
  case 1:
    arena.number ++;
//    number = NO_BCIRCLES;
//    if (arena.number < 2000)
     number = arena.number / 40;
    if (number > NO_BCIRCLES)
     number = NO_BCIRCLES;

    speed = 256 + arena.number / 20;

    if (speed > 1536)
     speed = 1536;

    for (j = 0; j < number; j ++)
    {
      bcircle_y [0] [j] += speed;
      bcircle_size [0] [j] += bcircle_delta_size [0] [j];
      bcircle_delta_size [0] [j] -= 3;
      if (bcircle_size [0] [j] <= 1024)
      {
       new_bcircle(0, j);
      }
    }

    number = arena.number / 40 - 180;
    if (number > NO_BCIRCLES)
     number = NO_BCIRCLES;

    speed *= 2;

    for (j = 0; j < number; j ++)
    {
       bcircle_y [1] [j] += speed;
       bcircle_size [1] [j] += bcircle_delta_size [1] [j];
       bcircle_delta_size [1] [j] -= 7;
       if (bcircle_size [1] [j] <= 1024)
       {
        new_bcircle(1, j);
       }
    }

    number = arena.number / 40 - 250;
    if (number > NO_BCIRCLES)
     number = NO_BCIRCLES;

     speed *= 1.5;

    for (j = 0; j < number; j ++)
    {
       bcircle_y [2] [j] += speed;
       bcircle_size [2] [j] += bcircle_delta_size [2] [j];
       bcircle_delta_size [2] [j] -= 12;
       if (bcircle_size [2] [j] <= 1024)
       {
        new_bcircle(2, j);
       }
    }
   break;



  case 2:
   river_x += river_x_speed;
   river_x_speed += grand(100) - grand(100);
   if (river_x_speed > 2000)
    river_x_speed = 2000;
   if (river_x_speed < -2000)
    river_x_speed = -2000;
   if (river_x < 0)
    river_x += 614400;
   if (river_x > 614400)
    river_x -= 614400;

#define MAX_PANELS 30

   arena.number ++;
     number = arena.number / 40;
    if (number > MAX_PANELS)
     number = MAX_PANELS;

    speed = 1536; //256 + arena.number / 10;
//    number = NO_BCIRCLES;

//    if (speed > 1536)
//     speed = 1536;

    for (j = 0; j < number; j ++)
    {
      panel_y [0] [j] += speed;
      if (panel_x_size [0] [j] == 0 || panel_y [0] [j] > 616400 + (panel_y_size [0] [j] << 10))
       new_panel(0, j);
    }

    number = arena.number / 50 - 100;
    if (number > MAX_PANELS)
     number = MAX_PANELS;

    speed *= 2;

    for (j = 0; j < number; j ++)
    {
      panel_y [1] [j] += speed;
      if (panel_x_size [1] [j] == 0 || panel_y [1] [j] > 616400 + (panel_y_size [1] [j] << 10))
       new_panel(1, j);
    }

    number = arena.number / 60 - 150;
    if (number > MAX_PANELS)
     number = MAX_PANELS;

     speed *= 1.5;

    for (j = 0; j < number; j ++)
    {
      panel_y [2] [j] += speed;
      if (panel_x_size [2] [j] == 0 || panel_y [2] [j] > 616400 + (panel_y_size [2] [j] << 10))
       new_panel(2, j);
    }

   break;

  default:
  case 3:
    arena.number ++;
//    number = NO_BCIRCLES;
//    if (arena.number < 2000)
     number = arena.number / 40;
    if (number > NO_BCIRCLES)
     number = NO_BCIRCLES;

    speed = 256 + arena.number / 20;

    if (speed > 1536)
     speed = 1536;

    for (j = 0; j < number; j ++)
    {
      panel_y [0] [j] += speed;
      if (panel_growth [0] [j] > 0)
      {
       if (j & 1)
        panel_x_size [0] [j] += 200;
         else
          panel_y_size [0] [j] += 200;
       panel_growth [0] [j] --;
      }
       else
       {
        if (grand(100) == 0)
        {
         panel_growth [0] [j] = -1;
        }
        if (panel_growth [0] [j] == -1)
        {
         if (j & 1)
          panel_y_size [0] [j] -= 200;
           else
            panel_x_size [0] [j] -= 200;
        }
       }
     if (panel_x_size [0] [j] < 1024 || panel_y_size [0] [j] < 1024)
      new_l3panel(0, j);
    }

    number = arena.number / 40 - 180;
    if (number > NO_BCIRCLES)
     number = NO_BCIRCLES;

    speed *= 2;

    for (j = 0; j < number; j ++)
    {
      panel_y [1] [j] += speed;
      if (panel_growth [1] [j] > 0)
      {
       if (j & 1)
        panel_x_size [1] [j] += 300;
         else
          panel_y_size [1] [j] += 300;
       panel_growth [1] [j] --;
      }
       else
       {
        if (grand(100) == 0)
        {
         panel_growth [1] [j] = -1;
        }
        if (panel_growth [1] [j] == -1)
        {
         if (j & 1)
          panel_y_size [1] [j] -= 300;
           else
            panel_x_size [1] [j] -= 300;
        }
       }
       if (panel_x_size [1] [j] < 1024 || panel_y_size [1] [j] < 1024)
        new_l3panel(1, j);
    }

    number = arena.number / 40 - 250;
    if (number > NO_BCIRCLES)
     number = NO_BCIRCLES;

     speed *= 1.5;

    for (j = 0; j < number; j ++)
    {
      panel_y [2] [j] += speed;
      if (panel_growth [2] [j] > 0)
      {
       if (j & 1)
        panel_x_size [2] [j] += 400;
         else
          panel_y_size [2] [j] += 400;
       panel_growth [2] [j] --;
      }
       else
       {
        if (grand(100) == 0)
        {
         panel_growth [2] [j] = -1;
        }
        if (panel_growth [2] [j] == -1)
        {
         if (j & 1)
          panel_y_size [2] [j] -= 400;
           else
            panel_x_size [2] [j] -= 400;
        }
       }
      if (panel_x_size [2] [j] < 1024 || panel_y_size [2] [j] < 1024)
       new_l3panel(2, j);
    }


   break;


 }

}

void new_bcircle(int i, int j)
{
 if (arena.level_finished > 0)
  return;
 bcircle_size [i] [j] = 1024;
 bcircle_x [i] [j] = grand(X_MAX);
 bcircle_y [i] [j] = grand(Y_MAX + 100000) - 100000;
 bcircle_delta_size [i] [j] = 130 + grand(130);
 if (i == 1)
  bcircle_delta_size [i] [j] *= 2;
 if (i == 2)
  bcircle_delta_size [i] [j] *= 3;
}

void new_panel(int i, int j)
{
  if (arena.level_finished > 0)
   return;
  switch(i)
  {
   case 0:
    panel_y [i] [j] = - 10000;
    panel_x [i] [j] = river_x + grand(100000) - grand(100000);
    if (grand(4) == 0) panel_x [i] [j] += grand(50000) - grand(50000);
    panel_x_size [i] [j] = (6 + grand(7));
    panel_y_size [i] [j] = (6 + grand(7));
    break;
   case 1:
    panel_y [i] [j] = - 200000;
    panel_x [i] [j] = river_x + grand(150000) - grand(150000);
    if (grand(4) == 0) panel_x [i] [j] += grand(100000) - grand(100000);
    panel_x_size [i] [j] = (10 + grand(11));
    panel_y_size [i] [j] = (10 + grand(11));
    break;
   case 2:
    panel_y [i] [j] = - 300000;
    panel_x [i] [j] = river_x + grand(220000) - grand(220000);
    if (grand(4) == 0) panel_x [i] [j] += grand(130000) - grand(130000);
    panel_x_size [i] [j] = (14 + grand(12));
    panel_y_size [i] [j] = (14 + grand(12));
    break;
  }

  if (panel_x [i] [j] < 0)
   panel_x [i] [j] += 614400;
  if (panel_x [i] [j] > 614400)
   panel_x [i] [j] -= 614400;

}

void new_l3panel(int i, int j)
{
 panel_x_size [i] [j] = 1024;
 panel_y_size [i] [j] = 1024;
 if (arena.level_finished > 0)
 {
  panel_x_size [i] [j] = 0;
  panel_y_size [i] [j] = 0;
  return;
 }
 if (j & 1)
  panel_y_size [i] [j] = 10000 + grand(10000) + grand(10000);
   else
    panel_x_size [i] [j] = 10000 + grand(10000) + grand(10000);
 panel_x [i] [j] = grand(X_MAX);
// panel_y [i] [j] = grand(Y_MAX + 100000) - 100000;
 panel_y [i] [j] = grand(Y_MAX + 300000) - 300000;
 panel_growth [i] [j] = 30 + grand(50);
/* if (i == 1)
  bcircle_delta_size [i] [j] *= 2;
 if (i == 2)
  bcircle_delta_size [i] [j] *= 3;*/
//  exit(1);
}



void init_background(void)
{

 int j;


 switch(arena.level)
 {
  case 1:
  for (j = 0; j < NO_BCIRCLES; j ++)
  {
//   new_bcircle(0, j);
//   new_bcircle(1, j);
   bcircle_size [0] [j] = -1000;
   bcircle_size [1] [j] = -1000;
   bcircle_size [2] [j] = -1000;
  }
  break;
  case 2:
   for (j = 0; j < NO_BCIRCLES; j ++)
   {
    panel_x_size [0] [j] = 0;
    panel_x_size [1] [j] = 0;
    panel_x_size [2] [j] = 0;
    panel_y [0] [j] = 620000;
    panel_y [1] [j] = 620000;
    panel_y [2] [j] = 620000;
   }
   river_x = 300000;
   river_x_speed = 0;
   break;
  case 3:
   for (j = 0; j < NO_BCIRCLES; j ++)
   {
    panel_x_size [0] [j] = 0;
    panel_x_size [1] [j] = 0;
    panel_x_size [2] [j] = 0;
    panel_y [0] [j] = 620000;
    panel_y [1] [j] = 620000;
    panel_y [2] [j] = 620000;
   }
   river_x = 300000;
   river_x_speed = 0;
   break;
 }

}


/*
void run_stars(void)
{
 int i, j;

 for (i = 0; i < NO_STARS; i ++)
 {
  for (j = 0; j < 3; j ++)
  {
   star_y [i] [j] += star_y_speed [i] [j];
   if (star_y [i] [j] > 481000)
   {
    star_x [i] [j] = grand(640);
    star_y [i] [j] = 0;
    switch(j)
    {
     case 0: star_y_speed [i] [j] = 1000 + grand(1000); break;
     case 1: star_y_speed [i] [j] = 2000 + grand(1000); break;
     case 2: star_y_speed [i] [j] = 3000 + grand(1000); break;
    }
   }
  }
 }

}

void make_stars(void)
{

 int i, j;

 for (i = 0; i < NO_STARS; i ++)
 {
  for (j = 0; j < 3; j ++)
  {
    star_x [i] [j] = grand(640);
    star_y [i] [j] = grand(480000);
    switch(j)
    {
     case 0: star_y_speed [i] [j] = 1000 + grand(1000); break;
     case 1: star_y_speed [i] [j] = 2000 + grand(1000); break;
     case 2: star_y_speed [i] [j] = 3000 + grand(1000); break;
    }
  }
 }

}
*/
