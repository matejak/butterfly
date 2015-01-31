
#include "config.h"

#include "allegro.h"

#include "globvars.h"

#include "stuff.h"
#include "enemy.h"
#include "pbullet.h"
#include "ebullet.h"
#include "display.h"
#include "cloud.h"

#include "palette.h"

#include <math.h>
#include <string.h>

#include "sound.h"

#define NO_TRACKS 3
#define TRACK_SIZE 10


enum
{
TRACK_WANDER,
TRACK_DARTER,
TRACK_STOPPER,
TRACK_SIDES,
TRACK_DIVER,
TRACK_DART_STOPPER,
TRACK_DRIFTER,
TRACK_CRUISER,
TRACK_BOSS,
TRACK_SLIDER,
TRACK_STROLLER,
TRACK_STROLLER2,

TRACK_GLIDER,
TRACK_MARCHER,
TRACK_ATTACKER,
TRACK_BEAMER,
TRACK_BOSS1,
TRACK_BOSS2,

TRACK_MB1



};

enum
{
DIST_RANDOM,
DIST_ALL_W,
DIST_ALL_B,
DIST_ALTERNATE_WB,
DIST_ALTERNATE_BW,
DIST_PART_WB,
DIST_PART_BW

};


struct trackstruct
{
  int type;
  int multiple;
  int count;
  int count2;
//  int next_track;
  int total;
  int x [TRACK_SIZE + 1];
  int y [TRACK_SIZE + 1];
  int reverse;
  int appear;

  int style;

  int x_target [MAX_TARGETS];
  int y_target [MAX_TARGETS];
  int target_time;

  int status [4];

};

int next_track;

struct trackstruct track [NO_TRACKS];


/*
struct overtrackstruct
{
  int subtrack [NO_TRACKS];
  int position;
};

struct overtrackstruct overtrack [OVERTRACK_SIZE];
*/


enum
{
T_EMPTY,
T_OVERTRACK_END,
T_CHANGE_MUSIC,
T_END_LEVEL,
T_PALETTE,

// remember to add to track_score!
T_FIGHTER,
T_DOUBLE,
T_DOME,
T_FIGHTER2,
T_FIGHTER3,
T_FIGHTER4,
T_MACHINE,
T_BIG,
T_BIG2,
T_DOUBLE2,
T_MACHINE2,
T_FIGHTER5,
T_FIGHTER6,
T_MEDIUM,
T_DOUBLE3,
T_BIG3,
T_RISER,
T_DOME2,

T_MACHINE3,
T_WIDE,
T_BIG4,
T_FIGHTER7,
T_FIGHTER8,
T_DOUBLE4,
T_DOME3,

T_BOSS1_1,
T_BOSS1_2,
T_BOSS1_3,
T_BOSS2_1,
T_BOSS2_2,
T_BOSS3_1,
T_BOSS3_2,
T_BOSS3_0,

T_END
};

/*
int track_score [T_END] [2] =
{
{0, 0}, //T_EMPTY,
{0, 0}, //T_OVERTRACK_END,
{0, 0}, //T_CHANGE_MUSIC,
{0, 0}, //T_END_LEVEL,
{0, 0}, //T_ESCORTS_0,
{0, 0}, //T_ESCORTS_1,
{0, 0}, //T_ESCORTS_2,
{0, 0}, //T_ESCORTS_3,
{4, ENEMY_SWOOPER1}, //T_SWOOPER1,
{4, ENEMY_DARTER1}, //T_DARTER1,
{1, ENEMY_STOPPER1}, //T_STOPPER1,
{3, ENEMY_DIVER1}, //T_DIVER1,
{1, ENEMY_CARRIER1}, //T_CARRIER1,
{2, ENEMY_POD1}, //T_POD1,
{0, 0}, //T_BOSS1,
{4, ENEMY_SWOOPER2}, //T_SWOOPER2,
{3, ENEMY_DARTER2}, //T_DARTER2,
{3, ENEMY_SMALL}, //T_SMALL,
{2, ENEMY_LARGER}, //T_LARGER,
{3, ENEMY_SMALL2}, //T_SMALL2,
{2, ENEMY_LARGER2}, //T_LARGER2,
{2, ENEMY_WINGS}, //T_WINGS,
{2, ENEMY_WINGS2}, //T_WINGS2,
{1, ENEMY_MEGA}, //T_MEGA,
{0, 0}, //T_BOSS3,
{3, ENEMY_DRIFTER1}, //T_DRIFTER1,
{3, ENEMY_BEAMER}, //T_BEAMER,
{6, ENEMY_RISER}, //T_RISER,
{1, ENEMY_CRUISER1}, //T_CRUISER1,
{1, ENEMY_CRUISER2}, //T_CRUISER2,
{1, ENEMY_CRUISER3}, //T_CRUISER3,
{1, ENEMY_CRUISER4}, //T_CRUISER4,
{1, ENEMY_CRUISER5}, //T_CRUISER5,
{1, ENEMY_DRIFTER2}, //T_DRIFTER2,
{2, ENEMY_SHOOTER}, //T_SHOOTER,
{4, ENEMY_HOOK}, //T_SHOOTER,
{1, ENEMY_WALKER}, //T_SHOOTER,
{4, ENEMY_DROPPER}, //T_SHOOTER,
{1, ENEMY_RING}, //T_SHOOTER,
{2, ENEMY_HUNTER}, //T_SHOOTER,
{1, ENEMY_SPINNER}, //T_SHOOTER,
{4, ENEMY_HOOK2}, //T_SHOOTER,
{0, 0}, //T_BOSS4,

//T_END

};
*/

int overtrack_pos;
int overtrack_pos2;

#define OT_AP 6
#define OT_NEXT 9

#define AP_LEFT -1
#define AP_RIGHT -2
#define AP_CENTRE -3
#define AP_NONE -4
#define AP_RANDOM -5


