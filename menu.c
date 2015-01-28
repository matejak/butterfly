#include "config.h"

#include "allegro.h"

#include "globvars.h"

#include "palette.h"
#include "game.h"
#include "input.h"

#include "display.h"

#include "stuff.h"

#define TRANS_MODE drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
#define END_TRANS_MODE drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
#define END_TRANS drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);

#define MENU_CIRCLES 6

 extern RGB palet [256];

 RGB splash_palette [256];

 RLE_SPRITE *white_RLE;
 BITMAP *butter_bitmap;

 RLE_SPRITE *icon_RLEs [30]; // externed in display_init

 extern RLE_SPRITE *eRLE_player [S_PLAYER_RLES];

void key_box(const char ktext [], int command);
int acceptable_char(int scode);
void define_keys(void);
int ship_select(void);
void print_standard_names(int x, int y, int w);
void set_options(void);

void aabox(int x1, int y1, int x2, int y2);

void congratulations(void);
void congrat_wait(int waiting);

extern volatile unsigned char ticked;

extern BITMAP *display;

int menu_select;
int key_wait;
int thing;

int menu_counter;
int counter2;

int new_menu_circle;

int menu_circle_number [MENU_CIRCLES];
int menu_circle_size [MENU_CIRCLES];
int menu_circle_max_size [MENU_CIRCLES];
int menu_circle_rad [MENU_CIRCLES];
int menu_circle_rot [MENU_CIRCLES];
int menu_circle_grow_rate [MENU_CIRCLES];
int menu_circle_angle [MENU_CIRCLES];

int minicircle_size [MENU_CIRCLES];
int minicircle_max_size [MENU_CIRCLES];
int minicircle_distance [MENU_CIRCLES];

void run_menu_circles(void);
void init_menu_circles(void);
void draw_menu_circle(int i, int j);
void goodbye_menu_circles(void);

enum
{
MENU_START,
MENU_DIFFICULTY,
MENU_JOY_OR_KEYS,
MENU_KEYS,
MENU_OPTIONS,
MENU_EXIT
};


enum
{
OPT_SFX_VOLUME,
OPT_MUSIC_VOLUME,
OPT_MODE,
OPT_VSYNC,
OPT_JOY1,
OPT_JOY2,
OPT_JOY3,
OPT_JOYSLOW,
OPT_EXIT
};

int menu_command(int cmd);
int any_joystick_input(void);

enum
{
MC_UP,
MC_LEFT,
MC_DOWN,
MC_RIGHT,
MC_SELECT
};

void set_options(void);


void reset_menu_palette(void);
void run_menu_background(void);

int select_a_ship(void);


void reset_menu_palette(void)
{

 vsync();
// clear_bitmap(screen);
 clear_bitmap(display);

// arena.target_palette = 0;
// set_base_palette();

//   set_base_palette();
//   int newpal = build_new_palette(arena.change_palette, 0, 0, 0);
   vsync();
//   clear_bitmap(screen);
//   set_palette(palet [newpal]);
}



