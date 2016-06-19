#ifndef SHIP_H
#define SHIP_H

#include <math_3d.h>
#include <geom.h>
#define SHIP_SIZE 18
extern unsigned int ship_vbo, ship_ibo, ship_ibo_size;
extern float ship_vbo_data[35][4];
extern unsigned int* ship_ibo_data;
extern float matrixes[6][4];

void init_ship_object();
void create_ships();
struct ship {
    Matrix3f pos;
    int ibo_size;
    char strength, rot;
    bool* is_damaged;
    ship(int i = 1) {
        rot = 0;
        ibo_size = SHIP_SIZE * i;
        is_damaged = new bool[i];
        memset(is_damaged, 0, i);

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
        delete[] is_damaged;
        is_damaged = new bool[i];
        memset(is_damaged, 0, i);
    }
    point get_point(int j);
    bool in_ship(point a);
    bool is_alive() {
        for (int i = 0; i < strength; i++) {
            if (!is_damaged[i]) {
                return true;
            }
        }
        return false;
    }
    char* print_ship(char* msg);
    char* write_ship(char* msg);

};
#endif //SHIP_H
