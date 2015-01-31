
#include "config.h"

#include "allegro.h"

#include "globvars.h"
#include "palette.h"
#include "display.h"
#include "game.h"
#include "stuff.h"

#define TRANS_MODE drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
#define END_TRANS_MODE drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
#define END_TRANS drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);



extern RGB palet [256];


BITMAP *display;

extern volatile int frames_per_second;
extern int slacktime;
extern volatile int long_slacktime_store;
extern volatile int long_slacktime;

/*
BITMAP *underlay;

struct RLE_STRUCT eRLE_flower [5] [50];
RLE_SPRITE *platform_RLE [PLATFORM_RLES];
int platform_tile [9] [PLAT_Y];
int platform_position;
int platform_step;
// externed in game.cc

RLE_SPRITE *island1_RLE [ISLAND1_RLES];
int island1_tile [9] [ISLAND1_Y];
int island1_position;
int island1_step;

int star_x [NO_STARS] [3];
int star_y [NO_STARS] [3];
int star_y_speed [NO_STARS] [3];

RLE_SPRITE *trans_RLE [TRANS_RLES];
*/

int bcircle_x [3] [NO_BCIRCLES];
int bcircle_y [3] [NO_BCIRCLES];
int bcircle_size [3] [NO_BCIRCLES];
int bcircle_delta_size [3] [NO_BCIRCLES];
// externed in game.c

int panel_x [3] [NO_BCIRCLES];
int panel_y [3] [NO_BCIRCLES];
int panel_x_size [3] [NO_BCIRCLES];
int panel_y_size [3] [NO_BCIRCLES];
int panel_growth [3] [NO_BCIRCLES];

int river_x;
int river_x_speed;


RLE_SPRITE *RLE_bcircle [3] [RLE_BCIRCLES];
RLE_SPRITE *RLE_ccircle_basic [4] [RLE_CCIRCLES];
RLE_SPRITE *RLE_ccircle_4cols [4] [RLE_CCIRCLES];
RLE_SPRITE *RLE_ccircle_3cols [4] [RLE_CCIRCLES];

RLE_SPRITE *RLE_ibullet [4] [IBULLETS];

RLE_SPRITE *RLE_ibullet_2 [3] [IBULLETS_2];
RLE_SPRITE *RLE_ibullet_rgb [IBULLETS_3];

RLE_SPRITE *RLE_ring [RLE_RINGS];
RLE_SPRITE *RLE_bright_ring [RLE_RINGS];

struct RLE_STRUCT RLE_barrel1 [SMALL_ROTATIONS];

RLE_SPRITE *RLE_small_shock [4] [30];
RLE_SPRITE *RLE_large_shock [4] [50];
RLE_SPRITE *RLE_huge_shock [4] [50];

void draw_background(void);

int damage_per_time;

void indicate_fps(BITMAP *bmp, int play);

void draw_player(int show);
void draw_HUD(void);
int detect_player_collision(void);
int check_pixel(int x, int y);
void battle_report(void);

void draw_enemies(void);
void draw_an_enemy(int e);
void draw_clouds(void);
void draw_a_cloud(int c);

void draw_ebullets(void);
void draw_an_ebullet(int b);

void draw_pbullets(void);
void draw_a_pbullet(int b);
void draw_hand(void);
void draw_orb(void);

void draw_turret(int x, int y, int d1, int d2, int angle, int angle_rad, int angle_size, int col1, int col2, int col3);

void fcircle(int x, int y, int size, int col);
void bcircle(int x, int y, int size, unsigned char col);
void bcircle3(int x, int y, int size, unsigned char col);
void bcircle4(int x, int y, int size, unsigned char col);
void draw_ring(int x, int y, int size);
void draw_bright_ring(int x, int y, int size);

void pline(BITMAP *bmp, int x1, int y1, int x2, int y2, int colour);
void poly4(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int col);
void draw_trail_line(BITMAP *bmp, int x, int y, int size);
void draw_trail_line2(BITMAP *bmp, int x, int y, int size);

struct RLE_STRUCT dart_bullet [SMALL_ROTATIONS] [9];
struct RLE_STRUCT diamond_bullet [SMALL_ROTATIONS] [9];
struct RLE_STRUCT sdart_bullet [SMALL_ROTATIONS] [9];

struct RLE_STRUCT RLE_beam [SMALL_ROTATIONS];

//struct RLE_STRUCT splinter [SMALL_ROTATIONS];

RLE_SPRITE *RLE_circle [RLE_CIRCLES] [9];
/*
struct RLE_STRUCT RLE_mshock [4] [20];
struct RLE_STRUCT RLE_lshock [4] [50];
struct RLE_STRUCT RLE_hshock [4] [50];
struct RLE_STRUCT RLE_blade [SMALL_ROTATIONS];
struct RLE_STRUCT RLE_lwbeamshock [15];
struct RLE_STRUCT RLE_rshock [5] [20];
struct RLE_STRUCT RLE_mg_shock [10];
struct RLE_STRUCT RLE_ring [20];
*/
struct RLE_STRUCT petal1 [SMALL_ROTATIONS];
struct RLE_STRUCT petal2 [SMALL_ROTATIONS];
struct RLE_STRUCT petal3 [SMALL_ROTATIONS];

RLE_SPRITE *eRLE_player [S_PLAYER_RLES];

RLE_SPRITE *eRLE_small1 [S_ENEMY_RLES];
RLE_SPRITE *eRLE_large1 [L_ENEMY_RLES];
RLE_SPRITE *eRLE_large2 [L_ENEMY2_RLES];

struct RLE_STRUCT RLE_multi [WPN_TYPES] [8];

int points4 [8];

void print_number(int x, int y, int n);

#ifdef TRUE


#define TRACK_SIZE 10

struct trackstruct
{
  int type;
  int multiple;
  int count;
  int count2;
//  int next_track;
  int delay;
  int total;
  int x [TRACK_SIZE + 1];
  int y [TRACK_SIZE + 1];
  int reverse;

  int style;

  int x_target [MAX_TARGETS];
  int y_target [MAX_TARGETS];
  int target_time;

};


extern struct trackstruct track [TRACK_SIZE];

extern int next_track;
extern int overtrack_pos;
extern int overtrack_pos2;

#endif

int beam_colours [4] [5] =
{
{
    TRANS_RED1,
    TRANS_RED2,
    TRANS_RED3,
    TRANS_RED4,
    TRANS_RED5,
},
{
    TRANS_GREEN1,
    TRANS_GREEN2,
    TRANS_GREEN3,
    TRANS_GREEN4,
    TRANS_GREEN5,
},
{
    TRANS_BLUE1,
    TRANS_BLUE2,
    TRANS_BLUE3,
    TRANS_BLUE4,
    TRANS_BLUE5,
},

{
    TRANS_RED1,
    TRANS_PURPLE2,
    TRANS_PURPLE3,
    TRANS_PURPLE4,
    TRANS_BLUE5,
}


};







void run_display(int show)
{
  clear_to_color(display, COL_BACK1);

  if (show == 1)
  {
//   switch(arena.level)
//   {
//    default: draw_background(); break;
//   }
   draw_background();
   draw_enemies();
  }
  if (player.in_play == 1)
   draw_player(show);
  draw_ebullets();

  if (player.in_play == 1 && player.grace == 0)
  {
   if (detect_player_collision() == 1)
    player_hit();
  }
  // this has been moved to draw_player

  if (show == 0)
   return; // skip the frame

  if (player.type == PTYPE_HAND)
     draw_hand();

  if (player.type == PTYPE_SQUID)
     draw_orb();

  draw_pbullets();
  draw_clouds();

  draw_HUD();
//#define SDFSDFG
//  int i, count = 0, ypos = 100;

//  textprintf_right_ex(display, font, 595, 580, -1, -1, "%i fps", frames_per_second);
/*  textprintf_ex(display, font, 10, 130, -1, -1, "%i slack", slacktime);
  textprintf_ex(display, font, 10, 150, -1, -1, "%i slack2", long_slacktime_store);*/
/*
#ifdef SHOW_PLAYER_DAMAGE
  textprintf_ex(display, font, 10, 170, -1, -1, "%i", player.wstrength [0]);
  textprintf_ex(display, font, 10, 190, -1, -1, "%i", player.wstrength [1]);
  textprintf_ex(display, font, 10, 210, -1, -1, "%i", player.wstrength [2]);
#endif
*/
//#define SDFSDFG

#ifdef SDFSDFG
int ypos = 400, count = 0, i;;
  textprintf_ex(display, font, 10, ypos, -1, -1, "%i fps", frames_per_second);
  textprintf_ex(display, font, 10, ypos + 15, -1, -1, "Press Escape to quit");
  for (i = 0; i < NO_ENEMIES; i ++)
  {
   if (enemy[i].type != ENEMY_NONE)
    count++;
  }
  textprintf_ex(display, font, 10, ypos + 30, -1, -1, "e %i", count);
  count = 0;
  for (i = 0; i < NO_EBULLETS; i ++)
  {
   if (ebullet[i].type != EBULLET_NONE)
    count++;
  }
  textprintf_ex(display, font, 10, ypos + 45, -1, -1, "b %i", count);
  count = 0;
  for (i = 0; i < NO_PBULLETS; i ++)
  {
   if (pbullet[i].type != PBULLET_NONE)
    count++;
  }
  textprintf_ex(display, font, 10, ypos + 60, -1, -1, "p %i", count);
  count = 0;
  for (i = 0; i < NO_CLOUDS; i ++)
  {
   if (cloud[i].type != CLOUD_NONE)
    count++;
  }
  textprintf_ex(display, font, 10, ypos + 75, -1, -1, "c %i", count);

/*  print_number(10, ypos + 90, player.wxp [0]);
  print_number(10, ypos + 105, player.wxp [1]);
  print_number(10, ypos + 120, player.wxp [2]);*/
  print_number(10, ypos + 180, damage_per_time);

 print_number(10, ypos + 90, track [0].count);
 print_number(10, ypos + 105, track [1].count);
 print_number(10, ypos + 120, track [2].count);
 print_number(10, ypos + 135, next_track);
 print_number(10, ypos + 150, overtrack_pos);
 print_number(10, ypos + 165, overtrack_pos2);
#endif
//  textprintf_ex(display, font, 10, 10, COL_WHITE, -1, "%i fps", frames_per_second);

 if (arena.level_finished > 0 && arena.level_finished < 390)
 {
/*
  int y = 140 - arena.level_finished;
  if (y > 17)
   y = 17;
  if (arena.level_finished < 17)
   y = arena.level_finished;
  int x = 140 - arena.level_finished;
  x *= 4;
  if (x > 110)
   x = 110;
  if (arena.level_finished < 20)
  {
   x = arena.level_finished * 4;
   if (x > 110)
    x = 110;
  }*/


  if (arena.level_finished < 360 && arena.level_finished > 20)
  {
   switch(arena.level)
   {
    case 1:
     textprintf_centre_ex(display, font, 300, 180, -1, -1, "STAGE  ONE  COMPLETE");
     break;
    case 2:
     textprintf_centre_ex(display, font, 300, 180, -1, -1, "STAGE  TWO  COMPLETE");
     break;
    case 3:
     textprintf_centre_ex(display, font, 300, 180, -1, -1, "STAGE  THREE  COMPLETE");
     break;

   }
   battle_report();
  }
//   textprintf_centre_ex(display, font, 320, 200, -1, -1, "SEE  WWW.SHMUP-DEV.COM  FOR  THE  COMPETITION  VERSION!");
 }

 if (arena.game_over > 0)
 {
//  TRANS_MODE
  int y = 200 - arena.game_over;
  if (y > 17)
   y = 17;
  int x = 200 - arena.game_over;
  x *= 4;
  if (x > 80)
   x = 80;


  if (arena.game_over < 180)
   textprintf_centre_ex(display, font, 300, 180, -1, -1, "G A M E      O V E R");
 }

 if (arena.new_level_sign > 0)
 {
  int y = 120 - arena.new_level_sign;
  if (y > 17)
   y = 17;
  if (arena.new_level_sign < 17)
   y = arena.new_level_sign;
  int x = 120 - arena.new_level_sign;
  x *= 4;
  if (x > 80)
   x = 80;
  if (arena.new_level_sign < 20)
  {
   x = arena.new_level_sign * 4;
   if (x > 80)
    x = 80;
  }


  if (arena.new_level_sign < 100 && arena.new_level_sign > 20)
   switch(arena.level)
   {
       case 1:
        textprintf_centre_ex(display, font, 300, 180, -1, -1, "S T A G E   O N E"); break;
       case 2:
        textprintf_centre_ex(display, font, 300, 180, -1, -1, "S T A G E   T W O"); break;
       case 3:
        textprintf_centre_ex(display, font, 300, 180, -1, -1, "S T A G E   T H R E E"); break;
       case 4:
        textprintf_centre_ex(display, font, 300, 180, -1, -1, "F I N A L   S T A G E"); break;
   }
 }

/* if (arena.change_palette != -1)
 {
   int newpal = build_new_palette(arena.level, 0, 0, 0);
   if (options.run_vsync > 0)
    vsync();
   if (arena.new_level_sign > 118)
    clear_bitmap(screen);
     else
      blit(display, screen, 0, 0, 0, 0, 640, 480);
   set_palette(palet [newpal]);
   if (arena.shift_palette == 0)
    arena.change_palette = -1;
 }

   else*/

 if (arena.shift_palette != 0)
 {
   if (options.run_vsync > 0)
    vsync();
//   blit(display, screen, 0, 0, 0, 0, 640, 480);
   blit(display, screen, 0, 0, 100, 0, 600, 600);
   set_palette(palet);
   if (arena.shift_palette == 1)
    arena.shift_palette = 0;
 }
  else
   {
    if (options.run_vsync == 2)
     vsync();
//    TRANS_MODE
/*    rectfill(display, 100, 100, 150, 200, TRANS_RED1);
    rectfill(display, 150, 100, 200, 200, TRANS_RED2);
    rectfill(display, 200, 100, 250, 200, TRANS_RED3);
    rectfill(display, 250, 100, 300, 200, TRANS_RED4);
    rectfill(display, 300, 100, 350, 200, TRANS_RED5);*/

/*    rectfill(display, 0, 160, 600, 165, TRANS_GREEN1);
    rectfill(display, 0, 165, 600, 170, TRANS_GREEN2);
    rectfill(display, 0, 170, 600, 175, TRANS_GREEN3);
    rectfill(display, 0, 175, 600, 180, TRANS_GREEN4);
    rectfill(display, 0, 180, 600, 185, TRANS_GREEN5);


    rectfill(display, 100, 100, 150, 200, TRANS_RED1);
    rectfill(display, 150, 100, 200, 200, TRANS_PURPLE2);
    rectfill(display, 200, 100, 250, 200, TRANS_PURPLE3);
    rectfill(display, 250, 100, 300, 200, TRANS_PURPLE4);
    rectfill(display, 300, 100, 350, 200, TRANS_BLUE5);

    END_TRANS_MODE*/

/*    TRANS_MODE
    rectfill(display, 100, 100, 150, 200, TRANS_BLUE1);
    rectfill(display, 150, 100, 200, 200, TRANS_BLUE2);
    rectfill(display, 200, 100, 250, 200, TRANS_BLUE3);
    rectfill(display, 250, 100, 300, 200, TRANS_BLUE4);
    rectfill(display, 300, 100, 350, 200, TRANS_BLUE5);
    END_TRANS_MODE*/
/*TRANS_MODE
    rectfill(display, 0, 100, 600, 200, TRANS_RED1);
    rectfill(display, 0, 200, 600, 300, TRANS_RED2);
    rectfill(display, 0, 300, 600, 400, TRANS_RED3);
    rectfill(display, 0, 400, 600, 500, TRANS_RED4);
    rectfill(display, 0, 500, 600, 600, TRANS_RED5);
END_TRANS_MODE*/
    blit(display, screen, 0, 0, 100, 0, 600, 600);
   }




}

void battle_report(void)
{

 if (arena.level_finished < 310)
   textprintf_centre_ex(display, font, 300, 220, -1, -1, "DESTRUCTION RATE: %i%%", arena.destruction_rate);

 if (arena.level_finished < 270)
   textprintf_right_ex(display, font, 300, 250, -1, -1, "RANK:");

 if (arena.level_finished < 230)
 {
  switch(arena.stage_rank)
  {
   case RANK_C: textprintf_ex(display, font, 310, 250, -1, -1, "C"); break;
   case RANK_B: textprintf_ex(display, font, 310, 250, -1, -1, "B"); break;
   case RANK_A: textprintf_ex(display, font, 310, 250, -1, -1, "A");
                textprintf_centre_ex(display, font, 300, 275, -1, -1, "WELL DONE"); break;
   case RANK_S: textprintf_ex(display, font, 310, 250, -1, -1, "S");
                textprintf_centre_ex(display, font, 300, 275, -1, -1, "OUTSTANDING!"); break;
   case RANK_P: textprintf_ex(display, font, 310, 250, -1, -1, "P");
                textprintf_centre_ex(display, font, 300, 275, -1, -1, "PERFECT!"); break;
  }

 }

 if (arena.level_finished < 230 && (arena.level == 1 || arena.level == 2))
 {

     int k;

#define Y_PLACE 330

     textprintf_right_ex(display, font, 342, Y_PLACE - 2, -1, -1, "base power level");

     TRANS_MODE

     for (k = 0; k < options.current_power; k ++)
     {
      rectfill(display, 379 + k * 4, Y_PLACE + 3, 381 + k * 4, Y_PLACE + 8, TRANS_RED1);
      rect(display, 379 + k * 4, Y_PLACE + 3, 381 + k * 4, Y_PLACE + 8, TRANS_RED4);
     }

     rect(display, 350, Y_PLACE, 376, Y_PLACE + 11, TRANS_BLUE3);

     if (options.current_wpixels > 0)
     {
      rectfill(display, 353, Y_PLACE + 3, 353 + options.current_wpixels, Y_PLACE + 8, TRANS_RED1);
      rect(display, 353, Y_PLACE + 3, 353 + options.current_wpixels, Y_PLACE + 8, TRANS_RED5);
     }

#define Y_SEP 30

     END_TRANS

     textprintf_right_ex(display, font, 342, Y_PLACE + Y_SEP - 2, -1, -1, "previous best");

     TRANS_MODE

     for (k = 0; k < options.previous_power; k ++)
     {
      rectfill(display, 379 + k * 4, Y_PLACE + 3 + Y_SEP, 381 + k * 4, Y_PLACE + 8 + Y_SEP, TRANS_RED1);
      rect(display, 379 + k * 4, Y_PLACE + 3 + Y_SEP, 381 + k * 4, Y_PLACE + 8 + Y_SEP, TRANS_RED4);
     }

     rect(display, 350, Y_PLACE + Y_SEP, 376, Y_PLACE + 11 + Y_SEP, TRANS_BLUE3);

     if (options.previous_wpixels > 0)
     {
      rectfill(display, 353, Y_PLACE + 3 + Y_SEP, 353 + options.previous_wpixels, Y_PLACE + 8 + Y_SEP, TRANS_RED1);
      rect(display, 353, Y_PLACE + 3 + Y_SEP, 353 + options.previous_wpixels, Y_PLACE + 8 + Y_SEP, TRANS_RED5);
     }

      END_TRANS

     if ((options.current_power > options.previous_power)
      || (options.current_power == options.previous_power && options.current_bar > options.previous_bar))
       textprintf_centre_ex(display, font, 300, 420, -1, -1, "new power level set");


 }

 if (arena.level_finished < 230 && arena.level == 3)
 {

   if (arena.cleared == 1)
    textprintf_centre_ex(display, font, 300, 380, -1, -1, "full mission cleared!");
   if (arena.unlocked == 1)
    textprintf_centre_ex(display, font, 300, 420, -1, -1, "chaos squid unlocked");


 }


}