void startup_menu(void)
{


 reset_menu_palette();


 menu_select = 0;
 key_wait = 30;

// int counter;

 menu_counter = 0;
 counter2 = 0;

 arena.difficulty = 0;

 int y1 = 170;
 int y2 = 190 + menu_select * 30;
 int y3 = 218 + menu_select * 30;
 int y4 = 218 + menu_select * 30;

 int anykey = 0;

 int i;

//#define TEST_MUSIC


#ifdef TEST_MUSIC

 arena.level = 3;

 init_beat_new_level();

#endif

 init_menu_circles();

 while (TRUE)
 {


#ifdef TEST_MUSIC
run_beat();

#endif



 clear_to_color(display, COL_BACK1);


 run_menu_background();



 menu_counter += 4;
 if (menu_counter >= 40)
  menu_counter = 0;

 counter2 ++;
 if (counter2 >= 256)
  counter2 = 0;

// textprintf_centre_ex(display, font, 300, 190, -1, -1, "W H I T E   B U T T E R F L Y");
 draw_rle_sprite(display, white_RLE, 150, 150);

 int my = 300;

 y1 = my - 23;
 y2 = my - 10 + menu_select * 30;
 y3 = my + 19 + menu_select * 30;
 y4 = my + 150;


// rectfill(display, 370, y1, 600, y2, COL_COL1);
 TRANS_MODE
 rectfill(display, 370, y2 + 9, 640, y3 - 2, TRANS_BLUE3);
 rect(display, 368, y2 + 7, 641, y3 - 0, TRANS_BLUE3);
 END_TRANS
// rectfill(display, 370, y3, 600, y4, TRANS_DGREEN);


 textprintf_ex(display, font, 400, my, -1, -1, "START GAME");
 switch(arena.difficulty)
 {
      case 0: textprintf_ex(display, font, 400, my + 30, -1, -1, "DIFFICULTY - NORMAL"); break;
      case 1: textprintf_ex(display, font, 400, my + 30, -1, -1, "DIFFICULTY - HARD"); break;
      case 2: textprintf_ex(display, font, 400, my + 30, -1, -1, "DIFFICULTY - PUNISHMENT"); break;
 }
// textprintf_ex(display [2], font, 400, 260, -1, -1, "STAGE - %i", arena.starting_level);
 textprintf_ex(display, font, 400, my + 90, -1, -1, "SET  KEYS");
 textprintf_ex(display, font, 400, my + 120, -1, -1, "OPTIONS");
 textprintf_ex(display, font, 400, my + 150, -1, -1, "EXIT");

// textprintf_ex(display, font, 40, 10, -1, -1, "%i", joy[0].stick[options.joy_stick].axis[0].pos);
// textprintf_ex(display, font, 40, 30, -1, -1, "%i", joy[0].stick[options.joy_stick].axis[1].pos);

/*
 if (options.joystick)
  textprintf_ex(display, font, 400, my + 120, -1, -1, "CALIBRATE JOYSTICK");
   else
   {
    textprintf_ex(display, font, 400, my + 120, -1, -1, "NO JOYSTICK");
    drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
    rectfill(display, 399, my + 110, 530, my + 135, CONVERT_WHITE_TO_GREY);
    drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);
   }
*/
 if (options.joystick)
 {
  if (options.key_or_joy == 1)
  textprintf_ex(display, font, 400, my + 60, -1, -1, "CONTROLS - JOYSTICK");
   else
    textprintf_ex(display, font, 400, my + 60, -1, -1, "CONTROLS - KEYBOARD");
 }
  else
  {
   textprintf_ex(display, font, 400, my + 60, -1, -1, "NO JOYSTICK");
   drawing_mode(DRAW_MODE_TRANS, NULL, 0, 0);
   rectfill(display, 399, my + 50, 530, my + 75, CONVERT_WHITE_TO_GREY);
   drawing_mode(DRAW_MODE_SOLID, NULL, 0, 0);

  }


// textprintf_ex(display, font, 5, 5, -1, -1, "%i", any_joystick_input());



 my = 330;

 y2 = my + 7 + arena.just_got_highscore * 30;
 y3 = my + 29 + arena.just_got_highscore * 30;


 if (arena.just_got_highscore > 0)
 {
   rectfill(display, 0, y2 - 5, 250, y3, COL_COL3);
   rect(display, -1, y2 - 5 - 2, 250 + 2, y3 + 2, COL_COL4);
 }

// textprintf_right_ex(display, font, 137, my, -1, -1, "HIGH");
// textprintf_ex(display, font, 157, my, -1, -1, "SCORES");

 textprintf_centre_ex(display, font, 147, my, -1, -1, "HIGHSCORES");

 textprintf_right_ex(display, font, 137, my + 40, -1, -1, "NORMAL");
 textprintf_ex(display, font, 157, my + 40, -1, -1, "%i", options.highscore [0]);

 textprintf_right_ex(display, font, 137, my + 70, -1, -1, "HARD");
 textprintf_ex(display, font, 157, my + 70, -1, -1, "%i", options.highscore [1]);

 textprintf_right_ex(display, font, 137, my + 100, -1, -1, "PUNISHMENT");
 textprintf_ex(display, font, 157, my + 100, -1, -1, "%i", options.highscore [2]);

 textprintf_right_ex(display, font, 600, 570, -1, -1, "COPYRIGHT 2009 LINLEY HENZELL");

// should print last score here...

 anykey = 0;

  for (i = KEY_A; i < KEY_CAPSLOCK + 1; i ++)
  {
   if (key [i])
   {
    anykey = 1;
   }
  }

  if (anykey == 0 && (options.joystick == 0 || any_joystick_input() == 0))
   key_wait = 0;



 if (key_wait == 0)
 {
  if (menu_command(MC_UP))
  {
   menu_select --;
   if (menu_select < 0)
    menu_select = MENU_EXIT;
   if (menu_select == MENU_JOY_OR_KEYS && options.joystick == 0)
    menu_select = MENU_JOY_OR_KEYS - 1;
   key_wait = 7;
  }
  if (menu_command(MC_DOWN))
  {
   menu_select ++;
   if (menu_select > MENU_EXIT)
    menu_select = 0;
   if (menu_select == MENU_JOY_OR_KEYS && options.joystick == 0)
    menu_select = MENU_KEYS;
   key_wait = 7;
  }
  if (menu_command(MC_LEFT))
  {
//   if (menu_select == 2)
//    arena.starting_level = 1;
   if (menu_select == MENU_JOY_OR_KEYS)
   {
    if (options.key_or_joy == 0)
     options.key_or_joy = 1;
      else
       options.key_or_joy = 0;
   }
   if (menu_select == MENU_DIFFICULTY)
   {
    arena.difficulty --;
    if (arena.difficulty < 0)
     arena.difficulty = 0;
   }
   key_wait = 7;
  }
  if (menu_command(MC_RIGHT))
  {
//   if (menu_select == 2)
//    arena.starting_level = 2;
   if (menu_select == MENU_JOY_OR_KEYS)
   {
    if (options.key_or_joy == 0)
     options.key_or_joy = 1;
      else
       options.key_or_joy = 0;
   }
   if (menu_select == MENU_DIFFICULTY)
   {
    arena.difficulty ++;
    if (arena.difficulty > 2)
     arena.difficulty = 2;
   }
   key_wait = 7;
  }

//  if (key [KEY_ESC])
//   exit(0);
  if (menu_command(MC_SELECT))
  {
   if (menu_select == MENU_EXIT)
   {
    goodbye_menu_circles();
    exit(0);
   }

   if (menu_select == MENU_KEYS)
   {
    key_wait = 10;
    define_keys();
    key_wait = 10;
   }

   if (menu_select == MENU_OPTIONS)
   {
    key_wait = 10;
    set_options();
    key_wait = 10;
   }

/*   if (menu_select == MENU_CALIBRATE)
   {
    jstick_calibrate();
    key_wait = 20;
    ticked = 0;
   }*/

   if (menu_select == MENU_START)
   {
    arena.level = 1;//arena.starting_level;
    ticked = 0;
    key_wait = 30;
    if (ship_select() == 1)
    {
     vsync();
//     clear_bitmap(screen);
//     clear_bitmap(display);
     player.type = PTYPE_BASIC;
//     player.type = PTYPE_BOMBER;
//     player.type = PTYPE_ANENOME;
//     player.type = PTYPE_HAND;
     player.weapon_level = 0;
     int ship_select = select_a_ship();
     if (ship_select != -1)
     {
      switch(ship_select)
      {
       default:
       case 0: player.type = PTYPE_BASIC; break;
       case 1: player.type = PTYPE_ANENOME; break;
       case 2: player.type = PTYPE_BOMBER; break;
       case 3: player.type = PTYPE_HAND; break;
       case 4: player.type = PTYPE_SQUID; break;
      }
      if (arena.level == 1)
       arena.from_start = 1;
        else
         arena.from_start = 0;
      arena.cleared = 0;
      arena.unlocked = 0;
      menu_counter = 0;
      goodbye_menu_circles();
      new_game();
      game_loop();
      if (arena.level == 4)
       congratulations();
      arena.target_palette = PAL_MENU;
      arena.shift_palette = 2;
      init_menu_circles();
//     reset_menu_palette();
      key_wait = 10; // was 1
//      flower_dir *= -1;
      ticked = 0;
     }
    }
     else key_wait = 20;
   }
    else
     key_wait = 20;
  }
 }
  else
   key_wait --;




    do
    {
        thing ++;
    } while (ticked == 0);
    ticked = 0;

 if (arena.shift_palette > 0 && menu_counter % 12 == 0)
 {
  run_palette_shift();
  build_new_palette(0, 0, 0, 0);
  set_palette(palet);
  if (arena.shift_palette == 1)
   arena.shift_palette = 0;
 }

 vsync();
 blit(display, screen, 0, 0, 100, 0, 600, 600);



 };


}

#define MENU_SENSE 100

int menu_command(int cmd)
{

 if (options.joystick)
  poll_joystick();

 switch(cmd)
 {
  case MC_UP:
   if (key [KEY_UP] || key [KEY_8_PAD])
    return 1;
   if (options.joystick && joy[0].stick[options.joy_stick].axis[1].pos < -MENU_SENSE)
    return 1;
   return 0;
  case MC_DOWN:
   if (key [KEY_DOWN] || key [KEY_2_PAD])
    return 1;
   if (options.joystick && joy[0].stick[options.joy_stick].axis[1].pos > MENU_SENSE)
    return 1;
   return 0;
  case MC_LEFT:
   if (key [KEY_LEFT] || key [KEY_4_PAD])
    return 1;
   if (options.joystick && joy[0].stick[options.joy_stick].axis[0].pos < -MENU_SENSE)
    return 1;
   return 0;


  case MC_RIGHT:
   if (key [KEY_RIGHT] || key [KEY_6_PAD])
    return 1;
   if (options.joystick && joy[0].stick[options.joy_stick].axis[0].pos > MENU_SENSE)
    return 1;
   return 0;
  case MC_SELECT:
   if (key [KEY_SPACE] || key [KEY_ENTER] || key [KEY_ENTER_PAD] || key [KEY_Z])
    return 1;
   if (options.joystick && joy[0].button[options.joy_button [0]].b)
    return 1;
   return 0;

 }

 return 0;

}