int overtrack [4] [91] [10] =
{
{
{T_EMPTY, T_EMPTY, T_EMPTY, 2, 2, 2, AP_NONE, AP_NONE, AP_NONE, 30}, // doesn't appear
//{T_CHANGE_MUSIC, OVERMUSIC_L1_2, 0, 0, 0},

//{T_BOSS1_3, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 420},

/*
{T_RISER, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 2420},
//{T_CHANGE_MUSIC, OVERMUSIC_L1_2, 0, 0, 0, 0, 0, 0, 0, 0},
{T_DOUBLE2, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 420},
{T_DOUBLE2, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 420},
//{T_CHANGE_MUSIC, OVERMUSIC_L1_2, 0, 0, 0, 0, 0, 0, 0, 0},


{T_DOUBLE3, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 420},
*/
/*
{T_DOUBLE2, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 220},
{T_DOUBLE2, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 220},
{T_DOUBLE2, T_DOUBLE2, T_DOUBLE2, 1, 100, 200, AP_LEFT, AP_CENTRE, AP_RIGHT, 220},*/


{T_PALETTE, PAL_BASE, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 2},
/*
//{T_END_LEVEL, T_EMPTY, T_EMPTY, 1, 1, 1, AP_NONE, AP_NONE, AP_NONE, 80},
{T_FIGHTER, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 220},

{T_PALETTE, PAL_L1_2, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 3},
{T_FIGHTER, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 220},

{T_PALETTE, PAL_L1_3, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 3},
{T_FIGHTER, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 220},

{T_PALETTE, PAL_L2_1, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 3},
{T_FIGHTER, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 220},

{T_PALETTE, PAL_L2_2, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 3},
{T_FIGHTER, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 220},
*/

{T_FIGHTER, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_DOUBLE, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_DOUBLE, T_FIGHTER, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 420},
{T_MACHINE, T_EMPTY, T_EMPTY, 1, 1, 2, AP_NONE, AP_NONE, AP_NONE, 130},
{T_DOUBLE, T_MACHINE, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 320},


{T_FIGHTER, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_DOUBLE, T_DOUBLE, T_EMPTY, 1, 1, 200, AP_LEFT, AP_RIGHT, AP_NONE, 320},
{T_FIGHTER, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_DOUBLE, T_DOUBLE, T_EMPTY, 1, 1, 200, AP_LEFT, AP_RIGHT, AP_NONE, 320},
{T_FIGHTER2, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_FIGHTER2, T_FIGHTER, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_DOME, T_FIGHTER, T_EMPTY, 1, 0, 0, AP_NONE, AP_NONE, AP_NONE, 230},
{T_FIGHTER2, T_FIGHTER, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_DOME, T_MACHINE, T_EMPTY, 1, 0, 0, AP_NONE, AP_NONE, AP_NONE, 230},
{T_DOUBLE, T_DOUBLE, T_EMPTY, 1, 1, 200, AP_LEFT, AP_RIGHT, AP_NONE, 320},
{T_FIGHTER2, T_FIGHTER, T_FIGHTER, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_FIGHTER2, T_MACHINE, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_DOME, T_DOME, T_FIGHTER2, 1, 1, 200, AP_LEFT, AP_RIGHT, AP_NONE, 320},
{T_DOUBLE, T_DOUBLE, T_EMPTY, 1, 1, 200, AP_LEFT, AP_RIGHT, AP_NONE, 320},
{T_FIGHTER, T_MACHINE, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_FIGHTER2, T_MACHINE, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 620},

{T_BOSS1_1, T_EMPTY, T_EMPTY, 1, 1, 2, AP_NONE, AP_NONE, AP_NONE, 2},

{T_PALETTE, PAL_L1_2, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 3},

{T_EMPTY, T_EMPTY, T_EMPTY, 1, 1, 2, AP_NONE, AP_NONE, AP_NONE, 180},

{T_FIGHTER2, T_FIGHTER, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_FIGHTER2, T_FIGHTER, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_FIGHTER3, T_FIGHTER2, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_FIGHTER3, T_MACHINE, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 420},
{T_BIG2, T_FIGHTER, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 420},
{T_FIGHTER3, T_MACHINE, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 320},
{T_DOUBLE, T_FIGHTER, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_FIGHTER3, T_FIGHTER2, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 320},
{T_DOUBLE, T_FIGHTER, T_MACHINE2, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_FIGHTER2, T_FIGHTER, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 320},
{T_BIG2, T_DOME, T_DOME, 1, 1, 2, AP_CENTRE, AP_LEFT, AP_RIGHT, 420},
{T_FIGHTER3, T_MACHINE, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 420},
{T_FIGHTER, T_MACHINE2, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_FIGHTER2, T_MACHINE2, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 320},
{T_DOUBLE, T_DOUBLE, T_EMPTY, 1, 1, 200, AP_LEFT, AP_RIGHT, AP_NONE, 320},
{T_BIG2, T_FIGHTER, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 620},

{T_BOSS1_2, T_EMPTY, T_EMPTY, 1, 1, 2, AP_RIGHT, AP_NONE, AP_NONE, 3},

{T_PALETTE, PAL_L1_3, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 3},

{T_EMPTY, T_EMPTY, T_EMPTY, 1, 1, 2, AP_NONE, AP_NONE, AP_NONE, 180},

{T_FIGHTER4, T_FIGHTER, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_DOUBLE, T_FIGHTER2, T_MACHINE2, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_BIG2, T_FIGHTER, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 320},
{T_FIGHTER4, T_DOUBLE2, T_EMPTY, 1, 100, 200, AP_NONE, AP_CENTRE, AP_NONE, 320},
{T_DOUBLE, T_MACHINE, T_MACHINE2, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 420},
{T_BIG2, T_DOME, T_DOME, 1, 160, 160, AP_CENTRE, AP_LEFT, AP_RIGHT, 420},
{T_FIGHTER2, T_FIGHTER3, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 320},
{T_FIGHTER, T_FIGHTER2, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_FIGHTER, T_FIGHTER4, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_DOUBLE2, T_MACHINE, T_MACHINE2, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 320},
{T_FIGHTER, T_FIGHTER2, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_FIGHTER, T_MACHINE, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_BIG2, T_FIGHTER4, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_DOUBLE2, T_MACHINE, T_EMPTY, 1, 100, 200, AP_CENTRE, AP_NONE, AP_NONE, 230},
{T_FIGHTER2, T_FIGHTER3, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_FIGHTER, T_FIGHTER2, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 230},
{T_FIGHTER, T_FIGHTER4, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 620},

{T_BOSS1_3, T_EMPTY, T_EMPTY, 1, 1, 2, AP_RIGHT, AP_NONE, AP_NONE, 30},

{T_END_LEVEL, T_EMPTY, T_EMPTY, 1, 1, 1, AP_NONE, AP_NONE, AP_NONE, 80},


},

// STAGE 2:

{

{T_EMPTY, T_EMPTY, T_EMPTY, 2, 2, 2, AP_NONE, AP_NONE, AP_NONE, 30}, // doesn't appear

{T_PALETTE, PAL_L2_1, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 2},

{T_DOUBLE2, T_DOUBLE2, T_EMPTY, 1, 1, 200, AP_LEFT, AP_RIGHT, AP_NONE, 250},
{T_FIGHTER5, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_FIGHTER4, T_FIGHTER5, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_DOUBLE2, T_DOUBLE2, T_BIG2, 200, 200, 1, AP_LEFT, AP_RIGHT, AP_CENTRE, 350},
{T_DOME2, T_FIGHTER5, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_FIGHTER, T_FIGHTER4, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 270},
{T_MACHINE2, T_MACHINE2, T_MACHINE2, 1, 1, 1, AP_LEFT, AP_RIGHT, AP_CENTRE, 250},
{T_MACHINE2, T_FIGHTER5, T_EMPTY, 1, 1, 200, AP_NONE, AP_NONE, AP_NONE, 320},
{T_FIGHTER5, T_BIG3, T_EMPTY, 1, 150, 1, AP_NONE, AP_CENTRE, AP_NONE, 420},
{T_RISER, T_EMPTY, T_EMPTY, 1, 1, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_DOUBLE2, T_DOUBLE2, T_RISER, 1, 1, 150, AP_LEFT, AP_RIGHT, AP_NONE, 300},
{T_MACHINE2, T_MACHINE2, T_MACHINE2, 1, 1, 1, AP_LEFT, AP_RIGHT, AP_CENTRE, 300},
{T_MACHINE2, T_FIGHTER5, T_EMPTY, 1, 1, 200, AP_NONE, AP_NONE, AP_NONE, 300},
{T_FIGHTER4, T_DOME2, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_FIGHTER2, T_FIGHTER3, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_BIG2, T_BIG3, T_EMPTY, 1, 1, 1, AP_LEFT, AP_RIGHT, AP_NONE, 300},
{T_BIG3, T_RISER, T_EMPTY, 1, 1, 1, AP_CENTRE, AP_NONE, AP_NONE, 300},
{T_RISER, T_EMPTY, T_EMPTY, 1, 1, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_MACHINE2, T_MACHINE2, T_MACHINE2, 1, 1, 1, AP_LEFT, AP_RIGHT, AP_CENTRE, 300},
{T_DOUBLE2, T_DOUBLE2, T_RISER, 1, 1, 150, AP_LEFT, AP_RIGHT, AP_NONE, 300},
{T_MACHINE2, T_MACHINE2, T_MACHINE2, 1, 1, 1, AP_LEFT, AP_RIGHT, AP_CENTRE, 300},
{T_MACHINE2, T_FIGHTER5, T_EMPTY, 1, 1, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_MEDIUM, T_EMPTY, T_EMPTY, 1, 1, 150, AP_NONE, AP_RIGHT, AP_NONE, 400},

{T_BOSS2_1, T_EMPTY, T_EMPTY, 1, 1, 2, AP_CENTRE, AP_NONE, AP_NONE, 60},

{T_PALETTE, PAL_L2_2, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 60},

{T_EMPTY, T_EMPTY, T_EMPTY, 1, 1, 2, AP_NONE, AP_NONE, AP_NONE, 50},

{T_DOME2, T_DOME2, T_DOME2, 1, 1, 1, AP_LEFT, AP_RIGHT, AP_CENTRE, 320},
{T_DOUBLE3, T_DOUBLE3, T_RISER, 1, 1, 150, AP_LEFT, AP_RIGHT, AP_NONE, 320},
{T_MACHINE2, T_FIGHTER6, T_EMPTY, 1, 1, 200, AP_NONE, AP_NONE, AP_NONE, 260},
{T_FIGHTER5, T_FIGHTER6, T_EMPTY, 1, 1, 200, AP_NONE, AP_NONE, AP_NONE, 260},
{T_MACHINE2, T_FIGHTER6, T_EMPTY, 1, 1, 200, AP_NONE, AP_NONE, AP_NONE, 260},
{T_DOUBLE3, T_EMPTY, T_EMPTY, 1, 1, 200, AP_CENTRE, AP_NONE, AP_NONE, 260},
{T_DOUBLE3, T_MEDIUM, T_EMPTY, 1, 100, 200, AP_CENTRE, AP_NONE, AP_NONE, 260},
{T_BIG3, T_RISER, T_EMPTY, 1, 1, 1, AP_CENTRE, AP_NONE, AP_NONE, 260},
{T_FIGHTER5, T_FIGHTER6, T_EMPTY, 1, 150, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_FIGHTER5, T_FIGHTER6, T_EMPTY, 1, 150, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_FIGHTER5, T_FIGHTER6, T_EMPTY, 1, 150, 200, AP_NONE, AP_NONE, AP_NONE, 255},
{T_DOUBLE3, T_DOUBLE3, T_BIG3, 1, 1, 100, AP_LEFT, AP_RIGHT, AP_CENTRE, 250},
{T_MACHINE2, T_FIGHTER6, T_EMPTY, 1, 1, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_FIGHTER5, T_FIGHTER6, T_EMPTY, 1, 1, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_MEDIUM, T_FIGHTER6, T_EMPTY, 1, 1, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_MEDIUM, T_EMPTY, T_EMPTY, 1, 1, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_DOUBLE3, T_EMPTY, T_EMPTY, 1, 1, 200, AP_CENTRE, AP_NONE, AP_NONE, 250},
{T_DOUBLE3, T_FIGHTER6, T_EMPTY, 1, 100, 200, AP_CENTRE, AP_NONE, AP_NONE, 250},
{T_BIG3, T_RISER, T_EMPTY, 1, 1, 1, AP_CENTRE, AP_NONE, AP_NONE, 520},

{T_BOSS2_2, T_EMPTY, T_EMPTY, 1, 1, 2, AP_CENTRE, AP_NONE, AP_NONE, 330},

{T_END_LEVEL, T_EMPTY, T_EMPTY, 1, 1, 1, AP_NONE, AP_NONE, AP_NONE, 80},

},

// Stage 3:

{

{T_EMPTY, T_EMPTY, T_EMPTY, 2, 2, 2, AP_NONE, AP_NONE, AP_NONE, 30}, // doesn't appear

{T_PALETTE, PAL_L3_1, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 2},

{T_FIGHTER7, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_FIGHTER7, T_FIGHTER7, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_DOUBLE3, T_MEDIUM, T_EMPTY, 1, 100, 200, AP_CENTRE, AP_NONE, AP_NONE, 350},
{T_DOME3, T_DOME3, T_DOME3, 1, 1, 1, AP_LEFT, AP_RIGHT, AP_CENTRE, 250},
{T_BIG3, T_RISER, T_EMPTY, 1, 1, 1, AP_CENTRE, AP_NONE, AP_NONE, 520},
{T_FIGHTER7, T_FIGHTER6, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_FIGHTER7, T_FIGHTER6, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_DOME3, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_DOME3, T_FIGHTER8, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_FIGHTER7, T_FIGHTER8, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_BIG4, T_DOUBLE3, T_DOUBLE3, 1, 61, 121, AP_LEFT, AP_RIGHT, AP_LEFT, 422},
{T_BIG4, T_FIGHTER8, T_FIGHTER8, 1, 61, 121, AP_RIGHT, AP_RIGHT, AP_LEFT, 322},
{T_DOME3, T_DOME3, T_DOME3, 1, 1, 1, AP_LEFT, AP_RIGHT, AP_CENTRE, 250},
{T_DOME3, T_FIGHTER8, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_FIGHTER8, T_FIGHTER8, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 550},

{T_BOSS3_0, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 320},

{T_FIGHTER7, T_FIGHTER8, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_BIG3, T_RISER, T_EMPTY, 1, 1, 1, AP_CENTRE, AP_NONE, AP_NONE, 520},
{T_FIGHTER7, T_FIGHTER8, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_MEDIUM, T_EMPTY, T_EMPTY, 1, 100, 200, AP_CENTRE, AP_NONE, AP_NONE, 250},
{T_BIG4, T_DOUBLE3, T_DOUBLE3, 1, 61, 121, AP_CENTRE, AP_RIGHT, AP_LEFT, 422},
{T_FIGHTER7, T_FIGHTER7, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_DOME3, T_FIGHTER8, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_FIGHTER7, T_FIGHTER6, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_FIGHTER7, T_FIGHTER6, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_BIG4, T_BIG4, T_DOME3, 1, 61, 121, AP_LEFT, AP_RIGHT, AP_CENTRE, 322},
{T_MACHINE3, T_MACHINE3, T_MACHINE3, 1, 1, 1, AP_LEFT, AP_RIGHT, AP_CENTRE, 290},
{T_MACHINE3, T_MACHINE3, T_MACHINE3, 1, 1, 1, AP_LEFT, AP_RIGHT, AP_CENTRE, 310},
{T_DOME3, T_DOME3, T_DOME3, 1, 1, 1, AP_LEFT, AP_RIGHT, AP_CENTRE, 250},
{T_MEDIUM, T_EMPTY, T_EMPTY, 1, 100, 200, AP_CENTRE, AP_NONE, AP_NONE, 250},
{T_FIGHTER7, T_FIGHTER6, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_FIGHTER7, T_FIGHTER6, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_DOUBLE4, T_EMPTY, T_EMPTY, 1, 1, 200, AP_CENTRE, AP_RIGHT, AP_NONE, 310},
{T_BIG4, T_BIG4, T_DOME3, 1, 61, 121, AP_LEFT, AP_RIGHT, AP_CENTRE, 422},
{T_DOUBLE4, T_DOUBLE4, T_EMPTY, 1, 1, 200, AP_LEFT, AP_RIGHT, AP_NONE, 610},

{T_BOSS3_1, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 420},

{T_PALETTE, PAL_L3_2, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 2},

{T_MACHINE3, T_FIGHTER7, T_EMPTY, 1, 100, 1, AP_NONE, AP_RIGHT, AP_CENTRE, 250},
{T_MACHINE3, T_FIGHTER6, T_EMPTY, 1, 100, 1, AP_NONE, AP_RIGHT, AP_CENTRE, 250},
{T_MACHINE3, T_FIGHTER8, T_EMPTY, 1, 100, 1, AP_NONE, AP_RIGHT, AP_CENTRE, 250},
{T_DOUBLE4, T_DOUBLE4, T_EMPTY, 1, 1, 200, AP_LEFT, AP_RIGHT, AP_NONE, 240},
{T_DOUBLE4, T_BIG4, T_EMPTY, 1, 1, 200, AP_CENTRE, AP_NONE, AP_NONE, 240},
{T_DOME3, T_DOME3, T_DOME3, 1, 1, 1, AP_LEFT, AP_RIGHT, AP_CENTRE, 250},
{T_DOME3, T_FIGHTER8, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_FIGHTER8, T_FIGHTER8, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_FIGHTER7, T_FIGHTER8, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_BIG3, T_RISER, T_EMPTY, 1, 1, 1, AP_CENTRE, AP_NONE, AP_NONE, 320},
{T_WIDE, T_EMPTY, T_EMPTY, 1, 1, 1, AP_CENTRE, AP_NONE, AP_NONE, 250},
{T_WIDE, T_WIDE, T_WIDE, 1, 61, 121, AP_LEFT, AP_RIGHT, AP_CENTRE, 122},
{T_FIGHTER8, T_FIGHTER8, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_FIGHTER7, T_FIGHTER8, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_WIDE, T_DOUBLE4, T_EMPTY, 1, 1, 1, AP_NONE, AP_NONE, AP_NONE, 150},
{T_FIGHTER7, T_FIGHTER8, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_WIDE, T_DOUBLE4, T_EMPTY, 1, 1, 1, AP_NONE, AP_NONE, AP_NONE, 150},
{T_FIGHTER7, T_FIGHTER6, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_WIDE, T_EMPTY, T_EMPTY, 1, 1, 1, AP_CENTRE, AP_NONE, AP_NONE, 150},
{T_MACHINE3, T_MACHINE3, T_EMPTY, 1, 100, 200, AP_LEFT, AP_RIGHT, AP_NONE, 250},
{T_DOUBLE4, T_DOUBLE4, T_EMPTY, 1, 1, 200, AP_LEFT, AP_RIGHT, AP_NONE, 310},
{T_FIGHTER7, T_FIGHTER6, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},
{T_WIDE, T_DOUBLE4, T_FIGHTER6, 1, 1, 1, AP_CENTRE, AP_NONE, AP_NONE, 150},
{T_MACHINE3, T_MACHINE3, T_DOUBLE4, 1, 100, 200, AP_LEFT, AP_RIGHT, AP_NONE, 630},

{T_BOSS3_2, T_EMPTY, T_EMPTY, 1, 100, 200, AP_NONE, AP_NONE, AP_NONE, 250},

{T_END_LEVEL, T_EMPTY, T_EMPTY, 1, 1, 1, AP_NONE, AP_NONE, AP_NONE, 80},

}

};



