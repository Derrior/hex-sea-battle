#ifndef GL_H
#define GL_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <background.h>
#include <math_3d.h>
#include <polygon.h>
#include <drawing.h>
#include <shader.h>
#include <iostream>
#include <field.h>
#include <geom.h>
#include <cstdio>
#include <file.h>
#include <ship.h>

#define D_X 1

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
#define CONST_SPEED_CAMERA 20
extern unsigned int vbo, ibo_buffer, program, menu;
extern unsigned int f_color_loc, world_loc, coord_loc, angle_loc, camera_loc, scale_loc;
extern polygon* Field;
extern Matrix3f World;
extern Matrix3f Camera;
extern int amount_of_polygons, ibo_size, amount_of_ships, curr_ship;
extern ship* ships;
extern field field1, field2;
extern int mouse_x, mouse_y;
extern float ship_color[], current_ship_color[];
extern float bomb_color[];
extern int cnt;
extern background bg;
#endif //GL_H
