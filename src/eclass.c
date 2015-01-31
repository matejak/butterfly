#include "config.h"


struct eclassstruct eclass [NO_ENEMY_TYPES] =
{
{
0, // int max_hp
0, // int xsize
0, // int ysize
0, // int xoffset
0, // int yoffset
 {
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}
 },
0, // ai
0, // speed1
0, // speed2
0, // speed3
0, // priority
0
}, // NONE
{
400, // int max_hp
13000, // int xsize
15000, // int ysize
0,
0,
 {
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}

 },
AI_STROLLER,
4000, // speed1
10, // speed2
0, // speed3
2, // priority
10 // score
}, // FIGHTER
{
5000, // int max_hp
32000, // int xsize
36000, // int ysize
0,
0,
 {
  {-32, -36, -12, 36},
  {12, -37, 33, 36},
  {-13, -31, 13, 8},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}

 },
AI_STOPPER,
3072, // speed1
80, // speed2
40, // speed3
0, // priority
50 // score
}, // DOUBLE

{
7000, // int max_hp
34000, // int xsize
58000, // int ysize
0,
22000,
 {
  {-34, -6, 34, 31},
  {-23, -35, 23, -6},
  {-32, 21, -12, 81},
  {10, 26, 32, 81},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}


 },
AI_STOPPER,
3072, // speed1
70, // speed2
40, // speed3
0, // priority
70 // score
}, // DOME

{
700, // int max_hp
13000, // int xsize
17000, // int ysize
0,
0,
 {
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}
 },
AI_STROLLER,
3000, // speed1
7, // speed2
0, // speed3
2, // priority
15 // score
}, // FIGHTER2

{
900, // int max_hp
13000, // int xsize
17000, // int ysize
0,
0,
 {
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}
 },
AI_STROLLER,
2500, // speed1
5, // speed2
0, // speed3
2, // priority
15 // score
}, // FIGHTER3

{
400, // int max_hp
19000, // int xsize
14000, // int ysize
0,
3000,
 {
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}
 },
AI_STROLLER,
5500, // speed1
12, // speed2
0, // speed3
2, // priority
15 // score
}, // FIGHTER4

{
2000, // int max_hp
22000, // int xsize
19000, // int ysize
0,
-10000,
 {
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}
 },
AI_DARTER,
3500, // speed1 - entering speed
300, // speed2 - accel
120, // speed3 - time spent open
1, // priority
25 // score
}, // MACHINE

{
10000, // int max_hp
34000, // int xsize
58000, // int ysize
0,
22000,
 {
  {-34, -6, 34, 31},
  {-23, -35, 23, -6},
  {-32, 21, -12, 81},
  {10, 26, 32, 81},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}

 },
AI_STOPPER,
3072, // speed1
70, // speed2
40, // speed3
0, // priority
70 // score
}, // BIG


{
12000, // int max_hp
72000, // int xsize
65000, // int ysize
0,
14000,
 {
  {-47, -50, 47, 71},
  {-19, 40, 19, 81},
  {-73, 40, -46, 77},
  {46, 40, 73, 77},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}
/*
  {-34, -6, 34, 31},
  {-23, -35, 23, -6},
  {-32, 21, -12, 81},
  {10, 26, 32, 81}*/

 },
AI_STOPPER,
1024, // speed1
10, // speed2
10, // speed3
0, // priority
70 // score
}, // BIG2

{
8000, // int max_hp
72000, // int xsize
46000, // int ysize
0,
9000,
 {
  {-72, -26, 72, 9},
  {-51, -24, 51, 23},
  {-23, -37, -7, 55},
  {7, -37, 23, 55},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}

 },
AI_STOPPER,
2048, // speed1
400, // speed2
40, // speed3
0, // priority
80 // score
}, // DOUBLE2

{
600, // int max_hp
22000, // int xsize
17000, // int ysize
0,
0,
 {
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}
 },
AI_STROLLER2,
250, //400, // speed1
7, //6, // speed2
960, //940, // speed3
2, // priority
15 // score
}, // FIGHTER5

{
500, // int max_hp
18000, // int xsize
16000, // int ysize
0,
0,
 {
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}
 },
AI_STROLLER,
4000, //400, // speed1
5, //6, // speed2
0, //940, // speed3
2, // priority
15 // score
}, // FIGHTER6

{
3000, // int max_hp
20000, // int xsize
26000, // int ysize
0,
-9000,
 {
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}

 },
