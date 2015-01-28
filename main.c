
/*

NOTE: Codeblocks text size is changed by ctrl-mousewheel
I keep forgetting this and doing it by accident.

*/

#include "config.h"

#include "allegro.h"

#include <string.h>
#include <math.h>

#include "globvars.h"
#include "stuff.h"
#include "game.h"
#include "display_init.h"
#include "sound.h"
#include "menu.h"
#include "input.h"
#include "palette.h"

// timer interupt functions and variables:
void framecount(void);

volatile int framecounter;
volatile int frames_per_second;

void tickover(void);

volatile unsigned char ticked;
int slacktime;
volatile int long_slacktime_store;
volatile int long_slacktime;

void init_at_startup(void);
void begin_game(void);

void game_loop(void);

struct playerstruct player;
struct enemystruct enemy [NO_ENEMIES];
struct ebulletstruct ebullet [NO_EBULLETS];
struct pbulletstruct pbullet [NO_PBULLETS];
struct arenastruct arena;
struct eclassstruct eclass [NO_ENEMY_TYPES];
struct cloudstruct cloud [NO_CLOUDS];
struct bossstruct boss;

struct optionstruct options;

void framecount(void)
{
   frames_per_second = framecounter;
   framecounter = 0;
   long_slacktime_store = long_slacktime;
   long_slacktime = 0;
}
END_OF_FUNCTION (framecount);

/*
Is called every 25ms; lets the game know that it's time to move on to the next frame.
*/
void tickover(void)
{
 ticked ++;
}
END_OF_FUNCTION (tickover);



int main(void)
{

int allint =  allegro_init();
   if (allint == -1)
   {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      allegro_message("Failed to initialise Allegro! This isn't going to work, sorry.");
      exit(1);
   }


   install_keyboard();
   install_timer();

   three_finger_flag = 0;
   key_led_flag = 0;

 init_at_startup();

 startup_menu();

 return 0;

}

END_OF_MAIN();

