#ifndef ENGINE_H
#define ENGINE_H

#include <gl.h>
#include <ship.h>
#include <iostream>
#define INIT_MODE 0
#define SHIP_MODE 1
#define BATTLE_MODE 2
#define OTHER_MODE 3
#define CELLS_AT_ALL 20

extern int mode;

int get_cell_idx(point a);

void get_triangle(int idx, point* &res);

int check(field& r, ship* b);

bool turn(int x, int y, field& r, ship* b);

void next_mode(int &mode); 
#endif //ENGINE_H