int detect_player_collision(void)
{

//  if (player.in_play != 1 || player.grace != 0)
//   return 0;

  int x = player.x >> 10;
  int y = player.y >> 10;

//  textprintf_ex(display, font, 510, 100, -1, -1, "%i", getpixel(display, x, y));
//  textprintf_ex(display, font, 510, 100, -1, -1, "%i", player.hand_phase);

  if (check_pixel(x, y)
   || check_pixel(x + 1, y + 1)
   || check_pixel(x + 1, y - 1)
   || check_pixel(x - 1, y + 1)
   || check_pixel(x - 1, y - 1))
   {
//    player_hit();
    return 1;
   }

  if (//check_pixel(x, y) ||
   check_pixel(x - 2, y - 2)
   || check_pixel(x + 2, y - 2)
   || check_pixel(x - 2, y + 2)
   || check_pixel(x + 2, y + 2)
   || check_pixel(x + 2, y + 2)
   || check_pixel(x, y + 2))
//   || check_pixel(x, y - 3)
//   || check_pixel(x - 1, y - 3)
   //|| check_pixel(x + 1, y - 3))
   {
//    player_hit();
    return 1;
   }

  return 0;

}

int check_pixel(int x, int y)
{
  int pix = getpixel(display, x, y);

/*  if ((pix >= TRANS_RED5 && pix < TRANS_GREEN5)
   || (pix >= TRANS_GREEN5 && pix < TRANS_BLUE5)
   || (pix >= TRANS_BLUE5 && pix < TRANS_PURPLE2))*/
  if (pix >= TRANS_RED5 && pix < TRANS_PURPLE2)
    return 1;

//  putpixel(display, x, y, grand(100));

 return 0;

}



void draw_player(int show)
{

 int x = player.x >> 10;
 int y = player.y >> 10;
 int xa, ya, xb, yb, xc, j, i, k;
 xb = 0;

 switch(player.type)
 {
  case PTYPE_BASIC:
   xa = S_PLAYER1_1;
   if (player.recoil > 0)
    xa = S_PLAYER1_2;
   if (player.recoil > 15)
    xa = S_PLAYER1_3;
   draw_rle_sprite(display, eRLE_player [xa], x - 16, y - 20);


   xa = 4 + grand(4);
   draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1, y - xa - 1 + 8);
   TRANS_MODE
   ya = 14 + grand(7);
   vline(display, x, y + 8, y + ya, TRANS_RED4);
   yb = ya + 3 + grand(3);
   vline(display, x, y + ya, y + yb, TRANS_RED3);
   yb = yb + 2 + grand(2);
   vline(display, x, y + ya, y + yb, TRANS_RED2);
   yb = yb + 2 + grand(2);
   vline(display, x, y + ya, y + yb, TRANS_RED1);

   vline(display, x - 1, y + 8, y + ya - 5, TRANS_RED3);
   vline(display, x + 1, y + 8, y + ya - 5, TRANS_RED3);

   vline(display, x - 1, y + 8, y + ya - 1, TRANS_RED1);
   vline(display, x + 1, y + 8, y + ya - 1, TRANS_RED1);
   END_TRANS_MODE

   if (player.charge > 0)
   {
       if (player.charge > 6000)
        bcircle(x, y - 12, 1 + player.charge / 3000 + grand(4), CLOUD_COL_1);
         else
          bcircle3(x, y - 12, 1 + player.charge / 3000 + grand(4), CLOUD_COL_1);
   }

   if (player.charge >= 20000)
   {
        bcircle(x - 14, y - 9, 3 + grand(4), CLOUD_COL_1);
        bcircle(x - 17 - grand(2), y - 9, 2 + grand(2), CLOUD_COL_1);

        bcircle(x + 14, y - 9, 3 + grand(4), CLOUD_COL_1);
        bcircle(x + 17 + grand(2), y - 9, 2 + grand(2), CLOUD_COL_1);
   }

//   vline(display, x, y - 10, y + 10, COL_OUTLINE);
//   hline(display, x - 10, y, x + 10, COL_OUTLINE);
//   textprintf_right_ex(display, font, 300, 300, -1, -1, "%i, %i", player.x, player.y);
   break;
   case PTYPE_ANENOME:
   xa = S_PLAYER2_1;
   xc = 0;
   if (player.recoil > 0)
   {
    xa = S_PLAYER2_2;
    xc = 1;
   }
   if (player.recoil > 15)
   {
    xa = S_PLAYER2_3;
    xc = 2;
   }
   draw_rle_sprite(display, eRLE_player [xa], x - 19 - xc, y - 8);

   xa = 4 + grand(4);
   draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1, y - xa - 1 + 11);
   TRANS_MODE
   ya = 17 + grand(7);
   vline(display, x, y + 12, y + ya, TRANS_RED4);
   yb = ya + 3 + grand(3);
   vline(display, x, y + ya, y + yb, TRANS_RED3);
   yb = yb + 2 + grand(2);
   vline(display, x, y + ya, y + yb, TRANS_RED2);
   yb = yb + 2 + grand(2);
   vline(display, x, y + ya, y + yb, TRANS_RED1);

   vline(display, x - 1, y + 11, y + ya - 5, TRANS_RED3);
   vline(display, x + 1, y + 11, y + ya - 5, TRANS_RED3);

   vline(display, x - 1, y + 11, y + ya - 1, TRANS_RED1);
   vline(display, x + 1, y + 11, y + ya - 1, TRANS_RED1);
   END_TRANS_MODE

   if (player.blade_pos == BLADE_LEFT)
    bcircle(x - 16 - xc, y + 19 + xc, 3 + grand(3), 2);
   if (player.blade_pos == BLADE_RIGHT)
    bcircle(x + 16 + xc, y + 19 + xc, 3 + grand(3), 2);
/*
   j = player.old_blade_pos;
   k = 0;
   TRANS_MODE
   do
   {
    old_x = player.old_blade_x [j] >> 10;
    old_y = player.old_blade_y [j] >> 10;
//    old_x2 = player.old_blade_x2 [j] / GRAIN;
//    old_y2 = player.old_blade_y2 [j] / GRAIN;
    j --;
    if (j == -1)
     j = 9;
    k ++;
    if (j == player.old_blade_pos || k >= player.old_blade_length)
     break;
    xa = ((player.old_blade_length - k) / 2);
    pline(display, old_x, old_y, player.old_blade_x [j] >> 10, player.old_blade_y [j] >> 10, TRANS_RED3);

//     do_line(display, old_x, old_y, player.old_blade_x [j] >> 10, player.old_blade_y [j] >> 10, xa, draw_trail_line);

//    xa /= player.old_blade_strength [j];

//    xa = 8;
//    xb = player.wlevel [player.blade_index];
//    xa *= 10 + xb;
//    xa /= 10;
//    bcircle(player.old_blade_x [j] >> 10, player.old_blade_y [j] >> 10, xa, 0);

   } while (TRUE);
   END_TRANS_MODE
*/

   break;
   case PTYPE_MULTI:
    xa = S_PLAYER6_1;
    draw_rle_sprite(display, eRLE_player [xa], x - 16, y - 8);
    break;
   case PTYPE_SQUID:
    xa = S_PLAYER5_1;
    xc = 0;
    if (player.grip_frame > 0)
    {
     xa = S_PLAYER5_2;
     xc = 1;
    }
    if (player.grip_frame > 15)
    {
     xa = S_PLAYER5_3;
     xc = 2;
    }
    draw_rle_sprite(display, eRLE_player [xa], x - 17 + xc, y - 21);

   xa = 4 + grand(4);
   draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1, y - xa - 1 + 8);
   TRANS_MODE
   ya = 14 + grand(7);
   vline(display, x, y + 8, y + ya, TRANS_RED4);
   yb = ya + 3 + grand(3);
   vline(display, x, y + ya, y + yb, TRANS_RED3);
   yb = yb + 2 + grand(2);
   vline(display, x, y + ya, y + yb, TRANS_RED2);
   yb = yb + 2 + grand(2);
   vline(display, x, y + ya, y + yb, TRANS_RED1);

   vline(display, x - 1, y + 8, y + ya - 5, TRANS_RED3);
   vline(display, x + 1, y + 8, y + ya - 5, TRANS_RED3);

   vline(display, x - 1, y + 8, y + ya - 1, TRANS_RED1);
   vline(display, x + 1, y + 8, y + ya - 1, TRANS_RED1);
   END_TRANS_MODE

    break;

   case PTYPE_BOMBER:
   xa = 4 + grand(4);
   ya = 0;
   draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 - 16, y - xa - 1 + 7);
   draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 + 16, y - xa - 1 + 7);
   xa = 3 + grand(3);
   ya += xa;
   draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [xa], x - xa - 1 - 16, y - xa - 1 + 7 + ya);
   draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [xa], x - xa - 1 + 16, y - xa - 1 + 7 + ya);
   xa = 2 + grand(3);
   ya += xa;
   draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1 - 16, y - xa - 1 + 7 + ya);
   draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1 + 16, y - xa - 1 + 7 + ya);

   xa = S_PLAYER3_1;
   draw_rle_sprite(display, eRLE_player [xa], x - 22, y - 7);

   xa = player.bomb_recycle1 / 200;
   rectfill(display, x - 10, y + 1, x - 9, y - 3 + xa, COL_WHITE);
   if (player.bomb_recycle1 == 0)
   {
    bcircle(x - 9, y - 11, 5 + grand(3), 0);
//    draw_ring(x - 9, y - 9, 10 + grand(3));
   }

   xa = player.bomb_recycle2 / 200;
   rectfill(display, x + 9, y + 1, x + 10, y - 3 + xa, COL_WHITE);
   if (player.bomb_recycle2 == 0)
   {
    bcircle(x + 9, y - 11, 5 + grand(3), 0);
//    draw_ring(x + 9, y - 9, 10 + grand(3));
   }

   draw_ring(x, y, player.ring_size);
   k = 5 + player.weapon_level / 4 + grand(3);
   bcircle(x + xpart(player.ring_angle, player.ring_size + 2), y + ypart(player.ring_angle, player.ring_size + 2), k, 0);
   k = 5 + player.weapon_level / 4 + grand(3);
   bcircle(x + xpart(player.ring_angle + ANGLE_3, player.ring_size + 2), y + ypart(player.ring_angle + ANGLE_3, player.ring_size + 2), k, 0);
   k = 5 + player.weapon_level / 4 + grand(3);
   bcircle(x + xpart(player.ring_angle - ANGLE_3, player.ring_size + 2), y + ypart(player.ring_angle - ANGLE_3, player.ring_size + 2), k, 0);

   if (player.beam_firing > 0)
   {
    for (i = 0; i < 3; i ++)
    {
     xa = x + xpart(player.ring_angle + i * ANGLE_3, player.ring_size + 2);
     ya = y + ypart(player.ring_angle + i * ANGLE_3, player.ring_size + 2);
     TRANS_MODE
     k = grand(player.weapon_level) / 3;
     switch(k)
     {
      case 0:
       vline(display, xa, ya, player.beam_y [i] >> 10, TRANS_RED3);
       vline(display, xa - 1, ya, player.beam_y [i] >> 10, TRANS_RED1);
       vline(display, xa + 1, ya, player.beam_y [i] >> 10, TRANS_RED1);
       bcircle4(xa, player.beam_y [i] >> 10, 4 + grand(3), 0);
       break;
      case 1:
       vline(display, xa, ya, player.beam_y [i] >> 10, TRANS_RED3);
       vline(display, xa - 1, ya, player.beam_y [i] >> 10, TRANS_RED2);
       vline(display, xa + 1, ya, player.beam_y [i] >> 10, TRANS_RED2);
       bcircle4(xa, player.beam_y [i] >> 10, 5 + grand(4), 0);
       break;
      case 2:
       vline(display, xa, ya, player.beam_y [i] >> 10, TRANS_RED3);
       vline(display, xa - 1, ya, player.beam_y [i] >> 10, TRANS_RED2);
       vline(display, xa + 1, ya, player.beam_y [i] >> 10, TRANS_RED2);
       vline(display, xa - 2, ya, player.beam_y [i] >> 10, TRANS_RED1);
       vline(display, xa + 2, ya, player.beam_y [i] >> 10, TRANS_RED1);
       bcircle4(xa, player.beam_y [i] >> 10, 6 + grand(4), 0);
       break;
      default:
      case 3:
       vline(display, xa, ya, player.beam_y [i] >> 10, TRANS_RED4);
       vline(display, xa - 1, ya, player.beam_y [i] >> 10, TRANS_RED3);
       vline(display, xa + 1, ya, player.beam_y [i] >> 10, TRANS_RED3);
       vline(display, xa - 2, ya, player.beam_y [i] >> 10, TRANS_RED1);
       vline(display, xa + 2, ya, player.beam_y [i] >> 10, TRANS_RED1);
       bcircle4(xa, player.beam_y [i] >> 10, 7 + grand(5), 0);
       break;
// can't use red5 because it triggers player death!
     }
//     vline(display, xa - 2, ya, player.beam_y [i] >> 10, TRANS_RED1);
//     vline(display, xa + 2, ya, player.beam_y [i] >> 10, TRANS_RED1);
     END_TRANS_MODE
     j = ya;
     while (j > player.beam_y [i] >> 10)
     {
      bcircle4(xa, j, 1 + grand(3) + k, 0);
      j -= 5 + grand(10);
     }

    }

   }
/*   TRANS_MODE
   ya = 17 + grand(7);
   vline(display, x - 16, y + 8, y + ya, TRANS_RED4);
   vline(display, x + 16, y + 8, y + ya, TRANS_RED4);
   yb = ya + 3 + grand(3);
   vline(display, x - 16, y + ya, y + yb, TRANS_RED3);
   vline(display, x + 16, y + ya, y + yb, TRANS_RED3);
   yb = yb + 2 + grand(2);
   vline(display, x - 16, y + ya, y + yb, TRANS_RED2);
   vline(display, x + 16, y + ya, y + yb, TRANS_RED2);
   yb = yb + 2 + grand(2);
   vline(display, x - 16, y + ya, y + yb, TRANS_RED1);
   vline(display, x + 16, y + ya, y + yb, TRANS_RED1);

   vline(display, x - 17, y + 7, y + ya - 5, TRANS_RED3);
   vline(display, x - 15, y + 7, y + ya - 5, TRANS_RED3);

   vline(display, x - 17, y + 7, y + ya - 1, TRANS_RED1);
   vline(display, x - 15, y + 7, y + ya - 1, TRANS_RED1);
*/
/*   xa = 4 + grand(4);
   draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 19, y - xa - 1 + Y_DISP);
   TRANS_MODE

   ya = 3 + grand(3);
   pline(display, x - 19, y + Y_DISP, x - 19 - ya, y + Y_DISP + ya, TRANS_RED4);
   yb = ya + 3 + grand(3);
   pline(display, x - 19 - ya, y + Y_DISP + ya, x - 19 - yb, y + Y_DISP + yb, TRANS_RED4);
   ya = yb + 3 + grand(3);
   pline(display, x - 19 - yb, y + Y_DISP + yb, x - 19 - ya, y + Y_DISP + ya, TRANS_RED3);
   yb = ya + 3 + grand(3);
   pline(display, x - 19 - ya, y + Y_DISP + ya, x - 19 - yb, y + Y_DISP + yb, TRANS_RED2);
   ya = yb + 3 + grand(3);
   pline(display, x - 19 - yb, y + Y_DISP + yb, x - 19 - ya, y + Y_DISP + ya, TRANS_RED1);
*/
//   END_TRANS_MODE

   break;
  case PTYPE_HAND:
   xa = 4 + grand(4);
   ya = 0;
   draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 - 15 - ya, y - xa - 1 + 3 + ya);
   draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 + 15 + ya, y - xa - 1 + 3 + ya);
   xa = 3 + grand(3);
   ya += xa / 2;
   draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [xa], x - xa - 1 - 15 - ya, y - xa - 1 + 5 + ya);
   draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [xa], x - xa - 1 + 15 + ya, y - xa - 1 + 5 + ya);
   xa = 2 + grand(3);
   ya += xa / 2;
   draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1 - 15 - ya, y - xa - 1 + 7 + ya);
   draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1 + 15 + ya, y - xa - 1 + 7 + ya);

   xa = S_PLAYER4_3L;
   if (player.grip_frame > 0)
    xa = S_PLAYER4_2L;
 if (player.grip_frame >= 10)
  xa = S_PLAYER4_1L;
   draw_rle_sprite(display, eRLE_player [xa], x - 16, y + 3);


   xa = S_PLAYER4_1;
   if (player.recoil > 0)
    xa = S_PLAYER4_2;
 if (player.recoil >= 10)
  xa = S_PLAYER4_3;
   draw_rle_sprite(display, eRLE_player [xa], x - 16, y - 9);




   if (player.hand_acquire > 0)
    draw_bright_ring(x, y - 120, 19);
     else
      draw_ring(x, y - 120, 15);

   break;
 }

 if (player.grace > 40)
  draw_ring(x, y, 25);
   else
    if (player.grace > 0)
    {
     if (arena.counter & 4)
      draw_ring(x, y, 25);
    }


//      drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
/*   draw_rle_sprite(display, eRLE_small1 [S_ENEMY_PLAYER], x - 10, y - 7);

// draw_rle_sprite(display, eRLE_small1 [xa], x - 10, y - 7);
   draw_rle_sprite(display, eRLE_small1 [S_ENEMY_PLAYER_UP_L], x - 11 + player.wings1 / 10, y - 8);
   draw_rle_sprite(display, eRLE_small1 [S_ENEMY_PLAYER_UP_R], x + 2 + player.wings1 / 10, y - 8);
   draw_rle_sprite(display, eRLE_small1 [S_ENEMY_PLAYER_DOWN_L], x - 10 - player.wings2 / 10, y + 2 - player.wings2 / 10);
   draw_rle_sprite(display, eRLE_small1 [S_ENEMY_PLAYER_DOWN_R], x + 2 + player.wings2 / 10, y + 2 - player.wings2 / 10);
*/
//   xa = S_ENEMY_PLAYER1;
/*   if (player.recycle_speed > 0
       && player.recycle > 0)
   {

//    xa = S_ENEMY_PLAYER1;
    ya = 0;
//    draw_rle_sprite(display, eRLE_small1 [xa], x - 7 - ya, y - 2);
   }*/


}

