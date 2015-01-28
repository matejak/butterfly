
#include "config.h"

#include "allegro.h"

#include <math.h>

#include "globvars.h"
#include "palette.h"
#include "stuff.h"
#include "display.h"


enum
{
 FLIP_H,
 FLIP_V,
 FLIP_VH
};


extern RLE_SPRITE *white_RLE; // menu


extern volatile unsigned char ticked;

extern BITMAP *display;
//extern RGB palet [256];
extern RGB palet [256];

extern RLE_SPRITE *player_icon_RLE;

extern RLE_SPRITE *eRLE_player [S_PLAYER_RLES];
extern RLE_SPRITE *eRLE_small1 [S_ENEMY_RLES];
extern RLE_SPRITE *eRLE_large1 [L_ENEMY_RLES];
extern RLE_SPRITE *eRLE_large2 [L_ENEMY2_RLES];

BITMAP *new_bitmap(int x, int y, const char errtxt []);

void bordered_poly4(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int col, int border_col);
void bordered_poly6(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, int x6, int y6, int col, int border_col);
void bordered_triangle(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int col, int border_col);
void fast_bordered_poly4(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int col, int border_col, int fillx, int filly);
void fast_bordered_triangle(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int col, int border_col, int fillx, int filly);
void new_rle_struct(BITMAP *source, const char errtxt [], struct RLE_STRUCT *str, int bl);
void prepare_barrels(void);
void prepare_enemy_bullets(void);
void prepare_platform_rles(void);
void prepare_s_enemy_rles(void);
void prepare_player_rles(void);
void prepare_l_enemy_rles(void);
void prepare_trans_rles(void);
void prepare_blades(void);
void prepare_circles(void);


void fix_outline(BITMAP *source);
void fix_trans(BITMAP *source);
void coat_sprite(BITMAP *target, int col);
void coat_file_sprite_xy(BITMAP *target, int col, int x1, int y1, int x2, int y2);
void prepare_flowers(void);
void extract_rle_struct(BITMAP *source, int x1, int y1, int x2, int y2, const char errtxt [], struct RLE_STRUCT *str, int bl);
void extract_rle_struct2(BITMAP *source, struct RLE_STRUCT *str, int x_source, int y_source, int x, int y, int b1);
void prepare_underlay(void);

void new_rle_struct_colours(BITMAP *source, const char errtxt [], struct RLE_STRUCT str [SMALL_ROTATIONS] [9], int bl, int index2, int col1, int col2);


//extern struct RLE_STRUCT sdart_bullet [SMALL_ROTATIONS] [9];

//extern RLE_SPRITE *trans_RLE [TRANS_RLES];

extern RLE_SPRITE *RLE_circle [RLE_CIRCLES] [9];
extern RLE_SPRITE *RLE_bcircle [3] [RLE_BCIRCLES];
extern RLE_SPRITE *RLE_ccircle_basic [4] [RLE_CCIRCLES];
extern RLE_SPRITE *RLE_ccircle_4cols [4] [RLE_CCIRCLES];
extern RLE_SPRITE *RLE_ccircle_3cols [4] [RLE_CCIRCLES];
extern RLE_SPRITE *RLE_small_shock [4] [30];
extern RLE_SPRITE *RLE_large_shock [4] [50];
extern RLE_SPRITE *RLE_huge_shock [4] [50];
extern RLE_SPRITE *RLE_ibullet [4] [IBULLETS];
extern RLE_SPRITE *RLE_ibullet_2 [3] [IBULLETS_2];
extern RLE_SPRITE *RLE_ibullet_rgb [IBULLETS_3];

extern RLE_SPRITE *RLE_ring [RLE_RINGS];
extern RLE_SPRITE *RLE_bright_ring [RLE_RINGS];

extern struct RLE_STRUCT RLE_barrel1 [SMALL_ROTATIONS];

BITMAP *load_up_bitmap(const char fname []);
RLE_SPRITE *extract_rle_sprite(BITMAP *source, int x_source, int y_source, int x, int y);
RLE_SPRITE *extract_flip_rle_sprite(BITMAP *source, int x_source, int y_source, int x, int y, int flip_type);
RLE_SPRITE *sized_rle_sprite(BITMAP *source, int x, int y, const char errtxt []);
RLE_SPRITE *new_rle_sprite(BITMAP *source, const char errtxt []);


extern int beam_colours [4] [5];






/*

Basic display init functions.

*/

/*
Call during start-up.
*/
void prepare_display(void)
{

 define_palette();

// set_palette(palet [0]);
 set_palette(palet);

 clear_to_color(screen, COL_OUTLINE);

 DATAFILE *datf = load_datafile("gfx//garden.dat");
 if (datf == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Error: Couldn't find data.dat! \n");
  allegro_message("\n\r");
  exit(1);
 }
// int i, j;

 font = (FONT *)datf[0].dat;

/*

splash bitmap

*/
/*
 RGB splash_palette [256];


 BITMAP *splash_bitmap = load_bitmap("gfx//splash.bmp", splash_palette);

 if (splash_bitmap == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Failed to load in bitmap! (File not found?)  \n%s", "splash.bmp");
  exit(1);
 }
*/
/* BITMAP *wait_bitmap = load_bitmap("gfx//wait.bmp", splash_palette);

 if (wait_bitmap == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Failed to load in bitmap! (File not found?)  \n%s", "wait.bmp");
  exit(1);
 }*/


 vsync();

 clear_to_color(screen, 0);
 rectfill(screen, 100, 0, 700, 600, COL_BACK1);

 textprintf_centre_ex(screen, font, 400, 300, -1, -1, "loading . . .");
/*
int l = 0;
int ly = 10;

for (l = 0; l < 900000; l ++)
{
 if (key [KEY_ESC])
  exit(1);
 if ((l >> 10) != (l / 1024))
 {
  textprintf_ex(screen, font, 40, ly, -1, -1, "%i %i %i", l, l >> 10, l / 1024);
  ly += 10;
 }

}*/

/*

end splash

*/

//#define FIX_FONT

#ifdef FIX_FONT

 int i, j;

 BITMAP *font_bitmap = load_up_bitmap("g_font4.bmp");

 int back_col = getpixel(font_bitmap, 0, 0);
 int back_col2 = getpixel(font_bitmap, 1, 0);
 int out_col = getpixel(font_bitmap, 17, 4);
 int white_col = getpixel(font_bitmap, 18, 4);
// int lgrey_col = getpixel(font_bitmap, 8, 19);

 for (i = 0; i < font_bitmap->w; i ++)
 {
  for (j = 0; j < font_bitmap->h; j ++)
  {
   if (getpixel(font_bitmap, i, j) == back_col
       || getpixel(font_bitmap, i, j) == back_col2)
   {
       putpixel(font_bitmap, i, j, 255);
       continue;
   }
   if (getpixel(font_bitmap, i, j) == out_col)
   {
       putpixel(font_bitmap, i, j, COL_OUTLINE);
       continue;
   }
   if (getpixel(font_bitmap, i, j) == white_col)
   {
       putpixel(font_bitmap, i, j, COL_WHITE);
       continue;
   }
/*   if (getpixel(font_bitmap, i, j) == lgrey_col)
   {
       putpixel(font_bitmap, i, j, COL_LGREY);
       continue;*/
   }
  }


 save_bitmap("g_font3.bmp", font_bitmap, palet);

#endif

      drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);


 //prepare_trans_rles();
 prepare_s_enemy_rles();
 prepare_player_rles();
 prepare_circles();
 prepare_barrels();

 prepare_l_enemy_rles();
// prepare_enemy_bullets();
// prepare_platform_rles();

 display = new_bitmap(600, 600, "Display");

 BITMAP *temp_white_bitmap = load_up_bitmap("gfx//white.bmp");
 fix_outline(temp_white_bitmap);

 white_RLE = extract_rle_sprite(temp_white_bitmap, 1, 1, 280, 53);

 destroy_bitmap(temp_white_bitmap);



}

