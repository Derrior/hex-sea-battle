#ifndef SHIP_H
#define SHIP_H

#include <math_3d.h>
#define SHIP_SIZE 14
extern unsigned int ship_vbo, ship_ibo, ship_ibo_size;
extern float ship_vbo_data[SHIP_SIZE];
void init_ship();

struct ship {
    Matrix3f pos;
};
#endif //SHIP_H
