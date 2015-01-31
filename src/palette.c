/*

This function is straight out of Excellent Bifurcation.
I haven't fixed it up for this example at all.
Use the colour enum in palette.h.

*/


#include "config.h"

#include "allegro.h"


#include "palette.h"
#include "globvars.h"

COLOR_MAP trans_table;
//COLOR_MAP trans_table2;

//int blend_function(int base, int trans, RGB *rgbl);
int blend_function(int trans, int base, RGB *rgbl);
int blend_function2(int trans, int base, RGB *rgbl);
int limit_colour(int colour_input);
void set_a_colour(int col, int r, int g, int b);

void create_inner_bullet_colour(int pa, int index, int r, int g, int b);
void create_outer_bullet_colour(int pa, int index, int r, int g, int b);

void build_new_palette(int a, int b, int c, int d);

void halfway_colour(int base, int target, int r, int g, int b);


RGB other_palet [3048];
RGB palet [256];
RGB palet2 [256];
//RGB light_palet [256];
//RGB dark_palet [256];
RGB other_palet2 [3048];
// what seems to be a weird bug in Allegro forces me to put padding around
//  the palet array, or it becomes corrupted. The same thing happened in
//  World of Violence and I have no idea why.

/*


IMPORTANT TO REMEMBER:

    I created the palette for the GIMP by saving a 16x16 image with all colours
    as a gif then importing the palette from that. This let me get the colours
    in the right order!


 In the GIMP, must go to image|mode|indexed colour and change the settings there.






*/


void special_create_color_table(COLOR_MAP *table);//, AL_CONST PALETTE pal);
void special_create_color_table2(COLOR_MAP *table, AL_CONST PALETTE pal);
int base_colour(int y);

void colour_table(const char *which_call);


/*
Colour scheme:

Remember, 0 is black so base 0 is actually indexed as 1

outline 0
white 1
lgrey 2
dgrey 3
col1 4
col2 5
col3 6
col4 7
back1 9
back2 9
back3 10
back4 11
back5 12


*/


int base_colours [NO_PALETTES] [13] [3] =
{
// menu
 {
  {5, 5, 5}, // outline
  {50, 50, 50}, // white
  {35, 35, 35}, // lgrey
  {22, 22, 22}, // dgrey

  {13, 13, 20}, // col1
  {15, 15, 27}, // col2
  {17, 17, 32}, // col3
  {20, 20, 37}, // col4

  {13, 13, 20}, // back1
  {20, 20, 27}, // back2
  {22, 22, 32}, // back3
  {24, 24, 37}, // back4
  {26, 26, 41}, // back5

/*  {5, 5, 5}, // outline
  {50, 50, 50}, // white
  {35, 35, 35}, // lgrey
  {22, 22, 22}, // dgrey

  {33, 33, 33}, // col1
  {38, 38, 38}, // col2
  {43, 43, 43}, // col3
  {48, 48, 48}, // col4

  {12, 12, 12}, // back1
  {23, 23, 23}, // back2
  {26, 26, 26}, // back3
  {29, 29, 29}, // back4
  {32, 32, 32}, // back5
*/
 },
// base (stage 1)
 {
  {5, 5, 5}, // outline
  {50, 50, 50}, // white
  {35, 35, 35}, // lgrey
  {22, 22, 22}, // dgrey

  {22, 5, 5}, // col1
  {33, 8, 8}, // col2
  {40, 12, 12}, // col3
  {50, 17, 17}, // col4

  {11, 11, 16}, // back1
  {14, 14, 19}, // back2
  {17, 17, 22}, // back3
  {20, 20, 25}, // back4
  {23, 23, 28}, // back5
 },
// stage 1_2
 {
  {5, 5, 5}, // outline
  {50, 50, 50}, // white
  {35, 35, 35}, // lgrey
  {22, 22, 22}, // dgrey

  {20, 8, 5}, // col1
  {30, 11, 8}, // col2
  {37, 16, 12}, // col3
  {42, 23, 17}, // col4

  {9, 13, 13}, // back1
  {12, 16, 16}, // back2
  {15, 19, 19}, // back3
  {18, 22, 22}, // back4
  {21, 25, 25} // back5

/*  {20, 10, 10}, // back1
  {23, 13, 13}, // back2
  {26, 16, 16}, // back3
  {29, 19, 19}, // back4
  {32, 22, 22}, // back5*/
 },
// stage 1_3
 {
  {5, 5, 5}, // outline
  {50, 50, 50}, // white
  {35, 35, 35}, // lgrey
  {22, 22, 22}, // dgrey

  {15, 12, 5}, // col1
  {25, 16, 8}, // col2
  {31, 23, 16}, // col3
  {39, 31, 23}, // col4

  {9, 12, 12}, // back1
  {13, 15, 15}, // back2
  {15, 19, 19}, // back3
  {18, 20, 20}, // back4
  {21, 23, 23} // back5
 },

// stage 2_1
 {
  {5, 5, 5}, // outline
  {50, 50, 50}, // white
  {35, 35, 35}, // lgrey
  {22, 22, 22}, // dgrey

  {10, 10, 15}, // col1
  {13, 13, 25}, // col2
  {23, 23, 36}, // col3
  {33, 33, 42}, // col4

  {15, 10, 10}, // back1
  {18, 13, 13}, // back2
  {21, 16, 16}, // back3
  {24, 19, 19}, // back4
  {29, 22, 22} // back5
 },
// stage 2_2
 {
  {5, 5, 5}, // outline
  {50, 50, 50}, // white
  {35, 35, 35}, // lgrey
  {22, 22, 22}, // dgrey

  {20, 5, 5}, // col1
  {30, 8, 8}, // col2
  {36, 12, 12}, // col3
  {45, 17, 17}, // col4

  {10, 10, 15}, // back1
  {13, 13, 18}, // back2
  {16, 16, 21}, // back3
  {19, 19, 24}, // back4
  {22, 22, 27}, // back5
 },

// stage 3_1
 {
  {5, 5, 5}, // outline
  {50, 50, 50}, // white
  {35, 35, 35}, // lgrey
  {22, 22, 22}, // dgrey

  {20, 20, 27}, // col1
  {24, 24, 34}, // col2
  {31, 31, 42}, // col3
  {39, 39, 49}, // col4

  {15, 15, 15}, // back1
  {25, 22, 22}, // back2
  {29, 25, 25}, // back3
  {32, 27, 27}, // back4
  {36, 29, 29}, // back5

 },

// stage 3_2
 {
  {5, 5, 5}, // outline
  {50, 50, 50}, // white
  {35, 35, 35}, // lgrey
  {22, 22, 22}, // dgrey

  {27, 20, 20}, // col1
  {34, 23, 23}, // col2
  {42, 28, 28}, // col3
  {49, 35, 35}, // col4

  {15, 15, 15}, // back1
  {22, 22, 25}, // back2
  {25, 25, 29}, // back3
  {27, 27, 32}, // back4
  {29, 29, 36}, // back5

 },

};