void prepare_circles(void)
{

// int cx, cy, ci, cs, ca, cd, cj, cn;

 int i, j;

 BITMAP *temp_bitmap;

 for (i = 0; i < RLE_BCIRCLES; i ++)
 {
  temp_bitmap = new_bitmap((i + 1) * 2, (i + 1) * 2, "prepare bcircles");
  clear_bitmap(temp_bitmap);
  for (j = 0; j < 3; j ++)
  {
   circlefill(temp_bitmap, i + 1, i + 1, i, COL_BCIRCLE_1 + j);
   circle(temp_bitmap, i + 1, i + 1, i, COL_BCIRCLE_OUT_1 + j);
   RLE_bcircle [j] [i] = get_rle_sprite(temp_bitmap);
  }
  destroy_bitmap(temp_bitmap);
 }

 int rad;

 for (i = 0; i < RLE_CCIRCLES; i ++)
 {
  temp_bitmap = new_bitmap((i + 1) * 2, (i + 1) * 2, "prepare ccircles");
  clear_bitmap(temp_bitmap);
  for (j = 0; j < 4; j ++)
  {



   rad = i;
   circlefill(temp_bitmap, i + 1, i + 1, rad, beam_colours [j] [0]);
   rad *= 0.90;
   if (rad > 0)
    circlefill(temp_bitmap, i + 1, i + 1, rad, beam_colours [j] [1]);
   rad *= 0.90;
   if (rad > 0)
    circlefill(temp_bitmap, i + 1, i + 1, rad, beam_colours [j] [2]);

   RLE_ccircle_3cols [j] [i] = get_rle_sprite(temp_bitmap);

   rad *= 0.90;
   if (rad > 0)
    circlefill(temp_bitmap, i + 1, i + 1, rad, beam_colours [j] [3]);

/*
   for (ci = 0; ci < 3 + i; ci ++)
   {
    ca = grand(ANGLE_1);
    cd = i - grand(3) - grand(3);

    cx = i + 1 + xpart(ca, cd);
    cy = i + 1 + ypart(ca, cd);
    cs = 1 + grand(3) + grand(i / 10);
    if (cs > i/2)
     cs = i/2;
    cn = cs;
    if (cn > 5)
     cn = 5;
    for (cj = 0; cj < cn; cj ++)
    {
     circlefill(temp_bitmap, cx, cy, cj, beam_colours [j] [5 - cj]);
    }
   }

*/
   RLE_ccircle_4cols [j] [i] = get_rle_sprite(temp_bitmap);

   rad *= 0.90;
   if (rad > 0)
    circlefill(temp_bitmap, i + 1, i + 1, rad, beam_colours [j] [4]);

   RLE_ccircle_basic [j] [i] = get_rle_sprite(temp_bitmap);
  }

  destroy_bitmap(temp_bitmap);
 }


 int rad2;

 for (i = 0; i < 30; i ++)
 {
  rad = i + 15; // see below
  temp_bitmap = new_bitmap((rad + 1) * 2, (rad + 1) * 2, "prepare small_shock");
  clear_bitmap(temp_bitmap);
  for (j = 0; j < 4; j ++)
  {
   rad = i + 15; // see above
   rad2 = rad;

   circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [0]);
   if (i < 22)
   {
    rad2 --;
    circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [2]);
   }
   if (i < 22 && 0)
   {
    rad2 --;
    circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [4]);
   }
   if (i < 22)
   {
    rad2 --;
    circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [3]);
   }
   if (i < 24)
   {
    rad2 --;
    circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [2]);
   }
   if (i < 26)
   {
    rad2 --;
    circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [1]);
   }
   if (i < 28)
   {
    rad2 -= 2;
    circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [0]);
   }

   rad2 -= 2;

   if (rad2 > -1)
   circlefill(temp_bitmap, rad + 1, rad + 1, rad2, 0);

   RLE_small_shock [j] [i] = get_rle_sprite(temp_bitmap);
  }

  destroy_bitmap(temp_bitmap);
 }


 for (i = 0; i < 50; i ++)
 {
  rad = i + 25; // see below
  temp_bitmap = new_bitmap((rad + 1) * 2, (rad + 1) * 2, "prepare large_shock");
  clear_bitmap(temp_bitmap);
  for (j = 0; j < 4; j ++)
  {
   rad = i + 25; // see above
   rad2 = rad;

   circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [0]);
   if (i < 42)
   {
    rad2 --;
    if (i < 32)
     rad2 --;
    circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [2]);
   }
   if (i < 42 && 0)
   {
    rad2 --;
    if (i < 34)
     rad2 --;
    circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [4]);
   }
   if (i < 42)
   {
    rad2 --;
    if (i < 36)
     rad2 --;
    circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [3]);
   }
   if (i < 44)
   {
    rad2 --;
    if (i < 38)
     rad2 --;
    circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [2]);
   }
   if (i < 46)
   {
    rad2 --;
    if (i < 40)
     rad2 --;
    circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [1]);
   }
   if (i < 48)
   {
    rad2 -= 2;
    if (i < 42)
     rad2 -= 2;
    circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [0]);
   }

   rad2 -= 2;
    if (i < 44)
     rad2 -= 2;

   if (rad2 > -1)
   circlefill(temp_bitmap, rad + 1, rad + 1, rad2, 0);

   RLE_large_shock [j] [i] = get_rle_sprite(temp_bitmap);
  }

  destroy_bitmap(temp_bitmap);
 }


 for (i = 0; i < 50; i ++)
 {
  rad = i + 50; // see below
  temp_bitmap = new_bitmap((rad + 1) * 2, (rad + 1) * 2, "prepare huge_shock");
  clear_bitmap(temp_bitmap);
  for (j = 0; j < 3; j ++)
  {
   rad = i + 50; // see above
   rad2 = rad;

   circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [0]);
   if (i < 42)
   {
    rad2 --;
    if (i < 32)
     rad2 --;
    circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [2]);
   }
   if (i < 42 && 0)
   {
    rad2 --;
    if (i < 34)
     rad2 --;
    circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [4]);
   }
   if (i < 42)
   {
    rad2 --;
    if (i < 36)
     rad2 --;
    circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [3]);
   }
   if (i < 44)
   {
    rad2 --;
    if (i < 38)
     rad2 --;
    circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [2]);
   }
   if (i < 46)
   {
    rad2 --;
    if (i < 40)
     rad2 --;
    circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [1]);
   }
   if (i < 48)
   {
    rad2 -= 2;
    if (i < 42)
     rad2 -= 2;
    circlefill(temp_bitmap, rad + 1, rad + 1, rad2, beam_colours [j] [0]);
   }

   rad2 -= 2;
    if (i < 44)
     rad2 -= 2;

   if (rad2 > -1)
   circlefill(temp_bitmap, rad + 1, rad + 1, rad2, 0);

   RLE_huge_shock [j] [i] = get_rle_sprite(temp_bitmap);
  }

  destroy_bitmap(temp_bitmap);
 }



 temp_bitmap = new_bitmap(13, 13, "prepare ibullets");

#define IB_CENTRE 6

  for (j = 0; j < 4; j ++)
  {
   for (i = 0; i < IBULLETS; i ++)
   {
    rad = IBULLETS - i;
    clear_bitmap(temp_bitmap);
     circlefill(temp_bitmap, IB_CENTRE, IB_CENTRE, IBULLETS, beam_colours [j] [0]);
     circlefill(temp_bitmap, IB_CENTRE, IB_CENTRE, IBULLETS - 1, beam_colours [j] [2]);
     circlefill(temp_bitmap, IB_CENTRE, IB_CENTRE, IBULLETS - 2, beam_colours [j] [4]);

     if (rad > 0)
     {
      circlefill(temp_bitmap, IB_CENTRE, IB_CENTRE, rad, beam_colours [j] [2]);
      rad --;
      if (rad > 0)
      {
       circlefill(temp_bitmap, IB_CENTRE, IB_CENTRE, rad, beam_colours [j] [0]);
       rad --;
       if (rad > 0)
       {
        circlefill(temp_bitmap, IB_CENTRE, IB_CENTRE, rad, beam_colours [j] [2]);
        rad --;
        if (rad > 0)
         circlefill(temp_bitmap, IB_CENTRE, IB_CENTRE, rad, beam_colours [j] [4]);
       }
      }
     }

     RLE_ibullet [j] [i] = get_rle_sprite(temp_bitmap);

   }
  }

 int col1, col2;

 int second_col1, second_col2;

 for (j = 0; j < 3; j ++)
 {
   switch(j)
   {
    default: col1 = 0; col2 = 1; break;
    case 1: col1 = 0; col2 = 2; break;
    case 2: col1 = 1; col2 = 2; break;
   }
   for (i = 0; i < IBULLETS_2; i ++)
   {
    rad = IBULLETS - i;
    if (rad < 1)
     rad += IBULLETS;
    clear_bitmap(temp_bitmap);
    if (i < IBULLETS)
    {
     second_col1 = col1;
     second_col2 = col2;
    }
     else
     {
      second_col1 = col2;
      second_col2 = col1;
     }
     circlefill(temp_bitmap, IB_CENTRE, IB_CENTRE, IBULLETS, TRANS_RED1 + (TRANS_DIFFERENCE * second_col1));
     circlefill(temp_bitmap, IB_CENTRE, IB_CENTRE, IBULLETS - 1, TRANS_RED3 + (TRANS_DIFFERENCE * second_col1));
     circlefill(temp_bitmap, IB_CENTRE, IB_CENTRE, IBULLETS - 2, TRANS_RED5 + (TRANS_DIFFERENCE * second_col1));

     if (rad > 0)
     {
      circlefill(temp_bitmap, IB_CENTRE, IB_CENTRE, rad, TRANS_RED3 + (TRANS_DIFFERENCE * second_col2));
      rad --;
      if (rad > 0)
      {
       circlefill(temp_bitmap, IB_CENTRE, IB_CENTRE, rad, TRANS_RED1 + (TRANS_DIFFERENCE * second_col2));
       rad --;
       if (rad > 0)
       {
        circlefill(temp_bitmap, IB_CENTRE, IB_CENTRE, rad, TRANS_RED3 + (TRANS_DIFFERENCE * second_col2));
        rad --;
        if (rad > 0)
         circlefill(temp_bitmap, IB_CENTRE, IB_CENTRE, rad, TRANS_RED5 + (TRANS_DIFFERENCE * second_col2));
       }
      }
     }

     RLE_ibullet_2 [j] [i] = get_rle_sprite(temp_bitmap);

   }
 }



 for (i = 0; i < RLE_RINGS; i ++)
 {
  rad = i + 3;
  temp_bitmap = new_bitmap((rad + 1) * 2, (rad + 1) * 2, "prepare rings");
  clear_bitmap(temp_bitmap);
  //for (j = 0; j < 3; j ++)
  {
   circlefill(temp_bitmap, rad + 1, rad + 1, rad, TRANS_RED1);
   circlefill(temp_bitmap, rad + 1, rad + 1, rad - 1, TRANS_RED2);
   circlefill(temp_bitmap, rad + 1, rad + 1, rad - 2, TRANS_RED1);
   circlefill(temp_bitmap, rad + 1, rad + 1, rad - 3, 0);

   RLE_ring [i] = get_rle_sprite(temp_bitmap);
  }
  destroy_bitmap(temp_bitmap);
 }


 for (i = 0; i < RLE_RINGS; i ++)
 {
  rad = i + 3;
  temp_bitmap = new_bitmap((rad + 1) * 2, (rad + 1) * 2, "prepare bright rings");
  clear_bitmap(temp_bitmap);
  //for (j = 0; j < 3; j ++)
  {
   circlefill(temp_bitmap, rad + 1, rad + 1, rad, TRANS_RED1);
   circlefill(temp_bitmap, rad + 1, rad + 1, rad - 1, TRANS_RED2);
   circlefill(temp_bitmap, rad + 1, rad + 1, rad - 2, TRANS_RED3);
   circlefill(temp_bitmap, rad + 1, rad + 1, rad - 3, TRANS_RED2);
   circlefill(temp_bitmap, rad + 1, rad + 1, rad - 4, TRANS_RED1);
   circlefill(temp_bitmap, rad + 1, rad + 1, rad - 5, 0);

   RLE_bright_ring [i] = get_rle_sprite(temp_bitmap);
  }
  destroy_bitmap(temp_bitmap);
 }

  destroy_bitmap(temp_bitmap);
  // temp_bitmap is not created each time through loop for icircles.

}