void spawn_enemy(int type, int pole);
int make_track(int t);
void run_tracks(void);

void make_group_track(int t, int type);
void set_dancer(int e, int t);
void set_hook(int e, int t);
void set_darter(int e, int t);
void set_stopper(int e, int t);
void set_diver(int e, int t);
void set_glider(int e, int t);
void set_attacker(int e, int t);
void set_dart_stopper(int e, int t, int m);
void set_drifter(int e, int t);
void set_cruiser(int e, int t);
void set_walker(int e, int t);
void give_escort(int e, int etype, int x1, int y1, int x, int y);
void set_stroller(int e, int t);
void set_stroller2(int e, int t);

void set_beamer(int e, int t);
void set_pauser(int e, int t);
void place_marchers(int t);

int create_part(int pe, int index, int type, int x, int y);
void set_boss(int e, int t);


void init_level(void)
{

 int i;

 for (i = 0; i < NO_TRACKS; i ++)
 {
   track[i].type = ENEMY_NONE;

 }

 overtrack_pos = 0;
 overtrack_pos2 = -1;
// boss.fight = 0;

 arena.new_level_sign = 100;
 arena.beam = 0;
 arena.old_beam = 0;
 arena.level_finished = 0;

 arena.underlay_position = 0;

 next_track = 50;

 arena.number = 0;

 switch(arena.level)
 {
  case 1:
   break;

 }

// init_mtracks(); sound!!! replace

// level_display_init();

}

void run_level(void)
{

     run_tracks();


}

void run_tracks(void)
{
 int t;
 int e;

//if (boss.fight)
// return;

 if (arena.level_finished > 0
  || arena.game_over > 0)
  return;

    if (boss.fight == 0)
    {
     next_track --;

     if (next_track <= 0)
     {
      for (t = 0; t < 3; t ++)
      {
       track[t].type = ENEMY_NONE;
       if (make_track(t) == 1)
        break;
      }
      next_track = overtrack [arena.level - 1] [overtrack_pos] [OT_NEXT]; // see also above
     }
    }

 for (t = 0; t < 3; t ++)
 {
     if (track[t].type == ENEMY_NONE || track[t].multiple == 0)
     {
       continue;
     }
     if (track[t].count <= 0)
     {
       if (track[t].multiple == 0)
       {
        continue;
       }
       while (track[t].count <= 0 && track[t].multiple > 0)
       {
       /*       if (track[t].style == TRACK_MARCHER)
       {
        place_marchers(t);
        track[t].multiple = 0;
        track[t].count = 0;
        track[t].type = ENEMY_NONE;
        continue;
       }*/
//       if (track[t].style == TRACK_BOSS1)
//        continue;
       e = create_enemy(track[t].type);
       if (e != -1)
       {
        enemy[e].x = track[t].x [track[t].multiple];
        enemy[e].y = track[t].y [track[t].multiple];
        enemy[e].angle = ANGLE_4;
        switch(eclass[enemy[e].type].ai)
        {
          default:
          case AI_STROLLER:
           set_stroller(e, t);
           break;
          case AI_STROLLER2:
           set_stroller2(e, t);
           break;
          case AI_DANCER:
           set_dancer(e, t);
           break;
          case AI_DARTER:
           set_darter(e, t);
           break;
          case AI_DART_STOPPER:
           set_dart_stopper(e, t, track[t].multiple);
           break;
          case AI_STOPPER:
           set_stopper(e, t);
           break;
          case AI_DIVER:
           set_diver(e, t);
           break;
          case AI_DRIFTER:
           set_drifter(e, t);
           break;
          case AI_CRUISER:
           set_cruiser(e, t);
           break;
          case AI_BOSS:
           set_boss(e, t);
           break;
          case AI_HOOK:
           set_hook(e, t);
           break;
        }
       }
       track[t].count = track[t].count2;
       track[t].multiple --;
       }
     } // count > 0. so -- it
     track[t].count --;

 }



}

