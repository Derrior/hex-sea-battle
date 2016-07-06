#ifndef INIT_WORLD_H
#define INIT_WORLD_H
#define INIT_MODE 0
#define SHIP_MODE 1
#define BATTLE_MODE 2

#include <gl.h>

void init_matrixes();
void set_ship(int& x, int& y, field& F);
void create_field_vbo();
void init_fields();
void init_audio();
void init_buttons();

#endif