void prepare_player_rles(void)
{


 BITMAP *file_bitmap = load_up_bitmap("gfx//player.bmp");

 fix_outline(file_bitmap);

    eRLE_player [S_PLAYER1_1] = extract_rle_sprite(file_bitmap, 1, 1, 33, 30);
    eRLE_player [S_PLAYER1_2] = extract_rle_sprite(file_bitmap, 35, 1, 33, 30);
    eRLE_player [S_PLAYER1_3] = extract_rle_sprite(file_bitmap, 69, 1, 33, 30);

    eRLE_player [S_PLAYER2_1] = extract_rle_sprite(file_bitmap, 1, 32, 39, 28);
    eRLE_player [S_PLAYER2_2] = extract_rle_sprite(file_bitmap, 41, 32, 41, 29);
    eRLE_player [S_PLAYER2_3] = extract_rle_sprite(file_bitmap, 83, 32, 43, 30);

    eRLE_player [S_PLAYER3_1] = extract_rle_sprite(file_bitmap, 141, 3, 45, 28);

    eRLE_player [S_PLAYER4_1] = extract_rle_sprite(file_bitmap, 107, 3, 33, 12);
    eRLE_player [S_PLAYER4_2] = extract_rle_sprite(file_bitmap, 281, 53, 33, 12);
    eRLE_player [S_PLAYER4_3] = extract_rle_sprite(file_bitmap, 315, 53, 33, 13);

    eRLE_player [S_PLAYER4_1L] = extract_rle_sprite(file_bitmap, 31, 72, 33, 16);
    eRLE_player [S_PLAYER4_2L] = extract_rle_sprite(file_bitmap, 64, 72, 33, 17);
    eRLE_player [S_PLAYER4_3L] = extract_rle_sprite(file_bitmap, 97, 72, 33, 18);

    eRLE_player [S_PLAYER5_1] = extract_rle_sprite(file_bitmap, 238, 6, 35, 36);
    eRLE_player [S_PLAYER5_2] = extract_rle_sprite(file_bitmap, 275, 6, 33, 37);
    eRLE_player [S_PLAYER5_3] = extract_rle_sprite(file_bitmap, 312, 6, 31, 38);

    eRLE_player [S_PLAYER6_1] = extract_rle_sprite(file_bitmap, 425, 49, 33, 33);

/*
    eRLE_small1 [S_ENEMY_SWOOPER1_1] = extract_rle_sprite(file_bitmap, 1, 1, 31, 32);
    eRLE_small1 [S_ENEMY_SWOOPER1_2] = extract_rle_sprite(file_bitmap, 33, 1, 33, 32);
    eRLE_small1 [S_ENEMY_SWOOPER1_3] = extract_rle_sprite(file_bitmap, 67, 1, 35, 32);
    eRLE_small1 [S_ENEMY_SWOOPER1_4] = extract_rle_sprite(file_bitmap, 103, 1, 37, 34);

    eRLE_small1 [S_ENEMY_DARTER1_MAIN] = extract_rle_sprite(file_bitmap, 141, 1, 49, 51);
    eRLE_small1 [S_ENEMY_DARTER1_CENTRE] = extract_rle_sprite(file_bitmap, 219, 1, 27, 28);
    eRLE_small1 [S_ENEMY_DARTER1_TOP] = extract_rle_sprite(file_bitmap, 191, 1, 27, 14);
    eRLE_small1 [S_ENEMY_DARTER1_INNER_L] = extract_rle_sprite(file_bitmap, 192, 16, 14, 41);
    eRLE_small1 [S_ENEMY_DARTER1_OUTER_L] = extract_rle_sprite(file_bitmap, 207, 16, 9, 31);
    eRLE_small1 [S_ENEMY_DARTER1_INNER_R] = extract_flip_rle_sprite(file_bitmap, 192, 16, 14, 41, FLIP_H);
    eRLE_small1 [S_ENEMY_DARTER1_OUTER_R] = extract_flip_rle_sprite(file_bitmap, 207, 16, 9, 31, FLIP_H);
*/

    destroy_bitmap(file_bitmap);

}

void prepare_s_enemy_rles(void)
{


 BITMAP *file_bitmap = load_up_bitmap("gfx//small.bmp");

 fix_outline(file_bitmap);

    eRLE_small1 [S_ENEMY_FIGHTER] = extract_rle_sprite(file_bitmap, 1, 1, 29, 27);
    eRLE_small1 [S_ENEMY_FIGHTER_2] = extract_rle_sprite(file_bitmap, 31, 1, 29, 27);
    eRLE_small1 [S_ENEMY_FIGHTER_3] = extract_rle_sprite(file_bitmap, 61, 1, 31, 28);
    eRLE_small1 [S_ENEMY_FIGHTER2] = extract_rle_sprite(file_bitmap, 93, 1, 41, 27);
    eRLE_small1 [S_ENEMY_FIGHTER2_2] = extract_rle_sprite(file_bitmap, 135, 1, 41, 27);
    eRLE_small1 [S_ENEMY_FIGHTER2_3] = extract_rle_sprite(file_bitmap, 177, 1, 41, 27);
    eRLE_small1 [S_ENEMY_FIGHTER2_4] = extract_rle_sprite(file_bitmap, 219, 1, 43, 27);
    eRLE_small1 [S_ENEMY_FIGHTER3] = extract_rle_sprite(file_bitmap, 1, 29, 35, 32);
    eRLE_small1 [S_ENEMY_FIGHTER3_2] = extract_rle_sprite(file_bitmap, 37, 31, 37, 32);
    eRLE_small1 [S_ENEMY_FIGHTER3_3] = extract_rle_sprite(file_bitmap, 75, 31, 39, 32);
    eRLE_small1 [S_ENEMY_FIGHTER4] = extract_rle_sprite(file_bitmap, 115, 31, 39, 29);
    eRLE_small1 [S_ENEMY_MACHINE] = extract_rle_sprite(file_bitmap, 155, 30, 43, 39);
    eRLE_small1 [S_ENEMY_MACHINE_2] = extract_rle_sprite(file_bitmap, 199, 30, 47, 39);
    eRLE_small1 [S_ENEMY_MACHINE_3] = extract_rle_sprite(file_bitmap, 247, 30, 51, 40);
    eRLE_small1 [S_ENEMY_MACHINE_4] = extract_rle_sprite(file_bitmap, 300, 30, 55, 41);

    eRLE_small1 [S_ENEMY_MACHINE2] = extract_rle_sprite(file_bitmap, 356, 1, 85, 43);
    eRLE_small1 [S_ENEMY_MACHINE2_2] = extract_rle_sprite(file_bitmap, 359, 47, 87, 44);
    eRLE_small1 [S_ENEMY_MACHINE2_3] = extract_rle_sprite(file_bitmap, 442, 1, 89, 45);
    eRLE_small1 [S_ENEMY_MACHINE2_4] = extract_rle_sprite(file_bitmap, 450, 47, 91, 46);

    eRLE_small1 [S_ENEMY_FIGHTER5] = extract_rle_sprite(file_bitmap, 534, 2, 39, 32);
    eRLE_small1 [S_ENEMY_FIGHTER5_2] = extract_rle_sprite(file_bitmap, 574, 2, 41, 33);
    eRLE_small1 [S_ENEMY_FIGHTER5_3] = extract_rle_sprite(file_bitmap, 616, 2, 43, 34);

    eRLE_small1 [S_ENEMY_FIGHTER6] = extract_rle_sprite(file_bitmap, 543, 39, 37, 33);

    eRLE_small1 [S_ENEMY_MEDIUM] = extract_rle_sprite(file_bitmap, 660, 2, 41, 53);
    eRLE_small1 [S_ENEMY_MEDIUM_2] = extract_rle_sprite(file_bitmap, 702, 2, 42, 54);
    eRLE_small1 [S_ENEMY_MEDIUM_3] = extract_rle_sprite(file_bitmap, 745, 2, 42, 55);
    eRLE_small1 [S_ENEMY_MEDIUM_4] = extract_rle_sprite(file_bitmap, 788, 2, 42, 56);

    eRLE_small1 [S_ENEMY_RISER] = extract_rle_sprite(file_bitmap, 831, 2, 37, 48);
    eRLE_small1 [S_ENEMY_RISER_2] = extract_rle_sprite(file_bitmap, 869, 2, 37, 48);
    eRLE_small1 [S_ENEMY_RISER_3] = extract_rle_sprite(file_bitmap, 907, 2, 37, 48);
    eRLE_small1 [S_ENEMY_RISER_4] = extract_rle_sprite(file_bitmap, 945, 2, 37, 48);
    eRLE_small1 [S_ENEMY_RISER_5] = extract_rle_sprite(file_bitmap, 983, 2, 37, 48);

    eRLE_small1 [S_ENEMY_MACHINE3] = extract_rle_sprite(file_bitmap, 1023, 1, 65, 56);
    eRLE_small1 [S_ENEMY_MACHINE3_2] = extract_rle_sprite(file_bitmap, 1089, 1, 67, 57);
    eRLE_small1 [S_ENEMY_MACHINE3_3] = extract_rle_sprite(file_bitmap, 1157, 1, 69, 58);
    eRLE_small1 [S_ENEMY_MACHINE3_4] = extract_rle_sprite(file_bitmap, 1228, 1, 71, 59);

    eRLE_small1 [S_ENEMY_FIGHTER7] = extract_rle_sprite(file_bitmap, 1301, 1, 43, 27);
    eRLE_small1 [S_ENEMY_FIGHTER8] = extract_rle_sprite(file_bitmap, 1301, 29, 27, 24);


/*
    eRLE_small1 [S_ENEMY_SWOOPER1_1] = extract_rle_sprite(file_bitmap, 1, 1, 31, 32);
    eRLE_small1 [S_ENEMY_SWOOPER1_2] = extract_rle_sprite(file_bitmap, 33, 1, 33, 32);
    eRLE_small1 [S_ENEMY_SWOOPER1_3] = extract_rle_sprite(file_bitmap, 67, 1, 35, 32);
    eRLE_small1 [S_ENEMY_SWOOPER1_4] = extract_rle_sprite(file_bitmap, 103, 1, 37, 34);

    eRLE_small1 [S_ENEMY_DARTER1_MAIN] = extract_rle_sprite(file_bitmap, 141, 1, 49, 51);
    eRLE_small1 [S_ENEMY_DARTER1_CENTRE] = extract_rle_sprite(file_bitmap, 219, 1, 27, 28);
    eRLE_small1 [S_ENEMY_DARTER1_TOP] = extract_rle_sprite(file_bitmap, 191, 1, 27, 14);
    eRLE_small1 [S_ENEMY_DARTER1_INNER_L] = extract_rle_sprite(file_bitmap, 192, 16, 14, 41);
    eRLE_small1 [S_ENEMY_DARTER1_OUTER_L] = extract_rle_sprite(file_bitmap, 207, 16, 9, 31);
    eRLE_small1 [S_ENEMY_DARTER1_INNER_R] = extract_flip_rle_sprite(file_bitmap, 192, 16, 14, 41, FLIP_H);
    eRLE_small1 [S_ENEMY_DARTER1_OUTER_R] = extract_flip_rle_sprite(file_bitmap, 207, 16, 9, 31, FLIP_H);
*/

    destroy_bitmap(file_bitmap);

}


