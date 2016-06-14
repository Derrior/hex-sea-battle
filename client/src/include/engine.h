#ifndef ENGINE_H
#define ENGINE_H

#include <gl.h>
#include <ship.h>
#include <iostream>
#define INIT_MODE 0
#define SHIP_MODE 1
#define BATTLE_MODE 2
#define OTHER_MODE 3

extern int mode;
extern bool check_pressed;

int get_cell_idx(point a);

void get_triangle(int idx, point* &res);

bool check(field& r, ship* b);

bool turn(int x, int y, field& r, ship* b);

#endif //ENGINE_H