int make_track(int t)
{

  int etype;

  track[t].total = 0;

  track[t].reverse = -1;

  int track_number = grand(3);

  if (t == 0 && grand(2) == 0)
   track_number = 3;

  int s_etype = 3;

  overtrack_pos2 ++;
  if (overtrack_pos2 > 2)
  {
   overtrack_pos2 = 0;
   overtrack_pos ++;
   if (overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2] == T_CHANGE_MUSIC)
   {
    change_music(overtrack [arena.level - 1] [overtrack_pos] [1]);
    overtrack_pos ++;
    overtrack_pos2 = -1;
    return 1;
   }
   if (overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2] == T_END_LEVEL)
   {
//    arena.level_finished = 300;
    overtrack_pos ++;
    overtrack_pos2 = -1;
    return 1;
   }
   if (overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2] == T_OVERTRACK_END)
    overtrack_pos = 0;
  }
  if (overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2] == T_EMPTY)
  {
   track[t].multiple = 0;
//   overtrack_pos ++;
//   overtrack_pos2 = -1;
   return 0;
  }
   if (overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2] == T_PALETTE)
   {
    if (arena.game_over <= 0)
    {
     arena.target_palette = overtrack [arena.level - 1] [overtrack_pos] [1];
     arena.shift_palette = 2;
    }
    overtrack_pos ++;
    overtrack_pos2 = -1;
    return 1;
   }

  s_etype = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2];

  track[t].appear = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + OT_AP];

  switch(s_etype)
  {
   case T_FIGHTER: etype = ENEMY_FIGHTER;
    track[t].type = etype;
    track[t].multiple = 4;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 20;
    track[t].style = TRACK_STROLLER;
    make_group_track(t, etype);
    break;
   case T_DOUBLE: etype = ENEMY_DOUBLE;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 20;
    track[t].style = TRACK_STOPPER;
    make_group_track(t, etype);
    break;
   case T_DOUBLE2: etype = ENEMY_DOUBLE2;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 20;
    track[t].style = TRACK_STOPPER;
    make_group_track(t, etype);
    break;
   case T_DOME: etype = ENEMY_DOME;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 20;
    track[t].style = TRACK_STOPPER;
    make_group_track(t, etype);
    break;
   case T_DOME3: etype = ENEMY_DOME3;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 20;
    track[t].style = TRACK_STOPPER;
    make_group_track(t, etype);
    break;
   case T_BIG: etype = ENEMY_BIG;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 20;
    track[t].style = TRACK_STOPPER;
    make_group_track(t, etype);
    break;
   case T_BIG2: etype = ENEMY_BIG2;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 20;
    track[t].style = TRACK_STOPPER;
    make_group_track(t, etype);
    break;
   case T_WIDE: etype = ENEMY_WIDE;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 20;
    track[t].style = TRACK_STOPPER;
    make_group_track(t, etype);
    break;
   case T_FIGHTER2: etype = ENEMY_FIGHTER2;
    track[t].type = etype;
    track[t].multiple = 3;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 25;
    track[t].style = TRACK_STROLLER;
    make_group_track(t, etype);
    break;
   case T_FIGHTER3: etype = ENEMY_FIGHTER3;
    track[t].type = etype;
    track[t].multiple = 3;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 20;
    track[t].style = TRACK_STROLLER;
    make_group_track(t, etype);
    break;
   case T_FIGHTER4: etype = ENEMY_FIGHTER4;
    track[t].type = etype;
    track[t].multiple = 5;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 15;
    track[t].style = TRACK_STROLLER;
    make_group_track(t, etype);
    break;
   case T_MACHINE: etype = ENEMY_MACHINE;
    track[t].type = etype;
    track[t].multiple = 3;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 15;
    track[t].style = TRACK_DARTER;
    make_group_track(t, etype);
    break;
   case T_DOME2: etype = ENEMY_DOME2;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 20;
    track[t].style = TRACK_STOPPER;
    make_group_track(t, etype);
    break;
   case T_BOSS1_1: etype = ENEMY_BOSS1_1;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 15;
    track[t].style = TRACK_BOSS;
    make_group_track(t, etype);
    break;
   case T_BOSS1_2: etype = ENEMY_BOSS1_2;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 15;
    track[t].style = TRACK_BOSS;
    make_group_track(t, etype);
    break;
   case T_BOSS1_3: etype = ENEMY_BOSS1_3;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 15;
    track[t].style = TRACK_BOSS;
    make_group_track(t, etype);
    break;
   case T_BOSS2_1: etype = ENEMY_BOSS2_1;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 15;
    track[t].style = TRACK_BOSS;
    make_group_track(t, etype);
    break;
   case T_BOSS2_2: etype = ENEMY_BOSS2_2;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 15;
    track[t].style = TRACK_BOSS;
    make_group_track(t, etype);
    break;
   case T_BOSS3_1: etype = ENEMY_BOSS3_1;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 15;
    track[t].style = TRACK_BOSS;
    make_group_track(t, etype);
    break;
   case T_BOSS3_2: etype = ENEMY_BOSS3_2;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 15;
    track[t].style = TRACK_BOSS;
    make_group_track(t, etype);
    break;
   case T_BOSS3_0: etype = ENEMY_BOSS3_0;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 15;
    track[t].style = TRACK_BOSS;
    make_group_track(t, etype);
    break;
   case T_MACHINE2: etype = ENEMY_MACHINE2;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 1;
    track[t].style = TRACK_DARTER;
    make_group_track(t, etype);
    break;
   case T_MACHINE3: etype = ENEMY_MACHINE3;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 1;
    track[t].style = TRACK_DARTER;
    make_group_track(t, etype);
    break;
   case T_FIGHTER5: etype = ENEMY_FIGHTER5;
    track[t].type = etype;
    track[t].multiple = 6;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 20;
    track[t].style = TRACK_STROLLER2;
    make_group_track(t, etype);
    break;
   case T_FIGHTER6: etype = ENEMY_FIGHTER6;
    track[t].type = etype;
    track[t].multiple = 4;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 5;
    track[t].style = TRACK_STROLLER;
    make_group_track(t, etype);
    break;
   case T_FIGHTER7: etype = ENEMY_FIGHTER7;
    track[t].type = etype;
    track[t].multiple = 6;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 20;
    track[t].style = TRACK_STROLLER;
    make_group_track(t, etype);
    break;
   case T_FIGHTER8: etype = ENEMY_FIGHTER8;
    track[t].type = etype;
    track[t].multiple = 2;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 5;
    track[t].style = TRACK_STROLLER;
    make_group_track(t, etype);
    break;
   case T_MEDIUM: etype = ENEMY_MEDIUM;
    track[t].type = etype;
    track[t].multiple = 4;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 1;
    track[t].style = TRACK_STOPPER;
    make_group_track(t, etype);
    break;
   case T_DOUBLE3: etype = ENEMY_DOUBLE3;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 20;
    track[t].style = TRACK_STOPPER;
    make_group_track(t, etype);
    break;
   case T_DOUBLE4: etype = ENEMY_DOUBLE4;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 20;
    track[t].style = TRACK_STOPPER;
    make_group_track(t, etype);
    break;
   case T_BIG3: etype = ENEMY_BIG3;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 20;
    track[t].style = TRACK_STOPPER;
    make_group_track(t, etype);
    break;
   case T_BIG4: etype = ENEMY_BIG4;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 20;
    track[t].style = TRACK_STOPPER;
    make_group_track(t, etype);
    break;
   case T_RISER: etype = ENEMY_RISER;
    track[t].type = etype;
    track[t].multiple = 5;
    track[t].count = overtrack [arena.level - 1] [overtrack_pos] [overtrack_pos2 + 3];
    track[t].count2 = 1;
    track[t].style = TRACK_STOPPER;
    make_group_track(t, etype);
    break;
/*   case T_BOSS1: etype = ENEMY_BOSS1;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = 15;
    track[t].count2 = track[t].count;
    track[t].delay = 80;
    track[t].style = TRACK_BOSS;
    make_group_track(t, etype);
    break;
   case T_CRUISER1: etype = ENEMY_CRUISER1;
    track[t].type = etype;
    track[t].multiple = 1;
    track[t].count = 15;
    track[t].count2 = track[t].count;
    track[t].delay = 80;
    track[t].style = TRACK_CRUISER;
    make_group_track(t, etype);
    break;
   case T_BEAMER: etype = ENEMY_BEAMER;
    track[t].type = etype;
    track[t].multiple = 3;
    track[t].count = 2;
    track[t].count2 = track[t].count;
    track[t].delay = 80;
    track[t].style = TRACK_STOPPER;
    make_group_track(t, etype);
    break;
   case T_DRIFTER1: etype = ENEMY_DRIFTER1;
    track[t].type = etype;
    track[t].multiple = 3;
    track[t].count = 25;
    track[t].count2 = track[t].count;
    track[t].delay = 80;
    track[t].style = TRACK_DRIFTER;
    make_group_track(t, etype);
    break;*/
  }

  //track[t].count = grand(overtrack [arena.level - 1] [overtrack_pos] [3 + t]);

 return 0;
}