AI_STOPPER,
-4096, // speed1
130, // speed2
40, // speed3 - time spent opening
1, // priority
30 // score
}, // MEDIUM


{
12000, // int max_hp
82000, // int xsize
40000, // int ysize
0,
0,
 {
  {-81, -11, 81, 19},
  {-51, -22, 51, 25},
  {-31, -40, -2, 40},
  {2, -40, 31, 40},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}

 },
AI_STOPPER,
2048, // speed1
400, // speed2
40, // speed3
0, // priority
90 // score
}, // DOUBLE3


{
16000, // int max_hp
90000, // int xsize
52000, // int ysize
0,
-2000,
 {
  {-90, -50, 90, -2},
  {-48, -3, -14, 49},
  {14, -3, 48, 49},
  {-15, -3, 15, 14},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}

/*    {-47, -50, 47, 71},
  {-19, 40, 19, 81},
  {-73, 40, -46, 77},
  {46, 40, 73, 77}*/
/*
  {-34, -6, 34, 31},
  {-23, -35, 23, -6},
  {-32, 21, -12, 81},
  {10, 26, 32, 81}*/

 },
AI_STOPPER,
1024, // speed1
10, // speed2
10, // speed3
0, // priority
90 // score
}, // BIG3

{
1000, // int max_hp
18000, // int xsize
24000, // int ysize
0,
14000,
 {
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}

 },
AI_STOPPER,
-5120, // speed1
150, // speed2
40, // speed3 - time spent opening
2, // priority
20 // score
}, // RISER


{
10000, // int max_hp
39000, // int xsize
50000, // int ysize
0,
-24000,
 {
  {-40, -24, 40, 15},
  {-28, -73, 28, 23},
  {-1, -3, 48, 49},
  {-1, -3, 15, 14},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}


 },
AI_STOPPER,
1024, // speed1
10, // speed2
10, // speed3
0, // priority
40 // score
}, // DOME2


{
4200, // int max_hp
32000, // int xsize
28000, // int ysize
0,
12000,
 {
  {-14, -16, 14, 25},
  {-25, 6, 25, 24},
  {-35, 20, 35, 41},
  {0, 0, 0, 0},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}

 },
AI_DARTER,
2600, // speed1 - entering speed
310, // speed2 - accel
120, // speed3 - time spent open
1, // priority
30 // score
}, // MACHINE3


{
12000, // int max_hp
75000, // int xsize
26000, // int ysize
0,
-1000,
 {
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}
 },
AI_STOPPER,
3096, // speed1
210, // speed2
100, // speed3
0, // priority
60 // score
}, // WIDE


{
18000, // int max_hp
60000, // int xsize
49000, // int ysize
0,
-1000,
 {
  {-60, -49, 60, -11},
  {-43, -18, 43, 32},
  {-25, 0, 25, 41},
  {-13, 0, 13, 49},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}
 },
AI_STOPPER,
2048, // speed1
160, // speed2
100, // speed3
0, // priority
90 // score
}, // BIG4



{
800, // int max_hp
21000, // int xsize
13000, // int ysize
0,
0,
 {
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}
 },
AI_STROLLER,
3000, //400, // speed1
4, //6, // speed2
0, //940, // speed3
2, // priority
20 // score
}, // FIGHTER7


{
500, // int max_hp
13000, // int xsize
12000, // int ysize
0,
0,
 {
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}
 },
AI_STROLLER,
5000, //400, // speed1
8, //6, // speed2
0, //940, // speed3
2, // priority
15 // score
}, // FIGHTER8


{
15000, // int max_hp
96000, // int xsize
53000, // int ysize
0,
0,
 {
  {-96, -9, 96, 44},
  {-56, -35, 56, 53},
  {-39, -53, -13, 0},
  {13, -53, 39, 0},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}

 },
AI_STOPPER,
2048, // speed1
400, // speed2
40, // speed3
0, // priority
90 // score
}, // DOUBLE3



{
10000, // int max_hp
41000, // int xsize
49000, // int ysize
0,
0,
 {
  {-41, -49, 41, 16},
  {-29, 0, -4, 49},
  {4, 0, 29, 49},
  {-1, 0, 0, 0},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}
 },
AI_STOPPER,
1024, // speed1
10, // speed2
10, // speed3
0, // priority
40 // score
}, // DOME3





/*

**********************************************************************************************

BOSSES

**********************************************************************************************

*/




