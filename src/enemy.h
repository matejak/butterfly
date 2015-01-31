
int create_enemy(int type);

void init_enemies(void);

void run_enemies(void);

int pbullet_hits_enemy(int b, int e);

int hurt_enemy(int e, int dam, int source);;
void set_wpixels(void);

int fire_dart(int x, int y, int angle, int colour, int wait, int accel);
int fire_ddart(int x, int y, int angle, int colour, int wait, int accel);
int fire_diamond(int x, int y, int angle, int colour, int speed, int rot, int wait);