void draw_hand(void)
{
   int x = player.hand_x >> 10;
   int y = player.hand_y >> 10;
   int i, size, angle;

   switch(player.hand_phase)
   {
    case HAND_DOCK:
    case HAND_RETURN:
     bcircle(x, y, 6 + grand(3), 0);
     break;
    case HAND_SEEK:
     bcircle(x, y, 8 + grand(4), 0);
     break;
    case HAND_STOP:
     bcircle(x, y, 12 + grand(4), 0);
     break;

   }

   if (player.hand_unfold < 30)
    draw_ring(x, y, (30 - player.hand_unfold) * 1);

   size = player.hand_unfold >> 2;
   if (size > 7) size = 7;
   if (size > 0)
   {
    for (i = 0; i < 3; i ++)
    {
      bcircle(x + xpart(player.hand_angle + ANGLE_3 * i, (player.hand_unfold >> 1) + 2), y + ypart(player.hand_angle + ANGLE_3 * i, (player.hand_unfold >> 1) + 2), size + grand(3), 2);
    }
   }// remember to change also in run_hand!


   if (player.hand_unfold > 30 + player.weapon_level)
   {
    size = player.hand_unfold >> 3;
    if (size > 4) size = 4;

    if (size > 0)
    {
     angle = ANGLE_1 - player.hand_angle;
     for (i = 0; i < 3; i ++)
     {
       bcircle(x + xpart(angle + ANGLE_3 * i, (player.hand_unfold >> 2) + 3), y + ypart(angle + ANGLE_3 * i, (player.hand_unfold >> 2) + 3), size + grand(3), 3);
     }
    } // remember to change also in draw_hand!


   }


   for (i = 0; i < 6; i ++)
   {
     if (player.lock [i] != -1)
     {
      x = enemy[player.lock [i]].x >> 10;
      y = enemy[player.lock [i]].y >> 10;
      draw_bright_ring(x, y, player.lock_size [i]);//player.lock_size [i]);
     }
   }




}


void draw_orb(void)
{
   int x = player.orb_x >> 10;
   int y = player.orb_y >> 10;


   switch(player.orb_phase)
   {
/*    case ORB_DOCK:
    case ORB_RETURN:
     bcircle(x, y, 6 + grand(3), 0);
     break;
    case ORB_SEEK:
     bcircle(x, y, 8 + grand(4), 0);
     break;
    case HAND_STOP:
     bcircle(x, y, 12 + grand(4), 0);
     break;*/
     default:
     bcircle(x, y, 8 + grand(3) + player.orb_unfold / 10, 2);
     bcircle3(x, y, player.orb_unfold, 2);
     break;

   }

}



void draw_HUD(void)
{

  int i, k, xa, ya;

 textprintf_right_ex(display, font, 600, 3, -1, -1, "%i", player.score);

// textprintf_right_ex(display, font, 600, 33, -1, -1, "%i", arena.number);
// textprintf_right_ex(display, font, 600, 43, -1, -1, "%i", arena.level_finished);

// textprintf_right_ex(display, font, 635, 45, -1, -1, "%i%%", arena.phase_bonus);

 for (i = 0; i < player.lives; i ++)
 {
  rectfill(display, 601 - i * 10 - 2, 23, 601 - i * 10 - 10, 31, COL_WHITE);
  rect(display, 601 - i * 10 - 2, 23, 601 - i * 10 - 10, 31, COL_OUTLINE);

 }

 int bar_colour;

 for (i = 0; i < 3; i ++)
 {
  xa = 2;
  ya = 15 * i + 1;
/*  rect(display, 2, ya, 58, ya + 14, COL_OUTLINE);
  rect(display, 3, ya + 1, 57, ya + 13, COL_LGREY);
  rect(display, 4, ya + 2, 56, ya + 12, COL_OUTLINE);
  TRANS_MODE
  if (player.rpixels [i] > 0)
  {
   if (player.rpixels [i] == 50)
   {
     rect(display, 5, ya + 3, 5 + player.rpixels [i], ya + 11, TRANS_RED3);
     rectfill(display, 6, ya + 4, 4 + player.rpixels [i], ya + 10, TRANS_RED5);
   }
    else
    {
     rect(display, 5, ya + 3, 5 + player.rpixels [i], ya + 11, TRANS_RED3);
     rectfill(display, 6, ya + 4, 4 + player.rpixels [i], ya + 10, TRANS_RED5);
    }
  }
  END_TRANS_MODE*/
/*  xa = 2;
  ya = 19 * i + 4;
  if (player.wlevel [i] < 9)
  {
   rect(display, 2, ya, 2 + player.wpixels2 [i] + 6, ya + 12, COL_OUTLINE);
   rect(display, 3, ya + 1, 2 + player.wpixels2 [i] + 5, ya + 11, COL_LGREY);
   rect(display, 4, ya + 2, 2 + player.wpixels2 [i] + 4, ya + 10, COL_OUTLINE);
   TRANS_MODE
   if (player.wpixels [i] > 0)
   {
    rect(display, 5, ya + 3, 5 + player.wpixels [i] + 1, ya + 9, TRANS_RED3);
    rectfill(display, 6, ya + 4, 5 + player.wpixels [i] + 0, ya + 8, TRANS_RED5);
   }
   END_TRANS_MODE
  }
  TRANS_MODE
  for (k = 0; k < player.wlevel [i]; k ++)
  {
   rect(display, 4 + k * 5, ya, 7 + k * 5, ya + 5, TRANS_RED3);
   rectfill(display, 5 + k * 5, ya + 1, 6 + k * 5, ya + 4, TRANS_RED5);
  }
  END_TRANS_MODE*/
#define WPIXELS2 20
  xa = 2;
  ya = 14 * i + 4;
  rect(display, 2, ya, 2 + WPIXELS2 + 6, ya + 12, COL_OUTLINE);
  rect(display, 3, ya + 1, 2 + WPIXELS2 + 5, ya + 11, COL_LGREY);
  rect(display, 4, ya + 2, 2 + WPIXELS2 + 4, ya + 10, COL_OUTLINE);
  TRANS_MODE
  bar_colour = TRANS_RED2;
  if (player.wflash [i] > 2)
   bar_colour = TRANS_RED3;
  if (player.wflash [i] > 4)
   bar_colour = TRANS_RED4;
  if (player.wflash [i] > 6)
   bar_colour = TRANS_RED5;
  if (player.wpixels [i] > 0)
  {
   rect(display, 5, ya + 3, 5 + player.wpixels [i] + 1, ya + 9, TRANS_RED5);
   rectfill(display, 6, ya + 4, 5 + player.wpixels [i] + 0, ya + 8, bar_colour);
  }
  if (player.wlevel [i] > player.weapon_level)
  {
   bar_colour = TRANS_RED1;
   if (player.bflash [i] > 3)
    bar_colour = TRANS_RED2;
   if (player.bflash [i] > 6)
    bar_colour = TRANS_RED3;
   if (player.bflash [i] > 9)
    bar_colour = TRANS_RED4;
   for (k = player.weapon_level; k < player.wlevel [i]; k ++)
   {
    rect(display, 30 + k * 4, ya + 3, 32 + k * 4, ya + 9, TRANS_RED4);
    rectfill(display, 31 + k * 4, ya + 4, 31 + k * 4, ya + 8, bar_colour);
   }
  }
  END_TRANS_MODE

 }

  if (player.weapon_level > 0)
  {
   bar_colour = TRANS_RED2;
   if (player.overbflash > 3)
    bar_colour = TRANS_RED3;
   if (player.overbflash > 6)
    bar_colour = TRANS_RED4;
   if (player.overbflash > 9)
    bar_colour = TRANS_RED5;
   if (player.overbflash > 0)
    textprintf_ex(display, font, 5, 43, -1, -1, "level %i!", player.weapon_level);
   for (k = 0; k < player.weapon_level; k ++)
   {
    rect(display, 30 + k * 4, 7, 32 + k * 4, 41, TRANS_RED5);
    rectfill(display, 31 + k * 4, 8, 31 + k * 4, 40, bar_colour);
   }
  }

 switch(player.type)
 {
    case PTYPE_BASIC:

#define BAR_Y 580
#define BAR_X 8

    rect(display, BAR_X, BAR_Y - 141, BAR_X + 21, BAR_Y + 1, COL_OUTLINE);
    rect(display, BAR_X + 1, BAR_Y - 140, BAR_X + 20, BAR_Y, COL_WHITE);
    rect(display, BAR_X + 2, BAR_Y - 139, BAR_X + 19, BAR_Y - 1, COL_OUTLINE);
    if (player.charge > 0)
    {
     TRANS_MODE
     if (player.charge >= 20000)
     {
      rectfill(display, BAR_X + 3, BAR_Y - 138, BAR_X + 18, BAR_Y - 2, beam_colours [0] [1 + (arena.counter / 4) % 4]);
     }
       else
       {
        ya = player.charge / 151;
        if (ya > 132)
         ya = 132;
        if (player.charge >= 6300)
         rectfill(display, BAR_X + 3, BAR_Y - 2 - ya, BAR_X + 18, BAR_Y - 2, TRANS_RED4);
          else
           rectfill(display, BAR_X + 3, BAR_Y - 2 - ya, BAR_X + 18, BAR_Y - 2, TRANS_RED2);
       }
     END_TRANS
    }

/*    rect(display, BAR_X, BAR_Y - 51, BAR_X + 11, BAR_Y + 1, COL_OUTLINE);
    rect(display, BAR_X + 1, BAR_Y - 50, BAR_X + 10, BAR_Y, COL_WHITE);
    rect(display, BAR_X + 2, BAR_Y - 49, BAR_X + 9, BAR_Y - 1, COL_OUTLINE);
    if (player.charge > 0)
    {
     TRANS_MODE
     if (player.charge >= 20000)
     {
      rectfill(display, BAR_X + 3, BAR_Y - 48, BAR_X + 8, BAR_Y - 2, beam_colours [0] [1 + (arena.counter / 4) % 4]);
     }
       else
       {
        ya = player.charge / 434;
        if (ya > 46)
         ya = 46;
        rectfill(display, BAR_X + 3, BAR_Y - 2 - ya, BAR_X + 8, BAR_Y - 2, TRANS_RED3);
       }
     END_TRANS
    }
*/     break;

    case PTYPE_BOMBER:

//#define BAR_Y 580
//#define BAR_X 8

    rect(display, BAR_X, BAR_Y - 141, BAR_X + 21, BAR_Y + 1, COL_OUTLINE);
    rect(display, BAR_X + 1, BAR_Y - 140, BAR_X + 20, BAR_Y, COL_WHITE);
    rect(display, BAR_X + 2, BAR_Y - 139, BAR_X + 19, BAR_Y - 1, COL_OUTLINE);
    vline(display, BAR_X + 10, BAR_Y - 140, BAR_Y, COL_WHITE);
    vline(display, BAR_X + 9, BAR_Y - 139, BAR_Y - 1, COL_OUTLINE);
    vline(display, BAR_X + 11, BAR_Y - 139, BAR_Y - 1, COL_OUTLINE);
      TRANS_MODE
      if (player.bomb_recycle1 <= 50)
      {
       rectfill(display, BAR_X + 3, BAR_Y - 138, BAR_X + 8, BAR_Y - 2, TRANS_RED5);
      }
        else
        {
         ya = (1000 - player.bomb_recycle1) / 7;
         if (ya > 136)
          ya = 136;
         if (ya < 1) ya = 1;
         rectfill(display, BAR_X + 3, BAR_Y - 2 - ya, BAR_X + 8, BAR_Y - 2, TRANS_RED3);
        }

      if (player.bomb_recycle2 <= 50)
      {
       rectfill(display, BAR_X + 12, BAR_Y - 138, BAR_X + 18, BAR_Y - 2, TRANS_RED5);
      }
        else
        {
         ya = (1000 - player.bomb_recycle2) / 7;
         if (ya > 136)
          ya = 136;
         if (ya < 1) ya = 1;
         rectfill(display, BAR_X + 12, BAR_Y - 2 - ya, BAR_X + 18, BAR_Y - 2, TRANS_RED3);
        }
      END_TRANS
    break;

 }

 if (boss.fight)
 {

  int line_x = (boss.hp * 100) / boss.max_hp;
  if (line_x > boss.hp_bar_size)
   line_x = boss.hp_bar_size;
  TRANS_MODE
//  if (boss.hp > 0)
  if (line_x > 0)
  {
   rectfill(display, 300 - line_x + 1, 10, 300 + line_x - 1, 15, TRANS_RED2);
   rect(display, 300 - line_x, 9, 300 + line_x, 16, TRANS_RED5);
  }
  END_TRANS_MODE
  line_x = boss.hp_bar_size;
  rect(display, 300 - 1 - line_x, 8, 300 + 1 + line_x, 17, COL_OUTLINE);
  rect(display, 300 - 2 - line_x, 7, 300 + 2 + line_x, 18, COL_LGREY);
  rect(display, 300 - 3 - line_x, 6, 300 + 3 + line_x, 19, COL_OUTLINE);

 }

}

void draw_enemies(void)
{

 int e;

  for (e = 0; e < NO_ENEMIES; e ++)
  {
   if (enemy[e].type == ENEMY_NONE)
    continue;
   if (eclass [enemy[e].type].priority == 0)
    draw_an_enemy(e);
  }

  for (e = 0; e < NO_ENEMIES; e ++)
  {
   if (enemy[e].type == ENEMY_NONE)
    continue;
   if (eclass [enemy[e].type].priority == 1)
    draw_an_enemy(e);
  }

  for (e = 0; e < NO_ENEMIES; e ++)
  {
   if (enemy[e].type == ENEMY_NONE)
    continue;
   if (eclass [enemy[e].type].priority == 2)
    draw_an_enemy(e);
  }

}