void prepare_l_enemy_rles(void)
{

// this will 'coat' a section of a bitmap file with outline pixels. Because doing it manually with the GIMP is a pain.
#ifdef FIXING

 RGB temp_palette [256];

 BITMAP *temp_bitmap = load_bitmap("gfx//large.bmp", temp_palette);

// set_palette(temp_palette);

 int col = getpixel(temp_bitmap, 2058, 23);

 coat_file_sprite_xy(temp_bitmap, col, 2200, 0, 2474, 199);

 save_bitmap("olarge.bmp", temp_bitmap, temp_palette);

#endif

 BITMAP *file_bitmap = load_up_bitmap("gfx//large.bmp");

 fix_outline(file_bitmap);



    eRLE_large1 [L_ENEMY_DOUBLE] = extract_rle_sprite(file_bitmap, 1, 1, 65, 73);
    eRLE_large1 [L_ENEMY_DOME] = extract_rle_sprite(file_bitmap, 67, 1, 69, 116);
    eRLE_large1 [L_ENEMY_BIG] = extract_rle_sprite(file_bitmap, 138, 1, 79, 138);
    eRLE_large1 [L_ENEMY_BIG2] = extract_rle_sprite(file_bitmap, 219, 1, 145, 132);
    eRLE_large1 [L_ENEMY_DOUBLE2] = extract_rle_sprite(file_bitmap, 365, 1, 145, 94);
    eRLE_large1 [L_ENEMY_DOUBLE3] = extract_rle_sprite(file_bitmap, 365, 113, 163, 81);
    eRLE_large1 [L_ENEMY_BIG3] = extract_rle_sprite(file_bitmap, 144, 142, 179, 103);
    eRLE_large1 [L_ENEMY_BOSS1_1] = extract_rle_sprite(file_bitmap, 512, 8, 297, 133);
    eRLE_large1 [L_ENEMY_DOME2] = extract_rle_sprite(file_bitmap, 1, 118, 79, 96);

    eRLE_large1 [L_ENEMY_WIDE1] = extract_rle_sprite(file_bitmap, 531, 144, 151, 52);
    eRLE_large1 [L_ENEMY_WIDE1_2] = extract_rle_sprite(file_bitmap, 530, 197, 153, 52);
    eRLE_large1 [L_ENEMY_WIDE1_3] = extract_rle_sprite(file_bitmap, 683, 144, 155, 52);
    eRLE_large1 [L_ENEMY_WIDE1_4] = extract_rle_sprite(file_bitmap, 684, 197, 157, 52);
    eRLE_large1 [L_ENEMY_BIG4] = extract_rle_sprite(file_bitmap, 810, 2, 121, 99);
    eRLE_large1 [L_ENEMY_DOUBLE4] = extract_rle_sprite(file_bitmap, 933, 5, 193, 107);
    eRLE_large1 [L_ENEMY_DOME3] = extract_rle_sprite(file_bitmap, 845, 111, 83, 99);

    destroy_bitmap(file_bitmap);



 file_bitmap = load_up_bitmap("gfx//large2.bmp");

 fix_outline(file_bitmap);



    eRLE_large2 [L_ENEMY2_BOSS1_2] = extract_rle_sprite(file_bitmap, 2, 3, 297, 124);
    eRLE_large2 [L_ENEMY2_BOSS1_3] = extract_rle_sprite(file_bitmap, 2, 132, 297, 134);

    eRLE_large2 [L_ENEMY2_BOSS2_1] = extract_rle_sprite(file_bitmap, 2, 270, 321, 218);
    eRLE_large2 [L_ENEMY2_BOSS2_2] = extract_rle_sprite(file_bitmap, 9, 494, 107, 70);

    eRLE_large2 [L_ENEMY2_BOSS3_1] = extract_rle_sprite(file_bitmap, 119, 490, 295, 170);
    eRLE_large2 [L_ENEMY2_BOSS3_2] = extract_rle_sprite(file_bitmap, 2, 663, 289, 172);
    eRLE_large2 [L_ENEMY2_BOSS3_0] = extract_rle_sprite(file_bitmap, 4, 840, 257, 126);


/*  blit(file_bitmap, screen, 0, 0, 0, 0, 800, 600);

  do
  {
   rest(20);
  } while(!key [KEY_8]);*/

    destroy_bitmap(file_bitmap);

}

void prepare_trans_rles(void)
{


 BITMAP *file_bitmap = load_up_bitmap("gfx//trans.bmp");

 fix_trans(file_bitmap);
/*
 extract_rle_struct2(file_bitmap, RLE_mg_shock, 1, 1, 1, 1, 0);
 extract_rle_struct2(file_bitmap, RLE_mg_shock, 3, 1, 3, 3, 1);
 extract_rle_struct2(file_bitmap, RLE_mg_shock, 7, 1, 7, 7, 2);
 extract_rle_struct2(file_bitmap, RLE_mg_shock, 15, 1, 9, 9, 3);
 extract_rle_struct2(file_bitmap, RLE_mg_shock, 25, 1, 11, 11, 4);
 extract_rle_struct2(file_bitmap, RLE_mg_shock, 37, 1, 13, 13, 5);
 extract_rle_struct2(file_bitmap, RLE_mg_shock, 51, 1, 17, 17, 6);
 extract_rle_struct2(file_bitmap, RLE_mg_shock, 69, 1, 19, 19, 7);
 extract_rle_struct2(file_bitmap, RLE_mg_shock, 89, 1, 21, 21, 8);
*/

    destroy_bitmap(file_bitmap);

}

void prepare_barrels(void)
{

 int i;

 BITMAP *temp_bitmap;

 temp_bitmap = new_bitmap(20, 20, "prepare_barrels1");

 int inc = ANGLE_1 / SMALL_ROTATIONS;
 int angle;

 for (i = 0; i < SMALL_ROTATIONS; i ++)
 {

 clear_bitmap(temp_bitmap);
 angle = i * inc;

 bordered_poly4(temp_bitmap,
  10 + xpart(angle + ANGLE_16, 4), 10 + ypart(angle + ANGLE_16, 4),
  10 + xpart(angle - ANGLE_16, 4), 10 + ypart(angle - ANGLE_16, 4),
  10 - xpart(angle + ANGLE_16, 4), 10 - ypart(angle + ANGLE_16, 4),
  10 - xpart(angle - ANGLE_16, 4), 10 - ypart(angle - ANGLE_16, 4),
  COL_COL4, COL_COL1);
//  circlefill(temp_bitmap, 10, 10, 5, COL_BACK2);

 new_rle_struct(temp_bitmap, "barrel1 RLE", &RLE_barrel1 [i], -1);

 } // end for i

  destroy_bitmap(temp_bitmap);


}

