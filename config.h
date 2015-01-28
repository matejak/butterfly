
#define ALLEGRO_STATICLINK


#define NO_KEYS CKEY_END
#define X_MIN 17120
#define Y_MIN 21120
#define X_MAX 597280
#define Y_MAX 609280
#define GRAIN 1024


#define ANGLEFULL_HEX 0x400
#define ANGLE_1 1024
#define ANGLE_2 512
#define ANGLE_3 341
// 3 is not exact
#define ANGLE_4 256
#define ANGLE_5 205
// not exact
#define ANGLE_6 171
// 6 is not exact
#define ANGLE_7 146
// 7 not exact
#define ANGLE_8 128
#define ANGLE_9 114
// 9 not exact
#define ANGLE_10 102
#define ANGLE_12 85
#define ANGLE_14 73
#define ANGLE_16 64
#define ANGLE_8_3 384
#define ANGLE_16_3 192
#define ANGLE_24 42
#define ANGLE_32 32
#define ANGLE_64 16
#define ANGLE_128 8
#define ANGLE_TO_FIXED 4

#ifndef PI
#define PI 3.141592
#endif

#define NO_ENEMIES 50
#define NO_EBULLETS 500
#define NO_PBULLETS 100
#define NO_CLOUDS 2000

#define SMALL_ROTATIONS 256

#define MAX_TARGETS 40

#define NO_STARS 40

#define RAND_X (grand(X_MAX - (X_MIN * 4)) + X_MIN * 2)
#define RAND_Y (grand(Y_MAX - (Y_MIN * 4)) + Y_MIN * 2)

#define RAND_X_IN (grand(X_MAX - (X_MIN * 12)) + X_MIN * 6)
#define RAND_Y_IN (grand(Y_MAX - (Y_MIN * 12)) + Y_MIN * 6)

#define X_MIDDLE 300 * 1024
#define Y_MIDDLE 300 * 1024

#define RLE_CIRCLES 30

#define NO_PARTS 6

#define NO_PETALS 8

#define AIMS 4

enum
{
CKEY_LEFT,
CKEY_RIGHT,
CKEY_UP,
CKEY_DOWN,
CKEY_FIRE1,
CKEY_FIRE2,
CKEY_FIRE3,
CKEY_SLOW,
CKEY_END

};

enum
{
WPN_SHOOT,
WPN_TYPES

};

enum
{
PBULLET_NONE,
PBULLET_SHOT,
PBULLET_CHARGE1,
PBULLET_SEEKER1,
PBULLET_ASHOT,
PBULLET_BLADE,
PBULLET_RINGSHOT,
PBULLET_BOMB,
PBULLET_HAND,
PBULLET_HSHOT,
PBULLET_ORB,
PBULLET_ORBSHOT,
PBULLET_ORBSEEKER,
PBULLET_ORBFIELD

};

enum
{
BLADE_LEFT,
BLADE_RIGHT,
BLADE_CW_SLASH,
BLADE_ACW_SLASH,
BLADE_CW_SWING,
BLADE_ACW_SWING,
BLADE_RETURN_CW_LEFT,
BLADE_RETURN_ACW_LEFT,
BLADE_RETURN_CW_RIGHT,
BLADE_RETURN_ACW_RIGHT
};

enum
{
HAND_DOCK,
HAND_SEEK,
HAND_STOP,
HAND_RETURN
};

enum
{
ORB_DOCK,
ORB_FIRED,
ORB_FLOAT,
ORB_RETURN,
ORB_AWAY

};

#define SHOW_PLAYER_DAMAGE

struct playerstruct
{

 int key [CKEY_END];
 int x;
 int y;
 int lives;
 int score;
 int respawning;
 int in_play;
 int grace;

 int type;

 int recycle1;
 int recycle2;
 int recycle3;
 int recycle_speed1;
 int recycle_speed2;
 int recycle_speed3;

 char hold_key1;
 char hold_key2;

 int weapon_level;

