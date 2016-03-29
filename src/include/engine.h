#ifndef ENGINE_H
#define ENGINE_H

#include <gl.h>
#include <ship.h>
#include <iostream>

int get_cell_idx(point a);

void get_triangle(int idx, point* &res);

bool check(field& r, ship* b);

bool turn(int x, int y, field& r, ship* b);

#endif //ENGINE_H