int any_joystick_input(void)
{
 if (options.joystick)
  poll_joystick();

  if (joy[0].stick[options.joy_stick].axis[0].pos < - MENU_SENSE
   || joy[0].stick[options.joy_stick].axis[0].pos > MENU_SENSE
   || joy[0].stick[options.joy_stick].axis[1].pos < -MENU_SENSE
   || joy[0].stick[options.joy_stick].axis[1].pos > MENU_SENSE)
    return 1;

   if (joy[0].button[options.joy_button [0]].b)
    return 1;

 return 0;
}


void init_menu_circles(void)
{

 int i;

 for (i = 0; i < MENU_CIRCLES; i ++)
 {
   menu_circle_size [i] = 0;

 }

 new_menu_circle = 100 + grand(200);

}

void run_menu_circles(void)
{

 new_menu_circle --;

 int i;

 if (new_menu_circle == 0)
 {
  for (i = 0; i < MENU_CIRCLES; i ++)
  {
   if (menu_circle_size [i] == 0)
   {
    menu_circle_max_size [i] = 500 + grand(1000) + grand(i * 100 + 1);
    menu_circle_size [i] = 1;
    menu_circle_grow_rate [i] = 10 + grand(10);
    menu_circle_rad [i] = 40 + grand(10) + i * 80 + (i * i);
    menu_circle_rot [i] = 1;
    if (grand(2))
     menu_circle_rot [i] = -1;
    if (i < 5 && grand(4) == 0)
     menu_circle_rot [i] *= 2;
      else
      {
       if (i < 3 && grand(4) == 0)
        menu_circle_rot [i] *= 3;
      }
    menu_circle_angle [i] = grand(ANGLE_1);
    menu_circle_number [i] = 2 + grand(2) + grand(2) + grand(i + 1);

    minicircle_size [i] = 0;
    minicircle_max_size [i] = 300 + grand(400);
    if (menu_circle_max_size [i] < 800)
     minicircle_max_size [i] = 100 + grand(300);
    minicircle_distance [i] = grand(20);

    new_menu_circle = 100 + grand(150);
    break;
   }
  }

 }

 int j;

 for (i = 0; i < MENU_CIRCLES; i ++)
 {
  if (menu_circle_size [i] == 0)
   break;

  if (menu_circle_size [i] < menu_circle_max_size [i])
   menu_circle_size [i] += menu_circle_grow_rate [i];
    else
    {
      if (minicircle_size [i] < minicircle_max_size [i])
       minicircle_size [i] += 10;
    }

  menu_circle_angle [i] += menu_circle_rot [i];
  menu_circle_angle [i] &= 1023;


  for (j = 0; j < menu_circle_number [i]; j ++)
  {
   draw_menu_circle(i, j);

  }
 }

}

void goodbye_menu_circles(void)
{

 int i, j;

 int finished = 1, thing, speed = 0, timeout = 0;

 do
 {

 clear_to_color(display, COL_BACK1);

 finished = 1;
 speed ++;
 timeout ++;
 if (timeout > 100)
  return;

 for (i = 0; i < MENU_CIRCLES; i ++)
 {
  if (menu_circle_size [i] <= 0)
   break;
  finished = 0;

  if (menu_circle_size [i] > 0)
   menu_circle_size [i] -= 100;

  if (minicircle_size [i] > 0)
   minicircle_size [i] -= 100;

  menu_circle_angle [i] += menu_circle_rot [i];
  if (menu_circle_rot [i] < 0)
   menu_circle_angle [i] += speed;
    else
     menu_circle_angle [i] -= speed;

  menu_circle_angle [i] &= 1023;

  for (j = 0; j < menu_circle_number [i]; j ++)
  {
   draw_menu_circle(i, j);

  }
 }

    do
    {
        thing ++;
    } while (ticked == 0);
    ticked = 0;

 vsync();
 blit(display, screen, 0, 0, 100, 0, 600, 600);


 } while (finished == 0);

}


#define CENTRE_X 470
#define CENTRE_Y 220

void draw_menu_circle(int i, int j)
{

  int angle = menu_circle_angle [i] + (ANGLE_1 / menu_circle_number [i]) * j;

  int x = CENTRE_X + xpart(angle, menu_circle_rad [i]);
  int y = CENTRE_Y + ypart(angle, menu_circle_rad [i]);

  circlefill(display, x, y, menu_circle_size [i] / 80, COL_COL2);

  if (minicircle_size [i] > 0)
  {
   circlefill(display, CENTRE_X + xpart(angle + minicircle_distance [i], menu_circle_rad [i]),
                       CENTRE_Y + ypart(angle + minicircle_distance [i], menu_circle_rad [i]),
                       minicircle_size [i] / 80, COL_COL2);
   circlefill(display, CENTRE_X + xpart(angle - minicircle_distance [i], menu_circle_rad [i]),
                       CENTRE_Y + ypart(angle - minicircle_distance [i], menu_circle_rad [i]),
                       minicircle_size [i] / 80, COL_COL2);
  }


  if (menu_circle_size [i] / 100 > 3)
   circlefill(display, x, y, menu_circle_size [i] / 100 - 3, COL_COL3);


  if (minicircle_size [i] / 100 > 2)
  {
   circlefill(display, CENTRE_X + xpart(angle + minicircle_distance [i], menu_circle_rad [i]),
                       CENTRE_Y + ypart(angle + minicircle_distance [i], menu_circle_rad [i]),
                       minicircle_size [i] / 100 - 2, COL_COL3);
   circlefill(display, CENTRE_X + xpart(angle - minicircle_distance [i], menu_circle_rad [i]),
                       CENTRE_Y + ypart(angle - minicircle_distance [i], menu_circle_rad [i]),
                       minicircle_size [i] / 100 - 2, COL_COL3);
  }

}


void define_keys(void)
{

 key_box("PRESS KEY FOR UP", CKEY_UP);
 key_box("PRESS KEY FOR LEFT", CKEY_LEFT);
 key_box("PRESS KEY FOR RIGHT", CKEY_RIGHT);
 key_box("PRESS KEY FOR DOWN", CKEY_DOWN);
 key_box("PRESS KEY FOR FIRE 1", CKEY_FIRE1);
 key_box("PRESS KEY FOR FIRE 2", CKEY_FIRE2);
 key_box("PRESS KEY FOR FIRE 3", CKEY_FIRE3);
 key_box("PRESS KEY FOR SLOW", CKEY_SLOW);

 set_config_int("Misc", "key_up", player.key [CKEY_UP]);
 set_config_int("Misc", "key_left", player.key [CKEY_LEFT]);
 set_config_int("Misc", "key_right", player.key [CKEY_RIGHT]);
 set_config_int("Misc", "key_down", player.key [CKEY_DOWN]);
 set_config_int("Misc", "key_fire1", player.key [CKEY_FIRE1]);
 set_config_int("Misc", "key_fire2", player.key [CKEY_FIRE2]);
 set_config_int("Misc", "key_fire3", player.key [CKEY_FIRE3]);
 set_config_int("Misc", "key_slow", player.key [CKEY_SLOW]);

}