void make_group_track(int t, int type)
{

 int a;//, b;

 int i;

 switch(track[t].style)
 {
  case TRACK_CRUISER:
   track[t].x [1] = RAND_X_IN;
   track[t].y [1] = -70000;
   break;
  case TRACK_DRIFTER:
   for (i = 1; i < track[t].multiple + 1; i ++)
   {
    switch(grand(5))
    {
     default:
     track[t].x [i] = RAND_X;
     track[t].y [i] = -30000;
     break;
     case 0:
     track[t].x [i] = -30000;
     track[t].y [i] = RAND_Y_IN - 100000;
     break;
     case 1:
     track[t].x [i] = 670000;
     track[t].y [i] = RAND_Y_IN - 100000;
     break;
    }
   }
   break;
  case TRACK_STOPPER:
/*   if (type == ENEMY_POD1
       || type == ENEMY_HUNTER)
   {
//    track[t].x [0] = 100000;
    track[t].x [1] = 100000 + grand(150000);
    track[t].x [2] = 540000 - grand(150000);
    track[t].y [1] = -40000;
    track[t].y [2] = -40000;
    break;
   }*/
/*   if (type == ENEMY_BEAMER
    || type == ENEMY_SHOOTER)
   {
    track[t].x [1] = 100000 + grand(100000);
    track[t].x [2] = 540000 - grand(100000);
    track[t].x [3] = 270000 + grand(100000);
    track[t].y [1] = 520000;
    track[t].y [2] = 520000;
    track[t].y [3] = 520000;
    break;
   }*/
   if (track[t].appear > 0)
    track[t].x [1] = track[t].appear;
     else
     {
      switch(track[t].appear)
      {
       case AP_NONE:
       case AP_RANDOM: track[t].x [1] = RAND_X_IN; break;
       case AP_LEFT: track[t].x [1] = 50000 + grand(120000); break;
       case AP_RIGHT: track[t].x [1] = 564400 - grand(120000); break;
       case AP_CENTRE: track[t].x [1] = 257200 + grand(100000); break;
      }
     }

//   track[t].x [1] = RAND_X_IN;
   track[t].y [1] = -50000;
   switch(type)
   {
    case ENEMY_DOME:
    case ENEMY_BIG:
     track[t].y [1] = -70000;
     break;
    case ENEMY_BIG2:
    case ENEMY_BIG3:
     track[t].y [1] = -120000;
     break;
    case ENEMY_MEDIUM:
     track[t].y [1] = 644400;
     track[t].y [2] = 644400 - eclass[ENEMY_MEDIUM].speed1 * 1;
     track[t].y [3] = 644400 - eclass[ENEMY_MEDIUM].speed1 * 2;
     track[t].y [4] = 644400 - eclass[ENEMY_MEDIUM].speed1 * 3;
     track[t].status [0] = 20 + grand(120);
     track[t].status [1] = 20 + grand(120);
     break;
    case ENEMY_RISER:
     track[t].y [1] = 644400;
     track[t].y [2] = track[t].y [1] + 50000 + grand(30000);
     track[t].y [3] = track[t].y [2] + 50000 + grand(30000);
     track[t].y [4] = track[t].y [3] + 50000 + grand(30000);
     track[t].y [5] = track[t].y [4] + 50000 + grand(30000);
//     track[t].status [0] = 20 + grand(120);
//     track[t].status [1] = 20 + grand(120);
     break;
    case ENEMY_BIG4:
     track[t].y [1] = 100000 + grand(200000);
     track[t].x [1] = -75000;
     if (track[t].appear == AP_RIGHT || (track[t].appear == AP_NONE && coin()))
     {
      track[t].x [1] = 614400 + 75000;
     }
     break;
   }
   break;
  case TRACK_STROLLER2:
    track[t].x [0] = RAND_X;
//    track[t].x [0] = (RAND_X / 3) + X_MAX / 3 * 2;
    track[t].y [0] = -22000;
    track[t].x_target [0] = RAND_X / 5 + X_MAX / 8;
    track[t].y_target [0] = 300000 + grand(50000);//(RAND_Y / 2) + Y_MAX / 5;
    track[t].x_target [1] = (RAND_X / 3) + (X_MAX / 3) * 2;
    track[t].y_target [1] = 150000 + grand(40000); //(RAND_Y / 2) + Y_MAX / 5;
    track[t].x_target [2] = RAND_X / 5 + X_MAX / 8;
    track[t].y_target [2] = (RAND_Y / 2) + Y_MAX / 2;
    track[t].x_target [3] = -1;
    track[t].y_target [3] = -1;
   break;
  case TRACK_STROLLER:
   break;
  default:
  case TRACK_WANDER:
   track[t].x [0] = RAND_X;
   track[t].y [0] = -30000;
   for (i = 1; i < TRACK_SIZE; i ++)
   {
    track[t].x [i] = track[t].x [0];
    track[t].y [i] = track[t].y [0];
   }
   switch(grand(3))
   {
    case 0: // R-L-R
    default:
    track[t].x [0] = RAND_X;
    track[t].x [0] = (RAND_X / 3) + X_MAX / 3 * 2;
    track[t].x_target [0] = RAND_X / 5 + X_MAX / 8;
    track[t].y_target [0] = (RAND_Y / 2) + Y_MAX / 5;
    track[t].x_target [1] = (RAND_X / 3) + (X_MAX / 3) * 2;
    track[t].y_target [1] = (RAND_Y / 2) + Y_MAX / 5;
    track[t].x_target [2] = -1;
    track[t].y_target [2] = -1;
    if (grand(2) == 0)
    {
     track[t].x [0] = X_MAX - track[t].x [0];
     track[t].x_target [0] = X_MAX - track[t].x_target [0];
     track[t].x_target [1] = X_MAX - track[t].x_target [1];
    }
    break;
   }
   track[t].target_time = 250;
   break;

  case TRACK_SLIDER:
   track[t].x [0] = -30000;
   track[t].y [0] = RAND_Y;
   for (i = 1; i < TRACK_SIZE; i ++)
   {
    track[t].x [i] = track[t].x [0];
    track[t].y [i] = track[t].y [0];
   }

    track[t].x_target [0] = 400000 + grand(200000);
    track[t].y_target [0] = RAND_Y;

    track[t].x_target [1] = 40000 + grand(200000);
    track[t].y_target [1] = RAND_Y;

    track[t].x_target [2] = 400000 + grand(200000);
    track[t].y_target [2] = RAND_Y;

    track[t].x_target [3] = -1;
    track[t].y_target [3] = -1;
    track[t].target_time = 250;
   break;

   case TRACK_DARTER:
   track[t].x [0] = grand(100000) * -1;
   track[t].y [0] = -40000 - grand(130000);

   for (i = 1; i < track[t].multiple + 1; i ++)
   {
    track[t].x [i] = track [t].x [i - 1] + 130000 + grand(110000);
    track[t].y [i] = -40000 - grand(130000);//track[t].y [0];
   }

   if (track[t].appear > 0)
   {
    track[t].x [1] = track[t].appear;
   }
     else
     {
      switch(track[t].appear)
      {
       case AP_NONE: break;
       case AP_RANDOM: track[t].x [1] = RAND_X_IN; break;
       case AP_LEFT: track[t].x [1] = 50000 + grand(120000); break;
       case AP_RIGHT: track[t].x [1] = 564400 - grand(120000); break;
       case AP_CENTRE: track[t].x [1] = 257200 + grand(100000); break;
      }
     }


    break;

   case TRACK_DIVER:
   track[t].x [0] = grand(50000) * -1;
   track[t].y [0] = -30000 - grand(140000);

   for (i = 1; i < track[t].multiple + 1; i ++)
   {
    track[t].x [i] = track [t].x [i - 1] + 120000 + grand(35000);
    track[t].y [i] = -30000 - 60000 * i;//grand(140000);//track[t].y [0];
   }




    break;

   case TRACK_DART_STOPPER:
    break;
  case TRACK_SIDES:
   track[t].reverse = grand(2);
   a = grand(2);
   track[t].x [0] = X_MIN + 40000 + grand(30000);
   if (a == 1)
    track[t].x [0] = X_MAX - 40000 - grand(30000);

   track[t].y [0] = -30000;

   for (i = 1; i < track[t].multiple + 1; i ++)
   {
    if (i % 2 == 1)
     track[t].x [i] = X_MAX - track[t].x [0];
      else
       track[t].x [i] = track[t].x [0];
    track[t].y [i] = track[t].y [0];

   }
   track[t].x_target [0] = X_MIN + RAND_X / 4;
   if (a == 1)
    track[t].x_target [0] = X_MAX - RAND_X / 4;
   track[t].y_target [0] = Y_MIN + RAND_Y / 3;
   track[t].x_target [1] = X_MIDDLE;
   track[t].y_target [1] = Y_MIDDLE; // rand?

   track[t].x_target [2] = X_MAX - RAND_X / 4;
   if (a == 1)
    track[t].x_target [2] = X_MIN + RAND_X / 4;
   track[t].y_target [2] = 500000;

   track[t].x_target [3] = -1;
   track[t].y_target [3] = -1;

   track[t].target_time = 280;
   break;

 }

}

/*int get_max_stage_score(void)
{
    int i, j;
    int s = 0;
    int ttype;
    int etype;

    for (i = 0; i < 71; i ++)
    {
        for (j = 0; j < 3; j ++)
        {
         ttype = overtrack [arena.level - 1] [i] [j];
         if (ttype == T_END_LEVEL)
         {
          i = 80;
          break;
         }
//         etype = track_score [ttype] [1];
//         if (track_score [ttype] [0] == 0)
//          continue;
//         s += eclass [etype].xp * track_score [ttype] [0];
        }

    }

    return s;

}

*/

void give_escort(int e, int etype, int x1, int y1, int x, int y)
{


       int e2 = create_enemy(etype);
       if (e2 != -1)
       {
        enemy[e2].x = x;
        enemy[e2].y = y;
        enemy[e2].angle = ANGLE_4;

        enemy[e2].persist = 1;

        enemy [e2].target = 0;
        enemy [e2].escorting = e;
        enemy[e2].pulse1 = x1;
        enemy[e2].pulse2 = y1;

        enemy [e2].target_time = eclass[etype].speed3;
        enemy[e2].phase = 0;

        enemy [e2].angle1 = ANGLE_4;
        enemy [e2].angle = ANGLE_4;

        enemy[e2].x_target [0] = enemy[e].x;
        enemy[e2].y_target [0] = enemy[e].y;
        enemy[e2].angle = grand(ANGLE_1);
        enemy [e2].x_speed = 0;//xpart(enemy[e2].angle, 3000);
        enemy [e2].y_speed = 0;//ypart(enemy[e2].angle, 3000);
       }





}

