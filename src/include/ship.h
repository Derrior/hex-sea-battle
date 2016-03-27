#ifndef SHIP_H
#define SHIP_H

#include <math_3d.h>
#include <geom.h>
#define SHIP_SIZE 18
extern unsigned int ship_vbo, ship_ibo, ship_ibo_size;
extern float ship_vbo_data[SHIP_SIZE * 5];
extern unsigned int* ship_ibo_data;
extern float matrixes[6][4];

void init_ship_object();
void create_ships();
struct ship {
    Matrix3f pos;
    int rot;
    int ibo_size;
    int strength;
    ship(int i = 1) {
        rot = 0;
        ibo_size = SHIP_SIZE * i;
    }
    void rotate() {
        rot++;
        if (rot == 6) {
            rot = 0;
        }
    }
    void unrotate() {
        rot--;
        if (rot == -1) {
            rot = 5;
        }
    }
    void power(int i) {
        ibo_size = SHIP_SIZE * i;
        strength = i;
    }
    point get_point(int j);
    bool in_ship(point a);
};
#endif //SHIP_H