void key_box(const char ktext [], int command)
{

 rectfill(display, 200, 240, 400, 290, COL_OUTLINE);
 rect(display, 201, 241, 399, 289, COL_COL1);

 textprintf_centre_ex(display, font, 300, 257, -1, -1, ktext);

 vsync();
 blit(display, screen, 0, 0, 100, 0, 600, 600);

 int inputted = KEY_ESC;

 int i;
 int anykey = 0;

 char holding = 1;

 do
 {

  do
    {
        thing ++;
    } while (ticked == 0);
    ticked = 0;
  key_wait --;


 anykey = 0;

  for (i = KEY_A; i < KEY_CAPSLOCK + 1; i ++)
  {
   if (key [i])
   {
    anykey = 1;
   }
  }

  if (anykey == 0)
   holding = 0;


//  if (key_wait > 0)
//   continue;


  if (holding == 0)
  {

  for (i = KEY_A; i < KEY_CAPSLOCK + 1; i ++)
  {
   if (key [i])
   {
    inputted = i;
    holding = 1;
   }
  }
 }
//   while(acceptable_char(inputted) == 0);
  if (acceptable_char(inputted) != 0)
   break;

 } while (TRUE);


 player.key [command] = inputted;

 key_wait = 10;

}


void set_options(void)
{

 int i, anykey = 0;

 int option_select = 0;

 while (TRUE)
 {

 rectfill(display, 200, 240, 400, 510, COL_OUTLINE);
 rect(display, 201, 241, 399, 509, COL_COL1);



 textprintf_centre_ex(display, font, 300, 257, -1, -1, "options");

 rectfill(display, 202, 300 + option_select * 20, 398, 318 + option_select * 20, TRANS_GREEN3);
 hline(display, 202, 298 + option_select * 20, 398, TRANS_GREEN2);
 hline(display, 202, 320 + option_select * 20, 398, TRANS_GREEN2);

 textprintf_ex(display, font, 220, 300, -1, -1, "effects volume - %i", options.sfx_volume);
 textprintf_ex(display, font, 220, 320, -1, -1, "music volume - %i", options.ambience_volume);

 if (options.windowed == 0)
  textprintf_ex(display, font, 220, 340, -1, -1, "mode - fullscreen");
   else
    textprintf_ex(display, font, 220, 340, -1, -1, "mode - windowed");

 if (options.run_vsync == 0)
  textprintf_ex(display, font, 220, 360, -1, -1, "vsync - off");
   else
    textprintf_ex(display, font, 220, 360, -1, -1, "vsync - on");

 if (options.joystick)
 {
  textprintf_ex(display, font, 220, 380, -1, -1, "fire 1 - joystick %i", options.joy_button [0] + 1);
  textprintf_ex(display, font, 220, 400, -1, -1, "fire 2 - joystick %i", options.joy_button [1] + 1);
  textprintf_ex(display, font, 220, 420, -1, -1, "fire 3 - joystick %i", options.joy_button [3] + 1);
  textprintf_ex(display, font, 220, 440, -1, -1, "slow move - joystick %i", options.joy_button [2] + 1);

 }
  else
  {
   textprintf_ex(display, font, 220, 400, -1, -1, "no joystick");

  }

  textprintf_ex(display, font, 220, 460, -1, -1, "exit");


 vsync();
 blit(display, screen, 0, 0, 100, 0, 600, 600);

 anykey = 0;


  for (i = KEY_A; i < KEY_CAPSLOCK + 1; i ++)
  {
   if (key [i])
   {
    anykey = 1;
   }
  }

  if (anykey == 0 && (options.joystick == 0 || any_joystick_input() == 0))
   key_wait = 0;



 if (key_wait <= 0)
 {
  if (menu_command(MC_UP))
  {
   option_select --;
   if (option_select < 0)
    option_select = OPT_EXIT;
   if (option_select == OPT_JOYSLOW && options.joystick == 0)
    option_select = OPT_VSYNC;
   key_wait = 7;
  }
  if (menu_command(MC_DOWN))
  {
   option_select ++;
   if (option_select > OPT_EXIT)
    option_select = 0;
   if (option_select == OPT_JOY1 && options.joystick == 0)
    option_select = OPT_EXIT;
   key_wait = 7;
  }

  if (menu_command(MC_LEFT))
  {
   if (option_select == OPT_SFX_VOLUME)
   {
    options.sfx_volume -= 10;
    if (options.sfx_volume < 0)
     options.sfx_volume = 0;
   }
   if (option_select == OPT_MUSIC_VOLUME)
   {
    options.ambience_volume -= 10;
    if (options.ambience_volume < 0)
     options.ambience_volume = 0;
   }

   if (option_select == OPT_JOY1)
   {
    if (options.joy_button [0] == 0)
     options.joy_button [0] = joy[0].num_buttons - 1;
      else
       options.joy_button [0] --;
   }
   if (option_select == OPT_JOY2)
   {
    if (options.joy_button [1] == 0)
     options.joy_button [1] = joy[0].num_buttons - 1;
      else
       options.joy_button [1] --;
   }
   if (option_select == OPT_JOYSLOW)
   {
    if (options.joy_button [2] == 0)
     options.joy_button [2] = joy[0].num_buttons - 1;
      else
       options.joy_button [2] --;
   }
   if (option_select == OPT_JOY3)
   {
    if (options.joy_button [3] == 0)
     options.joy_button [3] = joy[0].num_buttons - 1;
      else
       options.joy_button [3] --;
   }
   key_wait = 7;
  }

  if (menu_command(MC_RIGHT))
  {
   if (option_select == OPT_SFX_VOLUME)
   {
    options.sfx_volume += 10;
    if (options.sfx_volume > 100)
     options.sfx_volume = 100;
   }
   if (option_select == OPT_MUSIC_VOLUME)
   {
    options.ambience_volume += 10;
    if (options.ambience_volume > 100)
     options.ambience_volume = 100;
   }

   if (option_select == OPT_JOY1)
   {
    if (options.joy_button [0] >= joy[0].num_buttons - 1)
     options.joy_button [0] = 0;
      else
       options.joy_button [0] ++;
   }
   if (option_select == OPT_JOY2)
   {
    if (options.joy_button [1] >= joy[0].num_buttons - 1)
     options.joy_button [1] = 0;
      else
       options.joy_button [1] ++;
   }
   if (option_select == OPT_JOY3)
   {
    if (options.joy_button [3] >= joy[0].num_buttons - 1)
     options.joy_button [3] = 0;
      else
       options.joy_button [3] ++;
   }
   if (option_select == OPT_JOYSLOW)
   {
    if (options.joy_button [2] >= joy[0].num_buttons - 1)
     options.joy_button [2] = 0;
      else
       options.joy_button [2] ++;
   }
   key_wait = 7;
  }

  if (key [KEY_ESC])
   break;

  if (menu_command(MC_SELECT))
  {
   if (option_select == OPT_EXIT)
   {
    break;
   }
  }

  if (menu_command(MC_SELECT) || menu_command(MC_LEFT) || menu_command(MC_RIGHT))
  {
   if (option_select == OPT_MODE)
   {
    options.windowed ^= 1;
    key_wait = 7;
   }
   if (option_select == OPT_VSYNC)
   {
    options.run_vsync ^= 1;
    key_wait = 7;
   }
  }


/*   if (menu_select == MENU_KEYS)
   {
    key_wait = 10;
    define_keys();
    key_wait = 10;
   }

   if (menu_select == MENU_OPTIONS)
   {
    key_wait = 10;
    set_options();
    key_wait = 10;
   }*/



 }
//  else
   key_wait --;




    do
    {
        thing ++;
    } while (ticked == 0);
    ticked = 0;

 vsync();
 blit(display, screen, 0, 0, 100, 0, 600, 600);



 };

 set_config_int("Misc", "sfx_volume", options.sfx_volume);
 set_config_int("Misc", "ambience_volume", options.ambience_volume);
 if (options.joystick)
 {
  set_config_int("Misc", "joy_button_1", options.joy_button [0]);
  set_config_int("Misc", "joy_button_2", options.joy_button [1]);
  set_config_int("Misc", "joy_button_slow", options.joy_button [2]);
  set_config_int("Misc", "joy_button_3", options.joy_button [3]);
 }
 set_config_int("Misc", "vsync", options.run_vsync);
 set_config_int("Misc", "Windowed", options.windowed);


}