 int wstrength [3]; // stores damage per second information for display - debugging/balancing

// ship 1
 char recoil;
 char recoiling;
 int charge;
 char key_holding;

// anenome
 char blade_pos;
 int blade_angle;
 int blade_x;
 int blade_y;
 int blade_distance;
 int blade_size;
 int swing_hold;
 int old_blade_x [10];
 int old_blade_y [10];
 int old_blade_x2 [10];
 int old_blade_y2 [10];
 int old_blade_x_speed [10];
 int old_blade_y_speed [10];
 int old_blade_strength [10];
 int old_blade_pos;
 int old_blade_length;
 int blade_redness;

// bomber
 int ring_angle;
 int ring_speed;
 int ring_size;
 int beam_y [3];
 char orb_firing;
 char beam_firing;
 int bomb_recycle1;
 int bomb_recycle2;

// hand
 int hand_x;
 int hand_y;
 int hand_x_speed;
 int hand_y_speed;
 int lock [6];
 int lock_size [6];
 int hand_stop;
 char hand_phase;
 char arc_firing;
 int hand_unfold;
 int hand_angle;
 char hand_acquire;
 char grip_frame;

// squid
 int orb_x;
 int orb_y;
 int orb_x_speed;
 int orb_y_speed;
 int orb_lock;
 int orb_unfold;
 int orb_lock_x;
 int orb_lock_y;
 char orb_frame;
 char orb_phase;
 char orb_hold_key2;
 int field_angle;
 int field_angle_inc;
 int field_size;
 int field_radius;

// int recycle_speed [3];
 int recycle [3];
 //int wstrength [3];
 int rpixels [3];
 int wtype [3];
 int weapon [3];
 int wx [3];
 int wy [3];
 int wstyle [3];
 int wstatus [3];
 int wlevel [3];
 int wxp [3];
 int wpixels [3];
 int wpixels2 [3];
 int wgained [3];
 int wflash [3];
 int bflash [3];
 int overbflash;
 int wfired;
 int wframe;
 int wframe_speed;
 int wframe_max;

 int orbitals;
 int orbital_angle;
 int firing;

 int beam [3];
 int beam_counter [3];
 int beam_fade [3];
// int beam_index;
 int beam_open [3];
 char beam_flicker [3];

 int swbeam [3];
 int swbeam_counter [3];
 int swbeam_x1 [3], swbeam_y1 [3], swbeam_x2 [3], swbeam_y2 [3];
 int swbeam_level [3];

 int wings1;
 int wings2;

/* int swing_x [10];
 int swing_y [10];
 int swing_x2 [10];
 int swing_y2 [10];
 int swing_strength [10];
 int swing_pos;
// int swing_pos2;
 int swing_angle;
 int swing_sprite_angle;
// int swing_speed_x;
 //int swing_speed_y;
 int swing_state;
 int swing_length;
 int swing_hold;
 int blade_index;*/

/* int ring_angle;
 int ring_hold;
 int ring_index;*/
 int move_x;
 int move_y;


};

enum
{
PTYPE_NONE,
PTYPE_BASIC,
PTYPE_ANENOME,
PTYPE_BOMBER,
PTYPE_HAND,
PTYPE_SQUID,
PTYPE_MULTI
};
// set in menu.c

struct eclassstruct
{
 int max_hp;
 int xsize;
 int ysize;
 int xoffset;
 int yoffset;
 int cbox [7] [4];
 int ai;
 int speed1;
 int speed2;
 int speed3;
 char priority;
 int score;

// int speed;


};

enum
{
    CLOUD_NONE,
    CLOUD_CIRCLE1_2,
    CLOUD_CIRCLE1,
    CLOUD_CIRCLE2,
    CLOUD_CIRCLE4,
    CLOUD_CIRCLE8,

// flickering
    CLOUD_FCIRCLE1_2,
    CLOUD_FCIRCLE1,
    CLOUD_FCIRCLE2,
    CLOUD_FCIRCLE4,
    CLOUD_FCIRCLE8,

// 3 colours, flickering
    CLOUD_3FCIRCLE1_2,
    CLOUD_3FCIRCLE1,
    CLOUD_3FCIRCLE2,
    CLOUD_3FCIRCLE4,
    CLOUD_3FCIRCLE8,

// 3 colours, fixed size:
    CLOUD_3PCIRCLE1_2,
    CLOUD_3PCIRCLE1,
    CLOUD_3PCIRCLE2,
    CLOUD_3PCIRCLE4,
    CLOUD_3PCIRCLE8,