void draw_an_enemy(int e)
{

    int x = enemy[e].x >> 10;
    int y = enemy[e].y >> 10;
    int xa, ya;
    int angle;
    int i;

    switch(enemy[e].type)
    {
        default:
         circlefill(display, x, y, 10, COL_WHITE);
         break;
        case ENEMY_WIDE:
         switch(enemy[e].frame)
         {
          default:
           draw_rle_sprite(display, eRLE_large1 [L_ENEMY_WIDE1], x - 75, y - 25);
           break;
          case 1:
           draw_rle_sprite(display, eRLE_large1 [L_ENEMY_WIDE1_2], x - 76, y - 25);
           break;
          case 2:
           draw_rle_sprite(display, eRLE_large1 [L_ENEMY_WIDE1_3], x - 77, y - 25);
           break;
          case 3:
           draw_rle_sprite(display, eRLE_large1 [L_ENEMY_WIDE1_4], x - 78, y - 25);
           break;
         }
           xa = 8 + grand(3);
           ya = xa;
           bcircle(x - 14, y + 26 + ya, xa + grand(4), CLOUD_COL_2);
           bcircle(x + 14, y + 26 + ya, xa + grand(4), CLOUD_COL_2);
           xa -= 2 + grand(2);
           ya += xa;
           bcircle(x - 14, y + 26 + ya, xa + grand(4), CLOUD_COL_2);
           bcircle(x + 14, y + 26 + ya, xa + grand(4), CLOUD_COL_2);
           xa -= 2 + grand(2);
           ya += xa;
           bcircle(x - 14, y + 26 + ya, xa + grand(4), CLOUD_COL_2);
           bcircle(x + 14, y + 26 + ya, xa + grand(4), CLOUD_COL_2);
           break;
        case ENEMY_DOME2:
           draw_rle_sprite(display, eRLE_large1 [L_ENEMY_DOME2], x - 39, y - 72);
           xa = 10 + grand(3);
           ya = xa;
           bcircle(x, y + 20 + ya, xa + grand(6), CLOUD_COL_3);
           xa -= 2 + grand(2);
           ya += xa;
           bcircle(x, y + 20 + ya, xa + grand(6), CLOUD_COL_3);
           xa -= 2 + grand(2);
           ya += xa;
           bcircle(x, y + 20 + ya, xa + grand(6), CLOUD_COL_3);
           xa -= 2 + grand(2);
           ya += xa;
           bcircle(x, y + 20 + ya, xa + grand(6), CLOUD_COL_3);
           break;

        case ENEMY_BOSS2_2:
           draw_rle_sprite(display, eRLE_large2 [L_ENEMY2_BOSS2_2], x - 53, y - 32);
           circle(display, x, y - 15, 8, COL_COL2);
           circle(display, x, y + 17, 10, COL_COL2);
           circle(display, x - 33, y + 12, 6, COL_COL2);
           circle(display, x + 33, y + 12, 6, COL_COL2);

           xa = 18 + grand(3);
           ya = xa;
           bcircle(x, y + 28 + ya, xa + grand(6), CLOUD_COL_3);
           xa -= 3 + grand(4);
           ya += xa;
           bcircle(x, y + 28 + ya, xa + grand(6), CLOUD_COL_3);
           xa -= 3 + grand(4);
           ya += xa;
           bcircle(x, y + 28 + ya, xa + grand(6), CLOUD_COL_3);
           xa -= 3 + grand(4);
           ya += xa;
           bcircle(x, y + 28 + ya, xa + grand(6), CLOUD_COL_3);
           break;


        case ENEMY_BOSS2_1_DEAD:
           draw_rle_sprite(display, eRLE_large2 [L_ENEMY2_BOSS2_2], x - 53, y - 109 + (enemy[e].burst >> 10));
           circle(display, x, y - 92 + (enemy[e].burst >> 10), 8, COL_COL2);
           circle(display, x, y - 60 + (enemy[e].burst >> 10), 10, COL_COL2);
           circle(display, x - 33, y - 65 + (enemy[e].burst >> 10), 6, COL_COL2);
           circle(display, x + 33, y - 65 + (enemy[e].burst >> 10), 6, COL_COL2);

           xa = 18 + grand(3);
           ya = xa;
           bcircle(x, y - 45 + ya + (enemy[e].burst >> 10), xa + grand(6), CLOUD_COL_3);
           xa -= 3 + grand(4);
           ya += xa;
           bcircle(x, y - 45 + ya + (enemy[e].burst >> 10), xa + grand(6), CLOUD_COL_3);
           xa -= 3 + grand(4);
           ya += xa;
           bcircle(x, y - 45 + ya + (enemy[e].burst >> 10), xa + grand(6), CLOUD_COL_3);
           xa -= 3 + grand(4);
           ya += xa;
           bcircle(x, y - 45 + ya + (enemy[e].burst >> 10), xa + grand(6), CLOUD_COL_3);


           if (enemy[e].phase < -10)
           {
            x += grand(3) - grand(3);
            y += grand(3) - grand(3);
           }
           if (enemy[e].phase < -50)
           {
            x += grand(5) - grand(5);
            y += grand(5) - grand(5);
           }

           draw_rle_sprite(display, eRLE_large2 [L_ENEMY2_BOSS2_1], x - 160, y - 109);

           circlefill(display, x - 90, y + 27, 13, COL_COL3);
           circle(display, x - 90, y + 27, 13, COL_COL2);
           circlefill(display, x + 90, y + 27, 13, COL_COL3);
           circle(display, x + 90, y + 27, 13, COL_COL2);

           circlefill(display, x - 50, y - 13, 13, COL_COL3);
           circle(display, x - 50, y - 13, 13, COL_COL2);
           circlefill(display, x + 50, y - 13, 13, COL_COL3);
           circle(display, x + 50, y - 13, 13, COL_COL2);
         break;
        case ENEMY_BOSS2_1:
           draw_rle_sprite(display, eRLE_large2 [L_ENEMY2_BOSS2_2], x - 53, y - 109);
           draw_rle_sprite(display, eRLE_large2 [L_ENEMY2_BOSS2_1], x - 160, y - 109);
           draw_turret(x - 90, y + 27, 10, 13, enemy[e].aim [0], 8, 2, COL_COL4, COL_COL3, COL_COL2);
           draw_turret(x + 90, y + 27, 10, 13, enemy[e].aim [1], 8, 2, COL_COL4, COL_COL3, COL_COL2);
           draw_turret(x - 50, y - 13, 10, 13, enemy[e].aim [2], 8, 2, COL_COL4, COL_COL3, COL_COL2);
           draw_turret(x + 50, y - 13, 10, 13, enemy[e].aim [3], 8, 2, COL_COL4, COL_COL3, COL_COL2);
//           draw_turret(x, y - 92, 7, 9, 0, 0, 0, COL_COL4, COL_COL3, COL_COL2);
           circle(display, x, y - 92, 8, COL_COL2);
           circle(display, x, y - 60, 10, COL_COL2);
           circle(display, x - 33, y - 65, 6, COL_COL2);
           circle(display, x + 33, y - 65, 6, COL_COL2);
           bcircle(x, y + 116, 14 + grand(6), CLOUD_COL_3);
           bcircle(x - 111, y + 95, 10 + grand(6), CLOUD_COL_3);
           bcircle(x + 111, y + 95, 10 + grand(6), CLOUD_COL_3);

           xa = 10 + grand(5);
           ya = xa;
           bcircle(x, y + 116 + ya, xa + grand(6) - 6, CLOUD_COL_3);
           bcircle(x - 111, y + 95 + ya, xa + grand(6) - 8, CLOUD_COL_3);
           bcircle(x + 111, y + 95 + ya, xa + grand(6) - 8, CLOUD_COL_3);
           xa -= 3 + grand(4);
           ya += xa;
           bcircle(x, y + 116 + ya, xa + grand(6) - 4, CLOUD_COL_3);
           bcircle(x - 111, y + 95 + ya, xa + grand(6) - 6, CLOUD_COL_3);
           bcircle(x + 111, y + 95 + ya, xa + grand(6) - 6, CLOUD_COL_3);
           xa -= 3 + grand(4);
           ya += xa;
           bcircle(x, y + 116 + ya, xa + grand(6) - 3, CLOUD_COL_3);
           bcircle(x - 111, y + 95 + ya, xa + grand(6) - 3, CLOUD_COL_3);
           bcircle(x + 111, y + 95 + ya, xa + grand(6) - 3, CLOUD_COL_3);

//           bcircle(x, y + 6, 14 + grand(6), CLOUD_COL_1);
           break;
        case ENEMY_BOSS1_3:
           draw_rle_sprite(display, eRLE_large2 [L_ENEMY2_BOSS1_3], x - 148, y - 56);

           draw_turret(x - 76, y - 11, 8, 12, enemy[e].aim [0], 6, 3, COL_COL4, COL_COL3, COL_COL2);
           draw_turret(x + 76, y - 11, 8, 12, enemy[e].aim [1], 6, 3, COL_COL4, COL_COL3, COL_COL2);
           draw_turret(x - 99, y + 42, 10, 14, enemy[e].aim [0], 0, 0, COL_COL4, COL_COL3, COL_COL2);
           draw_turret(x + 99, y + 42, 10, 14, enemy[e].aim [1], 0, 0, COL_COL4, COL_COL3, COL_COL2);

          xa = 9 + grand(8);
          ya = 0;
          bcircle(x - 113, y + 84 + ya, xa, 1);
          bcircle(x + 113, y + 84 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(4);
          bcircle(x - 113, y + 91 + ya, xa, 1);
          bcircle(x + 113, y + 91 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(4);
          bcircle(x - 113, y + 98 + ya, xa, 1);
          bcircle(x + 113, y + 98 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(4);
          bcircle(x - 113, y + 105 + ya, xa, 1);
          bcircle(x + 113, y + 105 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(4);
          bcircle(x - 113, y + 112 + ya, xa, 1);
          bcircle(x + 113, y + 112 + ya, xa, 1);

//           draw_turret(x, y + 41, 10, 14, 0, 0, 0, COL_COL4, COL_COL3, COL_COL2);
/*          xa = 8 + grand(5);
          ya = 0;
          bcircle(x - 53, y + 66 + ya, xa, 1);
          bcircle(x + 53, y + 66 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(3);
          bcircle(x - 53, y + 73 + ya, xa, 1);
          bcircle(x + 53, y + 73 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(3);
          bcircle(x - 53, y + 80 + ya, xa, 1);
          bcircle(x + 53, y + 80 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(3);
          bcircle(x - 53, y + 87 + ya, xa, 1);
          bcircle(x + 53, y + 87 + ya, xa, 1);

          xa = 6 + grand(4);
          ya = 0;
          bcircle(x - 95, y + 54 + ya, xa, 1);
          bcircle(x + 95, y + 54 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(3);
          bcircle(x - 95, y + 59 + ya, xa, 1);
          bcircle(x + 95, y + 59 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(3);
          bcircle(x - 95, y + 64 + ya, xa, 1);
          bcircle(x + 95, y + 64 + ya, xa, 1);
*/
           break;
        case ENEMY_BOSS1_2:
           draw_rle_sprite(display, eRLE_large2 [L_ENEMY2_BOSS1_2], x - 148, y - 56);

           draw_turret(x - 76, y - 11, 8, 12, enemy[e].aim [0], 6, 3, COL_COL4, COL_COL3, COL_COL2);
           draw_turret(x + 76, y - 11, 8, 12, enemy[e].aim [1], 6, 3, COL_COL4, COL_COL3, COL_COL2);
           draw_turret(x, y + 41, 10, 14, 0, 0, 0, COL_COL4, COL_COL3, COL_COL2);
          xa = 8 + grand(5);
          ya = 0;
          bcircle(x - 53, y + 66 + ya, xa, 1);
          bcircle(x + 53, y + 66 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(3);
          bcircle(x - 53, y + 73 + ya, xa, 1);
          bcircle(x + 53, y + 73 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(3);
          bcircle(x - 53, y + 80 + ya, xa, 1);
          bcircle(x + 53, y + 80 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(3);
          bcircle(x - 53, y + 87 + ya, xa, 1);
          bcircle(x + 53, y + 87 + ya, xa, 1);

          xa = 6 + grand(4);
          ya = 0;
          bcircle(x - 95, y + 54 + ya, xa, 1);
          bcircle(x + 95, y + 54 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(3);
          bcircle(x - 95, y + 59 + ya, xa, 1);
          bcircle(x + 95, y + 59 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(3);
          bcircle(x - 95, y + 64 + ya, xa, 1);
          bcircle(x + 95, y + 64 + ya, xa, 1);

           break;
        case ENEMY_BOSS1_1:
           draw_rle_sprite(display, eRLE_large1 [L_ENEMY_BOSS1_1], x - 148, y - 56);

           draw_turret(x - 76, y - 11, 8, 12, enemy[e].aim [0], 6, 3, COL_COL4, COL_COL3, COL_COL2);
           draw_turret(x + 76, y - 11, 8, 12, enemy[e].aim [1], 6, 3, COL_COL4, COL_COL3, COL_COL2);
          xa = 8 + grand(5);
          ya = 0;
          bcircle(x - 53, y + 66 + ya, xa, 1);
          bcircle(x + 53, y + 66 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(3);
          bcircle(x - 53, y + 73 + ya, xa, 1);
          bcircle(x + 53, y + 73 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(3);
          bcircle(x - 53, y + 80 + ya, xa, 1);
          bcircle(x + 53, y + 80 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(3);
          bcircle(x - 53, y + 87 + ya, xa, 1);
          bcircle(x + 53, y + 87 + ya, xa, 1);

          xa = 6 + grand(4);
          ya = 0;
          bcircle(x - 95, y + 37 + ya, xa, 1);
          bcircle(x + 95, y + 37 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(3);
          bcircle(x - 95, y + 42 + ya, xa, 1);
          bcircle(x + 95, y + 42 + ya, xa, 1);
          xa -= 1 + grand(3);
          ya += grand(3);
          bcircle(x - 95, y + 47 + ya, xa, 1);
          bcircle(x + 95, y + 47 + ya, xa, 1);

           break;
        case ENEMY_BOSS3_0:
           draw_rle_sprite(display, eRLE_large2 [L_ENEMY2_BOSS3_0], x - 128, y - 93);
           draw_turret(x - 71, y - 12, 8, 10, 0, 0, 0, COL_COL4, COL_COL3, COL_COL2);
           draw_turret(x + 71, y - 12, 8, 10, 0, 0, 0, COL_COL4, COL_COL3, COL_COL2);
           break;
        case ENEMY_BOSS3_1:
           draw_rle_sprite(display, eRLE_large2 [L_ENEMY2_BOSS3_1], x - 147, y - 85);

           draw_turret(x - 55, y + 7, 10, 13, enemy[e].aim [0], 0, 0, COL_COL4, COL_COL3, COL_COL2);
           draw_turret(x - 106, y + 65, 9, 12, enemy[e].aim [0], 7, 2, COL_COL4, COL_COL3, COL_COL2);

           draw_turret(x + 55, y + 7, 10, 13, enemy[e].aim [0], 0, 0, COL_COL4, COL_COL3, COL_COL2);
           draw_turret(x + 106, y + 65, 9, 12, enemy[e].aim [1], 7, 2, COL_COL4, COL_COL3, COL_COL2);

           xa = 14 + grand(3);
           ya = xa;
           bcircle(x - 55, y + 28 + ya, xa + grand(6), CLOUD_COL_3);
           bcircle(x + 55, y + 28 + ya, xa + grand(6), CLOUD_COL_3);
           xa -= 3 + grand(3);
           ya += xa;
           bcircle(x - 55, y + 28 + ya, xa + grand(6), CLOUD_COL_3);
           bcircle(x + 55, y + 28 + ya, xa + grand(6), CLOUD_COL_3);
           xa -= 3 + grand(3);
           ya += xa;
           bcircle(x - 55, y + 28 + ya, xa + grand(6), CLOUD_COL_3);
           bcircle(x + 55, y + 28 + ya, xa + grand(6), CLOUD_COL_3);
           xa -= 3 + grand(3);
           ya += xa;
           bcircle(x - 55, y + 28 + ya, xa + grand(6), CLOUD_COL_4);
           bcircle(x + 55, y + 28 + ya, xa + grand(6), CLOUD_COL_4);
           break;
        case ENEMY_BOSS3_2:
           draw_rle_sprite(display, eRLE_large2 [L_ENEMY2_BOSS3_2], x - 144, y - 95);

           draw_turret(x, y, 12, 15, 0, 0, 0, COL_COL4, COL_COL3, COL_COL2);

           draw_turret(x - 78, y - 8, 10, 13, enemy[e].aim [0], 8, 3, COL_COL4, COL_COL3, COL_COL2);
           draw_turret(x + 78, y - 8, 10, 13, enemy[e].aim [1], 8, 3, COL_COL4, COL_COL3, COL_COL2);

/*           draw_turret(x - 55, y + 7, 10, 13, enemy[e].aim [0], 0, 0, COL_COL4, COL_COL3, COL_COL2);
           draw_turret(x - 106, y + 65, 9, 12, enemy[e].aim [0], 7, 2, COL_COL4, COL_COL3, COL_COL2);

           draw_turret(x + 55, y + 7, 10, 13, enemy[e].aim [0], 0, 0, COL_COL4, COL_COL3, COL_COL2);
           draw_turret(x + 106, y + 65, 9, 12, enemy[e].aim [1], 7, 2, COL_COL4, COL_COL3, COL_COL2);*/

           xa = 14 + grand(3);
           ya = xa;
           bcircle(x - 81, y + 52 + ya, xa + grand(6), CLOUD_COL_4);
           bcircle(x + 81, y + 52 + ya, xa + grand(6), CLOUD_COL_4);
           xa -= 3 + grand(3);
           ya += xa;
           bcircle(x - 81, y + 52 + ya, xa + grand(6), CLOUD_COL_4);
           bcircle(x + 81, y + 52 + ya, xa + grand(6), CLOUD_COL_4);
           xa -= 3 + grand(3);
           ya += xa;
           bcircle(x - 81, y + 52 + ya, xa + grand(6), CLOUD_COL_4);
           bcircle(x + 81, y + 52  + ya, xa + grand(6), CLOUD_COL_4);
           xa -= 3 + grand(3);
           ya += xa;
           bcircle(x - 81, y + 52 + ya, xa + grand(6), CLOUD_COL_4);
           bcircle(x + 81, y + 52 + ya, xa + grand(6), CLOUD_COL_4);
           break;
        case ENEMY_MACHINE:
         switch(enemy[e].frame)
         {
          default:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MACHINE], x - 21, y - 31);
           break;
          case 1:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MACHINE_2], x - 23, y - 31);
           break;
          case 2:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MACHINE_3], x - 25, y - 31);
           break;
          case 3:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MACHINE_4], x - 27, y - 31);
           break;
         }
         xa = 6 + grand(4);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1, y - xa - 1 + 12);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [xa], x - xa - 1, y - xa - 1 + 16);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1, y - xa - 1 + 20);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1, y - xa - 1 + 24);
           break;
        case ENEMY_MACHINE2:
         switch(enemy[e].frame)
         {
          default:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MACHINE2], x - 42, y - 21);
           break;
          case 1:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MACHINE2_2], x - 43, y - 21);
           break;
          case 2:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MACHINE2_3], x - 44, y - 21);
           break;
          case 3:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MACHINE2_4], x - 45, y - 21);
           break;
         }
         xa = 6 + grand(4);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 - 16 - enemy[e].frame, y - xa - 1 + 28);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 + 16 + enemy[e].frame, y - xa - 1 + 28);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [xa], x - xa - 1 - 16 - enemy[e].frame, y - xa - 1 + 32);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [xa], x - xa - 1 + 16 + enemy[e].frame, y - xa - 1 + 32);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1 - 16 - enemy[e].frame, y - xa - 1 + 36);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1 + 16 + enemy[e].frame, y - xa - 1 + 36);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1 - 16 - enemy[e].frame, y - xa - 1 + 40);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1 + 16 + enemy[e].frame, y - xa - 1 + 40);
           break;

        case ENEMY_MACHINE3:
         switch(enemy[e].frame)
         {
          default:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MACHINE3], x - 32, y - 15);
           break;
          case 1:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MACHINE3_2], x - 33, y - 15);
           break;
          case 2:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MACHINE3_3], x - 34, y - 15);
           break;
          case 3:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MACHINE3_4], x - 35, y - 15);
           break;
/*          case 1:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MACHINE2_2], x - 43, y - 21);
           break;
          case 2:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MACHINE2_3], x - 44, y - 21);
           break;
          case 3:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MACHINE2_4], x - 45, y - 21);
           break;*/
         }
         xa = 5 + grand(3);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 - 19 - enemy[e].frame, y - xa - 1 + 44 + enemy[e].frame);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 + 19 + enemy[e].frame, y - xa - 1 + 44 + enemy[e].frame);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [xa], x - xa - 1 - 19 - enemy[e].frame, y - xa - 1 + 46 + enemy[e].frame);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [xa], x - xa - 1 + 19 + enemy[e].frame, y - xa - 1 + 46 + enemy[e].frame);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1 - 19 - enemy[e].frame, y - xa - 1 + 49 + enemy[e].frame);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1 + 19 + enemy[e].frame, y - xa - 1 + 49 + enemy[e].frame);
           break;
        case ENEMY_MEDIUM:
         xa = 4 + grand(4);
         switch(enemy[e].frame)
         {
          default:
           case 3:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MEDIUM], x - 20, y - 37);
           break;
          case 2:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MEDIUM_2], x - 20, y - 37);
           xa += 1;
           break;
          case 1:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MEDIUM_3], x - 20, y - 37);
           xa += 1;
           break;
          case 0:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_MEDIUM_4], x - 20, y - 37);
           xa += 1 + grand(3);
           break;
         }
         draw_trans_rle_sprite(display, RLE_ccircle_basic [2] [xa], x - xa - 1, y - xa - 1 + 12);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [2] [xa], x - xa - 1, y - xa - 1 + 16);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [2] [xa], x - xa - 1, y - xa - 1 + 21);
           break;
        case ENEMY_FIGHTER4:
         xa = 4 + grand(3);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 - 7, y - xa - 1 + 19);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 + 7, y - xa - 1 + 19);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [xa], x - xa - 1 - 7, y - xa - 1 + 23);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [xa], x - xa - 1 + 7, y - xa - 1 + 23);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1 - 7, y - xa - 1 + 27);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1 + 7, y - xa - 1 + 27);
         draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER4], x - 19, y - 6);
         if (enemy[e].recycle < 45)
         {
          bcircle(x, y + 4, 15 - enemy[e].recycle / 3, CLOUD_COL_3);
          angle = enemy[e].recycle * 12;
          if (e & 1)
           angle = ANGLE_1 - (angle & 1023);
          for (i = 0; i < 8; i ++)
          {
              angle += ANGLE_8;
              xa = enemy[e].recycle * 2;
              bcircle4(x + xpart(angle, xa), y + 4 + ypart(angle, xa), 5 - enemy[e].recycle / 10, CLOUD_COL_3);

          }
          angle = ANGLE_1 - angle;
          for (i = 0; i < 8; i ++)
          {
              angle += ANGLE_8;
              xa = enemy[e].recycle;
              bcircle4(x + xpart(angle, xa), y + 4 + ypart(angle, xa), 5 - enemy[e].recycle / 10, CLOUD_COL_3);

          }
//          draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [47], x - xa - 1 - 7, y - xa - 1 + 27);
         }
         break;
        case ENEMY_FIGHTER3:
         xa = 4 + grand(3);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 - 7, y - xa - 1 + 10);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 + 7, y - xa - 1 + 10);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [xa], x - xa - 1 - 7, y - xa - 1 + 14);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [xa], x - xa - 1 + 7, y - xa - 1 + 14);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1 - 7, y - xa - 1 + 18);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1 + 7, y - xa - 1 + 18);
         switch(enemy[e].frame)
         {
          default:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER3], x - 17, y - 10); break;
          case 1:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER3_2], x - 18, y - 10); break;
          case 2:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER3_3], x - 19, y - 10); break;
         }

         break;
        case ENEMY_FIGHTER2:
         switch(enemy[e].frame)
         {
          default:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER2], x - 20, y - 14); break;
          case 1:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER2_2], x - 20, y - 14); break;
          case 2:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER2_3], x - 20, y - 14); break;
          case 3:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER2_4], x - 21, y - 14); break;
         }

         xa = 4 + grand(4);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1, y - xa - 1 + 5);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [xa], x - xa - 1, y - xa - 1 + 9);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1, y - xa - 1 + 13);