int current_base [13] [3];

//int swap;

void set_base_palette(void)
{

 int i, j;

 for (i = 0; i < 13; i ++)
 {
   for (j = 0; j < 3; j ++)
   {
    current_base [i] [j] = base_colours [PAL_MENU] [i] [j];
   }
 }

 build_new_palette(0, 0, 0, 0);

}


void run_palette_shift(void)
{

 int i, j;
 char changed = 0;

 for (i = 0; i < 13; i ++)
 {
   for (j = 0; j < 3; j ++)
   {
    if (current_base [i] [j] < base_colours [arena.target_palette] [i] [j])
    {
     current_base [i] [j] ++;
     changed = 1;
    }
    if (current_base [i] [j] > base_colours [arena.target_palette] [i] [j])
    {
     current_base [i] [j] --;
     changed = 1;
    }
   }
 }

 if (changed == 0)
  arena.shift_palette = 1;

//  circlefill(screen, 10, 10, 5, grand(100));

}


// only called at startup.
void define_palette(void)
{

int i;

 palet [0].r = 0;
 palet [0].g = 0;
 palet [0].b = 0;

 for (i = 0; i < 13; i ++)
 {
    current_base [i] [0] = base_colours [PAL_MENU] [i] [0];
    current_base [i] [1] = base_colours [PAL_MENU] [i] [1];
    current_base [i] [2] = base_colours [PAL_MENU] [i] [2];
 }


 build_new_palette(0, 0, 0, 0);

   vsync();
   set_palette(palet);

//#define CREATE_PALETTE

#ifdef CREATE_PALETTE

 int j;

 BITMAP *palt = new_bitmap(16, 16, "Palette");

 for (i = 0; i < 16; i ++)
 {
  for (j = 0; j < 16; j ++)
  {
   putpixel(palt, i, j, i + j * 16);
  }
 }

 save_bitmap("palt.bmp", palt, palet);

#endif

   special_create_color_table(&trans_table);//, palet);

   color_map = &trans_table;

//   set_palette(palet2);

}