/*
Self-explanatory.
*/
void init_at_startup(void)
{


   LOCK_FUNCTION (framecount);
   LOCK_FUNCTION (tickover);
   LOCK_VARIABLE (ticked);
   LOCK_VARIABLE (frames_per_second);
   LOCK_VARIABLE (framecounter);
   LOCK_VARIABLE (turns_per_second);
   LOCK_VARIABLE (turncounter);

   install_int (framecount, 1000);
   install_int (tickover, 20);

   set_color_depth(8);

/*
The following code is for use if your system is set up to prevent an application writing to its own directory.
Normally this will prevent highscores and key configurations being saved, but the following code puts the
initfile in a better place. This may be a default on some systems (eg Unixes and possibly Windows Vista)
and can be set on some other systems.

The only problem with this is that you'll have to manually type or copy the various other options (windowed,
vsync, joystick buttons etc) into the initfile in this new location (or just copy the initfile across, or
just rely on the default values and let the game make a new initfile).

Thanks to Thomas Harte for this code! I had no idea it was necessary (it isn't on my Windows XP computer, and
I haven't tested it).

*/

#define UNIX_OSX_VISTA_ETC


#ifdef UNIX_OSX_VISTA_ETC

   {

//     char *HPath = getenv("HOME");

// use this if for some reason you're running Windows Vista:
        char *HPath = getenv("APPDATA");

	 char ConfigPath[2048];

	 sprintf(ConfigPath, "%s/.Whitebutterfly", HPath);
	 set_config_file(ConfigPath);

   }
#else
   set_config_file("init.txt");
#endif

/*
   {

     char *HPath = getenv("APPDATA");

	 char ConfigPath[2048];

	 sprintf(ConfigPath, "%s/.Whitebutterfly", HPath);
	 set_config_file(ConfigPath);

   }
*/

   options.run_vsync = get_config_int("Misc", "vsync", 0);

   options.highscore [0] = get_config_int("Scores", "Easy", 100);
   options.highscore [1] = get_config_int("Scores", "Normal", 100);
   options.highscore [2] = get_config_int("Scores", "Hard", 100);

   int randseed = get_config_int("Misc", "Seed", 0);
   srand(randseed);

   options.windowed = get_config_int("Misc", "Windowed", 0);


 int windowed;
 switch(options.windowed)
 {
    default:
     case 1: windowed = GFX_AUTODETECT_WINDOWED; break;
     case 0: windowed = GFX_AUTODETECT_FULLSCREEN; break;
 }

//windowed = GFX_AUTODETECT_FULLSCREEN;

   if (set_gfx_mode(windowed, 800, 600, 0, 0) != 0)
   {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      allegro_message("Unable to set 800x600 display mode\n%s\n", allegro_error);
      exit(1);
   }

 init_trig();

 prepare_display();
 init_sound();

 char diff_string [20];

 strcpy(diff_string, "Stage_normal");

 int i;

 for (i = 0; i < 3; i ++)
 {

     if (i == 1)
      strcpy(diff_string, "Stage_hard");
     if (i == 2)
      strcpy(diff_string, "Stage_punishment");

 options.stage_power [0] [0] [i] = get_config_int(diff_string, "power0_0", 0);
 options.stage_power [1] [0] [i] = get_config_int(diff_string, "power1_0", 0);
 options.stage_power [2] [0] [i] = get_config_int(diff_string, "power2_0", 0);
 options.stage_power [3] [0] [i] = get_config_int(diff_string, "power3_0", 0);
 options.stage_power [4] [0] [i] = get_config_int(diff_string, "power4_0", 0);

 options.stage_power [0] [1] [i] = get_config_int(diff_string, "power0_1", 0);
 options.stage_power [1] [1] [i] = get_config_int(diff_string, "power1_1", 0);
 options.stage_power [2] [1] [i] = get_config_int(diff_string, "power2_1", 0);
 options.stage_power [3] [1] [i] = get_config_int(diff_string, "power3_1", 0);
 options.stage_power [4] [1] [i] = get_config_int(diff_string, "power4_1", 0);

 options.stage_power_bar [0] [0] [i] = get_config_int(diff_string, "power_bar0_0", 0);
 options.stage_power_bar [1] [0] [i] = get_config_int(diff_string, "power_bar1_0", 0);
 options.stage_power_bar [2] [0] [i] = get_config_int(diff_string, "power_bar2_0", 0);
 options.stage_power_bar [3] [0] [i] = get_config_int(diff_string, "power_bar3_0", 0);
 options.stage_power_bar [4] [0] [i] = get_config_int(diff_string, "power_bar4_0", 0);

 options.stage_power_bar [0] [1] [i] = get_config_int(diff_string, "power_bar0_1", 0);
 options.stage_power_bar [1] [1] [i] = get_config_int(diff_string, "power_bar1_1", 0);
 options.stage_power_bar [2] [1] [i] = get_config_int(diff_string, "power_bar2_1", 0);
 options.stage_power_bar [3] [1] [i] = get_config_int(diff_string, "power_bar3_1", 0);
 options.stage_power_bar [4] [1] [i] = get_config_int(diff_string, "power_bar4_1", 0);

 options.clear [0] [i] = get_config_int(diff_string, "clear0", 0);
 options.clear [1] [i] = get_config_int(diff_string, "clear1", 0);
 options.clear [2] [i] = get_config_int(diff_string, "clear2", 0);
 options.clear [3] [i] = get_config_int(diff_string, "clear3", 0);
 options.clear [4] [i] = get_config_int(diff_string, "clear4", 0);

 options.finished [0] [i] = get_config_int(diff_string, "finished0", 0);
 options.finished [1] [i] = get_config_int(diff_string, "finished1", 0);
 options.finished [2] [i] = get_config_int(diff_string, "finished2", 0);
 options.finished [3] [i] = get_config_int(diff_string, "finished3", 0);
 options.finished [4] [i] = get_config_int(diff_string, "finished4", 0);

 options.unlock [i] = get_config_int(diff_string, "unlock", 0);

 }


/* options.stage_power [1] [0] [0] = 4;
 options.stage_power_bar [1] [0] [0] = 4500;
 options.stage_power [1] [1] [0] = 6;
 options.stage_power_bar [1] [1] [0] = 750;

 options.stage_power [3] [0] [2] = 2;
 options.stage_power [3] [1] [2] = 5;
 options.finished [0] [0] = 1;
 options.unlock [0] = 1;*/

 player.key [CKEY_UP] = get_config_int("Misc", "key_up", KEY_UP);
 player.key [CKEY_DOWN] = get_config_int("Misc", "key_down", KEY_DOWN);
 player.key [CKEY_LEFT] = get_config_int("Misc", "key_left", KEY_LEFT);
 player.key [CKEY_RIGHT] = get_config_int("Misc", "key_right", KEY_RIGHT);
 player.key [CKEY_FIRE1] = get_config_int("Misc", "key_fire1", KEY_Z);
 player.key [CKEY_FIRE2] = get_config_int("Misc", "key_fire2", KEY_X);
 player.key [CKEY_FIRE3] = get_config_int("Misc", "key_fire3", KEY_C);
 player.key [CKEY_SLOW] = get_config_int("Misc", "key_slow", KEY_LSHIFT);


 options.joy_stick = get_config_int("Misc", "joy_stick", 0);

 options.joy_sensitivity = get_config_int("Misc", "joy_sensitivity", 70);
 options.init_joystick = get_config_int("Misc", "joy_init", 1);
 options.joystick = 0;
 options.key_or_joy = 0; // don't put in initfile!

 options.sfx_volume = get_config_int("Misc", "sfx_volume", 20);
 options.ambience_volume = get_config_int("Misc", "ambience_volume", 100);

// set_config_int("Misc", "Tourist", 3);
// set_config_int("Misc", "joy_stick", 0);

 if (options.init_joystick)
  init_joystick();

 if (options.joystick == 1) // set in init_joystick
 {
  options.joy_button [0] = get_config_int("Misc", "joy_button_1", 0);
  if (options.joy_button [0] > joy[0].num_buttons)
   options.joy_button [0] = joy[0].num_buttons - 1;
  options.joy_button [1] = get_config_int("Misc", "joy_button_2", 1);
  if (options.joy_button [1] > joy[0].num_buttons)
   options.joy_button [1] = joy[0].num_buttons - 1;
  options.joy_button [2] = get_config_int("Misc", "joy_button_slow", 3);
  if (options.joy_button [2] > joy[0].num_buttons)
   options.joy_button [2] = joy[0].num_buttons - 1;
  options.joy_button [3] = get_config_int("Misc", "joy_button_3", 2);
  if (options.joy_button [3] > joy[0].num_buttons)
   options.joy_button [3] = joy[0].num_buttons - 1;
 }

 ticked = 0;
//#endif

// clear_bitmap(screen);
// vsync();
 define_palette();

}