/*
void prepare_enemy_bullets(void)
{
 int i;//, o;

 BITMAP *temp_bitmap;

 temp_bitmap = new_bitmap(50, 50, "prepare_glass_bullets");

 int inc = ANGLE_1 / SMALL_ROTATIONS;
 int angle;

 for (i = 0; i < SMALL_ROTATIONS; i ++)
 {
  clear_bitmap(temp_bitmap);
  angle = i * inc;

bordered_poly4(temp_bitmap,
  25 + xpart(angle, 23), 25 + ypart(angle, 23),
  25 + xpart(angle + ANGLE_8, 11), 25 + ypart(angle + ANGLE_8, 11),
  25, 25,
  25 + xpart(angle - ANGLE_8, 11), 25 + ypart(angle - ANGLE_8, 11),
  col, border);

  new_rle_struct_colours(temp_bitmap, "prepare dart bullets", dart_bullet, -1, i, TCOL_B1, TCOL_B5);

// Now let's make small darts:

  clear_bitmap(temp_bitmap);
  angle = i * inc;

  bordered_poly4(temp_bitmap,
  25 + xpart(angle, 17), 25 + ypart(angle, 17),
  25 + xpart(angle + ANGLE_8, 7), 25 + ypart(angle + ANGLE_8, 7),
  25, 25,
  25 + xpart(angle - ANGLE_8, 7), 25 + ypart(angle - ANGLE_8, 7),
  col, border);

  new_rle_struct_colours(temp_bitmap, "prepare sdart bullets", sdart_bullet, -1, i, TCOL_B1, TCOL_B5);

// now let's do circles!!
 int j;

 for (i = 0; i < RLE_CIRCLES; i ++)
 {
  temp_bitmap = new_bitmap((i + 1) * 2, (i + 1) * 2, "prepare circles");
  clear_bitmap(temp_bitmap);
//  clear_to_color(temp_bitmap, TRANS_YELLOW_OUT);
  for (j = 0; j < 9; j ++)
  {
   circlefill(temp_bitmap, i + 1, i + 1, i, 28 + j * 26);
   circle(temp_bitmap, i + 1, i + 1, i, 15 + j * 26);
   RLE_circle [i] [j] = get_rle_sprite(temp_bitmap);
  }
  destroy_bitmap(temp_bitmap);
 }

 // now it's beam time!

 temp_bitmap = new_bitmap(60, 60, "prepare beams");

 for (i = 0; i < SMALL_ROTATIONS; i ++)
 {
  clear_bitmap(temp_bitmap);
  angle = i * inc;
  fast_bordered_poly4(temp_bitmap,
   30 + xpart(angle, 25) + xpart(angle + ANGLE_4, 4), 30 + ypart(angle, 25) + ypart(angle + ANGLE_4, 4),
   30 + xpart(angle, 25) + xpart(angle - ANGLE_4, 4), 30 + ypart(angle, 25) + ypart(angle - ANGLE_4, 4),
   30 - xpart(angle, 25) + xpart(angle - ANGLE_4, 4), 30 - ypart(angle, 25) + ypart(angle - ANGLE_4, 4),
   30 - xpart(angle, 25) + xpart(angle + ANGLE_4, 4), 30 - ypart(angle, 25) + ypart(angle + ANGLE_4, 4),
   TRANS_B5_IN,TRANS_B5_OUT,
   30, 30);

   new_rle_struct(temp_bitmap, "beam RLE", &RLE_beam [i], -1);

 }

 destroy_bitmap(temp_bitmap);

}
*/

/*
void prepare_blades(void)
{

 int i;

 BITMAP *temp_bitmap;

 temp_bitmap = new_bitmap(80, 80, "prepare_blades");

 int inc = ANGLE_1 / SMALL_ROTATIONS;
 int angle;

 for (i = 0; i < SMALL_ROTATIONS; i ++)
 {

 clear_bitmap(temp_bitmap);
 angle = i * inc;

 bordered_poly4(temp_bitmap,
  40 + xpart(angle, 23), 40 + ypart(angle, 23),
  40 - xpart(angle + ANGLE_4 + ANGLE_8, 8), 40 - ypart(angle + ANGLE_4 + ANGLE_8, 8),
  40, 40,
  40 - xpart(angle - ANGLE_4 - ANGLE_8, 8), 40 - ypart(angle - ANGLE_4 - ANGLE_8, 8),
  TRANS_YELLOW_IN, TRANS_YELLOW_OUT);

 new_rle_struct(temp_bitmap, "blade RLE", &RLE_blade [i], -1);

 } // end for i

  destroy_bitmap(temp_bitmap);


}*/