int ship_select(void)
{

 return 1;

}



int acceptable_char(int scode)
{

 switch(scode)
 {
  case KEY_ESC:
   key_wait = 7;
   return 0;
  case KEY_ENTER:
  case KEY_ENTER_PAD:
   key_wait = 7;
   return 0;
 }
 return 1;

}


void run_menu_background(void)
{

 run_menu_circles();
}

void aabox(int x1, int y1, int x2, int y2)
{

 TRANS_MODE
 rectfill(display, x1, y1, x2, y2, TRANS_BLUE1);
 rect(display, x1, y1, x2, y2, TRANS_BLUE3);
 rect(display, x1 + 1, y1 - 1, x2 + 1, y2 - 1, TRANS_BLUE2);
 rect(display, x1 - 1, y1 + 1, x2 - 1, y2 + 1, TRANS_BLUE2);
 rect(display, x1 - 2, y1 + 2, x2 - 2, y2 + 2, TRANS_BLUE1);
 END_TRANS

}

int select_a_ship(void)
{

 int ship_box [8] = {0,0,0,0,0,0,0,0};

 int sbox2;

 int ship_chosen = 0;

 int level_chosen = 1;

 int max_level, wpixels;

 menu_counter = 0;
 counter2 = 0;



 int anykey = 0;

 int i, x, y, k;

 int holding_key = 1;

 while (TRUE)
 {
     menu_counter ++;

 clear_to_color(display, COL_BACK1);
 run_menu_background();

 x = 30;
 y = 40;

#define Y_INTERVAL 100
#define Y_BASE 45


// stages

 aabox(200, 50, 550, 250);
TRANS_MODE
 rectfill(display, 375 - 80, 70, 375 + 80, 100, TRANS_BLUE2);
 rect(display, 375 - 80, 70 - 0, 375 + 80, 100, TRANS_BLUE3);
 rect(display, 375 - 81, 70 - 1, 375 + 81, 100 + 1, TRANS_BLUE2);
END_TRANS

 textprintf_centre_ex(display, font, 375, 80, -1, -1, "select stage");

 switch(arena.difficulty)
 {
      case 0: textprintf_centre_ex(display, font, 375, 115, -1, -1, "NORMAL"); break;
      case 1: textprintf_centre_ex(display, font, 375, 115, -1, -1, "HARD"); break;
      case 2: textprintf_centre_ex(display, font, 375, 115, -1, -1, "PUNISHMENT"); break;
 }

 for (i = 0; i < 3; i ++)
 {
  if (i == 0)
  {
   aabox(230 + i * 70, 140 + i * 25, 270 + i * 70, 180 + i * 25);
   textprintf_centre_ex(display, font, 250 + i * 70, 152 + i * 25, -1, -1, "%i", i + 1);
   rect(display, 276 + i * 70, 141 + i * 25, 302 + i * 70, 152 + i * 25, TRANS_BLUE3);
  }
   else
    if (options.stage_power [ship_chosen] [i - 1] [arena.difficulty] != 0 || options.stage_power_bar [ship_chosen] [i - 1] [arena.difficulty] != 0)
    {
     aabox(230 + i * 70, 140 + i * 25, 270 + i * 70, 180 + i * 25);
     if (options.stage_power [ship_chosen] [i - 1] [arena.difficulty] < 0 || options.stage_power [ship_chosen] [i - 1] [arena.difficulty] > 9)
      options.stage_power [ship_chosen] [i - 1] [arena.difficulty] = 0; // sanity check

     textprintf_centre_ex(display, font, 250 + i * 70, 152 + i * 25, -1, -1, "%i", i + 1);
     TRANS_MODE
     for (k = 0; k < options.stage_power [ship_chosen] [i - 1] [arena.difficulty]; k ++)
     {
      rectfill(display, 305 + i * 70 + k * 4, 143 + i * 25, 307 + i * 70 + k * 4, 150 + i * 25, TRANS_RED1);
      rect(display, 305 + i * 70 + k * 4, 143 + i * 25, 307 + i * 70 + k * 4, 150 + i * 25, TRANS_RED4);
     }

//     rect(display, 275 + i * 70, 143 + i * 25, 303 + i * 70, 152 + i * 25, COL_OUTLINE);
//     rect(display, 276 + i * 70, 144 + i * 25, 302 + i * 70, 151 + i * 25, COL_LGREY);
     rect(display, 276 + i * 70, 141 + i * 25, 302 + i * 70, 152 + i * 25, TRANS_BLUE3);


#define LEVEL_CALC2 (3500 + (1100 * options.stage_power [ship_chosen] [i - 1] [arena.difficulty]))
// duplicated in enemy.c and level.c

     wpixels = ((float) (options.stage_power_bar [ship_chosen] [i - 1] [arena.difficulty] * 20) / LEVEL_CALC2);
     if (wpixels >= 20 || wpixels < 0)
      wpixels = 19;

     if (wpixels > 0)
     {
      rectfill(display, 279 + i * 70, 143 + i * 25, 279 + i * 70 + wpixels, 150 + i * 25, TRANS_RED1);
      rect(display, 278 + i * 70, 143 + i * 25, 278 + wpixels + i * 70, 150 + i * 25, TRANS_RED5);
     }

      END_TRANS

    }


  TRANS_MODE
  if (level_chosen - 1 == i)
    rectfill(display, 230 + i * 70, 140 + i * 25, 270 + i * 70, 180 + i * 25, TRANS_BLUE2);
  END_TRANS

 }


 if (options.clear [ship_chosen] [arena.difficulty])
 {
  TRANS_MODE
  rectfill(display, 207, 236, 214, 243, TRANS_BLUE4);
  END_TRANS

  textprintf_ex(display, font, 219, 231, -1, -1, "cleared");

 }
  else
   if (options.finished [ship_chosen] [arena.difficulty])
   {
    TRANS_MODE
    rectfill(display, 207, 236, 214, 243, TRANS_RED4);
    END_TRANS

    textprintf_ex(display, font, 219, 231, -1, -1, "finished");

   }

// ships

 draw_rle_sprite(display, eRLE_player [S_PLAYER1_1], x + 25 - 16, y + Y_BASE - 20 + 7);
 draw_rle_sprite(display, eRLE_player [S_PLAYER2_1], x + 25 - 19, y + Y_INTERVAL + Y_BASE - 8 - 5);
 draw_rle_sprite(display, eRLE_player [S_PLAYER3_1], x + 25 - 22, y + Y_INTERVAL * 2 + Y_BASE - 7 - 6);
 draw_rle_sprite(display, eRLE_player [S_PLAYER4_1], x + 25 - 16, y + Y_INTERVAL * 3 + Y_BASE - 9 - 4);
 draw_rle_sprite(display, eRLE_player [S_PLAYER4_1L], x + 25 - 16, y + Y_INTERVAL * 3 + Y_BASE - 4 + 3);
 draw_rle_sprite(display, eRLE_player [S_PLAYER5_1], x + 25 - 17, y + Y_INTERVAL * 4 + Y_BASE - 21);
// draw_rle_sprite(display, eRLE_player [S_PLAYER6_1], x + 25 - 16, y + Y_INTERVAL * 5 + 25 - 16);
/*
 textprintf_centre_ex(display, font, 100, 32, -1, -1, "g");
 textprintf_centre_ex(display, font, 100, 44, -1, -1, "g");
 textprintf_centre_ex(display, font, 100, 56, -1, -1, "g");
 textprintf_centre_ex(display, font, 100, 68, -1, -1, "g");
 textprintf_centre_ex(display, font, 100, 80, -1, -1, "g");
*/

 TRANS_MODE
 for (i = 0; i < 5; i ++)
 {
  rectfill(display, x, y - 25 + Y_BASE + i * Y_INTERVAL, x + 50, y + 25 + Y_BASE + i * Y_INTERVAL, TRANS_BLUE1);
  rect(display, x - 1, y - 25 + Y_BASE + i * Y_INTERVAL - 1, x + 50 + 1, y + 25 + Y_BASE + i * Y_INTERVAL + 1, TRANS_BLUE2);
  rect(display, x - 2, y - 25 + Y_BASE + i * Y_INTERVAL - 2, x + 50 + 2, y + 25 + Y_BASE + i * Y_INTERVAL + 2, TRANS_BLUE3);
  rect(display, x - 3, y - 25 + Y_BASE + i * Y_INTERVAL - 3, x + 50 + 3, y + 25 + Y_BASE + i * Y_INTERVAL + 3, TRANS_BLUE1);
  if (ship_chosen == i)
  {
    switch((menu_counter / 4) % 4)
    {
        case 0:
        default:
         rectfill(display, x - 1, y + i * Y_INTERVAL - 1 - 25 + Y_BASE, x + 50 + 1, y + 25 + Y_BASE + i * Y_INTERVAL + 1, TRANS_BLUE2);
         break;
/*        case 1:
         rectfill(display, x - 1, y + i * Y_INTERVAL - 1, x + 50 + 1, y + 50 + i * Y_INTERVAL + 1, TRANS_BLUE3);
         break;
        case 2:
         rectfill(display, x - 1, y + i * Y_INTERVAL - 1, x + 50 + 1, y + 50 + i * Y_INTERVAL + 1, TRANS_BLUE2);
         break;
        case 3:
         rectfill(display, x - 1, y + i * Y_INTERVAL - 1, x + 50 + 1, y + 50 + i * Y_INTERVAL + 1, TRANS_BLUE1);
         break;*/
  }
  }

  if (ship_box [i] > 0)
  {
   sbox2 = ship_box [i];
//   if (ship_box [ship_chosen] > 12 + xpart(menu_counter * 8, 5))
//    sbox2 = 12 + xpart(menu_counter * 8, 5);
   rect(display, x - 3 - sbox2, y + i * Y_INTERVAL - 3 - sbox2 - 25 + Y_BASE, x + 50 + 3 + sbox2, y + 25 + Y_BASE + i * Y_INTERVAL + 3 + sbox2, TRANS_BLUE3);
   ship_box [i] --;
  }

 }
 END_TRANS

 ship_box [ship_chosen] += 2;
 if (ship_box [ship_chosen] > 15)// + xpart(menu_counter * 8, 5))
  ship_box [ship_chosen] = 15;

#define TITLE_X 375
#define TITLE_Y 320

#define LINE_X 220

#define LINE1_Y 350
#define LINE2_Y 365
#define LINE3_Y 380
#define LINE4_Y 395


 TRANS_MODE
 rectfill(display, 200, 300, 550, 550, TRANS_BLUE1);
 rect(display, 200 - 1, 300 - 1, 550 + 1, 550 + 1, TRANS_BLUE2);
 rect(display, 200 - 2, 300 - 2, 550 + 2, 550 + 2, TRANS_BLUE3);
 rect(display, 200 - 3, 300 - 3, 550 + 3, 550 + 3, TRANS_BLUE2);
 rect(display, 200 - 4, 300 - 4, 550 + 4, 550 + 4, TRANS_BLUE1);

 sbox2 = ship_box [ship_chosen];

 rect(display, 200 - sbox2 - 0, 300 - sbox2 - 0, 550 + sbox2 + 0, 550 + sbox2 + 0, TRANS_BLUE1);
 rect(display, 200 - sbox2 - 1, 300 - sbox2 - 1, 550 + sbox2 + 1, 550 + sbox2 + 1, TRANS_BLUE2);
 rect(display, 200 - sbox2 - 2, 300 - sbox2 - 2, 550 + sbox2 + 2, 550 + sbox2 + 2, TRANS_BLUE3);
 rect(display, 200 - sbox2 - 3, 300 - sbox2 - 3, 550 + sbox2 + 3, 550 + sbox2 + 3, TRANS_BLUE2);
 rect(display, 200 - sbox2 - 4, 300 - sbox2 - 4, 550 + sbox2 + 4, 550 + sbox2 + 4, TRANS_BLUE1);

 vline(display, TITLE_X, 550 + sbox2 + 2, 590, TRANS_BLUE3);
 hline(display, 5, 590, TITLE_X, TRANS_BLUE3);
 vline(display, 5, 40 + 25 + ship_chosen * Y_INTERVAL, 590, TRANS_BLUE3);
 hline(display, 5, 40 + 25 + ship_chosen * Y_INTERVAL, 30 - sbox2 - 3, TRANS_BLUE3);

 END_TRANS

char ftext [8] [9] [150] =
{
  {
   "white butterfly",
   "1 - autocannon",
   "2 - heavy cannon",
   "3 - seekers",
   "fire1 - autocannon",
   "fire2 - charge heavy cannon",
   "fire1 while charging - seekers",
   ""
  },
  {
   "anemone",
   "1 - single cannon",
   "2 - strong, short-range blade",
   "3 - weak, long-range blade",
   "fire1 - single cannon",
   "fire2 - blade slash",
   "fire1 + fire2 - blade swing",
   " hold blade buttons for full circle",
   ""
  },
  {
   "spinneret",
   "1 - orb beams",
   "2 - orb scatter fire",
   "3 - bomb launcher",
   "fire1 - orb beams",
   "fire1 + fire2 - orb fire",
   "fire2 - bomb launcher",
   ""
  },
  {
   "hand of xom",
   "1 - multi-cannon",
   "2 - closed hand",
   "3 - open hand",
   "fire1 - multi-cannon",
   "fire2 - open up hand"
   "",
   ""
  }
,
  {
   "chaos squid",
   "1 - chaos droplets",
   "2 - chaos swarm",
   "3 - orb field",
   "fire1 while orb docked - fire droplets",
   "fire1 while orb loose - fire swarm",
   "fire2 - control orb",
   "fire1 + fire2 - repel orb"
  }

};

#define TEXT_X 230

 TRANS_MODE
 rectfill(display, TITLE_X - 80, TITLE_Y - 0, TITLE_X + 80, TITLE_Y + 30, TRANS_BLUE2);
 rect(display, TITLE_X - 80, TITLE_Y - 0, TITLE_X + 80, TITLE_Y + 30, TRANS_BLUE3);
 rect(display, TITLE_X - 81, TITLE_Y - 1, TITLE_X + 81, TITLE_Y + 31, TRANS_BLUE2);

 textprintf_centre_ex(display, font, TITLE_X, TITLE_Y + 10, -1, -1, ftext [ship_chosen] [0]);


 if (ship_chosen == 4 && options.unlock [arena.difficulty] == 0)
 {

  textprintf_centre_ex(display, font, TITLE_X, TITLE_Y + 80, -1, -1, "finish stage 3 to unlock");

 }
  else
  {
 hline(display, 198, LINE1_Y + 30, 320, TRANS_BLUE3);
 textprintf_ex(display, font, TEXT_X - 5, LINE1_Y + 20, -1, -1, "weapons");

 textprintf_ex(display, font, TEXT_X, LINE1_Y + 40, -1, -1, ftext [ship_chosen] [1]);
 textprintf_ex(display, font, TEXT_X, LINE1_Y + 55, -1, -1, ftext [ship_chosen] [2]);
 textprintf_ex(display, font, TEXT_X, LINE1_Y + 70, -1, -1, ftext [ship_chosen] [3]);
// textprintf_ex(display, font, TEXT_X, LINE1_Y + 60, -1, -1, ftext [ship_chosen] [4]);

 hline(display, 198, LINE1_Y + 110, 320, TRANS_BLUE3);
 textprintf_ex(display, font, TEXT_X - 5, LINE1_Y + 100, -1, -1, "controls");

 textprintf_ex(display, font, TEXT_X, LINE1_Y + 120, -1, -1, ftext [ship_chosen] [4]);
 textprintf_ex(display, font, TEXT_X, LINE1_Y + 135, -1, -1, ftext [ship_chosen] [5]);
 textprintf_ex(display, font, TEXT_X, LINE1_Y + 150, -1, -1, ftext [ship_chosen] [6]);
 textprintf_ex(display, font, TEXT_X, LINE1_Y + 165, -1, -1, ftext [ship_chosen] [7]);
  }

 END_TRANS

//  ship_box [ship_chosen] = 15 + xpart(menu_counter * 8, 5);

/*
 draw_rle_sprite(display, eRLE_player [S_PLAYER1_1], x + 25 - 16, y + 25 - 20 + 7);
 draw_rle_sprite(display, eRLE_player [S_PLAYER2_1], x + 90 + 25 - 19, y + 25 - 8 - 5);
 draw_rle_sprite(display, eRLE_player [S_PLAYER3_1], x + 25 - 22, y + Y_INTERVAL + 25 - 7 - 6);
 draw_rle_sprite(display, eRLE_player [S_PLAYER4_1], x + 90 + 25 - 16, y + Y_INTERVAL + 25 - 9 - 4);

 textprintf_centre_ex(display, font, 30, 102, -1, -1, "g");
 textprintf_centre_ex(display, font, 30, 114, -1, -1, "g");
 textprintf_centre_ex(display, font, 30, 126, -1, -1, "g");
 textprintf_centre_ex(display, font, 30, 138, -1, -1, "g");
 textprintf_centre_ex(display, font, 30, 150, -1, -1, "g");


 TRANS_MODE
 for (i = 0; i < 4; i ++)
 {
  rectfill(display, x, y + i * Y_INTERVAL, x + 50, y + 50 + i * Y_INTERVAL, TRANS_BLUE1);
  rect(display, x - 1, y + i * Y_INTERVAL - 1, x + 50 + 1, y + 50 + i * Y_INTERVAL + 1, TRANS_BLUE2);
  rect(display, x - 2, y + i * Y_INTERVAL - 2, x + 50 + 2, y + 50 + i * Y_INTERVAL + 2, TRANS_BLUE3);
  rect(display, x - 3, y + i * Y_INTERVAL - 3, x + 50 + 3, y + 50 + i * Y_INTERVAL + 3, TRANS_BLUE1);

  rectfill(display, x + 90, y + i * Y_INTERVAL, x + 140, y + 50 + i * Y_INTERVAL, TRANS_BLUE1);
  rect(display, x + 90 - 1, y + i * Y_INTERVAL - 1, x + 140 + 1, y + 50 + i * Y_INTERVAL + 1, TRANS_BLUE2);
  rect(display, x + 90 - 2, y + i * Y_INTERVAL - 2, x + 140 + 2, y + 50 + i * Y_INTERVAL + 2, TRANS_BLUE3);
  rect(display, x + 90 - 3, y + i * Y_INTERVAL - 3, x + 140 + 3, y + 50 + i * Y_INTERVAL + 3, TRANS_BLUE1);

 }
 END_TRANS
*/

 if (key [KEY_ESC])
  return -1;

  anykey = 0;

  for (i = KEY_A; i < KEY_CAPSLOCK + 1; i ++)
  {
   if (key [i])
   {
    anykey = 1;
   }
  }

  if (anykey == 0 && (options.joystick == 0 || any_joystick_input() == 0))
  {
   key_wait = 0;
   holding_key = 0;
  }

/*  if (anykey == 0)
  {
   key_wait = 0;
   holding_key = 0;
  }*/


 max_level = 1;

 if (options.stage_power [ship_chosen] [0] [arena.difficulty] > 0 || options.stage_power_bar [ship_chosen] [0] [arena.difficulty] > 0)
  max_level = 2;
 if (options.stage_power [ship_chosen] [1] [arena.difficulty] > 0 || options.stage_power_bar [ship_chosen] [1] [arena.difficulty] > 0)
  max_level = 3;

// if (key_wait <= 0 && holding_key == 0)
if (holding_key == 0)
 {

   if (key [player.key [CKEY_UP]] || menu_command(MC_UP))
   {
    ship_chosen --;
    if (ship_chosen < 0)
     ship_chosen = 4;
    holding_key = 1;
    key_wait = 7;
    level_chosen = 1;
   }

   if (key [player.key [CKEY_DOWN]] || menu_command(MC_DOWN))
   {
    ship_chosen ++;
    if (ship_chosen > 4)
     ship_chosen = 0;
    holding_key = 1;
    key_wait = 7;
    level_chosen = 1;
   }

   if (key [player.key [CKEY_LEFT]] || menu_command(MC_LEFT))
   {
    level_chosen --;
    if (level_chosen < 1)
     level_chosen = max_level;
    holding_key = 1;
    key_wait = 7;
   }

   if (key [player.key [CKEY_RIGHT]] || menu_command(MC_RIGHT))
   {
    level_chosen ++;
    if (level_chosen > max_level)
     level_chosen = 1;
    holding_key = 1;
    key_wait = 7;
   }


   if (key [player.key [CKEY_FIRE1]] || key [KEY_ENTER] || key [KEY_SPACE] || key [KEY_Z] || menu_command(MC_SELECT))
   {
     if (ship_chosen != 4 || options.unlock [arena.difficulty] == 1)
     {
       arena.level = level_chosen;
       return ship_chosen;
     }
   }

//  if (key [KEY_UP] || key [KEY_8_PAD])

/*  if (key [KEY_UP] || key [KEY_8_PAD])
  {
   menu_select --;
   if (menu_select < 0)
    menu_select = MENU_EXIT;
   if (menu_select == MENU_CALIBRATE && options.joystick == 0)
    menu_select = MENU_KEYS;
   if (menu_select == MENU_JOY_OR_KEYS && options.joystick == 0)
    menu_select = MENU_JOY_OR_KEYS - 1;
//   key_wait = 7;
   holding_key = 1;
  }
  if (key [KEY_DOWN] || key [KEY_2_PAD])
  {
   menu_select ++;
   if (menu_select > MENU_EXIT)
    menu_select = 0;
   if (menu_select == MENU_CALIBRATE && options.joystick == 0)
    menu_select = MENU_EXIT;
   if (menu_select == MENU_JOY_OR_KEYS && options.joystick == 0)
    menu_select = MENU_KEYS;
//   key_wait = 7;
    holding_key = 1;
  }*/
/*  if (key [KEY_LEFT] || key [KEY_4_PAD])
  {
//   if (menu_select == 2)
//    arena.starting_level = 1;
   key_wait = 7;
   holding_key = 1;
  }*/

  }
// }
  else
   key_wait --;




    do
    {
        thing ++;
    } while (ticked == 0);
    ticked = 0;

 if (arena.shift_palette > 0 && menu_counter % 12 == 0)
 {
  run_palette_shift();
  build_new_palette(0, 0, 0, 0);
  set_palette(palet);
  if (arena.shift_palette == 1)
   arena.shift_palette = 0;
 }

 vsync();
 blit(display, screen, 0, 0, 100, 0, 600, 600);



 };


//}




}




