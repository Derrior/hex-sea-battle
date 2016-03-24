#ifndef SHIP_H
#define SHIP_H

#include <math_3d.h>
#define SHIP_SIZE 56
extern unsigned int ship_vbo, ship_ibo, ship_ibo_size;
extern float ship_vbo_data[SHIP_SIZE];
void init_ship_object();

struct ship {
    Matrix3f pos;
    int rot;
    int ibo_size;
    ship(int i = 1) {
        rot = 0;
        ibo_size = 4 * i;
    }
    void rotate() {
        rot++;
        if (rot == 6) {
            rot = 0;
        }
    }
};
#endif //SHIP_H
