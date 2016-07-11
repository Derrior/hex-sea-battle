#ifndef GL_H
#define GL_H

#include <GL/glew.h>
#include <music_player.h>
#include <GL/freeglut.h>
#include <background.h>
#include <math_3d.h>
#include <polygon.h>
#include <drawing.h>
#include <button.h>
#include <engine.h>
#include <iostream>
#include <player.h>
#include <shader.h>
#include <field.h>
#include <geom.h>
#include <cstdio>
#include <file.h>
#include <ship.h>
#include <ft.h>

#define D_X 1
#define BOMB_CONST 2
#define AMOUNT_COLORSCHEMES 4

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
#define CONST_SPEED_CAMERA 20
extern unsigned int vbo, ibo_buffer, program, menu;
extern unsigned int f_color_loc, world_loc, coord_loc, angle_loc, camera_loc, scale_loc, tex_loc, any_texture_loc, shadowing_loc;
extern long double curr_time, last_fps_update;
extern polygon* Field;
extern Matrix3f World;
extern Matrix3f Camera;
extern Matrix3f Empty;
extern int amount_of_polygons, ibo_size, amount_of_ships, curr_ship, fps_counter;
extern ship* ships;
extern field field1, field2;
extern int mouse_x, mouse_y;
extern float field_color[], white_color[], black_color[];
extern float ship_color[], current_ship_color[];
extern float candidates_color[];
extern float bomb_color[], aqua_color[];
extern int colorscheme;
extern GLuint tex_a[128];
extern background bg;
extern SDL_AudioSpec wav_spec;
extern bool window_should_close, play_audio, bombs_removed, check_result;
extern std::vector<std::vector<button> > buttons;
extern std::vector<button> candidates_buttons;
extern std::vector<player> candidates;
extern player opponent;
extern long double time_last_check;
extern unsigned int tex_coord_loc;
extern float world_scale;
extern std::string name;
void next_mode();

#endif //GL_H