void congratulations(void)
{

// clear_bitmap(screen);
 vsync();
 clear_bitmap(screen);
 clear_bitmap(display);

// arena.target_palette = BASECOL_CFLOWER;
// set_base_palette();

//   int newpal = build_new_palette(arena.change_palette, 0, 0, 0);
   vsync();
//   set_palette(palet [newpal]);

 int y;
 int y_int = 35;

 int c_count = 0;

// init_cflowers();

 do
 {

//   run_cflowers();

     y = 90;

 textprintf_centre_ex(display, font, 320, y, -1, -1, "v i c t o r y ! !");

 y += y_int * 2;

 if (c_count >= 100)
  textprintf_centre_ex(display, font, 320, y, -1, -1, "YOU  HAVE  TRAVERSED  THE  THREE  OUTER  WORLDS");
 y += y_int;

 if (c_count >= 200)
  textprintf_centre_ex(display, font, 320, y, -1, -1, "AND  FILLED  THEM  WITH  THE  BURNING  WRECKAGE  OF  YOUR  ENEMIES");
 y += y_int;

 if (c_count >= 350)
  textprintf_centre_ex(display, font, 320, y, -1, -1, "NICE  WORK.");

 y += y_int * 2;
 if (c_count >= 450)
 {
  switch(arena.difficulty)
  {
     case 0: textprintf_centre_ex(display, font, 320, y, -1, -1, "BUT  CAN  YOU  DEFEAT  A  MORE  CHALLENGING  LEVEL?"); break;
     case 1: textprintf_centre_ex(display, font, 320, y, -1, -1, "YOUR  ACHIEVEMENT  IS  IMPRESSIVE!"); break;
     case 2: textprintf_centre_ex(display, font, 320, y, -1, -1, "SURELY THIS HAS BEEN PUNISHMENT ENOUGH."); break;
  }
 }

 if (c_count >= 550)
 {
  y += y_int * 2;
  textprintf_centre_ex(display, font, 320, y, -1, -1, "PRESS  SPACE  TO  CONTINUE");

  TRANS_MODE
  rectfill(display, 100, y, 500, y + 15, CONVERT_WHITE_TO_GREY);
  END_TRANS_MODE

  if (key [KEY_SPACE])
   break;
 }

 blit(display, screen, 0, 0, 100, 0, 600, 600);

 do
 {
  rest(5);
 } while (ticked == 0);

 ticked --;
 c_count ++;
 if (c_count > 30000)
  c_count = 30000;

 } while(TRUE);

 clear_bitmap(screen);
 rest(200);




 clear_bitmap(screen);
 vsync();
 clear_bitmap(display);

// arena.target_palette = BASECOL_L1_SPRING;
// set_base_palette();

//   newpal = build_new_palette(arena.change_palette, 0, 0, 0);
   vsync();
//   set_palette(palet [newpal]);



}


