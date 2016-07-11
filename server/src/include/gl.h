#ifndef GL_H
#define GL_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <background.h>
#include <init_world.h>
#include <math_3d.h>
#include <polygon.h>
#include <button.h>
#include <battle.h>
#include <iostream>
#include <engine.h>
#include <field.h>
#include <geom.h>
#include <cstdio>
#include <file.h>
#include <ship.h>

#define D_X 1
#define BOMB_CONST 2
#define AMOUNT_COLORSCHEMES 4

extern long double curr_time;
extern polygon* Field;
extern int amount_of_polygons, amount_of_ships, curr_ship;
extern ship* ships;
extern field field1, field2;
extern long long time_last_check;
extern std::vector<battle> battles;
#endif //GL_H

