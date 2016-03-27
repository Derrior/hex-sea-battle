#ifndef ENGINE_H
#define ENGINE_H

#include <gl.h>
#include <iostream>

void get_triangle(int idx, point* &res);

bool check(field& r, ship* b);

void turn(int x, int y, field& r, ship* b);

#endif //ENGINE_H