    CLOUD_WAITCIRCLE1,
    CLOUD_WAITCIRCLE2,
    CLOUD_WAITCIRCLE4,

    CLOUD_SMALL_SHOCK,
    CLOUD_LARGE_SHOCK,
    CLOUD_HUGE_SHOCK,
    CLOUD_HUGE_SHOCK_FAST,

    CLOUD_SEEKER_TRAIL,
    CLOUD_ORBSEEKER_TRAIL,

    CLOUD_RUMBLE,

    CLOUD_BLADE_TRAIL,

    CLOUD_SHRINKING_LOCK,
    CLOUD_SHRINKING_BEAM

};

struct cloudstruct
{
 int type;
 int x;
 int y;
 int x2;
 int y2;
 int x_speed;
 int y_speed;
 int timeout;
 int colour;
 int status;
 int status2;
 int drag;
 int angle;
};

struct enemystruct
{
 int type;
 int x;
 int y;
 int x_speed;
 int y_speed;
 int recycle;
 int burst;
 int persist;
 int hp;
 int pulse1;
 int pulse2;
 int frame;

 int x_target [MAX_TARGETS];
 int y_target [MAX_TARGETS];
 int target_time;
 int max_target_time;
 int target;
 int phase;
 int phase2;

 int angle;

 int aim [AIMS];
 int aim_turning [AIMS];
 int aim_turning_count [AIMS];
 int angle1;
 int angle2;

 int angle_turning;
 int angle_turning_count;
 int angle1_turning;
 int angle1_turning_count;

 int distance;

 int whurt [3];

 int part [NO_PARTS];
 int part_index;
 int parent;

 int status;
 int status2;

 int escorting;

};

enum
{
ENEMY_NONE,
ENEMY_FIGHTER,
ENEMY_DOUBLE,
ENEMY_DOME,
ENEMY_FIGHTER2,
ENEMY_FIGHTER3,
ENEMY_FIGHTER4,
ENEMY_MACHINE,
ENEMY_BIG,
ENEMY_BIG2,
ENEMY_DOUBLE2,
ENEMY_FIGHTER5,
ENEMY_FIGHTER6,
ENEMY_MEDIUM,
ENEMY_DOUBLE3,
ENEMY_BIG3,
ENEMY_RISER,
ENEMY_DOME2,

ENEMY_MACHINE3,
ENEMY_WIDE,
ENEMY_BIG4,
ENEMY_FIGHTER7,
ENEMY_FIGHTER8,
ENEMY_DOUBLE4,
ENEMY_DOME3,

ENEMY_BOSS1_1,
ENEMY_BOSS1_2,
ENEMY_BOSS1_3,
ENEMY_BOSS2_1,

ENEMY_BOSS2_1_DEAD,
ENEMY_BOSS2_2,

ENEMY_BOSS3_1,
ENEMY_BOSS3_2,
ENEMY_BOSS3_0, // actually first L3 boss

// don't add things here unless they're going to exist!

ENEMY_MACHINE2,
NO_ENEMY_TYPES

};


enum
{
AI_SWOOPER,
AI_DSWOOPER,
AI_DARTER,
AI_STOPPER,
AI_DIVER,
AI_DART_STOPPER,
AI_DRIFTER,
AI_CRUISER,
AI_BOSS,
AI_HOOK,
AI_WALK,
AI_STROLLER,
AI_STROLLER2,
AI_FALL,
AI_DANCER,
AI_GLIDER,
AI_MARCH,
AI_ATTACKER,
AI_BEAMER,
AI_PAUSER,
AI_BOSS1,
AI_BOSS2_1_DEAD
};