/*         if (enemy[e].status > 0)
         {
          xa = 1 + enemy[e].status / 3 + grand(4);
          draw_trans_rle_sprite(display, RLE_ccircle_basic [CLOUD_COL_2] [xa], x - xa - 1, y - xa - 1);
         }*/
         break;
        case ENEMY_FIGHTER:
         switch(enemy[e].frame)
         {
          default:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER], x - 14, y - 13); break;
          case 1:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER_2], x - 14, y - 13); break;
          case 2:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER_3], x - 14, y - 13); break;
         }

         xa = 4 + grand(4);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1, y - xa - 1 + 8);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [xa], x - xa - 1, y - xa - 1 + 12);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1, y - xa - 1 + 16);
         if (enemy[e].status > 0)
         {
          xa = 1 + enemy[e].status / 3 + grand(4);
          draw_trans_rle_sprite(display, RLE_ccircle_basic [CLOUD_COL_2] [xa], x - xa - 1, y - xa - 1);
         }
         break;
        case ENEMY_FIGHTER5:
         switch(enemy[e].frame)
         {
          default:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER5], x - 19, y - 17);
           break;
          case 1:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER5_2], x - 20, y - 17);
           break;
          case 2:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER5_3], x - 21, y - 17);
           break;
         }
         xa = 4 + grand(4);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1, y - xa - 1 + 4);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [xa], x - xa - 1, y - xa - 1 + 8);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1, y - xa - 1 + 12);
//        line(display, x, y, enemy[e].x_target [0] >> 10, enemy[e].y_target [0] >> 10, COL_WHITE);
//        pline(display, enemy[e].x_target [0] >> 10, enemy[e].y_target [0] >> 10, enemy[e].x_target [1] >> 10, enemy[e].y_target [1] >> 10, COL_WHITE);
        //pline(display, enemy[e].x_target [1] >> 10, enemy[e].y_target [1] >> 10, enemy[e].x_target [2] >> 10, enemy[e].y_target [2] >> 10, COL_WHITE);
         break;

        case ENEMY_FIGHTER6:
/*         switch(enemy[e].frame)
         {
          default:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER5], x - 19, y - 17);
           break;
          case 1:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER5_2], x - 20, y - 17);
           break;
          case 2:
           draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER5_3], x - 21, y - 17);
           break;
         }*/
         draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER6], x - 18, y - 16);
         xa = 4 + grand(3);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [2] [xa], x - xa - 1, y - xa - 1 + 18);
//         draw_trans_rle_sprite(display, RLE_ccircle_basic [2] [xa], x - xa - 1 - 12, y - xa - 1 + 20);
//         draw_trans_rle_sprite(display, RLE_ccircle_basic [2] [xa], x - xa - 1 + 12, y - xa - 1 + 20);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [2] [xa], x - xa - 1, y - xa - 1 + 23);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [2] [xa], x - xa - 1 - 12, y - xa - 1 + 18);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [2] [xa], x - xa - 1 + 12, y - xa - 1 + 18);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [2] [xa], x - xa - 1, y - xa - 1 + 27);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [2] [xa], x - xa - 1 - 12, y - xa - 1 + 21);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [2] [xa], x - xa - 1 + 12, y - xa - 1 + 21);
//        line(display, x, y, enemy[e].x_target [0] >> 10, enemy[e].y_target [0] >> 10, COL_WHITE);
//        pline(display, enemy[e].x_target [0] >> 10, enemy[e].y_target [0] >> 10, enemy[e].x_target [1] >> 10, enemy[e].y_target [1] >> 10, COL_WHITE);
        //pline(display, enemy[e].x_target [1] >> 10, enemy[e].y_target [1] >> 10, enemy[e].x_target [2] >> 10, enemy[e].y_target [2] >> 10, COL_WHITE);
         break;

        case ENEMY_FIGHTER7:
         draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER7], x - 21, y - 13);
         xa = 4 + grand(4);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [2] [xa], x - xa - 1 - 10, y - xa - 1 + 17);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [2] [xa], x - xa - 1 - 10, y - xa - 1 + 21);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [2] [xa], x - xa - 1 - 10, y - xa - 1 + 25);

         xa = 4 + grand(4);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [2] [xa], x - xa - 1 + 10, y - xa - 1 + 17);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [2] [xa], x - xa - 1 + 10, y - xa - 1 + 21);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [2] [xa], x - xa - 1 + 10, y - xa - 1 + 25);

         /*         xa = 4 + grand(4);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [2] [xa], x - xa - 1 - 10, y - xa - 1 + 17);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [2] [xa], x - xa - 1 + 10, y - xa - 1 + 17);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [2] [xa], x - xa - 1 - 10, y - xa - 1 + 21);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [2] [xa], x - xa - 1 + 10, y - xa - 1 + 21);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [2] [xa], x - xa - 1 - 10, y - xa - 1 + 25);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [2] [xa], x - xa - 1 + 10, y - xa - 1 + 25);*/
         break;

        case ENEMY_FIGHTER8:
         draw_rle_sprite(display, eRLE_small1 [S_ENEMY_FIGHTER8], x - 13, y - 12);
         xa = 4 + grand(6);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [2] [xa], x - xa - 1, y - xa - 1 + 16);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [2] [xa], x - xa - 1, y - xa - 1 + 19);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [2] [xa], x - xa - 1, y - xa - 1 + 23);
         break;

        case ENEMY_DOUBLE3:
           draw_rle_sprite(display, eRLE_large1 [L_ENEMY_DOUBLE3], x - 81, y - 40);

           draw_turret(x - 18, y - 17, 7, 9, enemy[e].aim [0], 5, 3, COL_COL4, COL_COL3, COL_COL2);
           draw_turret(x + 18, y - 17, 7, 9, enemy[e].aim [1], 5, 3, COL_COL4, COL_COL3, COL_COL2);

         xa = 8 + grand(5);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 + 42, y - xa - 1 + 35);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 - 42, y - xa - 1 + 35);
         xa -= 2 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 + 42, y - xa - 1 + 42);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 - 42, y - xa - 1 + 42);
         xa -= 2 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 + 42, y - xa - 1 + 49);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 - 42, y - xa - 1 + 49);
           break;

        case ENEMY_DOME3:
           draw_rle_sprite(display, eRLE_large1 [L_ENEMY_DOME3], x - 41, y - 49);
           angle = enemy[e].angle;
           circle(display, x, y - 19, 19, COL_COL3);
           for (i = 0; i < 10; i ++)
           {

            circlefill(display, x + xpart(angle, 19), y - 19 + ypart(angle, 19), 3, COL_COL4);
            circle(display, x + xpart(angle, 19), y - 19 + ypart(angle, 19), 3, COL_COL3);

            angle += ANGLE_10;


           }
           break;


        case ENEMY_DOUBLE4:
           draw_rle_sprite(display, eRLE_large1 [L_ENEMY_DOUBLE4], x - 96, y - 53);

           draw_turret(x - 69, y + 19, 7, 9, enemy[e].aim [0], 5, 3, COL_COL4, COL_COL3, COL_COL2);
           draw_turret(x + 69, y + 19, 7, 9, enemy[e].aim [1], 5, 3, COL_COL4, COL_COL3, COL_COL2);

         xa = 8 + grand(5);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 + 44, y - xa - 1 + 60);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 - 44, y - xa - 1 + 60);
         xa -= 2 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 + 44, y - xa - 1 + 67);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 - 44, y - xa - 1 + 67);
         xa -= 2 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 + 44, y - xa - 1 + 74);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 - 44, y - xa - 1 + 74);
           break;

        case ENEMY_DOUBLE2:
           draw_rle_sprite(display, eRLE_large1 [L_ENEMY_DOUBLE2], x - 72, y - 36);
           circlefill(display, x - 19, y - 10, 10, COL_COL3);
           circle(display, x - 19, y - 10, 10, COL_COL2);
           circlefill(display, x - 19, y - 10, 7, COL_COL4);
           circle(display, x - 19, y - 10, 7, COL_COL2);
           circlefill(display, x - 19 + xpart(enemy[e].aim [0], 6), y - 10 + ypart(enemy[e].aim [0], 6), 2, COL_COL4);
           circle(display, x - 19 + xpart(enemy[e].aim [0], 6), y - 10 + ypart(enemy[e].aim [0], 6), 2, COL_COL2);


           circlefill(display, x + 19, y - 10, 10, COL_COL3);
           circle(display, x + 19, y - 10, 10, COL_COL2);
           circlefill(display, x + 19, y - 10, 7, COL_COL4);
           circle(display, x + 19, y - 10, 7, COL_COL2);
           circlefill(display, x + 19 + xpart(enemy[e].aim [1], 6), y - 10 + ypart(enemy[e].aim [1], 6), 2, COL_COL4);
           circle(display, x + 19 + xpart(enemy[e].aim [1], 6), y - 10 + ypart(enemy[e].aim [1], 6), 2, COL_COL2);

         xa = 8 + grand(5);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 + 43, y - xa - 1 + 31);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 - 43, y - xa - 1 + 31);
         xa -= 2 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 + 43, y - xa - 1 + 38);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 - 43, y - xa - 1 + 38);
         xa -= 2 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 + 43, y - xa - 1 + 45);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1 - 43, y - xa - 1 + 45);
           break;

        case ENEMY_DOUBLE:
           draw_rle_sprite(display, eRLE_large1 [L_ENEMY_DOUBLE], x - 32, y - 36);
           circlefill(display, x - 21, y + 10, 5, COL_COL4);
           circle(display, x - 21, y + 10, 5, COL_COL2);
           circlefill(display, x - 21 + xpart(enemy[e].aim [0], 4), y + 10 + ypart(enemy[e].aim [0], 4), 2, COL_COL4);
           circle(display, x - 21 + xpart(enemy[e].aim [0], 4), y + 10 + ypart(enemy[e].aim [0], 4), 2, COL_COL2);
/*           angle = (enemy[e].aim [0] / 4) & 0xff;
           xa = RLE_barrel1 [angle].x;
           ya = RLE_barrel1 [angle].y;
           draw_rle_sprite(display, RLE_barrel1 [angle].sprite, x - 21 + xpart(enemy[e].aim [0], 7) - xa, y + 10 + ypart(enemy[e].aim [0], 7) - ya);
     //textprintf_centre_ex(display, font, x, y, -1, -1, "%i, %i, %i", angle, xa, ya);
           circlefill(display, x - 21, y + 10, 5, COL_COL4);
           circle(display, x - 21, y + 10, 5, COL_COL1);*/

           circlefill(display, x + 21, y + 10, 5, COL_COL4);
           circle(display, x + 21, y + 10, 5, COL_COL2);
           circlefill(display, x + 21 + xpart(enemy[e].aim [1], 4), y + 10 + ypart(enemy[e].aim [1], 4), 2, COL_COL4);
           circle(display, x + 21 + xpart(enemy[e].aim [1], 4), y + 10 + ypart(enemy[e].aim [1], 4), 2, COL_COL2);

         xa = 5 + grand(4);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1, y - xa - 1 + 12);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [xa], x - xa - 1, y - xa - 1 + 16);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1, y - xa - 1 + 20);
         break;

        case ENEMY_DOME:
           draw_rle_sprite(display, eRLE_large1 [L_ENEMY_DOME], x - 34, y - 35);
         if (enemy[e].phase == 1 || enemy[e].phase == 2)
          xa = 7 + grand(4);
           else
            xa = 6 + grand(3);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [xa], x - xa - 1, y - xa - 1 + 37);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [xa], x - xa - 1, y - xa - 1 + 43);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1, y - xa - 1 + 49);
         if (enemy[e].phase == 1 || enemy[e].phase == 2)
         {
          xa -= 1 + grand(2);
          draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [xa], x - xa - 1, y - xa - 1 + 55);
         }
           break;
        case ENEMY_BIG:
           draw_rle_sprite(display, eRLE_large1 [L_ENEMY_BIG], x - 34, y - 35);
           break;
        case ENEMY_BIG2:
           draw_rle_sprite(display, eRLE_large1 [L_ENEMY_BIG2], x - 72, y - 50);
           bcircle(x, y + 95, 18 + grand(6), CLOUD_COL_1);
//           bcircle(x, y + 121, 10 + grand(5), CLOUD_COL_1);
           break;
        case ENEMY_BIG3:
           draw_rle_sprite(display, eRLE_large1 [L_ENEMY_BIG3], x - 89, y - 51);
           bcircle(x - 75, y + 6, 14 + grand(6), CLOUD_COL_1);
           bcircle(x + 75, y + 6, 14 + grand(6), CLOUD_COL_1);
           circlefill(display, x, y - 26, 12, COL_COL3);
           circle(display, x, y - 26, 12, COL_COL2);
           circlefill(display, x, y - 26, 9, COL_COL4);
           circle(display, x, y - 26, 9, COL_COL2);
           break;
        case ENEMY_BIG4:
           draw_rle_sprite(display, eRLE_large1 [L_ENEMY_BIG4], x - 60, y - 49);
           break;
        case ENEMY_RISER:
         ya = S_ENEMY_RISER_5 - enemy[e].frame;
         draw_rle_sprite(display, eRLE_small1 [ya], x - 18, y - 8);

         xa = 4 + grand(3) - enemy[e].frame / 2;
         draw_trans_rle_sprite(display, RLE_ccircle_basic [2] [xa], x - xa - 1 - 16, y - xa - 1 + 16);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [2] [xa], x - xa - 1 + 16, y - xa - 1 + 16);
         xa -= 1 + grand(2);
         if (xa > 1)
         {
          draw_trans_rle_sprite(display, RLE_ccircle_4cols [2] [xa], x - xa - 1 - 19, y - xa - 1 + 19);
          draw_trans_rle_sprite(display, RLE_ccircle_4cols [2] [xa], x - xa - 1 + 19, y - xa - 1 + 19);
          xa -= 1 + grand(2);
          if (xa > 1)
          {
           draw_trans_rle_sprite(display, RLE_ccircle_3cols [2] [xa], x - xa - 1 - 22, y - xa - 1 + 22);
           draw_trans_rle_sprite(display, RLE_ccircle_3cols [2] [xa], x - xa - 1 + 22, y - xa - 1 + 22);
          }
         }

/*         xa = 4 + grand(4);
         draw_trans_rle_sprite(display, RLE_ccircle_basic [2] [xa], x - xa - 1, y - xa - 1 + 22);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_4cols [2] [xa], x - xa - 1, y - xa - 1 + 26);
         xa -= 1 + grand(2);
         draw_trans_rle_sprite(display, RLE_ccircle_3cols [2] [xa], x - xa - 1, y - xa - 1 + 30);
*/
           break;

    }

// print_number(x, y + 40, enemy[e].hp);
//

//#define CBOXES

#ifdef CBOXES
    rect(display, x - eclass[enemy[e].type].xsize / 1000 + eclass[enemy[e].type].xoffset / 1000,
                  y - eclass[enemy[e].type].ysize / 1000 + eclass[enemy[e].type].yoffset / 1000,
                  x + eclass[enemy[e].type].xsize / 1000 + eclass[enemy[e].type].xoffset / 1000,
                  y + eclass[enemy[e].type].ysize / 1000 + eclass[enemy[e].type].yoffset / 1000, COL_WHITE);


 for (i = 0; i < 7; i ++)
 {
  if (eclass[enemy[e].type].cbox [i] [0] == -1)
   break;
    rect(display, x + eclass[enemy[e].type].cbox [i] [0],
                  y + eclass[enemy[e].type].cbox [i] [1],
                  x + eclass[enemy[e].type].cbox [i] [2],
                  y + eclass[enemy[e].type].cbox [i] [3], COL_LGREY);
 }
#endif

}

void draw_turret(int x, int y, int d1, int d2, int angle, int angle_rad, int angle_size, int col1, int col2, int col3)
{

           if (d2 > 0)
           {
            circlefill(display, x, y, d2, col2);
            circle(display, x, y, d2, col3);
           }
           circlefill(display, x, y, d1, col1);
           circle(display, x, y, d1, col3);
           if (angle_size > 0)
           {
            circlefill(display, x + xpart(angle, angle_rad), y + ypart(angle, angle_rad), angle_size, col1);
            circle(display, x + xpart(angle, angle_rad), y + ypart(angle, angle_rad), angle_size, col3);
           }
//           circle(display, x - 76 + xpart(enemy[e].aim [0], 5), y - 11 + ypart(enemy[e].aim [0], 5), 3, COL_COL2);

}

void draw_ebullets(void)
{

 int b;

  for (b = 0; b < NO_EBULLETS; b ++)
  {
   if (ebullet[b].type == EBULLET_NONE)
    continue;
   draw_an_ebullet(b);
  }

}