/*
void prepare_platform_rles(void)
{

 BITMAP *file_bitmap = load_up_bitmap("gfx//platform.bmp");


 fix_outline(file_bitmap);

      platform_RLE [PLATFORM_LR] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_LR * PLATFORM_SIZE, 1, 91, 91);
      platform_RLE [PLATFORM_UD] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UD * PLATFORM_SIZE, 1, 91, 91);

      platform_RLE [PLATFORM_UL] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UL * PLATFORM_SIZE, 1, 91, 91);
      platform_RLE [PLATFORM_DL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UL * PLATFORM_SIZE, 1, 91, 91, 1);
      platform_RLE [PLATFORM_UR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UL * PLATFORM_SIZE, 1, 91, 91, 0);
      platform_RLE [PLATFORM_DR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UL * PLATFORM_SIZE, 1, 91, 91, 2);

      platform_RLE [PLATFORM_UDL] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UDL * PLATFORM_SIZE, 1, 91, 91);
      platform_RLE [PLATFORM_UDR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UDL * PLATFORM_SIZE, 1, 91, 91, 0);

      platform_RLE [PLATFORM_ULR] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_ULR * PLATFORM_SIZE, 1, 91, 91);
      platform_RLE [PLATFORM_DLR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_ULR * PLATFORM_SIZE, 1, 91, 91, 1);

      platform_RLE [PLATFORM_L] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_L * PLATFORM_SIZE, 1, 91, 91);
      platform_RLE [PLATFORM_R] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_L * PLATFORM_SIZE, 1, 91, 91, 0);
      platform_RLE [PLATFORM_U] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_U * PLATFORM_SIZE, 1, 91, 91);
      platform_RLE [PLATFORM_D] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_U * PLATFORM_SIZE, 1, 91, 91, 1);


      platform_RLE [PLATFORM_UR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UL * PLATFORM_SIZE, 1, 91, 91, 0);
      platform_RLE [PLATFORM_DR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_UL * PLATFORM_SIZE, 1, 91, 91, 2);

      platform_RLE [PLATFORM_NODE] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_NODE * PLATFORM_SIZE, 1, 91, 91);
      platform_RLE [PLATFORM_NODE2] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_NODE2 * PLATFORM_SIZE, 1, 91, 91);
      platform_RLE [PLATFORM_NODE3] = extract_rle_sprite(file_bitmap, 1 + FILE_PLATFORM_NODE3 * PLATFORM_SIZE, 1, 91, 91);


      island1_RLE [ISLAND1_D] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_D * PLATFORM_SIZE, 93, 91, 91);
      island1_RLE [ISLAND1_U] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_D * PLATFORM_SIZE, 93, 91, 91, FLIP_V);

      island1_RLE [ISLAND1_R] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_R * PLATFORM_SIZE, 93, 91, 91);
      island1_RLE [ISLAND1_L] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_R * PLATFORM_SIZE, 93, 91, 91, FLIP_H);

      island1_RLE [ISLAND1_DR] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 93, 91, 91);
      island1_RLE [ISLAND1_DL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 93, 91, 91, FLIP_H);
      island1_RLE [ISLAND1_UR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 93, 91, 91, FLIP_V);
      island1_RLE [ISLAND1_UL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 93, 91, 91, FLIP_VH);

      island1_RLE [ISLAND1_NOT_UL] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 93, 91, 91);
      island1_RLE [ISLAND1_NOT_UR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 93, 91, 91, FLIP_H);
      island1_RLE [ISLAND1_NOT_DL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 93, 91, 91, FLIP_V);
      island1_RLE [ISLAND1_NOT_DR] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 93, 91, 91, FLIP_VH);

      island1_RLE [ISLAND1_UL_DR] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_UL_DR * PLATFORM_SIZE, 93, 91, 91);
      island1_RLE [ISLAND1_UR_DL] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_UL_DR * PLATFORM_SIZE, 93, 91, 91, FLIP_H);

      island1_RLE [ISLAND1_FULL] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_FULL * PLATFORM_SIZE, 93, 91, 91);
      island1_RLE [ISLAND1_FULL2] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_FULL2 * PLATFORM_SIZE, 93, 91, 91);
      island1_RLE [ISLAND1_FULL3] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_FULL3 * PLATFORM_SIZE, 93, 91, 91);
      island1_RLE [ISLAND1_FULL4] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_FULL4 * PLATFORM_SIZE, 93, 91, 91);
      island1_RLE [ISLAND1_FULL5] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_FULL5 * PLATFORM_SIZE, 93, 91, 91);


      island1_RLE [ISLAND1_D2] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_D * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_U2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_D * PLATFORM_SIZE, 185, 91, 91, FLIP_V);

      island1_RLE [ISLAND1_R2] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_R * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_L2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_R * PLATFORM_SIZE, 185, 91, 91, FLIP_H);

      island1_RLE [ISLAND1_DR2] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_DL2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_H);
      island1_RLE [ISLAND1_UR2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_V);
      island1_RLE [ISLAND1_UL2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_VH);

      island1_RLE [ISLAND1_NOT_UL2] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_NOT_UR2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_H);
      island1_RLE [ISLAND1_NOT_DL2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_V);
      island1_RLE [ISLAND1_NOT_DR2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_VH);

      island1_RLE [ISLAND1_UL_DR2] = extract_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_UL_DR * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_UR_DL2] = extract_flip_rle_sprite(file_bitmap, 1 + FILE_ISLAND1_UL_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_H);



      island1_RLE [ISLAND1_D3] = extract_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_D * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_U3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_D * PLATFORM_SIZE, 185, 91, 91, FLIP_V);

      island1_RLE [ISLAND1_R3] = extract_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_R * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_L3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_R * PLATFORM_SIZE, 185, 91, 91, FLIP_H);

      island1_RLE [ISLAND1_DR3] = extract_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_DL3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_H);
      island1_RLE [ISLAND1_UR3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_V);
      island1_RLE [ISLAND1_UL3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_VH);

      island1_RLE [ISLAND1_NOT_UL3] = extract_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_NOT_UR3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_H);
      island1_RLE [ISLAND1_NOT_DL3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_V);
      island1_RLE [ISLAND1_NOT_DR3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_NOT_UL * PLATFORM_SIZE, 185, 91, 91, FLIP_VH);

      island1_RLE [ISLAND1_UL_DR3] = extract_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_UL_DR * PLATFORM_SIZE, 185, 91, 91);
      island1_RLE [ISLAND1_UR_DL3] = extract_flip_rle_sprite(file_bitmap, 461 + FILE_ISLAND1_UL_DR * PLATFORM_SIZE, 185, 91, 91, FLIP_H);

      bone_RLE [BONE_HOOK1] = extract_rle_sprite(file_bitmap, 1, 278, 23, 22);
      bone_RLE [BONE_HOOK2] = extract_rle_sprite(file_bitmap, 25, 278, 23, 22);
      bone_RLE [BONE_HOOK3] = extract_rle_sprite(file_bitmap, 49, 278, 23, 22);
      bone_RLE [BONE_HOOK4] = extract_rle_sprite(file_bitmap, 76, 278, 20, 22);
      bone_RLE [BONE_HOOK5] = extract_rle_sprite(file_bitmap, 97, 278, 23, 18);
      bone_RLE [BONE_HOOK2F] = extract_flip_rle_sprite(file_bitmap, 25, 278, 23, 22, FLIP_H);
      bone_RLE [BONE_HOOK3F] = extract_flip_rle_sprite(file_bitmap, 49, 278, 23, 22, FLIP_H);
      bone_RLE [BONE_HOOK4F] = extract_flip_rle_sprite(file_bitmap, 76, 278, 20, 22, FLIP_H);
      bone_RLE [BONE_HOOK5F] = extract_flip_rle_sprite(file_bitmap, 97, 278, 23, 18, FLIP_H);


      bone_RLE [BONE_B_HOOK1] = extract_rle_sprite(file_bitmap, 1, 301, 11, 11);
      bone_RLE [BONE_B_HOOK2] = extract_rle_sprite(file_bitmap, 13, 301, 11, 11);
      bone_RLE [BONE_B_HOOK3] = extract_rle_sprite(file_bitmap, 25, 301, 11, 11);
      bone_RLE [BONE_B_HOOK4] = extract_rle_sprite(file_bitmap, 38, 301, 10, 11);
      bone_RLE [BONE_B_HOOK5] = extract_rle_sprite(file_bitmap, 49, 301, 11, 9);
      bone_RLE [BONE_B_HOOK2F] = extract_flip_rle_sprite(file_bitmap, 13, 301, 11, 11, FLIP_H);
      bone_RLE [BONE_B_HOOK3F] = extract_flip_rle_sprite(file_bitmap, 25, 301, 11, 11, FLIP_H);
      bone_RLE [BONE_B_HOOK4F] = extract_flip_rle_sprite(file_bitmap, 38, 301, 10, 11, FLIP_H);
      bone_RLE [BONE_B_HOOK5F] = extract_flip_rle_sprite(file_bitmap, 49, 301, 11, 9, FLIP_H);

      bone_RLE [BONE_DROP1] = extract_rle_sprite(file_bitmap, 122, 278, 23, 35);
      bone_RLE [BONE_DROP2] = extract_rle_sprite(file_bitmap, 146, 278, 23, 35);
      bone_RLE [BONE_DROP3] = extract_rle_sprite(file_bitmap, 170, 278, 23, 35);
      bone_RLE [BONE_DROP4] = extract_rle_sprite(file_bitmap, 194, 278, 23, 35);
      bone_RLE [BONE_DROP2F] = extract_flip_rle_sprite(file_bitmap, 146, 278, 23, 35, FLIP_H);
      bone_RLE [BONE_DROP3F] = extract_flip_rle_sprite(file_bitmap, 170, 278, 23, 35, FLIP_H);
      bone_RLE [BONE_DROP4F] = extract_flip_rle_sprite(file_bitmap, 194, 278, 23, 35, FLIP_H);

      bone_RLE [BONE_B_DROP1] = extract_rle_sprite(file_bitmap, 61, 301, 11, 16);
      bone_RLE [BONE_B_DROP2] = extract_rle_sprite(file_bitmap, 73, 301, 11, 16);
      bone_RLE [BONE_B_DROP3] = extract_rle_sprite(file_bitmap, 85, 301, 10, 16);
      bone_RLE [BONE_B_DROP4] = extract_rle_sprite(file_bitmap, 96, 304, 10, 13);
      bone_RLE [BONE_B_DROP2F] = extract_flip_rle_sprite(file_bitmap, 73, 301, 11, 16, FLIP_H);
      bone_RLE [BONE_B_DROP3F] = extract_flip_rle_sprite(file_bitmap, 85, 301, 10, 16, FLIP_H);
      bone_RLE [BONE_B_DROP4F] = extract_flip_rle_sprite(file_bitmap, 96, 304, 10, 13, FLIP_H);

      bone_RLE [BONE_RING1] = extract_rle_sprite(file_bitmap, 1, 322, 39, 39);
      bone_RLE [BONE_RING2] = extract_rle_sprite(file_bitmap, 41, 322, 39, 39);
      bone_RLE [BONE_RING3] = extract_rle_sprite(file_bitmap, 81, 322, 39, 39);
      bone_RLE [BONE_RING4] = extract_rle_sprite(file_bitmap, 121, 322, 39, 39);
      bone_RLE [BONE_RING5] = extract_rle_sprite(file_bitmap, 161, 322, 39, 39);
      bone_RLE [BONE_RING2FH] = extract_flip_rle_sprite(file_bitmap, 41, 322, 39, 39, FLIP_H);
      bone_RLE [BONE_RING3FH] = extract_flip_rle_sprite(file_bitmap, 81, 322, 39, 39, FLIP_H);
      bone_RLE [BONE_RING4FH] = extract_flip_rle_sprite(file_bitmap, 121, 322, 39, 39, FLIP_H);
      bone_RLE [BONE_RING5FH] = extract_flip_rle_sprite(file_bitmap, 161, 322, 39, 39, FLIP_H);
      bone_RLE [BONE_RING2FV] = extract_flip_rle_sprite(file_bitmap, 41, 322, 39, 39, FLIP_V);
      bone_RLE [BONE_RING3FV] = extract_flip_rle_sprite(file_bitmap, 81, 322, 39, 39, FLIP_V);
      bone_RLE [BONE_RING4FV] = extract_flip_rle_sprite(file_bitmap, 121, 322, 39, 39, FLIP_V);
      bone_RLE [BONE_RING5FV] = extract_flip_rle_sprite(file_bitmap, 161, 322, 39, 39, FLIP_V);
      bone_RLE [BONE_RING2FVH] = extract_flip_rle_sprite(file_bitmap, 41, 322, 39, 39, FLIP_VH);
      bone_RLE [BONE_RING3FVH] = extract_flip_rle_sprite(file_bitmap, 81, 322, 39, 39, FLIP_VH);
      bone_RLE [BONE_RING4FVH] = extract_flip_rle_sprite(file_bitmap, 121, 322, 39, 39, FLIP_VH);
      bone_RLE [BONE_RING5FVH] = extract_flip_rle_sprite(file_bitmap, 161, 322, 39, 39, FLIP_VH);

      bone_RLE [BONE_B_RING1] = extract_rle_sprite(file_bitmap, 1, 362, 23, 23);
      bone_RLE [BONE_B_RING2] = extract_rle_sprite(file_bitmap, 25, 362, 23, 23);
      bone_RLE [BONE_B_RING3] = extract_rle_sprite(file_bitmap, 49, 362, 23, 23);
      bone_RLE [BONE_B_RING4] = extract_rle_sprite(file_bitmap, 73, 362, 23, 23);
      bone_RLE [BONE_B_RING5] = extract_rle_sprite(file_bitmap, 97, 362, 23, 23);
      bone_RLE [BONE_B_RING2FH] = extract_flip_rle_sprite(file_bitmap, 25, 362, 23, 23, FLIP_H);
      bone_RLE [BONE_B_RING3FH] = extract_flip_rle_sprite(file_bitmap, 49, 362, 23, 23, FLIP_H);
      bone_RLE [BONE_B_RING4FH] = extract_flip_rle_sprite(file_bitmap, 73, 362, 23, 23, FLIP_H);
      bone_RLE [BONE_B_RING5FH] = extract_flip_rle_sprite(file_bitmap, 97, 362, 23, 23, FLIP_H);
      bone_RLE [BONE_B_RING2FV] = extract_flip_rle_sprite(file_bitmap, 25, 362, 23, 23, FLIP_V);
      bone_RLE [BONE_B_RING3FV] = extract_flip_rle_sprite(file_bitmap, 49, 362, 23, 23, FLIP_V);
      bone_RLE [BONE_B_RING4FV] = extract_flip_rle_sprite(file_bitmap, 73, 362, 23, 23, FLIP_V);
      bone_RLE [BONE_B_RING5FV] = extract_flip_rle_sprite(file_bitmap, 97, 362, 23, 23, FLIP_V);
      bone_RLE [BONE_B_RING2FVH] = extract_flip_rle_sprite(file_bitmap, 25, 362, 23, 23, FLIP_VH);
      bone_RLE [BONE_B_RING3FVH] = extract_flip_rle_sprite(file_bitmap, 49, 362, 23, 23, FLIP_VH);
      bone_RLE [BONE_B_RING4FVH] = extract_flip_rle_sprite(file_bitmap, 73, 362, 23, 23, FLIP_VH);
      bone_RLE [BONE_B_RING5FVH] = extract_flip_rle_sprite(file_bitmap, 97, 362, 23, 23, FLIP_VH);

      bone_RLE [BONE_HUNTER1] = extract_rle_sprite(file_bitmap, 201, 322, 45, 40);
      bone_RLE [BONE_HUNTER2] = extract_rle_sprite(file_bitmap, 247, 322, 45, 40);
      bone_RLE [BONE_HUNTER3] = extract_rle_sprite(file_bitmap, 293, 322, 45, 40);
      bone_RLE [BONE_HUNTER4] = extract_rle_sprite(file_bitmap, 339, 322, 41, 40);
      bone_RLE [BONE_HUNTER5] = extract_rle_sprite(file_bitmap, 381, 322, 45, 40);
      bone_RLE [BONE_HUNTER2F] = extract_flip_rle_sprite(file_bitmap, 247, 322, 45, 40, FLIP_H);
      bone_RLE [BONE_HUNTER3F] = extract_flip_rle_sprite(file_bitmap, 293, 322, 45, 40, FLIP_H);
      bone_RLE [BONE_HUNTER4F] = extract_flip_rle_sprite(file_bitmap, 339, 322, 41, 40, FLIP_H);
      bone_RLE [BONE_HUNTER5F] = extract_flip_rle_sprite(file_bitmap, 381, 322, 45, 40, FLIP_H);

      bone_RLE [BONE_B_HUNTER1] = extract_rle_sprite(file_bitmap, 199, 364, 27, 23);
      bone_RLE [BONE_B_HUNTER2] = extract_rle_sprite(file_bitmap, 227, 364, 27, 23);
      bone_RLE [BONE_B_HUNTER3] = extract_rle_sprite(file_bitmap, 255, 364, 27, 23);
      bone_RLE [BONE_B_HUNTER4] = extract_rle_sprite(file_bitmap, 285, 364, 25, 23);
      bone_RLE [BONE_B_HUNTER5] = extract_rle_sprite(file_bitmap, 311, 364, 27, 23);
      bone_RLE [BONE_B_HUNTER2F] = extract_flip_rle_sprite(file_bitmap, 227, 364, 27, 23, FLIP_H);
      bone_RLE [BONE_B_HUNTER3F] = extract_flip_rle_sprite(file_bitmap, 255, 364, 27, 23, FLIP_H);
      bone_RLE [BONE_B_HUNTER4F] = extract_flip_rle_sprite(file_bitmap, 285, 364, 25, 23, FLIP_H);
      bone_RLE [BONE_B_HUNTER5F] = extract_flip_rle_sprite(file_bitmap, 311, 364, 27, 23, FLIP_H);

      bone_RLE [BONE_B_SPINNER] = extract_rle_sprite(file_bitmap, 427, 288, 65, 107);

      bone_RLE [BONE_B_WALKER1] = extract_rle_sprite(file_bitmap, 493, 285, 89, 75);
      bone_RLE [BONE_B_WALKER2] = extract_rle_sprite(file_bitmap, 583, 285, 89, 75);
      bone_RLE [BONE_B_WALKER2F] = extract_flip_rle_sprite(file_bitmap, 583, 285, 89, 75, FLIP_H);

 destroy_bitmap(file_bitmap);

}


*/