struct ebulletstruct
{
 int type;
 int x;
 int y;
 int x_speed;
 int y_speed;
 int angle;
 int angle2;
 int timeout;
 int time;
 int accel;
 int sprite_angle;
 int colour;
 int status;
 char new_ebullet;
};

enum
{
EBULLET_NONE,
EBULLET_SHOT,
EBULLET_SHOT2,
EBULLET_WIGGLY,
EBULLET_FLAKE,
EBULLET_IBULLET,
EBULLET_IBULLET_2,
EBULLET_IBULLET_RGB,
EBULLET_BEAM,
EBULLET_B13SHOT,
EBULLET_FLAKE2,
EBULLET_FLAKE3,
EBULLET_BEAM2,
EBULLET_BLOB,
EBULLET_B21SHOT,
EBULLET_SPINNER,
EBULLET_SPINNER3,
EBULLET_STOPMINE,
EBULLET_DRAGGER,
EBULLET_BLOBSHOT


};

struct pbulletstruct
{
  int x;
  int y;
  int type;
  int x_speed;
  int y_speed;
  int angle;
  int timeout;
  int time;
  int persist;
  int xsize;
  int ysize;
  int dam;
  int source;
  int target;
  int status;
  int level;
};



enum
{
MULTI_NONE,
MULTI_BASIC
};

enum
{
  RANK_C,
  RANK_B,
  RANK_A,
  RANK_S,
  RANK_P
};

struct arenastruct
{
  int level;
  int difficulty;

  int level_finished;
  int game_over;
  int new_level_sign;

  int from_start;
  int cleared;
  int unlocked;

//  int change_palette;
  int shift_palette;
  int target_palette;

//  int shift_message;

  int phase;

  int underlay_position;

  unsigned char counter;

  int beam;
  int old_beam;

  int number;

  int stage_rank;
  int destruction_rate;

/*
  int block1_x [20];
  int block1_y [20];
  int block1_y_speed [20];
  int block1_size [20];
  int block2_x [20];
  int block2_y [20];
  int block2_y_speed [20];
  int block2_size [20];

  int flower_turn1;
  int flower_turn2;

  int beam;
  int old_beam;

  int part_x1 [18] [16]; // just needs to be [18] [14] but I added padding after a stupid mistake.
  int part_y1 [18] [16];
  int part_x2 [18] [16];
  int part_y2 [18] [16];
  int part_cx [18] [16];
  int part_cy [18] [16];
  int part_flow;
  int part_rank;
  int wave_y;
  int part_pulse1;
  int part_pulse2;

  int cruiser_escorts;
*/
  char just_got_highscore;

  int stage_score;
  int max_stage_score;
  int phase_bonus;
  int next_phase [8];
/*
  int bone_type [2] [NO_BONES];
  int bone_subtype [2] [NO_BONES];
  int bone_x [2] [NO_BONES];
  int bone_y [2] [NO_BONES];*/


};

struct bossstruct
{
  int fight;
  int index;

  int hp;
  int max_hp;
  int hp_bar_size;

  int phase;
  int phase2;
  int phase_count;
  int recycle;
  int recycle2;
  int recycle3;
  int recycle4;
  int status;
  int side;

  int beam;

  char end_of_stage;

};


struct optionstruct
{
 int sound_init; // if 0, sound isn't initialised at all. Changed in config file only.
 int sound_mode; // mono, stereo, reversed, off
 int run_vsync; // on or off
 int windowed;
 int sfx_volume; // sound fx volume; if 0 sound effects not played
 int ambience_volume; // if 0 ambience not played
 int joystick;
 int highscore [3];
 int joy_button [4];
 int joy_sensitivity;
 int init_joystick;
 int joy_analogue;
 int key_or_joy;
 int joy_stick;

 int stage_power [5] [2] [3];
 int stage_power_bar [5] [2] [3];
 int clear [5] [3];
 int finished [5] [3];
 int unlock [3];

 int previous_power;
 int previous_bar;
 int previous_wpixels;
 int current_power;
 int current_bar;
 int current_wpixels;


};

