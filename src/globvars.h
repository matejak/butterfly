// Global variables:
//  at the moment these are all defined in main.c.
//  but they have to be externed here so all modules with #include globvars.h can access them.

extern struct playerstruct player;

extern struct ebulletstruct ebullet [NO_EBULLETS];

extern struct pbulletstruct pbullet [NO_PBULLETS];

extern struct enemystruct enemy [NO_ENEMIES];

extern struct cloudstruct cloud [NO_CLOUDS];

extern unsigned char counter;

extern struct arenastruct arena;

extern struct eclassstruct eclass [NO_ENEMY_TYPES];

extern struct bossstruct boss;

extern struct optionstruct options;