void set_boss(int e, int t)
{
 enemy[e].persist = 1;


 enemy[e].x = 307200;
 enemy[e].y = -90000;
 enemy [e].x_speed = 0;
 enemy [e].y_speed = 2000;
 enemy [e].angle1 = ANGLE_4;
 enemy [e].angle = ANGLE_4;

 boss.fight = 1;
 boss.index = e;
 boss.phase = 0;
 boss.phase2 = 0;
 boss.phase_count = 0;
 boss.recycle = 50;
 boss.recycle2 = 50;
 boss.recycle3 = 50;
 boss.recycle4 = 50;
 boss.status = 0;
 boss.end_of_stage = 1;
 boss.hp_bar_size = 0;
 boss.beam = 0;

 switch(enemy[e].type)
 {
     case ENEMY_BOSS1_1:
      boss.end_of_stage = 0;
      boss.hp = 70000;// + arena.difficulty * 15000;
      boss.max_hp = boss.hp;
      boss.phase_count = 100;
/*      boss.out_sides = 0;
      boss.out_sides2 = 0;
      boss.out_up = 0;
      boss.out_out_up = 0;
      boss.t_angle [0] = ANGLE_2;
      boss.t_angle_turning [0] = 0;
      boss.t_angle [1] = 0;
      boss.t_angle_turning [1] = 0;
      boss.t_status = 0;
      boss.t_recycle [0] = 10;*/
      break;
     case ENEMY_BOSS1_2:
      boss.end_of_stage = 0;
      boss.hp = 90000;// + arena.difficulty * 15000;
      boss.max_hp = boss.hp;
      boss.phase_count = 100;
      boss.recycle3 = 550;
      break;
     case ENEMY_BOSS1_3:
      boss.end_of_stage = 1;
      boss.hp = 100000;// + arena.difficulty * 15000;
      boss.max_hp = boss.hp;
      boss.phase_count = 100;
      boss.recycle3 = 350;
      break;
     case ENEMY_BOSS2_1:
      boss.end_of_stage = 0;
      boss.hp = 120000;// + arena.difficulty * 15000;
      boss.max_hp = boss.hp;
      boss.phase_count = 100;
      boss.recycle = 50;
      boss.recycle2 = 70;
      boss.recycle3 = 90;
      boss.recycle4 = 110;

      enemy[e].y_speed = -4000;
      enemy[e].y = 700000;
      break;
     case ENEMY_BOSS2_2:
      boss.end_of_stage = 1;
      boss.hp = 100000;//120000;// + arena.difficulty * 15000;
      boss.max_hp = boss.hp;
      boss.phase_count = 100;
      boss.recycle = 50;
      boss.recycle2 = 70;
      boss.recycle3 = 9;
      boss.recycle4 = 110;
      enemy[e].burst = 9;
      enemy[e].status = ANGLE_32;
      break;
     case ENEMY_BOSS3_1:
      boss.end_of_stage = 0;
      boss.hp = 200000;// + arena.difficulty * 15000;
      boss.max_hp = boss.hp;
      boss.phase_count = 100;
      enemy[e].burst = 4;
      break;
     case ENEMY_BOSS3_2:
      boss.end_of_stage = 1;
      boss.hp = 250000;// + arena.difficulty * 15000;
      boss.max_hp = boss.hp;
      boss.phase_count = 100;
      enemy[e].burst = 4;
      break;
     case ENEMY_BOSS3_0:
      boss.end_of_stage = 0;
      boss.hp = 150000;
      boss.max_hp = boss.hp;
      boss.phase_count = 100;
      enemy[e].burst = 4;
      break;
/*     case ENEMY_BOSS2:
      boss.hp = 60000;// + arena.difficulty * 16000;
//      boss.hp = 3000;
      boss.max_hp = boss.hp;
      boss.hp_bar_size = 0;

      enemy[e].y = 550000;
      enemy [e].x_speed = 0;
      enemy [e].y_speed = -7000;

      boss.phase_count = 60;
      boss.out_sides = 0;
      boss.out_sides2 = 0;
      boss.t_status = grand(4);
      boss.t_status_change = 1;

      for (i = 0; i < 4; i ++)
      {
       boss.t_angle [i] = -ANGLE_4;
       boss.t_angle_turning [i] = 0;
       boss.t_recycle [i] = 10;
       boss.t_beam [i] = 0;
      }
      break;
     case ENEMY_BOSS3:
      boss.hp = 50000;// + arena.difficulty * 15000;
      //boss.hp = 3000;
      boss.max_hp = boss.hp;
      boss.hp_bar_size = 0;

      enemy[e].y = -250000;
      enemy [e].x_speed = 0;
      enemy [e].y_speed = 4000;

      boss.phase_count = 75;
      boss.phase2 = 3;


      boss.petal_angle [0] = grand(ANGLE_1);
      boss.petal_angle [1] = grand(ANGLE_1);
      boss.petal_angle [2] = grand(ANGLE_1);
      boss.petal_speed [0] = 4;
      boss.petal_speed [1] = 8;
      boss.petal_speed [2] = -6;
      boss.petal_out [0] = 90;
      boss.petal_out [1] = 140;
      boss.petal_out [2] = 190;

      boss.next_small = 50;

//      boss.out_sides = 0;
//      boss.out_sides2 = 0;
//      boss.t_status = grand(4);
//      boss.t_status_change = 1;
      break;
     case ENEMY_BOSS4:
      boss.hp = 70000;// + arena.difficulty * 18000;
//    boss.hp = 3000;
      boss.max_hp = boss.hp;
      boss.hp_bar_size = 0;
      enemy[e].y = -250000;
      boss.phase_count = 5000;
      boss.spin_size = 0;
      boss.spin_rad = 0;
      boss.spin_angle = 0;
      boss.spin_size2 = 0;
      boss.spin_rad2 = 0;
      boss.spin_angle2 = 0;
      boss.spin_size3 = 0;
      boss.spin_rad3 = 0;
      boss.spin_counter ++;
      boss.sides_recycle = 30;
      boss.outer_fire_type = 1;
      boss.outer_fire_count = 10;
      boss.recycle = 200;
      break;
*/
 }

}



void set_hook(int e, int t)
{
 int i;

 enemy[e].persist = 1;
 enemy[e].angle1 = 0;

 if (track[t].multiple & 1)
 {
  enemy[e].angle1 = ANGLE_2;
  enemy[e].x = 670000;
  for (i = 0; i < MAX_TARGETS; i ++)
  {
    enemy [e].x_target [i] = X_MAX - track[t].x_target [i];
    if (track[t].x_target [i] == -1)
     enemy [e].x_target [i] = -1;
    enemy [e].y_target [i] = track[t].y_target [i];
  }
 }
  else
  {
   for (i = 0; i < MAX_TARGETS; i ++)
   {
    enemy [e].x_target [i] = track[t].x_target [i];
      if (track[t].x_target [i] == -1)
       enemy [e].x_target [i] = -1;
      enemy [e].y_target [i] = track[t].y_target [i];
   }
  }

 enemy [e].target_time = track[t].target_time;
 enemy [e].max_target_time = track[t].target_time;
 enemy [e].target = 0;
 enemy[e].angle1_turning = 0;
 enemy[e].angle1_turning_count = 0;
 enemy[e].recycle = 5 + grand(10);

}



void set_stroller(int e, int t)
{
 int i;

 enemy[e].persist = 1;
 enemy[e].angle1 = ANGLE_4;

   for (i = 0; i < MAX_TARGETS; i ++)
   {
      enemy [e].x_target [i] = X_MIN + track[t].x_target [i];
      if (track[t].x_target [i] == -1)
       enemy [e].x_target [i] = -1;
      enemy [e].y_target [i] = Y_MIN + track[t].y_target [i];
   }

// come from top:
 if (coin())
 {
  enemy[e].x = RAND_X;
  enemy[e].y = -30000;
 }
  else
  {
   enemy[e].x = -30000;
   if (coin())
   {
    enemy[e].x = X_MAX + 30000;
   }
   enemy[e].y = 30000 + grand(200000);
  }

 enemy[e].x_target [0] = RAND_X;
 enemy[e].y_target [0] = 30000 + grand(160000);

 enemy[e].x_target [1] = RAND_X;
 enemy[e].y_target [1] = 150000 + grand(160000);

 enemy[e].x_target [2] = RAND_X;
 enemy[e].y_target [2] = 300000 + grand(160000);

 enemy[e].x_target [3] = -1;

 enemy[e].target_time = 150;
 enemy[e].max_target_time = 150;
 enemy[e].target = 0;
 enemy[e].angle1_turning = 0;
 enemy[e].angle1_turning_count = 0;
 enemy[e].recycle = 5 + grand(10);

 switch(enemy[e].type)
 {
   case ENEMY_FIGHTER:
   case ENEMY_FIGHTER7:
    enemy[e].recycle = 10 + grand(30);
    break;
   case ENEMY_FIGHTER3:
    enemy[e].recycle = 20 + grand(30);
    break;
   case ENEMY_FIGHTER4:
    enemy[e].recycle = 30 + grand(20);
    break;
   case ENEMY_FIGHTER2:
   case ENEMY_FIGHTER8:
    enemy[e].recycle = 20 + grand(40);
    enemy[e].burst = 3 + arena.difficulty * 2;
    break;
   case ENEMY_FIGHTER6:
    enemy[e].recycle = 50 + grand(40);
    break;
 }

}


