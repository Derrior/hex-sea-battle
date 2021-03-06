#ifndef ENGINE_H
#define ENGINE_H

#include <gl.h>
#include <ship.h>
#include <iostream>

#define SWITCH_DUR 0.1

#define INIT_MODE 0
#define SHIP_MODE 1
#define BATTLE_MODE 2
#define OTHER_MODE 3

extern int mode, best_opponent;
extern long double begin_switch_mode;
extern char shoot_cell;
extern bool check_pressed, go_pressed, go_allowed, shoot_pressed, best_opponent_changed;

int get_cell_idx(point a);

void get_triangle(int idx, point* &res);

bool check();

bool turn(int x, int y, field& r, ship* b);

#endif //ENGINE_H