{
1, // int max_hp
147000, // int xsize
66000, // int ysize
0,
9000,
 {
  {-147, -37, 147, 3},
  {-122, -55, 122, 27},
  {-72, 0, 72, 59},
  {-35, 0, 35, 76},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}

 },
AI_BOSS,
0, // speed1
0, // speed2
0, // speed3
0, // priority
0 // score
}, // BOSS1_1


{
1, // int max_hp
147000, // int xsize
66000, // int ysize
0,
9000,
 {
  {-147, -37, 147, 3},
  {-122, -55, 122, 40},
  {-72, 0, 72, 59},
  {-21, 0, 21, 66},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}

 },
AI_BOSS,
0, // speed1
0, // speed2
0, // speed3
0, // priority
0 // score
}, // BOSS1_2


{
1, // int max_hp
147000, // int xsize
66000, // int ysize
0,
9000,
 {
  {-147, -57, 147, 46},
  {-129, 0, -32, 76},
  {32, 0, 129, 72},
  {-16, 0, 16, 36},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}

 },
AI_BOSS,
0, // speed1
0, // speed2
0, // speed3
0, // priority
0 // score
}, // BOSS1_3

{
1, // int max_hp
160000, // int xsize
109000, // int ysize
0,
0,
 {
  {-103, -81, 103, 50},
  {-142, -55, 142, 87},
  {-161, 20, 161, 62},
  {-52, -109, 52, 109},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}

 },
AI_BOSS,
0, // speed1
0, // speed2
0, // speed3
0, // priority
0 // score
}, // BOSS2_1

{
999999, // int max_hp
160000, // int xsize
109000, // int ysize
0,
0,
 {
  {-103, -81, 103, 50},
  {-142, -55, 142, 87},
  {-161, 20, 161, 62},
  {-52, -109, 52, 109},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}

 },
AI_BOSS2_1_DEAD,
0, // speed1
0, // speed2
0, // speed3
0, // priority
0 // score
}, // BOSS2_1_DEAD

{
1, // int max_hp
53000, // int xsize
34000, // int ysize
0,
3000,
 {
  {-28, -31, 28, 37},
  {-53, -10, 53, 10},
  {-45, -20, 45, 20},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}

 },
AI_BOSS,
0, // speed1
0, // speed2
0, // speed3
0, // priority
0 // score
}, // BOSS2_2


{
1, // int max_hp
147000, // int xsize
85000, // int ysize
0,
0,
 {
  {-77, -85, 77, 25},
  {-147, -28, -80, 85},
  {80, -28, 147, 85},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}

 },
AI_BOSS,
0, // speed1
0, // speed2
0, // speed3
0, // priority
0 // score
}, // BOSS3_1


{
1, // int max_hp
145000, // int xsize
86000, // int ysize
0,
-9000,
 {
  {-111, -95, -7, 55},
  {7, -95, 111, 55},
  {-144, -70, -10, 6},
  {10, -70, 144, 6},
  {-30, -28, 30, 21},
  {-46, 0, -7, 78},
  {7, 0, 46, 78},

 },
AI_BOSS,
0, // speed1
0, // speed2
0, // speed3
0, // priority
0 // score
}, // BOSS3_2

{
1, // int max_hp
128000, // int xsize
63000, // int ysize
0,
-30000,
 {
  {-118, -92, -35, 31},
  {35, -92, 118, 31},
  {-127, -44, 127, -22},
  {-52, -40, 52, -3},
  {-11, -11, 9, 11},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}

 },
AI_BOSS,
0, // speed1
0, // speed2
0, // speed3
0, // priority
0 // score
}, // BOSS3_0



{
3200, // int max_hp
42000, // int xsize
21000, // int ysize
0,
0,
 {
  {-22, -19, 22, 23},
  {-45, 1, 45, 23},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1},
  {-1, -1, -1, -1}
 },
AI_DARTER,
2500, // speed1 - entering speed
280, // speed2 - accel
120, // speed3 - time spent open
1, // priority
30 // score
}, // MACHINE2