void build_new_palette(int a, int b, int c, int d)
{

    int i;

    set_a_colour(0, 0, 0, 0);

    for (i = 1; i < 14; i ++)
    {
      set_a_colour(i, current_base [i - 1] [0], current_base [i - 1] [1], current_base [i - 1] [2]);
    }


    for (i = 0; i < 13; i ++)
    {
      set_a_colour(TRANS_RED1 + i, current_base [i] [0] + 9, current_base [i] [1], current_base [i] [2] - 3);
      set_a_colour(TRANS_RED2 + i, current_base [i] [0] + 18, current_base [i] [1] + 1, current_base [i] [2] - 7);
      set_a_colour(TRANS_RED3 + i, current_base [i] [0] + 31, current_base [i] [1] + 5, current_base [i] [2] / 2);
      set_a_colour(TRANS_RED4 + i, current_base [i] [0] / 3 + 42, current_base [i] [1] / 3 + 25, current_base [i] [2] / 4);
      set_a_colour(TRANS_RED5 + i, current_base [i] [0] / 4 + 45, current_base [i] [1] / 4 + 32, current_base [i] [2] / 5);

      set_a_colour(TRANS_GREEN1 + i, current_base [i] [0], current_base [i] [1] + 3, current_base [i] [2] - 3);
      set_a_colour(TRANS_GREEN2 + i, current_base [i] [0] + 1, current_base [i] [1] + 10, current_base [i] [2] - 5);
      set_a_colour(TRANS_GREEN3 + i, current_base [i] [0] + 6, current_base [i] [1] + 20, current_base [i] [2] / 2);
//      set_a_colour(TRANS_GREEN3 + i, current_base [i] [0] + 3, current_base [i] [1] + 20, current_base [i] [2] / 2);
      set_a_colour(TRANS_GREEN4 + i, current_base [i] [0] / 3 + 30, current_base [i] [1] / 3 + 30, current_base [i] [2] / 4 + 10);
      set_a_colour(TRANS_GREEN5 + i, current_base [i] [0] / 4 + 45, current_base [i] [1] / 4 + 39, current_base [i] [2] / 5 + 15);
//      set_a_colour(TRANS_GREEN4 + i, current_base [i] [0] / 2 + 23, current_base [i] [1] / 2 + 30, current_base [i] [2] / 4 + 5);
//      set_a_colour(TRANS_GREEN5 + i, current_base [i] [0] / 3 + 37, current_base [i] [1] / 3 + 39, current_base [i] [2] / 5 + 12);

      set_a_colour(TRANS_BLUE1 + i, current_base [i] [0] - 1, current_base [i] [1] - 1, current_base [i] [2] + 7);
      set_a_colour(TRANS_BLUE2 + i, current_base [i] [0] + 1, current_base [i] [1] + 1, current_base [i] [2] + 15);
      set_a_colour(TRANS_BLUE3 + i, current_base [i] [0] / 2 + 18, current_base [i] [1] / 2 + 18, current_base [i] [2] / 2 + 30);
      set_a_colour(TRANS_BLUE4 + i, current_base [i] [0] / 4 + 36, current_base [i] [1] / 4 + 36, current_base [i] [2] / 4 + 50);
      set_a_colour(TRANS_BLUE5 + i, current_base [i] [0] / 5 + 51, current_base [i] [1] / 5 + 51, current_base [i] [2] / 5 + 65);

      set_a_colour(TRANS_PURPLE2 + i, current_base [i] [0] + 15, current_base [i] [1] + 1, current_base [i] [2] + 15);
      set_a_colour(TRANS_PURPLE3 + i, current_base [i] [0] / 2 + 25, current_base [i] [1] / 2 + 12, current_base [i] [2] / 2 + 25);
      set_a_colour(TRANS_PURPLE4 + i, current_base [i] [0] / 4 + 42, current_base [i] [1] / 4 + 30, current_base [i] [2] / 4 + 42);

/*      set_a_colour(TRANS_BLUE1 + i, base_colours [i] [0], base_colours [i] [1], base_colours [i] [2] + 5);
      set_a_colour(TRANS_BLUE2 + i, base_colours [i] [0], base_colours [i] [1], base_colours [i] [2] + 10);
      set_a_colour(TRANS_BLUE3 + i, base_colours [i] [0], base_colours [i] [1], base_colours [i] [2] + 15);
      set_a_colour(TRANS_BLUE4 + i, base_colours [i] [0], base_colours [i] [1], base_colours [i] [2] + 20);
      set_a_colour(TRANS_BLUE5 + i, base_colours [i] [0], base_colours [i] [1], base_colours [i] [2] + 25);*/
    }

}


void set_a_colour(int col, int r, int g, int b)
{

 palet [col].r = limit_colour(r);
 palet [col].g = limit_colour(g);
 palet [col].b = limit_colour(b);

}

int limit_colour(int colour_input)
{

 if (colour_input < 0) return 0;
 if (colour_input > 63) return 63;
 return colour_input;

}