void draw_an_ebullet(int b)
{

 int x = ebullet[b].x >> 10;
 int y = ebullet[b].y >> 10;
 int xb, yb, xa, ya, xc, yc, xd, i, j, size;

 switch(ebullet[b].type)
 {

    default:
     circlefill(display, x, y, 4, COL_LGREY);
     break;
    case EBULLET_BEAM:
     xa = x;
     ya = y;
     for (i = 0; i < 100; i ++)
     {
      xa += xpart(ebullet[b].angle, 20);
      ya += ypart(ebullet[b].angle, 20);
      if (xa < -50 || xa > 650 || ya < -50 || ya > 650)
       break;
     }




     TRANS_MODE
     yb = ebullet[b].angle;
     xb = (i + 1) * 20;

     xd = ebullet[b].status; // beam strength
     if (xd > 60)
      xd = 120 - xd;

     xc = xd / 6;

     switch(ebullet[b].colour)
     {
      case 0:
       bcircle(x, y, xc + grand(8), CLOUD_COL_1); break;
      case 1:
       bcircle(x, y, xc + grand(8), CLOUD_COL_2); break;
      case 2:
       bcircle(x, y, xc + grand(8), CLOUD_COL_3); break;
      case 3:
       bcircle(x, y, xc + grand(8), CLOUD_COL_4); break;
     }

     if (xc > 6)
      xc = 6;

     yc = xc;
     if (yc > 6)
      yc = 6;

     poly4(display, x + xpart(yb + ANGLE_4, yc), y + ypart(yb + ANGLE_4, yc),
                    x + xpart(yb - ANGLE_4, yc), y + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb - xc, xb), y + ypart(yb - xc, xb),
                    x + xpart(yb + xc, xb), y + ypart(yb + xc, xb), beam_colours [ebullet[b].colour] [0]);

     yc -= 2;
     if (yc < 0)
      yc = 0;

     xc -= 1;
     if (xc < 0)
      xc = 0;

     poly4(display, x + xpart(yb + ANGLE_4, yc), y + ypart(yb + ANGLE_4, yc),
                    x + xpart(yb - ANGLE_4, yc), y + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb - xc, xb), y + ypart(yb - xc, xb),
                    x + xpart(yb + xc, xb), y + ypart(yb + xc, xb), beam_colours [ebullet[b].colour] [1]);

     if (xd > 10)
     {
      yc -= 2;
      if (yc < 0)
       yc = 0;

      xc -= 1;
      if (xc < 0)
       xc = 0;

      poly4(display, x + xpart(yb + ANGLE_4, yc), y + ypart(yb + ANGLE_4, yc),
                    x + xpart(yb - ANGLE_4, yc), y + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb - xc, xb), y + ypart(yb - xc, xb),
                    x + xpart(yb + xc, xb), y + ypart(yb + xc, xb), beam_colours [ebullet[b].colour] [2]);
     }

     if (xd > 35)
     {
      yc -= 2;
      if (yc < 0)
       yc = 0;

      xc -= 1;
      if (xc < 0)
       xc = 0;

      poly4(display, x + xpart(yb + ANGLE_4, yc), y + ypart(yb + ANGLE_4, yc),
                    x + xpart(yb - ANGLE_4, yc), y + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb - xc, xb), y + ypart(yb - xc, xb),
                    x + xpart(yb + xc, xb), y + ypart(yb + xc, xb), beam_colours [ebullet[b].colour] [3]);
     }

     if (xd > 45)
     {
      yc -= 2;
      if (yc < 0)
       yc = 0;

      xc -= 1;
      if (xc < 0)
       xc = 0;

      poly4(display, x + xpart(yb + ANGLE_4, yc), y + ypart(yb + ANGLE_4, yc),
                    x + xpart(yb - ANGLE_4, yc), y + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb - xc, xb), y + ypart(yb - xc, xb),
                    x + xpart(yb + xc, xb), y + ypart(yb + xc, xb), beam_colours [ebullet[b].colour] [4]);
     }
     END_TRANS_MODE
     break;

    case EBULLET_BEAM2:
     xa = ebullet[b].x;
     ya = ebullet[b].y;

     xd = ebullet[b].status; // beam strength
     if (xd > 60)
      xd = 120 - xd;

     xc = xd / 7;

     for (i = 0; i < 500; i ++)
     {
      size = 6000 + grand(12500);
      xa += xpart(ebullet[b].angle, size);
      ya += ypart(ebullet[b].angle, size);
      if (xd > 45)
       bcircle(xa >> 10, ya >> 10, 3 + grand(xc), ebullet[b].colour);
        else
        {
          if (xd > 35)
           bcircle4(xa >> 10, ya >> 10, 3 + grand(xc), ebullet[b].colour);
            else
             bcircle3(xa >> 10, ya >> 10, 3 + grand(xc), ebullet[b].colour);
        }
      if (xa < 0 || xa > 614400 || ya < 0 || ya > 614400)
       break;
     }
     size = 0;

     xa = x;
     ya = y;
     for (i = 0; i < 100; i ++)
     {
      xa += xpart(ebullet[b].angle, 20);
      ya += ypart(ebullet[b].angle, 20);
      if (xa < 0 || xa > 600 || ya < 0 || ya > 600)
       break;
     }

     TRANS_MODE
     yb = ebullet[b].angle;
     xb = (i + 1) * 20;

     xd = ebullet[b].status; // beam strength
     if (xd > 60)
      xd = 120 - xd;

     xc = xd / 6;

     switch(ebullet[b].colour)
     {
      case 0:
       bcircle(x, y, xc + grand(8), CLOUD_COL_1); break;
      case 1:
       bcircle(x, y, xc + grand(8), CLOUD_COL_2); break;
      case 2:
       bcircle(x, y, xc + grand(8), CLOUD_COL_3); break;
      case 3:
       bcircle(x, y, xc + grand(8), CLOUD_COL_4); break;
     }

     if (xc > 16)
      xc = 16;

     xc += grand(3);

     yc = xc;
//     if (yc > 16)
//      yc = 12;

     poly4(display, x + xpart(yb + ANGLE_4, yc), y + ypart(yb + ANGLE_4, yc),
                    x + xpart(yb - ANGLE_4, yc), y + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb, xb) + xpart(yb - ANGLE_4, yc), y + ypart(yb, xb) + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb, xb) + xpart(yb + ANGLE_4, yc), y + ypart(yb, xb) + xpart(yb + ANGLE_4, yc), beam_colours [ebullet[b].colour] [0]);

     yc -= 2;
     if (yc < 0)
      yc = 0;

     xc -= 1;
     if (xc < 0)
      xc = 0;

     poly4(display, x + xpart(yb + ANGLE_4, yc), y + ypart(yb + ANGLE_4, yc),
                    x + xpart(yb - ANGLE_4, yc), y + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb, xb) + xpart(yb - ANGLE_4, yc), y + ypart(yb, xb) + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb, xb) + xpart(yb + ANGLE_4, yc), y + ypart(yb, xb) + xpart(yb + ANGLE_4, yc), beam_colours [ebullet[b].colour] [1]);

     if (xd > 10)
     {
      yc -= 2;
      if (yc < 0)
       yc = 0;

      xc -= 1;
      if (xc < 0)
       xc = 0;

      poly4(display, x + xpart(yb + ANGLE_4, yc), y + ypart(yb + ANGLE_4, yc),
                    x + xpart(yb - ANGLE_4, yc), y + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb, xb) + xpart(yb - ANGLE_4, yc), y + ypart(yb, xb) + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb, xb) + xpart(yb + ANGLE_4, yc), y + ypart(yb, xb) + xpart(yb + ANGLE_4, yc), beam_colours [ebullet[b].colour] [2]);
     }

     if (xd > 35)
     {
      yc -= 2;
      if (yc < 0)
       yc = 0;

      xc -= 1;
      if (xc < 0)
       xc = 0;

      poly4(display, x + xpart(yb + ANGLE_4, yc), y + ypart(yb + ANGLE_4, yc),
                    x + xpart(yb - ANGLE_4, yc), y + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb, xb) + xpart(yb - ANGLE_4, yc), y + ypart(yb, xb) + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb, xb) + xpart(yb + ANGLE_4, yc), y + ypart(yb, xb) + xpart(yb + ANGLE_4, yc), beam_colours [ebullet[b].colour] [3]);
     }

     if (xd > 45)
     {
      yc -= 2;
      if (yc < 0)
       yc = 0;

      xc -= 1;
      if (xc < 0)
       xc = 0;

      poly4(display, x + xpart(yb + ANGLE_4, yc), y + ypart(yb + ANGLE_4, yc),
                    x + xpart(yb - ANGLE_4, yc), y + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb, xb) + xpart(yb - ANGLE_4, yc), y + ypart(yb, xb) + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb, xb) + xpart(yb + ANGLE_4, yc), y + ypart(yb, xb) + xpart(yb + ANGLE_4, yc), beam_colours [ebullet[b].colour] [4]);
     }
     END_TRANS_MODE
     break;

/*    case EBULLET_BEAM:
     xa = x;
     ya = y;
     for (i = 0; i < 100; i ++)
     {
      xa += xpart(ebullet[b].angle, 20);
      ya += ypart(ebullet[b].angle, 20);
      if (xa < 0 || xa > 600 || ya < 0 || ya > 600)
       break;
     }


     TRANS_MODE
     yb = ebullet[b].angle;
     xb = (i + 1) * 20;

     xd = ebullet[b].status; // beam strength
     if (xd > 40)
      xd = 80 - xd;

     xc = xd / 4;

     switch(ebullet[b].colour)
     {
      case 0:
       bcircle(x, y, xc + grand(8), CLOUD_COL_1); break;
      case 1:
       bcircle(x, y, xc + grand(8), CLOUD_COL_2); break;
      case 2:
       bcircle(x, y, xc + grand(8), CLOUD_COL_3); break;
     }

     if (xc > 6)
      xc = 6;

     yc = xc;
     if (yc > 6)
      yc = 6;

     poly4(display, x + xpart(yb + ANGLE_4, yc), y + ypart(yb + ANGLE_4, yc),
                    x + xpart(yb - ANGLE_4, yc), y + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb - xc, xb), y + ypart(yb - xc, xb),
                    x + xpart(yb + xc, xb), y + ypart(yb + xc, xb), TRANS_RED1 + TRANS_DIFFERENCE * ebullet[b].colour);

     yc -= 2;
     if (yc < 0)
      yc = 0;

     xc -= 1;
     if (xc < 0)
      xc = 0;

     poly4(display, x + xpart(yb + ANGLE_4, yc), y + ypart(yb + ANGLE_4, yc),
                    x + xpart(yb - ANGLE_4, yc), y + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb - xc, xb), y + ypart(yb - xc, xb),
                    x + xpart(yb + xc, xb), y + ypart(yb + xc, xb), TRANS_RED2 + TRANS_DIFFERENCE * ebullet[b].colour);

     if (xd > 10)
     {
      yc -= 2;
      if (yc < 0)
       yc = 0;

      xc -= 1;
      if (xc < 0)
       xc = 0;

      poly4(display, x + xpart(yb + ANGLE_4, yc), y + ypart(yb + ANGLE_4, yc),
                    x + xpart(yb - ANGLE_4, yc), y + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb - xc, xb), y + ypart(yb - xc, xb),
                    x + xpart(yb + xc, xb), y + ypart(yb + xc, xb), TRANS_RED3 + TRANS_DIFFERENCE * ebullet[b].colour);
     }

     if (xd > 25)
     {
      yc -= 2;
      if (yc < 0)
       yc = 0;

      xc -= 1;
      if (xc < 0)
       xc = 0;

      poly4(display, x + xpart(yb + ANGLE_4, yc), y + ypart(yb + ANGLE_4, yc),
                    x + xpart(yb - ANGLE_4, yc), y + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb - xc, xb), y + ypart(yb - xc, xb),
                    x + xpart(yb + xc, xb), y + ypart(yb + xc, xb), TRANS_RED4 + TRANS_DIFFERENCE * ebullet[b].colour);
     }

     if (xd > 30)
     {
      yc -= 2;
      if (yc < 0)
       yc = 0;

      xc -= 1;
      if (xc < 0)
       xc = 0;

      poly4(display, x + xpart(yb + ANGLE_4, yc), y + ypart(yb + ANGLE_4, yc),
                    x + xpart(yb - ANGLE_4, yc), y + ypart(yb - ANGLE_4, yc),
                    x + xpart(yb - xc, xb), y + ypart(yb - xc, xb),
                    x + xpart(yb + xc, xb), y + ypart(yb + xc, xb), TRANS_RED5 + TRANS_DIFFERENCE * ebullet[b].colour);
     }
     END_TRANS_MODE
     break;
     */
    case EBULLET_DRAGGER:
     size = (ebullet[b].timeout / 13) + grand(3);
     bcircle(x, y, 5 + grand(3), ebullet[b].colour);
     bcircle(x - xpart(ebullet[b].angle, 2 + size), y - ypart(ebullet[b].angle, 2 + size), 3 + grand(3), ebullet[b].colour);
     bcircle(x - xpart(ebullet[b].angle, 4 + size), y - ypart(ebullet[b].angle, 4 + size), 2 + grand(3), ebullet[b].colour);
     break;

    case EBULLET_BLOBSHOT:
     size = 3 + ebullet[b].status / 2;
     bcircle(x, y, 2 + grand(5) + size, ebullet[b].colour);
     bcircle(x - xpart(ebullet[b].angle, 4 + size), y - ypart(ebullet[b].angle, 4 + size), 3 + grand(4), ebullet[b].colour);
     bcircle(x + xpart(ebullet[b].angle, 4 + size), y + ypart(ebullet[b].angle, 4 + size), 2 + grand(4), ebullet[b].colour);
     break;

    case EBULLET_FLAKE:
     xa = ebullet[b].timeout * 4;
     if (ebullet[b].angle2 == 1)
      xa = ANGLE_1 - (xa & 1023);
     if (ebullet[b].timeout > 3980)
     {
      size = (4000 - ebullet[b].timeout) / 4;
      if (size < 0)
       size = 0;
      xb = (4000 - ebullet[b].timeout) / 2;
      for (i = 0; i < 5; i ++)
      {
       TRANS_MODE
       circlefill(display, x + xpart(xa, xb), y + ypart(xa, xb), size, TRANS_BLUE3);
       END_TRANS_MODE
       draw_trans_rle_sprite(display, RLE_ccircle_basic [ebullet[b].colour] [size], x - size - 1 + xpart(xa, xb), y - size - 1 + ypart(xa, xb));
       xa += ANGLE_5;
      }
      break;
     }
     size = ebullet[b].status;
     for (i = 0; i < 5; i ++)
     {
      draw_trans_rle_sprite(display, RLE_ibullet [ebullet[b].colour] [size], x + xpart(xa, 11) - 5, y + ypart(xa, 11) - 5);
      xa += ANGLE_5;
     }
     break;
    case EBULLET_FLAKE2:
     xa = ebullet[b].timeout * 4;
     if (ebullet[b].angle2 == 1)
      xa = ANGLE_1 - (xa & 1023);
     ya = xa;//ANGLE_1 - ((ebullet[b].timeout * 2) & 1023);
     if (ebullet[b].timeout > 3980)
     {
      size = (4000 - ebullet[b].timeout) / 4;
      if (size < 0)
       size = 0;
      xb = (4000 - ebullet[b].timeout) / 2;
      yb = (4000 - ebullet[b].timeout) * 2;
      for (j = 0; j < 5; j ++)
      {
       ya += ANGLE_5;
       circlefill(display, x + xpart(ANGLE_1 - ya, xb * 1.5), y + ypart(ANGLE_1 - ya, xb * 1.5), size, TRANS_RED3);
       for (i = 0; i < 5; i ++)
       {
        TRANS_MODE
        circlefill(display, x + xpart(ya, yb) + xpart(xa, xb), y + ypart(ya, yb) + ypart(xa, xb), size, TRANS_BLUE3);
        END_TRANS_MODE
        draw_trans_rle_sprite(display, RLE_ccircle_basic [ebullet[b].colour] [size], x - size - 1 + xpart(ya, yb) + xpart(xa, xb), y - size - 1 + ypart(ya, yb) + ypart(xa, xb));
        xa += ANGLE_5;
       }
      }
      break;
     }
     size = ebullet[b].status;
     for (j = 0; j < 5; j ++)
     {
      ya += ANGLE_5;
      draw_trans_rle_sprite(display, RLE_ibullet [0] [size], x + xpart(ANGLE_1 - ya, 16) - 5, y + ypart(ANGLE_1 - ya, 16) - 5);
      for (i = 0; i < 5; i ++)
      {
       draw_trans_rle_sprite(display, RLE_ibullet [ebullet[b].colour] [size], x + xpart(ya, 40) + xpart(xa, 11) - 5, y + ypart(ya, 40) + ypart(xa, 11) - 5);
       xa += ANGLE_5;
      }
     }
     break;
    case EBULLET_FLAKE3:
     xa = ebullet[b].timeout * 4;
     if (ebullet[b].angle2 == 1)
      xa = ANGLE_1 - (xa & 1023);
     if (ebullet[b].timeout > 3980)
     {
      size = (4000 - ebullet[b].timeout) / 4;
      if (size < 0)
       size = 0;
      xb = (4000 - ebullet[b].timeout) / 2;
      circlefill(display, x, y, size, TRANS_BLUE3);
      for (i = 0; i < 3; i ++)
      {
       TRANS_MODE
       circlefill(display, x + xpart(xa, xb), y + ypart(xa, xb), size, TRANS_BLUE3);
       END_TRANS_MODE
       draw_trans_rle_sprite(display, RLE_ccircle_basic [ebullet[b].colour] [size], x - size - 1 + xpart(xa, xb), y - size - 1 + ypart(xa, xb));
       xa += ANGLE_3;
      }
      break;
     }
     size = ebullet[b].status;
     draw_trans_rle_sprite(display, RLE_ibullet_2 [1] [size], x - 5, y - 5);
     for (i = 0; i < 3; i ++)
     {
      draw_trans_rle_sprite(display, RLE_ibullet [ebullet[b].colour] [size], x + xpart(xa, 13) - 5, y + ypart(xa, 13) - 5);
      xa += ANGLE_3;
     }
     break;
    case EBULLET_IBULLET:
           size = ebullet[b].status;
           draw_trans_rle_sprite(display, RLE_ibullet [ebullet[b].colour] [size], x - 5, y - 5);
           break;
    case EBULLET_IBULLET_2:
           size = ebullet[b].status;
           draw_trans_rle_sprite(display, RLE_ibullet_2 [ebullet[b].colour] [size], x - 5, y - 5);
//     textprintf_centre_ex(display, font, x, y + 10, -1, -1, "%i", size);
           break;

    case EBULLET_WIGGLY:
     size = ebullet[b].status + grand(3);
     draw_trans_rle_sprite(display, RLE_ccircle_basic [ebullet[b].colour] [size], x - size - 1, y - size - 1);
     break;
    case EBULLET_BLOB:
     size = ebullet[b].status + grand(3);
     draw_trans_rle_sprite(display, RLE_ccircle_basic [ebullet[b].colour] [size], x - size - 1, y - size - 1);
     break;
    case EBULLET_SHOT2:
     size = 7 + grand(3);
     draw_trans_rle_sprite(display, RLE_ccircle_basic [ebullet[b].colour] [size], x - size - 1, y - size - 1);
     break;
    case EBULLET_B21SHOT:
     size = 5 + grand(3);
     draw_trans_rle_sprite(display, RLE_ccircle_basic [ebullet[b].colour] [size], x - size - 1, y - size - 1);
     break;
    case EBULLET_SHOT:
     size = 5 + grand(3);
     draw_trans_rle_sprite(display, RLE_ccircle_basic [ebullet[b].colour] [size], x - size - 1, y - size - 1);
     x = (ebullet[b].x - xpart(ebullet[b].angle, 3000)) >> 10;
     y = (ebullet[b].y - ypart(ebullet[b].angle, 3000)) >> 10;
     size -= 1 + grand(2);
     draw_trans_rle_sprite(display, RLE_ccircle_4cols [ebullet[b].colour] [size], x - size - 1, y - size - 1);
     x = (ebullet[b].x - xpart(ebullet[b].angle, 6000)) >> 10;
     y = (ebullet[b].y - ypart(ebullet[b].angle, 6000)) >> 10;
     size -= 1 + grand(2);
     draw_trans_rle_sprite(display, RLE_ccircle_3cols [ebullet[b].colour] [size], x - size - 1, y - size - 1);

/*     size -= grand(2);
     draw_trans_rle_sprite(display, RLE_ccircle_4cols [ebullet[b].colour] [size], x - size - 1, y - size - 1);
     size -= grand(2);
     draw_trans_rle_sprite(display, RLE_ccircle_3cols [ebullet[b].colour] [size], x - size - 1, y - size - 1);
     size -= grand(2) + 1;
     if (size < 0)
      size = 0;
     draw_trans_rle_sprite(display, RLE_ccircle_3cols [ebullet[b].colour] [size], x - size - 1, y - size - 1);
     size -= grand(2) + 1;
     if (size < 0)
      size = 0;
     draw_trans_rle_sprite(display, RLE_ccircle_3cols [ebullet[b].colour] [size], x - size - 1, y - size - 1);*/
     break;
    case EBULLET_B13SHOT:
     size = 10 + grand(3);
     bcircle(x, y, size, ebullet[b].colour);
     xa = ebullet[b].angle;
     size = 4 + grand(3);
     for (i = 0; i < 3; i ++)
     {
      bcircle(x + xpart(xa, 16), y + ypart(xa, 16), size, ebullet[b].colour);
      bcircle(x + xpart(xa - (ebullet[b].status * 6), 16), y + ypart(xa - (ebullet[b].status * 6), 16), 2 + grand(3), ebullet[b].colour);
      xa += ANGLE_3;
     }
     break;
    case EBULLET_SPINNER3:
//     size = 5 + grand(3);
     bcircle(x, y, 7, ebullet[b].colour);
     xa = ebullet[b].angle;
//     size = 4 + grand(3);
     for (i = 0; i < 3; i ++)
     {
      bcircle(x + xpart(xa, 6), y + ypart(xa, 6), 5, ebullet[b].colour);
//      bcircle(x + xpart(xa - (ebullet[b].status * 6), 16), y + ypart(xa - (ebullet[b].status * 6), 16), 2 + grand(3), ebullet[b].colour);
      xa += ANGLE_3;
     }
     break;
    case EBULLET_STOPMINE:
     bcircle(x, y, 11, ebullet[b].colour);
     bcircle3(x, y, 16, ebullet[b].colour);
     xa = ebullet[b].angle;
     for (i = 0; i < 3; i ++)
     {
      bcircle(x + xpart(xa, 12), y + ypart(xa, 12), 6, CLOUD_COL_4);//ebullet[b].colour);
      xa += ANGLE_3;
     }
     break;

 }


}





void draw_pbullets(void)
{

 int b;

  for (b = 0; b < NO_PBULLETS; b ++)
  {
   if (pbullet[b].type == PBULLET_NONE)
    continue;
   draw_a_pbullet(b);
  }

}

void draw_a_pbullet(int b)
{

 int x = pbullet[b].x >> 10;
 int y = pbullet[b].y >> 10;

 int ya, size, xb, yb, xc; //, xa, ya;

 switch(pbullet[b].type)
 {
    default:
     circlefill(display, x, y, 2, COL_COL1);
     break;
    case PBULLET_HAND:
    case PBULLET_ORB:
     break;
    case PBULLET_BOMB:
     bcircle(x, y, 7 + grand(3), CLOUD_COL_1);
     break;
    case PBULLET_RINGSHOT:
     size = grand(pbullet[b].level);
     bcircle(x, y, 3 + grand(3) + grand(pbullet[b].level / 2), CLOUD_COL_1);
     bcircle(x - xpart(pbullet[b].angle, 5 + size), y - ypart(pbullet[b].angle, 5 + size), 4 + grand(4) + grand(pbullet[b].level / 2), 0);
     bcircle(x - xpart(pbullet[b].angle, 10 + size), y - ypart(pbullet[b].angle, 10 + size), 3 + grand(3) + grand(pbullet[b].level / 2), 0);
     break;
    case PBULLET_ORBSHOT:
     size = 4 + pbullet[b].level / 3;
     bcircle(x, y, 4 + grand(2) + grand(pbullet[b].level / 2), CLOUD_COL_3);
     bcircle3(x - xpart(pbullet[b].angle, 1), y - ypart(pbullet[b].angle, 1), 4 + grand(4) + grand(pbullet[b].level / 2), 2);
     bcircle3(x - xpart(pbullet[b].angle, 1 + size), y - ypart(pbullet[b].angle, 1 + size), 3 + grand(3) + grand(pbullet[b].level / 2), 2);
     break;
    case PBULLET_HSHOT:
     size = pbullet[b].level / 3;
     bcircle(x, y, 4 + grand(3) + grand(pbullet[b].level / 2), CLOUD_COL_1);
     bcircle(x - xpart(pbullet[b].angle, 5 + size), y - ypart(pbullet[b].angle, 5 + size), 4 + grand(2) + grand(pbullet[b].level / 3), 0);
     bcircle(x - xpart(pbullet[b].angle, 10 + size), y - ypart(pbullet[b].angle, 10 + size), 3 + grand(2) + grand(pbullet[b].level / 4), 0);
/*     bcircle(x, y, 4 + grand(4), CLOUD_COL_1);
     bcircle4(x - xpart(pbullet[b].angle, 5), y - ypart(pbullet[b].angle, 5), 4 + grand(4), 0);
     bcircle3(x - xpart(pbullet[b].angle, 10), y - ypart(pbullet[b].angle, 10), 3 + grand(3), 0);*/
     break;
    case PBULLET_SEEKER1:
     bcircle(x, y, 5 + grand(3), CLOUD_COL_1);
     break;
    case PBULLET_ORBSEEKER:
     bcircle(x, y, 5 + grand(4), CLOUD_COL_3);
     break;
    case PBULLET_ORBFIELD:
     bcircle(x, y, pbullet[b].status, pbullet[b].target);
     break;
    case PBULLET_CHARGE1:
     //size = pbullet[b].dam / 800; // was 1400
     size = pbullet[b].dam / 1400; // was 1400
     bcircle(x, y, 3 + size + grand(3), CLOUD_COL_1);
     bcircle(x - size, y + 2, 3 + (size >> 1) + grand(3), CLOUD_COL_1);
     bcircle(x + size, y + 2, 3 + (size >> 1) + grand(3), CLOUD_COL_1);
     bcircle(x - size * 1.5, y + 5, 1 + (size >> 2) + grand(3), CLOUD_COL_1);
     bcircle(x + size * 1.5, y + 5, 1 + (size >> 2) + grand(3), CLOUD_COL_1);
     break;
    case PBULLET_SHOT:
     size = 6 + (pbullet[b].level >> 2) + grand(pbullet[b].level >> 2);
     draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [size], x - size - 1, y - size - 1);
     size -= grand(2);
     draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [size], x - size - 1, y - size - 1 + 4);
     size -= grand(2);
     draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [size], x - size - 1, y - size - 1 + 8);
     size -= grand(2) + 1;
     if (size < 0)
      size = 0;
     draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [size], x - size - 1, y - size - 1 + 12);
     size -= grand(2) + 1;
     if (size < 0)
      size = 0;
     draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [size], x - size - 1, y - size - 1 + 16);
     break;
    case PBULLET_ASHOT:
     size = 8;
     draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [size], x - size - 1, y - size - 1);
     size -= grand(2);
     draw_trans_rle_sprite(display, RLE_ccircle_basic [0] [size], x - size - 1, y - size - 1 + 4);
     size -= grand(2);
     draw_trans_rle_sprite(display, RLE_ccircle_4cols [0] [size], x - size - 1, y - size - 1 + 8);
     size -= grand(2) + 1;
     if (size < 0)
      size = 0;
     draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [size], x - size - 1, y - size - 1 + 12);
     size -= grand(2) + 1;
     if (size < 0)
      size = 0;
     draw_trans_rle_sprite(display, RLE_ccircle_3cols [0] [size], x - size - 1, y - size - 1 + 16);
     break;
    case PBULLET_BLADE:
// this is an awful hack. I shouldn't refer to player stats here but who can stop me?
     switch(player.blade_pos)
     {
      case BLADE_CW_SLASH:
      case BLADE_CW_SWING:
      case BLADE_RETURN_CW_LEFT:
      case BLADE_RETURN_CW_RIGHT:
       ya = -48;
       yb = -16;
       break;
      default:
       ya = 48;
       yb = 16;
      break;
     }
//     TRANS_MODE
/*        triangle(display,
                    (player.x + xpart(player.blade_angle, player.blade_distance)) >> 10,
                    (player.y + ypart(player.blade_angle, player.blade_distance)) >> 10,
                    (player.x + xpart(player.blade_angle, player.blade_distance) + xpart(player.blade_angle, player.blade_size)) >> 10,
                    (player.y + ypart(player.blade_angle, player.blade_distance) + ypart(player.blade_angle, player.blade_size)) >> 10,
                    (player.x + xpart(player.blade_angle + ya, player.blade_distance) + xpart(player.blade_angle + ya, player.blade_size >> 1)) >> 10,
                    (player.y + ypart(player.blade_angle + ya, player.blade_distance) + ypart(player.blade_angle + ya, player.blade_size >> 1)) >> 10,
                    TRANS_BLUE5);

        triangle(display,
                    (player.x + xpart(player.blade_angle - yb, player.blade_distance - 1000)) >> 10,
                    (player.y + ypart(player.blade_angle - yb, player.blade_distance - 1000)) >> 10,
                    (player.x + xpart(player.blade_angle - yb, player.blade_distance + 5000) + xpart(player.blade_angle, player.blade_size)) >> 10,
                    (player.y + ypart(player.blade_angle - yb, player.blade_distance + 5000) + ypart(player.blade_angle, player.blade_size)) >> 10,
                    (player.x + xpart(player.blade_angle + ya + yb, player.blade_distance) + xpart(player.blade_angle + ya + yb, player.blade_size >> 1)) >> 10,
                    (player.y + ypart(player.blade_angle + ya + yb, player.blade_distance) + ypart(player.blade_angle + ya + yb, player.blade_size >> 1)) >> 10,
                    TRANS_BLUE4);

                    yb *= 2;

        triangle(display,
                    (player.x + xpart(player.blade_angle - yb, player.blade_distance - 2000)) >> 10,
                    (player.y + ypart(player.blade_angle - yb, player.blade_distance - 2000)) >> 10,
                    (player.x + xpart(player.blade_angle - yb, player.blade_distance + 8000) + xpart(player.blade_angle, player.blade_size)) >> 10,
                    (player.y + ypart(player.blade_angle - yb, player.blade_distance + 8000) + ypart(player.blade_angle, player.blade_size)) >> 10,
                    (player.x + xpart(player.blade_angle + ya + yb, player.blade_distance) + xpart(player.blade_angle + ya + yb, player.blade_size >> 1)) >> 10,
                    (player.y + ypart(player.blade_angle + ya + yb, player.blade_distance) + ypart(player.blade_angle + ya + yb, player.blade_size >> 1)) >> 10,
                    TRANS_BLUE3);*/
/*
     poly4(display, (player.x + xpart(player.blade_angle, player.blade_distance) + xpart(player.blade_angle + ANGLE_4, 1000)) >> 10,
                    (player.y + ypart(player.blade_angle, player.blade_distance) + ypart(player.blade_angle + ANGLE_4, 1000)) >> 10,
                    (player.x + xpart(player.blade_angle, player.blade_distance) + xpart(player.blade_angle - ANGLE_4, 1000)) >> 10,
                    (player.y + ypart(player.blade_angle, player.blade_distance) + ypart(player.blade_angle - ANGLE_4, 1000)) >> 10,

                    (player.x + xpart(player.blade_angle, player.blade_distance) + xpart(player.blade_angle - ANGLE_4, 1000) + xpart(player.blade_angle, player.blade_size)) >> 10,
                    (player.y + ypart(player.blade_angle, player.blade_distance) + ypart(player.blade_angle - ANGLE_4, 1000) + ypart(player.blade_angle, player.blade_size)) >> 10,
                    (player.x + xpart(player.blade_angle, player.blade_distance) + xpart(player.blade_angle + ANGLE_4, 1000) + xpart(player.blade_angle, player.blade_size)) >> 10,
                    (player.y + ypart(player.blade_angle, player.blade_distance) + ypart(player.blade_angle + ANGLE_4, 1000) + ypart(player.blade_angle, player.blade_size)) >> 10,
                    TRANS_BLUE4);

     poly4(display, (player.x + xpart(player.blade_angle, player.blade_distance) + xpart(player.blade_angle + ANGLE_4, 2000)) >> 10,
                    (player.y + ypart(player.blade_angle, player.blade_distance) + ypart(player.blade_angle + ANGLE_4, 2000)) >> 10,
                    (player.x + xpart(player.blade_angle, player.blade_distance) + xpart(player.blade_angle - ANGLE_4, 2000)) >> 10,
                    (player.y + ypart(player.blade_angle, player.blade_distance) + ypart(player.blade_angle - ANGLE_4, 2000)) >> 10,
                    (player.x + xpart(player.blade_angle, player.blade_distance) + xpart(player.blade_angle - ANGLE_4, 2000) + xpart(player.blade_angle, player.blade_size)) >> 10,
                    (player.y + ypart(player.blade_angle, player.blade_distance) + ypart(player.blade_angle - ANGLE_4, 2000) + ypart(player.blade_angle, player.blade_size)) >> 10,
                    (player.x + xpart(player.blade_angle, player.blade_distance) + xpart(player.blade_angle + ANGLE_4, 2000) + xpart(player.blade_angle, player.blade_size)) >> 10,
                    (player.y + ypart(player.blade_angle, player.blade_distance) + ypart(player.blade_angle + ANGLE_4, 2000) + ypart(player.blade_angle, player.blade_size)) >> 10,
                    TRANS_BLUE3);

     poly4(display, (player.x + xpart(player.blade_angle, player.blade_distance) + xpart(player.blade_angle + ANGLE_4, 3000)) >> 10,
                    (player.y + ypart(player.blade_angle, player.blade_distance) + ypart(player.blade_angle + ANGLE_4, 3000)) >> 10,
                    (player.x + xpart(player.blade_angle, player.blade_distance) + xpart(player.blade_angle - ANGLE_4, 3000)) >> 10,
                    (player.y + ypart(player.blade_angle, player.blade_distance) + ypart(player.blade_angle - ANGLE_4, 3000)) >> 10,
                    (player.x + xpart(player.blade_angle, player.blade_distance) + xpart(player.blade_angle - ANGLE_4, 3000) + xpart(player.blade_angle, player.blade_size)) >> 10,
                    (player.y + ypart(player.blade_angle, player.blade_distance) + ypart(player.blade_angle - ANGLE_4, 3000) + ypart(player.blade_angle, player.blade_size)) >> 10,
                    (player.x + xpart(player.blade_angle, player.blade_distance) + xpart(player.blade_angle + ANGLE_4, 3000) + xpart(player.blade_angle, player.blade_size)) >> 10,
                    (player.y + ypart(player.blade_angle, player.blade_distance) + ypart(player.blade_angle + ANGLE_4, 3000) + ypart(player.blade_angle, player.blade_size)) >> 10,
                    TRANS_BLUE2);

                    END_TRANS*/

    xc = 2;
    if (grand(15) < player.blade_redness)
     xc = 0;

    xb = player.blade_size;
    if (xb > 40000)
     xb = 35000;

     bcircle((player.x + xpart(player.blade_angle - yb, player.blade_distance - 2000)) >> 10,
             (player.y + ypart(player.blade_angle - yb, player.blade_distance - 2000)) >> 10,
              1 + grand(5) + (xb / 6000), xc);

     bcircle((player.x + xpart(player.blade_angle - yb, player.blade_distance - 2000) + xpart(player.blade_angle, xb * 0.75)) >> 10,
             (player.y + ypart(player.blade_angle - yb, player.blade_distance - 2000) + ypart(player.blade_angle, xb * 0.75)) >> 10,
              1 + grand(6) + (xb / 5000), xc);

     bcircle((player.x + xpart(player.blade_angle - yb, player.blade_distance - 2000) + xpart(player.blade_angle, xb / 2)) >> 10,
             (player.y + ypart(player.blade_angle - yb, player.blade_distance - 2000) + ypart(player.blade_angle, xb / 2))>> 10,
              1 + grand(8) + (xb / 4000), xc);

     bcircle((player.x + xpart(player.blade_angle - yb, player.blade_distance - 2000) + xpart(player.blade_angle, xb / 4)) >> 10,
             (player.y + ypart(player.blade_angle - yb, player.blade_distance - 2000) + ypart(player.blade_angle, xb / 4))>> 10,
              1 + grand(6) + (xb / 5000), xc);

     bcircle((player.x + xpart(player.blade_angle - yb, player.blade_distance - 2000) + xpart(player.blade_angle, xb)) >> 10,
             (player.y + ypart(player.blade_angle - yb, player.blade_distance - 2000) + ypart(player.blade_angle, xb))>> 10,
              1 + grand(5) + (xb / 6000), xc);



/*     bcircle((player.x + xpart(player.blade_angle - yb, player.blade_distance - 2000)) >> 10,
             (player.y + ypart(player.blade_angle - yb, player.blade_distance - 2000)) >> 10,
              1 + grand(5) + grand(player.blade_size / 6000), 2);

     bcircle((player.x + xpart(player.blade_angle - yb, player.blade_distance - 2000) + xpart(player.blade_angle, player.blade_size * 0.75)) >> 10,
             (player.y + ypart(player.blade_angle - yb, player.blade_distance - 2000) + ypart(player.blade_angle, player.blade_size * 0.75)) >> 10,
              1 + grand(6) + grand(player.blade_size / 5000), 2);

     bcircle((player.x + xpart(player.blade_angle - yb, player.blade_distance - 2000) + xpart(player.blade_angle, player.blade_size / 2)) >> 10,
             (player.y + ypart(player.blade_angle - yb, player.blade_distance - 2000) + ypart(player.blade_angle, player.blade_size / 2))>> 10,
              1 + grand(8) + grand(player.blade_size / 4000), 2);

     bcircle((player.x + xpart(player.blade_angle - yb, player.blade_distance - 2000) + xpart(player.blade_angle, player.blade_size / 4)) >> 10,
             (player.y + ypart(player.blade_angle - yb, player.blade_distance - 2000) + ypart(player.blade_angle, player.blade_size / 4))>> 10,
              1 + grand(6) + grand(player.blade_size / 5000), 2);

     bcircle((player.x + xpart(player.blade_angle - yb, player.blade_distance - 2000) + xpart(player.blade_angle, player.blade_size)) >> 10,
             (player.y + ypart(player.blade_angle - yb, player.blade_distance - 2000) + ypart(player.blade_angle, player.blade_size))>> 10,
              1 + grand(5) + grand(player.blade_size / 6000), 2);*/

//     bcircle(x, y, 10, 2);
     break;
 }

}



void draw_clouds(void)
{

 int c;

  for (c = 0; c < NO_CLOUDS; c ++)
  {
   if (cloud[c].type == CLOUD_NONE)
    continue;
   draw_a_cloud(c);
  }

}