void set_stroller2(int e, int t)
{
 int i;

 enemy[e].persist = 1;
 enemy[e].angle1 = ANGLE_4;


 if (track[t].multiple & 1)
 {
  enemy[e].angle1 = ANGLE_4;
  enemy[e].x = track[t].x [0];
  enemy[e].y = -25000;
  for (i = 0; i < MAX_TARGETS; i ++)
  {
    enemy [e].x_target [i] = 614400 - track[t].x_target [i];
    if (track[t].x_target [i] == -1)
     enemy [e].x_target [i] = -1;
    enemy [e].y_target [i] = track[t].y_target [i];
  }
 }
  else
  {
   enemy[e].angle1 = ANGLE_4;
   enemy[e].x = 614400 - track[t].x [0];
   enemy[e].y = -25000;
   for (i = 0; i < MAX_TARGETS; i ++)
   {
    enemy [e].x_target [i] = track[t].x_target [i];
      if (track[t].x_target [i] == -1)
       enemy [e].x_target [i] = -1;
      enemy [e].y_target [i] = track[t].y_target [i];
   }
  }

/*   for (i = 0; i < MAX_TARGETS; i ++)
   {
      if (siding)
       enemy [e].x_target [i] = X_MIN + track[t].x_target [i];
        else
         enemy [e].x_target [i] = X_MAX - track[t].x_target [i];
      if (track[t].x_target [i] == -1)
       enemy [e].x_target [i] = -1;
      enemy [e].y_target [i] = Y_MIN + track[t].y_target [i];
   }*/

/*
// come from top:
 if (coin())
 {
  enemy[e].x = RAND_X;
  enemy[e].y = -30000;
 }
  else
  {
   enemy[e].x = -30000;
   if (coin())
   {
    enemy[e].x = X_MAX + 30000;
   }
   enemy[e].y = 30000 + grand(200000);
  }

 enemy[e].x_target [0] = RAND_X;
 enemy[e].y_target [0] = 30000 + grand(160000);

 enemy[e].x_target [1] = RAND_X;
 enemy[e].y_target [1] = 150000 + grand(160000);

 enemy[e].x_target [2] = RAND_X;
 enemy[e].y_target [2] = 300000 + grand(160000);
*/
 enemy[e].x_target [3] = -1;

 enemy[e].target_time = 150;
 enemy[e].max_target_time = 150;
 enemy[e].target = 0;
 enemy[e].angle1_turning = 0;
 enemy[e].angle1_turning_count = 0;
 enemy[e].recycle = 5 + grand(10);

 switch(enemy[e].type)
 {
   case ENEMY_FIGHTER5:
    enemy[e].recycle = 15 + track[t].multiple * 20;
    enemy[e].burst = 2 + arena.difficulty * 2;
    break;
 }

}


void set_dancer(int e, int t)
{
 int i;

 enemy[e].persist = 1;
 enemy[e].angle1 = ANGLE_4;

 if (track[t].reverse != -1 && track[t].multiple % 2 == track[t].reverse)
 {
  for (i = 0; i < MAX_TARGETS; i ++)
  {
    enemy [e].x_target [i] = X_MAX - track[t].x_target [i];
    if (track[t].x_target [i] == -1)
     enemy [e].x_target [i] = -1;
    enemy [e].y_target [i] = Y_MIN + track[t].y_target [i];
  }
 }
  else
  {
   for (i = 0; i < MAX_TARGETS; i ++)
   {
      enemy [e].x_target [i] = X_MIN + track[t].x_target [i];
      if (track[t].x_target [i] == -1)
       enemy [e].x_target [i] = -1;
      enemy [e].y_target [i] = Y_MIN + track[t].y_target [i];
   }
  }

 enemy [e].target_time = track[t].target_time;
 enemy [e].max_target_time = track[t].target_time;
 enemy [e].target = 0;
 enemy[e].angle1_turning = 0;
 enemy[e].angle1_turning_count = 0;
 enemy[e].recycle = 5 + grand(10);

 switch(enemy[e].type)
 {
   case ENEMY_FIGHTER:
    enemy[e].recycle = 30 + grand(40);
    break;
 }

}

void set_darter(int e, int t)
{

 enemy[e].persist = 1;

 enemy [e].target = 0;

 enemy [e].target_time = 50 + grand(70);
 enemy[e].phase = 0;
 enemy[e].phase2 = 0;

 enemy [e].x_speed = 0;
 enemy [e].y_speed = eclass[enemy[e].type].speed1;
 enemy [e].angle1 = ANGLE_4;
 enemy [e].angle = ANGLE_4;
}

void set_dart_stopper(int e, int t, int m)
{

 enemy[e].persist = 1;

 enemy [e].target = 0;

 enemy [e].target_time = 30 + grand(60);
 enemy[e].phase = 0;
 enemy[e].phase2 = 0;

 enemy [e].x_speed = 0;
 enemy [e].y_speed = 0;



 enemy[e].x = -50000;

 if (m % 2 == 0)
 {
  enemy[e].x = 690000;
  enemy[e].status *= -1;
 }

 enemy[e].y = RAND_Y;

 enemy[e].x_target [0] = RAND_X;
 enemy[e].y_target [0] = RAND_Y;
 enemy[e].x_target [1] = RAND_X_IN;
 enemy[e].y_target [1] = RAND_Y_IN;


 enemy[e].angle = radians_to_angle(atan2(enemy[e].y_target [0] - enemy[e].y, enemy[e].x_target [0] - enemy[e].x));

 switch(enemy[e].type)
 {

/*   case ENEMY_LARGER:
    enemy[e].petal_angle [0] = enemy[e].angle + ANGLE_2 - ANGLE_4;
    enemy[e].petal_angle [1] = enemy[e].angle + ANGLE_2 - ANGLE_8;
    enemy[e].petal_angle [2] = enemy[e].angle + ANGLE_2 + ANGLE_8;
    enemy[e].petal_angle [3] = enemy[e].angle + ANGLE_2 + ANGLE_4;
    break;
   case ENEMY_LARGER2:
    enemy[e].petal_angle [0] = enemy[e].angle + ANGLE_2 - ANGLE_8;
    enemy[e].petal_angle [1] = enemy[e].angle + ANGLE_2;
    enemy[e].petal_angle [2] = enemy[e].angle + ANGLE_2 + ANGLE_8;

    enemy[e].petal_angle [3] = enemy[e].angle + ANGLE_2 - ANGLE_16;
    enemy[e].petal_angle [4] = enemy[e].angle + ANGLE_2;
    enemy[e].petal_angle [5] = enemy[e].angle + ANGLE_2 + ANGLE_16;
    break;*/
 }


}

void set_drifter(int e, int t)
{
 enemy[e].persist = 1;

 enemy [e].target = 0;

 enemy [e].target_time = eclass[enemy[e].type].speed3;
 enemy[e].phase = 0;

 enemy [e].angle1 = ANGLE_4;
 enemy [e].angle = ANGLE_4;

 enemy[e].x_target [0] = RAND_X_IN;
 enemy[e].y_target [0] = RAND_Y_IN;
 enemy[e].angle = radians_to_angle(atan2(enemy [e].y_target [0] - enemy [e].y, enemy [e].x_target [0] - enemy [e].x));
 enemy [e].x_speed = xpart(enemy[e].angle, 3000);
 enemy [e].y_speed = ypart(enemy[e].angle, 3000);

 switch(enemy[e].type)
 {
/*  case ENEMY_DRIFTER2:
   give_escort(e, ENEMY_ESCORT, -50000, 0, enemy[e].x, enemy[e].y);
   give_escort(e, ENEMY_ESCORT, 50000, 0, enemy[e].x, enemy[e].y);
   break;
  case ENEMY_RING:
   enemy[e].angle2 = pos_or_neg(7 + grand(5));
   break;*/


 }

}



void set_stopper(int e, int t)
{
// enemy [e].target_time = track[t].target_time;
 enemy[e].persist = 1;

 enemy [e].target = 0;

 enemy [e].target_time = 60 + grand(60); // may be overridden below
// enemy [e].max_target_time = track[t].target_time;
// enemy [e].target = 0;
 enemy[e].phase = 0;
 enemy[e].phase2 = 0;

 enemy [e].x_speed = 0;
 enemy [e].y_speed = eclass[enemy[e].type].speed1;
 enemy [e].angle1 = ANGLE_4;
 enemy [e].angle = ANGLE_4;

 switch(enemy[e].type)
 {
   case ENEMY_BIG4:
    enemy [e].y_speed = 0;
    enemy [e].x_speed = eclass[enemy[e].type].speed1;
    if (enemy[e].x > 300000)
     enemy[e].x_speed *= -1;
    enemy[e].recycle = enemy[e].target_time;
    break;
   case ENEMY_DOUBLE:
    enemy[e].target_time = 35 + grand(50);
    enemy[e].recycle = enemy[e].target_time;
    break;
   case ENEMY_DOME:
   case ENEMY_BIG:
   case ENEMY_WIDE:
    enemy[e].target_time = 45 + grand(80);
    enemy[e].recycle = enemy[e].target_time;
    break;
   case ENEMY_DOME2:
   case ENEMY_DOME3:
    enemy[e].target_time = 195 + grand(40);
    enemy[e].recycle = enemy[e].target_time;
    break;
   case ENEMY_BIG2:
   case ENEMY_BIG3:
    enemy[e].target_time = 255 + grand(50);
    enemy[e].recycle = enemy[e].target_time;
    break;
   case ENEMY_MEDIUM:
    enemy[e].recycle = 80;
    if (track[t].multiple == 1 || track[t].multiple == 4)
     enemy[e].target_time = track[t].status [0];
      else
       enemy[e].target_time = track[t].status [1];
//    enemy[e].target_time = 60;
    enemy[e].x = 00000 + track[t].multiple * 122880;
    break;
   case ENEMY_RISER:
    enemy[e].x = RAND_X;
    enemy[e].target_time = 70;
    enemy[e].angle = grand(ANGLE_1);
    enemy[e].status = pos_or_neg(ANGLE_32);
    enemy[e].burst = 7;
    enemy[e].recycle = 5;
    break;
 }
}