/*
void prepare_mshockwaves(void)
{
 int i, j;

 BITMAP *tmp = new_bitmap(90, 90, "prepare_mshockwaves tmp bitmap");

 int rad1, rad2, c1, c2;
 i = 0;
 for (i = 0; i < 4; i ++)
 {

  switch(i)
  {
   case 0:
    c1 = TRANS_ORANGE_OUT;
    c2 = TRANS_ORANGE_IN;
    break;
   case 1:
    c1 = TRANS_YELLOW_OUT;
    c2 = TRANS_YELLOW_IN;
    break;
   case 2:
    c1 = TRANS_SH1_OUT;
    c2 = TRANS_SH1_IN;
    break;
   case 3:
    c1 = TRANS_SH2_OUT;
    c2 = TRANS_SH2_IN;
    break;
}
  rad1 = 30;
  rad2 = 1;
  for (j = 0; j < 20; j ++)
  {
    clear_bitmap(tmp);

    rad2 += 2;
    if (j % 2 == 0)
     rad1 ++;

    circlefill(tmp, 45, 45, rad1, c2);
    circle(tmp, 45, 45, rad1, c1);
    circlefill(tmp, 45, 45, rad2, 0);
    circle(tmp, 45, 45, rad2, c1);

    new_rle_struct(tmp, "mshockwaves rle", &RLE_mshock [i] [j], -1);
    extract_rle_struct2(tmp, &RLE_mshock [i] [j], 1, 1, 1, 1, -1);

  }
 }


 destroy_bitmap(tmp);
}
*/




/*
Safe wrapper for the Allegro create_bitmap function.
*/
BITMAP *new_bitmap(int x, int y, const char errtxt [])
{

 BITMAP *bmp = create_bitmap(x, y);
 if (bmp == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Bitmap creation failed!  \n%s\n\n\r%s", allegro_error, errtxt);
  exit(1);
 }

 return bmp;

}




void bordered_poly4(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int col, int border_col)
{

  BITMAP *temp_bitmap = new_bitmap(target->w, target->h, "Bordered poly4 temp_bitmap");

  clear_bitmap(temp_bitmap);

  int points [8];
  points [0] = x1;
  points [1] = y1;
  points [2] = x2;
  points [3] = y2;
  points [4] = x3;
  points [5] = y3;
  points [6] = x4;
  points [7] = y4;

  polygon(temp_bitmap, 4, points, col);

  coat_sprite(temp_bitmap, border_col);

  draw_sprite(target, temp_bitmap, 0, 0);

  destroy_bitmap(temp_bitmap);

}

void bordered_poly6(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, int x6, int y6, int col, int border_col)
{

  BITMAP *temp_bitmap = new_bitmap(target->w, target->h, "Bordered poly6 temp_bitmap");

  clear_bitmap(temp_bitmap);

  int points [12];
  points [0] = x1;
  points [1] = y1;
  points [2] = x2;
  points [3] = y2;
  points [4] = x3;
  points [5] = y3;
  points [6] = x4;
  points [7] = y4;
  points [8] = x5;
  points [9] = y5;
  points [10] = x6;
  points [11] = y6;

  polygon(temp_bitmap, 6, points, col);

  coat_sprite(temp_bitmap, border_col);

  draw_sprite(target, temp_bitmap, 0, 0);

  destroy_bitmap(temp_bitmap);

}


void bordered_triangle(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int col, int border_col)
{

  BITMAP *temp_bitmap = new_bitmap(target->w, target->h, "Bordered triangle temp_bitmap");

  clear_bitmap(temp_bitmap);

  triangle(temp_bitmap, x1, y1, x2, y2, x3, y3, col);

  coat_sprite(temp_bitmap, border_col);

  draw_sprite(target, temp_bitmap, 0, 0);

  destroy_bitmap(temp_bitmap);

}




void coat_sprite(BITMAP *target, int col)
{
 int i, j;

 for (i = 1; i < target->w - 1; i ++)
 {
  for (j = 1; j < target->h - 1; j ++)
  {
   if (_getpixel(target, i, j) == 0)
   {
    if ((_getpixel(target, i - 1, j) != 0 && _getpixel(target, i - 1, j) != col)
       || (_getpixel(target, i + 1, j) != 0 && _getpixel(target, i + 1, j) != col)
       || (_getpixel(target, i, j - 1) != 0 && _getpixel(target, i, j - 1) != col)
       || (_getpixel(target, i, j + 1) != 0 && _getpixel(target, i, j + 1) != col))
         putpixel(target, i, j, col);
   }
  }
 }

}

void coat_file_sprite_xy(BITMAP *target, int col, int x1, int y1, int x2, int y2)
{
 int i, j;

 for (i = x1; i < x2; i ++)
 {
  for (j = y1; j < y2; j ++)
  {
   if (getpixel(target, i, j) == 0)
   {
    if ((getpixel(target, i - 1, j) != 0 && getpixel(target, i - 1, j) != col)
       || (getpixel(target, i + 1, j) != 0 && getpixel(target, i + 1, j) != col)
       || (getpixel(target, i, j - 1) != 0 && getpixel(target, i, j - 1) != col)
       || (getpixel(target, i, j + 1) != 0 && getpixel(target, i, j + 1) != col))
         putpixel(target, i, j, col);
   }
  }
 }

}



void fast_bordered_poly4(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int col, int border_col, int fillx, int filly)
{

  BITMAP *temp_bitmap = new_bitmap(target->w, target->h, "Bordered poly4 temp_bitmap");

  clear_bitmap(temp_bitmap);

  line(temp_bitmap, x1, y1, x2, y2, border_col);
  line(temp_bitmap, x2, y2, x3, y3, border_col);
  line(temp_bitmap, x3, y3, x4, y4, border_col);
  line(temp_bitmap, x4, y4, x1, y1, border_col);

  floodfill(temp_bitmap, fillx, filly, col);

  draw_sprite(target, temp_bitmap, 0, 0);

  destroy_bitmap(temp_bitmap);

}