int blend_function(int trans, int base, RGB *rgbl)
{

/*
 if (trans >= TRANS_CONVERT_1 && trans <= TRANS_CONVERT_9)
 {
  if (base == 0)
   return 0;

  if (((base - 1) / 13) % 2 == 1)
   return (trans - TRANS_CONVERT_1) * 26 + 15;
    else
     return (trans - TRANS_CONVERT_1) * 26 + 28;

 }

 if (trans == CONVERT_WHITE_TO_GREY)
 {
  if (base == COL_WHITE)
   return COL_LGREY;
  return base;
 }

 if (trans == TRANS_BACK2_OUTLINE)
 {
      return base;
 }

 if (trans == TRANS_BACK3_OUTLINE)
 {
  if (base == COL_BACK1)//2 || base == COL_BACK1)
    return COL_OUTLINE;
     else
      return base;
 }

*/

 if (trans == CONVERT_WHITE_TO_GREY)
 {
  if (base == COL_WHITE)
   return COL_LGREY;
  return base;
 }

 if (trans >= COL_BCIRCLE_1 && trans < COL_BCIRCLE_OUT_1)
 {
    return trans - COL_BCIRCLE_1 + COL_BACK2;
 }

 if (trans == COL_BCIRCLE_OUT_1)
 {
    if (base == COL_BACK2)
     return COL_BACK2;
    return COL_OUTLINE;
 }
 if (trans == COL_BCIRCLE_OUT_2)
 {
    if (base == COL_BACK3)
     return base;
    if (base == COL_BACK2)
     return COL_BACK1;
    return COL_OUTLINE;
 }
 if (trans == COL_BCIRCLE_OUT_3)
 {
    if (base == COL_BACK4)
     return base;
    if (base == COL_BACK3)
     return COL_BACK2;
    if (base == COL_BACK2)
     return COL_BACK1;
    return COL_OUTLINE;
 }


/*
 if (trans >= COL_BCIRCLE_1 && trans < COL_BCIRCLE_OUT_1)
 {
    return trans - COL_BCIRCLE_1 + COL_BACK2;
 }

 if (trans == COL_BCIRCLE_OUT_1)
 {
    if (base == COL_BACK2)
     return COL_BACK2;
    return COL_OUTLINE;
 }
 if (trans == COL_BCIRCLE_OUT_2)
 {
    if (base == COL_BACK3)
     return base;
    return COL_OUTLINE;
 }
 if (trans == COL_BCIRCLE_OUT_3)
 {
    if (base == COL_BACK4)
     return base;
    return COL_OUTLINE;
 }

*/
 if (base < TRANS_RED1)
  return trans + base - 1;

 int trans_strength = (trans - 1) / 13;

 if (trans == TRANS_PURPLE2)
  trans_strength = 4;
 if (trans == TRANS_PURPLE3)
  trans_strength = 7;
 if (trans == TRANS_PURPLE4)
  trans_strength = 10;

 int base_trans_strength = (base - 1) / 13;

 if (base >= TRANS_PURPLE2 && base < TRANS_PURPLE3)
  base_trans_strength = 4;
 if (base >= TRANS_PURPLE3 && base < TRANS_PURPLE4)
  base_trans_strength = 7;
 if (base >= TRANS_PURPLE4)
  base_trans_strength = 10;


 if (trans_strength > base_trans_strength)
  return trans + ((base - 1) % 13);
   else return base;

 return trans;

}



/*int blend_function(int trans, int base, RGB *rgbl)
{

 if (base <= COLOUR_12)
  return trans + base;

 trans --;

// int low_colour = base % 12;
// int low_trans = trans / 12;
 int trans_strength = (trans / 12 - 1) % 6;

 if (trans == TRANS_PURPLE1)
  trans_strength = 10;
 if (trans == TRANS_PURPLE2)
  trans_strength = 11;

 int base_trans_strength = (base / 12 - 1) % 6;
 if (trans == TRANS_PURPLE1)
  base_trans_strength = 10;
 if (trans == TRANS_PURPLE2)
  base_trans_strength = 11;

 if (trans_strength >= base_trans_strength)
  return trans + (base % 12);
   else return base;

 return trans;

}
*/
/*
This function had to be modified from the allegro create_color_table
because the allegro version used bestfit_color, whereas we need
specific color values
*/
void special_create_color_table(COLOR_MAP *table)
{
   int x, y, z;
   RGB c;

   for (x = 0; x < PAL_SIZE; x++) {
      for (y = 0; y < PAL_SIZE; y++) {
   z = blend_function(x, y, &c);

      table->data [x] [y] = z;
      }

   }
}


