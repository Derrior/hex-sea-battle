#ifndef ENGINE_H
#define ENGINE_H

#include <gl.h>
#include <ship.h>
#include <iostream>

#define SWITCH_DUR 0.5

#define INIT_MODE 0
#define SHIP_MODE 1
#define BATTLE_MODE 2
#define OTHER_MODE 3

extern int mode;
extern long double begin_switch_mode;
extern bool check_pressed, go_pressed, go_allowed;

int get_cell_idx(point a);

void get_triangle(int idx, point* &res);

bool check();

bool turn(int x, int y, field& r, ship* b);

#endif //ENGINE_H