/*

{
300, // int max_hp
25000, // int xsize
25000, // int ysize
0,
4000,
AI_DARTER,
6000, // speed1
10, // speed2
0, // speed3
1, // priority
0,
20 // xp
}, // DARTER1
{
3800, // int max_hp
45000, // int xsize
75000, // int ysize
0,
-30000,
AI_STOPPER,
2000, // speed1
150, // speed2
60, // speed3
0, // priority
0,
80 // xp

}, // STOPPER1
{
200, // int max_hp
25000, // int xsize
21000, // int ysize
0,
-7000,
AI_DIVER,
6000, // speed1
10, // speed2
0, // speed3
1, // priority
0,
20 // xp
}, // DIVER1
{
7000, // int max_hp
68000, // int xsize
80000, // int ysize
0,
-5000,
AI_STOPPER,
1700, // speed1
520, // speed2
40, // speed3
0, // priority
0,
100 // xp

}, // CARRIER1
{
800, // int max_hp
16000, // int xsize
16000, // int ysize
0,
3000,
AI_DARTER,
8000, // speed1
10, // speed2
0, // speed3
0, // priority
0,
30 // xp
}, // CARRIER1_PART
{
4000, // int max_hp
57000, // int xsize
52000, // int ysize
0,
-13000,
AI_STOPPER,
3500, // speed1
580, // speed2
100, // speed3
0, // priority
0,
50 // xp
}, // POD1
{
50, // int max_hp
20000, // int xsize
20000, // int ysize
0,
0,
AI_SWOOPER,
6000, // speed1
14, // speed2
0, // speed3
2, // priority
0, // score
15 // xp
}, // SWOOPER2
{
400, // int max_hp
30000, // int xsize
28000, // int ysize
0,
0,
AI_DARTER,
8000, // speed1
15, // speed2
0, // speed3
1, // priority
0,
30 // xp
}, // DARTER2

{
5000, // int max_hp
110000, // int xsize
75000, // int ysize
0,
0,
AI_BOSS,
0, // speed1
0, // speed2
0, // speed3
0, // priority
0,
300 // xp
}, // BOSS1

{
2500, // int max_hp
20000, // int xsize
20000, // int ysize
0,
0,
AI_BOSS,
0, // speed1
0, // speed2
0, // speed3
0, // priority
0,
0 // xp
}, // BOSS1_PART


{
50, // int max_hp
22000, // int xsize
16000, // int ysize
0,
0,
AI_DRIFTER,
170, // speed1 - move
960, // speed2 - drag
600, // speed3 - before retreats
2, // priority
0,
10 // xp
}, // L2 - DRIFTER1

{
1500, // int max_hp
38000, // int xsize
25000, // int ysize
0,
-4000,
AI_STOPPER,
-6000, // speed1
410, // speed2
20, // speed3
1, // priority
0,
50 // xp
}, // L2 - BEAMER

{
80, // int max_hp
21000, // int xsize
20000, // int ysize
0,
5000,
AI_STOPPER,
-3000, // speed1
260, // speed2
20, // speed3
2, // priority
0,
20 // xp
}, // L2 - RISER

{
4800, // int max_hp
50000, // int xsize
62500, // int ysize
0,
0,
AI_CRUISER,
1200, // speed1
0, // speed2
0, // speed3
0, // priority
0,
80 // xp
}, // L2 - CRUISER1

{
4000, // int max_hp
50000, // int xsize
32500, // int ysize
0,
0,
AI_CRUISER,
1200, // speed1
0, // speed2
0, // speed3
0, // priority
0,
50 // xp
}, // L2 - CRUISER2

{
6000, // int max_hp
50000, // int xsize
62500, // int ysize
0,
0,
AI_CRUISER,
1200, // speed1
0, // speed2
0, // speed3
0, // priority
0,
100 // xp
}, // L2 - CRUISER3

{
4500, // int max_hp
45000, // int xsize
42500, // int ysize
0,
-5000,
AI_CRUISER,
1200, // speed1
0, // speed2
0, // speed3
0, // priority
0,
80 // xp
}, // L2 - CRUISER4

{
3500, // int max_hp
50000, // int xsize
62500, // int ysize
0,
0,
AI_CRUISER,
1200, // speed1
0, // speed2
0, // speed3
0, // priority
0,
150 // xp
}, // L2 - CRUISER5 - not implemented

{
250, // int max_hp
25000, // int xsize
25000, // int ysize
0,
8000,
AI_DRIFTER,
130, // speed1 - move
970, // speed2 - drag
700, // speed3 - before retreats
1, // priority
0,
20 // xp
}, // L2 - DRIFTER2

{
50, // int max_hp
23000, // int xsize
16000, // int ysize
0,
0,
AI_DRIFTER,
290, // speed1 - move
930, // speed2 - drag
700, // speed3 - before retreats
2, // priority
0,
10 // xp
}, // L2 - ESCORT


{
1500, // int max_hp
38000, // int xsize
25000, // int ysize
0,
-4000,
AI_STOPPER,
-6000, // speed1
210, // speed2
20, // speed3
1, // priority
0,
50 // xp
}, // L2 - SHOOTER

{
5000, // int max_hp
105000, // int xsize
55000, // int ysize
0,
5000,
AI_BOSS,
0, // speed1
0, // speed2
0, // speed3
0, // priority
0,
400 // xp
}, // BOSS2




{
50, // int max_hp
15000, // int xsize
15000, // int ysize
0,
0,
AI_SWOOPER,
4500, // speed1
10, // speed2
0, // speed3
2, // priority
0, // score
10 // xp
}, // L3 - SMALL

{
500, // int max_hp
19000, // int xsize
19000, // int ysize
0,
-4000,
AI_DART_STOPPER,
400, // speed1
15, // speed2
150, // speed3
1, // priority
0,
25 // xp
}, // L3 - LARGER

{
50, // int max_hp
15000, // int xsize
15000, // int ysize
0,
0,
AI_SWOOPER,
5500, // speed1
14, // speed2
0, // speed3
2, // priority
0, // score
15 // xp
}, // L3 - SMALL2

{
700, // int max_hp
19000, // int xsize
19000, // int ysize
0,
-4000,
AI_DART_STOPPER,
500, // speed1
18, // speed2
250, // speed3
1, // priority
0,
35 // xp
}, // L3 - LARGER2


{
2900, // int max_hp
22000, // int xsize
21000, // int ysize
0,
-4000,
AI_STOPPER,
-6000, // speed1
160, // speed2
40, // speed3
0, // priority
0,
70 // xp
}, // L3 - WINGS

{
3300, // int max_hp
22000, // int xsize
21000, // int ysize
0,
-4000,
AI_STOPPER,
-7000, // speed1
160, // speed2
40, // speed3
0, // priority
0,
80 // xp
}, // L3 - WINGS2

{
9000, // int max_hp
40000, // int xsize
38000, // int ysize
0,
-6000,
AI_STOPPER,
4000, // speed1
540, // speed2
40, // speed3
0, // priority
0,
120 // xp
}, // L3 - MEGA

{
5000, // int max_hp
40000, // int xsize
38000, // int ysize
0,
0,
AI_BOSS,
0, // speed1
0, // speed2
0, // speed3
0, // priority
0,
500 // xp
}, // BOSS3

{
80, // int max_hp
16000, // int xsize
17000, // int ysize
0,
0,
AI_HOOK,
4500, // speed1
15, // speed2
0, // speed3
2, // priority
0, // score
10 // xp
}, // HOOK
{
5800, // int max_hp
76000, // int xsize
61000, // int ysize
0,
-5000,
AI_WALK,
1500, // speed1
0, // speed2
0, // speed3
0, // priority
0,
90 // xp

}, // WALKER

{
250, // int max_hp
14000, // int xsize
21000, // int ysize
0,
0,
AI_CRUISER,
1500, // speed1
0, // speed2
0, // speed3
1, // priority
0,
20 // xp

}, // DROPPER

{
450, // int max_hp
24000, // int xsize
24000, // int ysize
0,
0,
AI_DRIFTER,
130, // speed1 - move
930, // speed2 - drag
600, // speed3 - before retreats
1, // priority
0,
30 // xp

}, // RING

{
1800, // int max_hp
28000, // int xsize
25000, // int ysize
0,
0,
AI_STOPPER,
4000, // speed1
150, // speed2
60, // speed3
1, // priority
0,
50 // xp

}, // HUNTER

{
7250, // int max_hp
52000, // int xsize
84000, // int ysize
0,
0,
AI_CRUISER,
1500, // speed1
0, // speed2
0, // speed3
0, // priority
0,
100 // xp

}, // SPINNER
{
120, // int max_hp
17000, // int xsize
17000, // int ysize
0,
0,
AI_HOOK,
4200, // speed1
18, // speed2
0, // speed3
2, // priority
0, // score
15 // xp
}, // HOOK2


{
5000, // int max_hp
114000, // int xsize
100000, // int ysize
0,
0,
AI_BOSS,
0, // speed1
0, // speed2
0, // speed3
0, // priority
0,
600 // xp
}, // BOSS4
*/

};