void fast_bordered_triangle(BITMAP *target, int x1, int y1, int x2, int y2, int x3, int y3, int col, int border_col, int fillx, int filly)
{

  BITMAP *temp_bitmap = new_bitmap(target->w, target->h, "Bordered triangle temp_bitmap");

  clear_bitmap(temp_bitmap);

  line(temp_bitmap, x1, y1, x2, y2, border_col);
  line(temp_bitmap, x2, y2, x3, y3, border_col);
  line(temp_bitmap, x3, y3, x1, y1, border_col);

  floodfill(temp_bitmap, fillx, filly, col);

  draw_sprite(target, temp_bitmap, 0, 0);

  destroy_bitmap(temp_bitmap);

}



BITMAP *load_up_bitmap(const char fname [])
{
 RGB temp_palette [256];

 BITMAP *temp_bitmap = load_bitmap(fname, temp_palette);
 if (temp_bitmap == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Failed to load in bitmap! (File not found?)  \n%s", fname);
  exit(1);
 }

 return temp_bitmap;

}



RLE_SPRITE *extract_rle_sprite(BITMAP *source, int x_source, int y_source, int x, int y)
{

 BITMAP *tmp = new_bitmap(x, y, "extract_rle_sprite");

 blit(source, tmp, x_source, y_source, 0, 0, x, y);

 RLE_SPRITE *retval = get_rle_sprite(tmp);

 if (retval == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Rle_sprite extraction failed!  \n%s\n\n\r%s", allegro_error, "extract_rle_sprite");
  exit(1);
 }

 destroy_bitmap(tmp);

 return retval;

}

RLE_SPRITE *extract_flip_rle_sprite(BITMAP *source, int x_source, int y_source, int x, int y, int flip_type)
{

 BITMAP *tmp = new_bitmap(x, y, "extract_hflip_rle_sprite");
 BITMAP *tmp2 = new_bitmap(x, y, "extract_hflip_rle_sprite");

 clear_bitmap(tmp2);
 blit(source, tmp, x_source, y_source, 0, 0, x, y);

 switch(flip_type)
 {
  case FLIP_H: draw_sprite_h_flip(tmp2, tmp, 0, 0); break;
  case FLIP_V: draw_sprite_v_flip(tmp2, tmp, 0, 0); break;
  case FLIP_VH: draw_sprite_vh_flip(tmp2, tmp, 0, 0); break;
 }


 RLE_SPRITE *retval = get_rle_sprite(tmp2);

 if (retval == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Rle_sprite extraction failed!  \n%s\n\n\r%s", allegro_error, "extract_rle_sprite");
  exit(1);
 }

 destroy_bitmap(tmp);
 destroy_bitmap(tmp2);

 return retval;

}



RLE_SPRITE *sized_rle_sprite(BITMAP *source, int x, int y, const char errtxt [])
{
 BITMAP *tmp = new_bitmap(x, y, errtxt);

 blit(source, tmp, 0, 0, 0, 0, x, y);

 RLE_SPRITE *retval = get_rle_sprite(tmp);

 if (retval == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Rle_sprite sized creation failed!  \n%s\n\n\r%s", allegro_error, errtxt);
  exit(1);
 }

 destroy_bitmap(tmp);

 return retval;

}



RLE_SPRITE *new_rle_sprite(BITMAP *source, const char errtxt [])
{

 RLE_SPRITE *retval = get_rle_sprite(source);
 if (retval == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Rle_sprite creation failed!  \n%s\n\n\r%s", allegro_error, errtxt);
  exit(1);
 }

 return retval;

}

void new_rle_struct_colours(BITMAP *source, const char errtxt [], struct RLE_STRUCT str [SMALL_ROTATIONS] [9], int bl, int index2, int col1, int col2)
{

 int i;
 int count = 0;
 int trans_col = 0;

 for (i = col1; i < col2 + 1; i ++)
 {
      switch(i)
      {
/*       case 0: trans_col = TRANS_CONVERT_1; break;
       case 1: trans_col = TRANS_CONVERT_2; break;
       case 2: trans_col = TRANS_CONVERT_3; break;
       case 3: trans_col = TRANS_CONVERT_4; break;
       case 4: trans_col = TRANS_CONVERT_5; break;
       case 5: trans_col = TRANS_CONVERT_6; break;
       case 6: trans_col = TRANS_CONVERT_7; break;
       case 7: trans_col = TRANS_CONVERT_8; break;
       case 8: trans_col = TRANS_CONVERT_9; break;*/
      }
      drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
      rectfill(source, 0, 0, source->w, source->h, trans_col);
      drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
      new_rle_struct(source, "new_rle_struct_colours", str [index2], i - col1);
      count ++;
 }

}

void extract_rle_struct2(BITMAP *source, struct RLE_STRUCT *str, int x_source, int y_source, int x, int y, int bl)
{

 BITMAP *tmp = new_bitmap(x, y, "extract_rle_struct");

 blit(source, tmp, x_source, y_source, 0, 0, x, y);

 if (bl == -1)
 {
   str->sprite = get_rle_sprite(tmp);
   str->x = tmp->w / 2;
   str->y = tmp->h / 2;
 }
  else
  {
     str [bl].sprite = get_rle_sprite(tmp);
     str [bl].x = tmp->w / 2;
     str [bl].y = tmp->h / 2;
  }


 if (str->sprite == NULL)
 {
  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
  allegro_message("Rle_struct extraction failed!  \n%s\n\n\r%s", allegro_error, "extract_rle_struct");
  exit(1);
 }

  destroy_bitmap(tmp);

}



void new_rle_struct(BITMAP *source, const char errtxt [], struct RLE_STRUCT *str, int bl)
{


 int x1 = -1, y1 = -1, x2 = -1, y2 = -1;

 int i, j;

 for (i = 0; i < source->w; i ++)
 {
  for (j = 0; j < source->h; j ++)
  {
    if (getpixel(source, i, j) != 0)
    {
     x1 = i;
     break;
    }
  }
  if (x1 != -1)
   break;
 }

  for (j = 0; j < source->h; j ++)
  {
   for (i = 0; i < source->w; i ++)
   {
    if (getpixel(source, i, j) != 0)
    {
     y1 = j;
     break;
    }
  }
  if (y1 != -1)
   break;
 }

 for (i = source->w - 1; i >= 0; i --)
 {
  for (j = 0; j < source->h; j ++)
  {
    if (getpixel(source, i, j) != 0)
    {
     x2 = i;
     break;
    }
  }
  if (x2 != -1)
   break;
 }


  for (j = source->h - 1; j >= 0; j --)
  {
   for (i = 0; i < source->w; i ++)
   {
    if (getpixel(source, i, j) != 0)
    {
     y2 = j;
     break;
    }
  }
  if (y2 != -1)
   break;
 }



 BITMAP *tmp = new_bitmap(x2 - x1 + 1, y2 - y1 + 1, "new_rle_struct temp bitmap");

 blit(source, tmp, x1, y1, 0, 0, x2 - x1 + 1, y2 - y1 + 1);

//rect(tmp, 0, 0, tmp->w - 1, tmp->h - 1, TRANS_RED_OUT);
 if (bl == -1)
 {
   str->sprite = get_rle_sprite(tmp);
   str->x = tmp->w / 2;
   str->y = tmp->h / 2;
 }
  else
  {
   if (bl == -2)
   {
     str->sprite = get_rle_sprite(tmp);
     str->x = source->w / 2 - x1;
     str->y = source->h / 2 - y1;
   }
    else
    {
     str [bl].sprite = get_rle_sprite(tmp);
     str [bl].x = tmp->w / 2; //source->w / 2 - x1;
     str [bl].y = tmp->h / 2; //source->h / 2 - y1;
    }
  }

 destroy_bitmap(tmp);
}



void fix_outline(BITMAP *source)
{
 int i, j, k, px;

 int outer [12]; //= getpixel(source, 0, 0);

 for (i = 0; i < 12; i ++)
 {
  outer [i] = getpixel(source, i, 0);
 }

 for (i = 0; i < source->w; i ++)
 {
  for (j = 0; j < source->h; j ++)
  {
   px = getpixel(source, i, j);
   for (k = 0; k < 12; k ++)
   {
    if (px == outer [k])
    {
     putpixel(source, i, j, k + 1);
     break;
    }
   }
/*   switch(px)
   {
       case 116:
        putpixel(source, i, j, COL_OUTLINE);
        break;
       case 61:
        putpixel(source, i, j, COL_COL4);
        break;
   }*/
  }
 }

}

void fix_trans(BITMAP *source)
{
 int i, j, px;

 for (i = 0; i < source->w; i ++)
 {
  for (j = 0; j < source->h; j ++)
  {
   px = getpixel(source, i, j);
   switch(px)
   {
       case 51://COL_WHITE:
//        putpixel(source, i, j, TRANS_ORANGE_OUT);
        break;
       case 30://COL_LGREY:
//        putpixel(source, i, j, TRANS_ORANGE_IN);
        break;
   }
  }
 }

}



void extract_rle_struct(BITMAP *source, int x1, int y1, int x2, int y2, const char errtxt [], struct RLE_STRUCT *str, int bl)
{

 BITMAP *tmp = new_bitmap(x2, y2, "extract_rle_struct temp bitmap");

 blit(source, tmp, x1, y1, 0, 0, x2, y2);

 new_rle_struct(tmp, errtxt, str, bl);

 destroy_bitmap(tmp);

}