void draw_a_cloud(int c)
{

 int x = cloud[c].x >> 10;
 int y = cloud[c].y >> 10;
// int xa, ya, xb, i, angle, yb;
 int size;

 switch(cloud[c].type)
 {
    default:
     TRANS_MODE
     circlefill(display, x, y, cloud[c].timeout, cloud[c].colour);
     END_TRANS_MODE
     break;
    case CLOUD_SHRINKING_BEAM:
     TRANS_MODE
     vline(display, x, y, cloud[c].colour >> 10, TRANS_RED2);// + grand(3) * 39);
     END_TRANS_MODE
     bcircle(x, y, 2 + grand(3), 0);
     bcircle(x, cloud[c].colour >> 10, 2 + grand(3), 0);
     size = y;
     while (size > cloud[c].colour >> 10)
     {
      bcircle(x, size, 1 + grand(3), 0);
      size -= 5 + grand(10);
     }
     break;
    case CLOUD_SHRINKING_LOCK:
     draw_ring(x, y, cloud[c].timeout);
     break;
    case CLOUD_SEEKER_TRAIL:
//     TRANS_MODE
     size = cloud[c].timeout / 4 + 1 + coin();
/*     size = TRANS_RED1 + (size * 39);
     if (size > TRANS_RED5)
      size = TRANS_RED5;
     pline(display, x, y, cloud[c].x2 >> 10, cloud[c].y2 >> 10, size);*/
     do_line(display, x, y, cloud[c].x2 >> 10, cloud[c].y2 >> 10, size, draw_trail_line);
//     END_TRANS
     break;
    case CLOUD_ORBSEEKER_TRAIL:
     size = cloud[c].timeout / 3 + 1 + coin();
     do_line(display, x, y, cloud[c].x2 >> 10, cloud[c].y2 >> 10, size, draw_trail_line2);
     break;
    case CLOUD_BLADE_TRAIL:
     size = cloud[c].timeout / 2;
     if (size > 3)
      size = 3;
      TRANS_MODE
     pline(display, x, y, cloud[c].x2 >> 10, cloud[c].y2 >> 10, TRANS_RED1 + 39 * size);
      END_TRANS
     break;
    case CLOUD_CIRCLE1_2:
     size = (cloud[c].timeout << 1) + grand(3);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     draw_trans_rle_sprite(display, RLE_ccircle_basic [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;
    case CLOUD_WAITCIRCLE1:
    case CLOUD_CIRCLE1:
     size = cloud[c].timeout + grand(3);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     draw_trans_rle_sprite(display, RLE_ccircle_basic [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;
    case CLOUD_WAITCIRCLE2:
    case CLOUD_CIRCLE2:
     size = (cloud[c].timeout >> 1) + grand(3);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     draw_trans_rle_sprite(display, RLE_ccircle_basic [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;
    case CLOUD_WAITCIRCLE4:
    case CLOUD_CIRCLE4:
     size = (cloud[c].timeout >> 2) + grand(3);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     draw_trans_rle_sprite(display, RLE_ccircle_basic [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;
    case CLOUD_CIRCLE8:
     size = (cloud[c].timeout >> 3) + grand(3);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     draw_trans_rle_sprite(display, RLE_ccircle_basic [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;



    case CLOUD_FCIRCLE1_2:
     size = (cloud[c].timeout << 1) + grand(3);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     if (!grand(3))
      draw_trans_rle_sprite(display, RLE_ccircle_basic [cloud[c].colour] [size], x - size - 1, y - size - 1);
       else
        draw_trans_rle_sprite(display, RLE_ccircle_4cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;
    case CLOUD_FCIRCLE1:
     size = cloud[c].timeout + grand(3);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     if (!grand(3))
      draw_trans_rle_sprite(display, RLE_ccircle_basic [cloud[c].colour] [size], x - size - 1, y - size - 1);
       else
        draw_trans_rle_sprite(display, RLE_ccircle_4cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;
    case CLOUD_FCIRCLE2:
     size = (cloud[c].timeout >> 1) + grand(3);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     if (!grand(3))
      draw_trans_rle_sprite(display, RLE_ccircle_basic [cloud[c].colour] [size], x - size - 1, y - size - 1);
       else
        draw_trans_rle_sprite(display, RLE_ccircle_4cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;
    case CLOUD_FCIRCLE4:
     size = (cloud[c].timeout >> 2) + grand(3);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     if (!grand(3))
      draw_trans_rle_sprite(display, RLE_ccircle_basic [cloud[c].colour] [size], x - size - 1, y - size - 1);
       else
        draw_trans_rle_sprite(display, RLE_ccircle_4cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;
    case CLOUD_FCIRCLE8:
     size = (cloud[c].timeout >> 3) + grand(3);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     if (!grand(3))
      draw_trans_rle_sprite(display, RLE_ccircle_basic [cloud[c].colour] [size], x - size - 1, y - size - 1);
       else
        draw_trans_rle_sprite(display, RLE_ccircle_4cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;

    case CLOUD_3FCIRCLE1_2:
     size = (cloud[c].timeout << 1) + grand(3);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     if (!grand(3))
      draw_trans_rle_sprite(display, RLE_ccircle_3cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
       else
        draw_trans_rle_sprite(display, RLE_ccircle_4cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;
    case CLOUD_3FCIRCLE1:
     size = cloud[c].timeout + grand(3);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     if (!grand(3))
      draw_trans_rle_sprite(display, RLE_ccircle_3cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
       else
        draw_trans_rle_sprite(display, RLE_ccircle_4cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;
    case CLOUD_3FCIRCLE2:
     size = (cloud[c].timeout >> 1) + grand(3);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     if (!grand(3))
      draw_trans_rle_sprite(display, RLE_ccircle_3cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
       else
        draw_trans_rle_sprite(display, RLE_ccircle_4cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;
    case CLOUD_3FCIRCLE4:
     size = (cloud[c].timeout >> 2) + grand(3);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     if (!grand(3))
      draw_trans_rle_sprite(display, RLE_ccircle_3cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
       else
        draw_trans_rle_sprite(display, RLE_ccircle_4cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;
    case CLOUD_3FCIRCLE8:
     size = (cloud[c].timeout >> 3) + grand(3);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     if (!grand(3))
      draw_trans_rle_sprite(display, RLE_ccircle_3cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
       else
        draw_trans_rle_sprite(display, RLE_ccircle_4cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;

    case CLOUD_SMALL_SHOCK:
     size = 29 - cloud[c].timeout;
     if (size >= 30)
      size = 29;
     if (size < 0)
      size = 0;
     draw_trans_rle_sprite(display, RLE_small_shock [cloud[c].colour] [size], x - size - 16, y - size - 16);
     break;

    case CLOUD_LARGE_SHOCK:
//     circle(display, x, y, 5, COL_WHITE);
     size = 49 - cloud[c].timeout;
     if (size >= 50)
      size = 49;
     if (size < 0)
      size = 0;
     draw_trans_rle_sprite(display, RLE_large_shock [cloud[c].colour] [size], x - size - 26, y - size - 26);
     break;

    case CLOUD_HUGE_SHOCK:
    case CLOUD_HUGE_SHOCK_FAST:
     size = 49 - cloud[c].timeout;
     if (size >= 50)
      size = 49;
     if (size < 0)
      size = 0;
     draw_trans_rle_sprite(display, RLE_huge_shock [cloud[c].colour] [size], x - size - 51, y - size - 51);
     break;

    case CLOUD_3PCIRCLE1_2:
     size = (cloud[c].timeout << 1);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     draw_trans_rle_sprite(display, RLE_ccircle_3cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;
    case CLOUD_3PCIRCLE1:
     size = cloud[c].timeout;
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     draw_trans_rle_sprite(display, RLE_ccircle_3cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;
    case CLOUD_3PCIRCLE2:
     size = (cloud[c].timeout >> 1);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     draw_trans_rle_sprite(display, RLE_ccircle_3cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;
    case CLOUD_3PCIRCLE4:
     size = (cloud[c].timeout >> 2);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     draw_trans_rle_sprite(display, RLE_ccircle_3cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;
    case CLOUD_3PCIRCLE8:
     size = (cloud[c].timeout >> 3);
     if (size >= RLE_CCIRCLES)
      size = RLE_CCIRCLES - 1;
     draw_trans_rle_sprite(display, RLE_ccircle_3cols [cloud[c].colour] [size], x - size - 1, y - size - 1);
     break;


 }


}


void draw_trail_line(BITMAP *bmp, int x, int y, int size)
{
    static char drawing;
    drawing ^= 1;
    if (drawing & 1)
     return;

    bcircle(x, y, size, 0);
}

void draw_trail_line2(BITMAP *bmp, int x, int y, int size)
{
    static char drawing;
    drawing ^= 1;
    if (drawing & 1)
     return;

    bcircle(x, y, size, 2);
}



void draw_background(void)
{
 int i;
 int size;

 switch(arena.level)
 {
  case 1:
  for (i = 0; i < NO_BCIRCLES; i ++)
  {
     if (bcircle_size [0] [i] <= 0)
      continue;
     size = bcircle_size [0] [i] >> 10;
     if (size > 31)
      size = 31;
     draw_trans_rle_sprite(display, RLE_bcircle [0] [size], (bcircle_x [0] [i] >> 10) - size, (bcircle_y [0] [i] >> 10) - size);
//     textprintf_centre_ex(display, font, bcircle_x [0] [i] >> 10, bcircle_y [0] [i] >> 10, -1, -1, "%i %i", size, i);
  }

  for (i = 0; i < NO_BCIRCLES; i ++)
  {
      if (bcircle_size [1] [i] <= 0)
       continue;
      size = bcircle_size [1] [i] >> 10;
      if (size > 31)
       size = 31;
      draw_trans_rle_sprite(display, RLE_bcircle [1] [size], (bcircle_x [1] [i] >> 10) - size, (bcircle_y [1] [i] >> 10) - size);
  }

  for (i = 0; i < NO_BCIRCLES; i ++)
  {
      if (bcircle_size [2] [i] <= 0)
       continue;
      size = bcircle_size [2] [i] >> 10;
      if (size > 31)
       size = 31;
      draw_trans_rle_sprite(display, RLE_bcircle [2] [size], (bcircle_x [2] [i] >> 10) - size, (bcircle_y [2] [i] >> 10) - size);
  }
  break;
  case 2:
   for (i = 0; i < NO_BCIRCLES; i ++)
   {
      if (panel_x_size [0] [i] <= 0
       || (panel_y [0] [i] >> 10) + panel_y_size [0] [i] < -1)
       continue;
      rectfill(display, (panel_x [0] [i] >> 10) - panel_x_size [0] [i], (panel_y [0] [i] >> 10) - panel_y_size [0] [i], (panel_x [0] [i] >> 10) + panel_x_size [0] [i], (panel_y [0] [i] >> 10) + panel_y_size [0] [i], COL_BACK2);
      TRANS_MODE
      rect(display, (panel_x [0] [i] >> 10) - panel_x_size [0] [i] - 1, (panel_y [0] [i] >> 10) - panel_y_size [0] [i] - 1, (panel_x [0] [i] >> 10) + panel_x_size [0] [i] + 1, (panel_y [0] [i] >> 10) + panel_y_size [0] [i] + 1, COL_BCIRCLE_OUT_1);
      END_TRANS
   }
   for (i = 0; i < NO_BCIRCLES; i ++)
   {
      if (panel_x_size [1] [i] <= 0
       || (panel_y [1] [i] >> 10) + panel_y_size [1] [i] < -1)
       continue;
      rectfill(display, (panel_x [1] [i] >> 10) - panel_x_size [1] [i], (panel_y [1] [i] >> 10) - panel_y_size [1] [i], (panel_x [1] [i] >> 10) + panel_x_size [1] [i], (panel_y [1] [i] >> 10) + panel_y_size [1] [i], COL_BACK3);
      TRANS_MODE
      rect(display, (panel_x [1] [i] >> 10) - panel_x_size [1] [i] - 1, (panel_y [1] [i] >> 10) - panel_y_size [1] [i] - 1, (panel_x [1] [i] >> 10) + panel_x_size [1] [i] + 1, (panel_y [1] [i] >> 10) + panel_y_size [1] [i] + 1, COL_BCIRCLE_OUT_2);
      END_TRANS
   }
   for (i = 0; i < NO_BCIRCLES; i ++)
   {
      if (panel_x_size [2] [i] <= 0
       || (panel_y [2] [i] >> 10) + panel_y_size [2] [i] < -1)
       continue;
      rectfill(display, (panel_x [2] [i] >> 10) - panel_x_size [2] [i], (panel_y [2] [i] >> 10) - panel_y_size [2] [i], (panel_x [2] [i] >> 10) + panel_x_size [2] [i], (panel_y [2] [i] >> 10) + panel_y_size [2] [i], COL_BACK4);
      TRANS_MODE
      rect(display, (panel_x [2] [i] >> 10) - panel_x_size [2] [i] - 1, (panel_y [2] [i] >> 10) - panel_y_size [2] [i] - 1, (panel_x [2] [i] >> 10) + panel_x_size [2] [i] + 1, (panel_y [2] [i] >> 10) + panel_y_size [2] [i] + 1, COL_BCIRCLE_OUT_3);
      END_TRANS
   }
   break;

  case 3:
   for (i = 0; i < NO_BCIRCLES; i ++)
   {
      if (panel_x_size [0] [i] <= 0
       || (panel_y [0] [i] >> 10) + (panel_y_size [0] [i] >> 10) < -1)
       continue;

      rectfill(display, (panel_x [0] [i] >> 10) - (panel_x_size [0] [i] >> 10), (panel_y [0] [i] >> 10) - (panel_y_size [0] [i] >> 10), (panel_x [0] [i] >> 10) + (panel_x_size [0] [i] >> 10), (panel_y [0] [i] >> 10) + (panel_y_size [0] [i] >> 10), COL_BACK2);
      TRANS_MODE
      rect(display, (panel_x [0] [i] >> 10) - (panel_x_size [0] [i] >> 10) - 1, (panel_y [0] [i] >> 10) - (panel_y_size [0] [i] >> 10) - 1, (panel_x [0] [i] >> 10) + (panel_x_size [0] [i] >> 10) + 1, (panel_y [0] [i] >> 10) + (panel_y_size [0] [i] >> 10) + 1, COL_BCIRCLE_OUT_1);
      END_TRANS
   }
   for (i = 0; i < NO_BCIRCLES; i ++)
   {
      if (panel_x_size [1] [i] <= 0
       || (panel_y [1] [i] >> 10) + panel_y_size [1] [i] < -1)
       continue;
      rectfill(display, (panel_x [1] [i] >> 10) - (panel_x_size [1] [i] >> 10), (panel_y [1] [i] >> 10) - (panel_y_size [1] [i] >> 10), (panel_x [1] [i] >> 10) + (panel_x_size [1] [i] >> 10), (panel_y [1] [i] >> 10) + (panel_y_size [1] [i] >> 10), COL_BACK3);
      TRANS_MODE
      rect(display, (panel_x [1] [i] >> 10) - (panel_x_size [1] [i] >> 10) - 1, (panel_y [1] [i] >> 10) - (panel_y_size [1] [i] >> 10) - 1, (panel_x [1] [i] >> 10) + (panel_x_size [1] [i] >> 10) + 1, (panel_y [1] [i] >> 10) + (panel_y_size [1] [i] >> 10) + 1, COL_BCIRCLE_OUT_2);
      END_TRANS
   }
   for (i = 0; i < NO_BCIRCLES; i ++)
   {
      if (panel_x_size [2] [i] <= 0
       || (panel_y [2] [i] >> 10) + panel_y_size [2] [i] < -1)
       continue;
      rectfill(display, (panel_x [2] [i] >> 10) - (panel_x_size [2] [i] >> 10), (panel_y [2] [i] >> 10) - (panel_y_size [2] [i] >> 10), (panel_x [2] [i] >> 10) + (panel_x_size [2] [i] >> 10), (panel_y [2] [i] >> 10) + (panel_y_size [2] [i] >> 10), COL_BACK4);
      TRANS_MODE
      rect(display, (panel_x [2] [i] >> 10) - (panel_x_size [2] [i] >> 10) - 1, (panel_y [2] [i] >> 10) - (panel_y_size [2] [i] >> 10) - 1, (panel_x [2] [i] >> 10) + (panel_x_size [2] [i] >> 10) + 1, (panel_y [2] [i] >> 10) + (panel_y_size [2] [i] >> 10) + 1, COL_BCIRCLE_OUT_3);
      END_TRANS
   }
   break;

 }

}



void display_pause(int pc)
{
  vsync();

  int x1 = 150;
  int y1 = 200;
  int y2 = 400;

  TRANS_MODE
  rectfill(display, x1, y1, 600 - x1, y2, TRANS_BLUE1);
  rect(display, x1 - 1, y1 - 1, 601 - x1, y2 + 1, TRANS_BLUE2);
  rect(display, x1 - 2, y1 - 2, 602 - x1, y2 + 2, TRANS_BLUE3);
  rect(display, x1 - 3, y1 - 3, 603 - x1, y2 + 3, TRANS_BLUE1);
  END_TRANS

/*  rect(display, 200, 100, 440, 200, TRANS_GREEN5 - 1);

  rect(display, 190, 90, 450, 210, TRANS_RED3 - 1);
  rect(display, 191, 91, 449, 209, TRANS_RED5 - 1);
  rect(display, 189, 89, 451, 211, TRANS_RED5 - 1);*/

  textprintf_centre_ex(display, font, 300, y1 + 40, -1, -1, "QUIT?");


  textprintf_centre_ex(display, font, 300, y1 + 100, -1, -1, "PRESS 'Y' TO QUIT");
  textprintf_centre_ex(display, font, 300, y1 + 140, -1, -1, "OR 'N' TO CONTINUE");


  blit(display, screen, 0, 0, 100, 0, 600, 600);

}

/*
void run_underlay(void)
{

 arena.underlay_position -= 2;
 if (arena.underlay_position <= 0)
  arena.underlay_position += 480;


}

*/

void fcircle(int x, int y, int size, int col)
{
 if (size < 0)
  size = 0;
 if (size >= RLE_CIRCLES)
  size = RLE_CIRCLES - 1;

 draw_trans_rle_sprite(display, RLE_circle [size] [col], x - size - 1, y - size - 1);

}





/*

Use this instead of Allegro's line function - it has better bounds checking.
Sometimes for some reason I get an extremely long line which slows things down.

*/
void pline(BITMAP *bmp, int x1, int y1, int x2, int y2, int colour)
{
 if (x1 < -500 || x1 > 900
     || x2 < -500 || x2 > 900
     || y2 < -500 || y2 > 900
     || y2 < -500 || y2 > 900)
      return;

 line(bmp, x1, y1, x2, y2, colour);

}



void poly4(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int col)
{

  points4 [0] = x1;
  points4 [1] = y1;
  points4 [2] = x2;
  points4 [3] = y2;
  points4 [4] = x3;
  points4 [5] = y3;
  points4 [6] = x4;
  points4 [7] = y4;

  polygon(target, 4, points4, col);

}


void print_number(int x, int y, int n)
{
  textprintf_ex(display, font, x, y, -1, -1, "%i", n);
}

void draw_ring(int x, int y, int size)
{
 if (size < 0) return;
 if (size >= RLE_RINGS) size = RLE_RINGS - 1;

 draw_trans_rle_sprite(display, RLE_ring [size], x - size - 4, y - size - 4);

}

void draw_bright_ring(int x, int y, int size)
{
 if (size < 0) return;
 if (size >= RLE_RINGS) size = RLE_RINGS - 1;

 draw_trans_rle_sprite(display, RLE_bright_ring [size], x - size - 4, y - size - 4);

}

void bcircle(int x, int y, int size, unsigned char col)
{
 if (size < 0) return;
 if (size >= RLE_CCIRCLES) size = RLE_CCIRCLES - 1;

 draw_trans_rle_sprite(display, RLE_ccircle_basic [col] [size], x - size - 1, y - size - 1);

}

void bcircle4(int x, int y, int size, unsigned char col)
{
 if (size < 0) return;
 if (size >= RLE_CCIRCLES) size = RLE_CCIRCLES - 1;

 draw_trans_rle_sprite(display, RLE_ccircle_4cols [col] [size], x - size - 1, y - size - 1);

}

void bcircle3(int x, int y, int size, unsigned char col)
{
 if (size < 0) return;
 if (size >= RLE_CCIRCLES) size = RLE_CCIRCLES - 1;

 draw_trans_rle_sprite(display, RLE_ccircle_3cols [col] [size], x - size - 1, y - size - 1);

}