void set_cruiser(int e, int t)
{

 enemy[e].persist = 1;

 enemy [e].target = 0;

 enemy[e].phase = 0;
 enemy[e].phase2 = 0;

 enemy [e].x_speed = 0;
 enemy [e].y_speed = eclass[enemy[e].type].speed1;
 enemy [e].angle1 = ANGLE_4;
 enemy [e].angle = ANGLE_4;

 switch(enemy[e].type)
 {
 }

 if (arena.level == 2)
 {
/*
  if (arena.cruiser_escorts == 2)
  {
   give_escort(e, ENEMY_ESCORT, -40000, 30000, enemy[e].x, enemy[e].y);
   give_escort(e, ENEMY_ESCORT, 40000, 30000, enemy[e].x, enemy[e].y);
  }
  if (arena.cruiser_escorts == 3)
  {
   give_escort(e, ENEMY_ESCORT, -50000, 40000, enemy[e].x, enemy[e].y);
   give_escort(e, ENEMY_ESCORT, 50000, 40000, enemy[e].x, enemy[e].y);
   give_escort(e, ENEMY_ESCORT, 0, -60000, enemy[e].x, enemy[e].y);
  }
  if (arena.cruiser_escorts == 1)
  {
   give_escort(e, ENEMY_ESCORT, 0, 0, enemy[e].x, enemy[e].y);
  }*/
 }


}


int create_part(int pe, int index, int type, int x, int y)
{
 int e = create_enemy(type);
 if (e == -1)
  return -1;

 enemy[e].parent = pe;
 enemy[e].part_index = index;
 enemy[pe].part [index] = e;
 enemy[e].x = x;
 enemy[e].y = y;
 enemy[e].x_speed = 0;
 enemy[e].y_speed = 0;

 return e;
}


void set_diver(int e, int t)
{

 enemy[e].persist = 1;

 enemy [e].target = 0;

 enemy [e].target_time = 65;// + grand(10);
 enemy[e].phase = 0;
 enemy[e].phase2 = 0;

 enemy [e].x_speed = 0;
 if (enemy[e].x < 180000 + grand(280000))
  enemy[e].x_speed = 900 + grand(200);
   else
    enemy[e].x_speed = -900 - grand(200);
 enemy [e].y_speed = 6000;
 enemy [e].angle1 = ANGLE_4;
 enemy [e].angle = ANGLE_4;
}





void set_glider(int e, int t)
{
// int i;
 enemy[e].target = 0;
 enemy[e].x_target [0] = track[t].x_target [0];
 enemy[e].y_target [0] = track[t].y_target [0];

 enemy[e].angle1 = radians_to_angle(atan2(enemy [e].y_target [0] - enemy [e].y, enemy [e].x_target [0] - enemy [e].x));

 int speed = 5000; //eclass[enemy [e].type].speed1;

 enemy[e].x_speed = xpart(enemy [e].angle1, speed);
 enemy[e].y_speed = ypart(enemy [e].angle1, speed);

}

void set_attacker(int e, int t)
{
// int i;

 enemy [e].angle1 = ANGLE_4;

 enemy [e].x_speed = 0;
 enemy [e].y_speed = 3000;
 enemy [e].target_time = grand(30) + 10;
//   track[t].target_time = grand(30) + 30;

}

void set_pauser(int e, int t)
{

 enemy [e].angle1 = ANGLE_4;

 enemy [e].x_speed = 0;
 enemy [e].y_speed = 3000;
 enemy [e].target_time = 30;//grand(30) + 10;

}


void set_beamer(int e, int t)
{

 enemy [e].angle1 = ANGLE_4;

 enemy [e].x_speed = 0;
 enemy [e].y_speed = 2000;
 enemy [e].target_time = grand(30) + 50;
 enemy [e].target = 0;

}


void place_marchers(int t)
{



}

void stage_finished(void)
{

 float proportion = ((float) arena.stage_score / (float) arena.max_stage_score) * 100;

 if (proportion > 99 && proportion < 100)
  proportion = 99; // just in case

 arena.destruction_rate = proportion; // truncates to int

 arena.stage_rank = RANK_C;

 if (arena.destruction_rate > 70)
  arena.stage_rank = RANK_B;

 if (arena.destruction_rate > 90)
  arena.stage_rank = RANK_A;

 if (arena.destruction_rate > 95)
  arena.stage_rank = RANK_S;

 if (arena.destruction_rate == 100)
  arena.stage_rank = RANK_P;

 char diff_string [20];

 strcpy(diff_string, "Stage_normal");

 if (arena.difficulty == 1)
  strcpy(diff_string, "Stage_hard");
 if (arena.difficulty == 2)
  strcpy(diff_string, "Stage_punishment");

 if (arena.level == 3)
 {
  if (options.unlock [arena.difficulty] == 0)
   arena.unlocked = 1; // signals to display a message in display.c
  options.unlock [arena.difficulty] = 1;
  set_config_int(diff_string, "unlock", 1);
  options.finished [player.type - 1] [arena.difficulty] = 1;
// finished just means that you've finished level 3. clear means you started at level 1. Finished
//  unlocks the Chaos Squid for that difficulty level; clear gives you a 'clear' sign on the
//  level select menu.
  switch(player.type - 1)
  {
   case 0: set_config_int(diff_string, "finished0", 1); break;
   case 1: set_config_int(diff_string, "finished1", 1); break;
   case 2: set_config_int(diff_string, "finished2", 1); break;
   case 3: set_config_int(diff_string, "finished3", 1); break;
   case 4: set_config_int(diff_string, "finished4", 1); break;
  }
  if (arena.from_start == 1)
  {
   options.clear [player.type - 1] [arena.difficulty] = 1;
   arena.cleared = 1;
   switch(player.type - 1)
   {
    case 0: set_config_int(diff_string, "clear0", 1); break;
    case 1: set_config_int(diff_string, "clear1", 1); break;
    case 2: set_config_int(diff_string, "clear2", 1); break;
    case 3: set_config_int(diff_string, "clear3", 1); break;
    case 4: set_config_int(diff_string, "clear4", 1); break;
   }
  }
  return; // don't need to set energy levels
 }

// so let's unlock things if necessary:


 int lowest = 0;

 if ((player.wlevel [1] < player.wlevel [0]) ||
      (player.wlevel [1] == player.wlevel [0] && player.wxp [1] < player.wxp [0]))
     lowest = 1;

 if (((player.wlevel [2] < player.wlevel [0]) ||
      (player.wlevel [2] == player.wlevel [0] && player.wxp [2] < player.wxp [0]))
      &&
      ((player.wlevel [2] < player.wlevel [1]) ||
      (player.wlevel [2] == player.wlevel [1] && player.wxp [2] < player.wxp [1])))
     lowest = 2;

 options.previous_power = options.stage_power [player.type - 1] [arena.level - 1] [arena.difficulty];
 options.previous_bar = options.stage_power_bar [player.type - 1] [arena.level - 1] [arena.difficulty];

#define LEVEL_CALC3 (3500 + (1100 * options.stage_power [player.type - 1] [arena.level - 1] [arena.difficulty]))

 options.previous_wpixels = ((float) (options.stage_power_bar [player.type - 1] [arena.level - 1] [arena.difficulty] * 20) / LEVEL_CALC3);
 if (options.previous_wpixels >= 20 || options.previous_wpixels < 0)
  options.previous_wpixels = 19;

 options.current_power = player.wlevel [lowest];
 options.current_bar = player.wxp [lowest];
 options.current_wpixels = player.wpixels [lowest];

 if ((player.wlevel [lowest] > options.stage_power [player.type - 1] [arena.level - 1] [arena.difficulty])
  || (player.wlevel [lowest] == options.stage_power [player.type - 1] [arena.level - 1] [arena.difficulty]
   && player.wxp [lowest] > options.stage_power_bar [player.type - 1] [arena.level - 1] [arena.difficulty]))
   {
    options.stage_power [player.type - 1] [arena.level - 1] [arena.difficulty] = player.wlevel [lowest];
    options.stage_power_bar [player.type - 1] [arena.level - 1] [arena.difficulty] = player.wxp [lowest];

// I'm too lazy to do this properly... stupid strings

 set_config_int(diff_string, "power0_0", options.stage_power [0] [0] [arena.difficulty]);
 set_config_int(diff_string, "power1_0", options.stage_power [1] [0] [arena.difficulty]);
 set_config_int(diff_string, "power2_0", options.stage_power [2] [0] [arena.difficulty]);
 set_config_int(diff_string, "power3_0", options.stage_power [3] [0] [arena.difficulty]);
 set_config_int(diff_string, "power4_0", options.stage_power [4] [0] [arena.difficulty]);

 set_config_int(diff_string, "power0_1", options.stage_power [0] [1] [arena.difficulty]);
 set_config_int(diff_string, "power1_1", options.stage_power [1] [1] [arena.difficulty]);
 set_config_int(diff_string, "power2_1", options.stage_power [2] [1] [arena.difficulty]);
 set_config_int(diff_string, "power3_1", options.stage_power [3] [1] [arena.difficulty]);
 set_config_int(diff_string, "power4_1", options.stage_power [4] [1] [arena.difficulty]);

 set_config_int(diff_string, "power_bar0_0", options.stage_power_bar [0] [0] [arena.difficulty]);
 set_config_int(diff_string, "power_bar1_0", options.stage_power_bar [1] [0] [arena.difficulty]);
 set_config_int(diff_string, "power_bar2_0", options.stage_power_bar [2] [0] [arena.difficulty]);
 set_config_int(diff_string, "power_bar3_0", options.stage_power_bar [3] [0] [arena.difficulty]);
 set_config_int(diff_string, "power_bar4_0", options.stage_power_bar [4] [0] [arena.difficulty]);

 set_config_int(diff_string, "power_bar0_1", options.stage_power_bar [0] [1] [arena.difficulty]);
 set_config_int(diff_string, "power_bar1_1", options.stage_power_bar [1] [1] [arena.difficulty]);
 set_config_int(diff_string, "power_bar2_1", options.stage_power_bar [2] [1] [arena.difficulty]);
 set_config_int(diff_string, "power_bar3_1", options.stage_power_bar [3] [1] [arena.difficulty]);
 set_config_int(diff_string, "power_bar4_1", options.stage_power_bar [4] [1] [arena.difficulty]);

   }


}

