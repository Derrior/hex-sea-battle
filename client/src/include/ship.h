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
    ship(int i = 1);
    void rotate(); 
    void unrotate(); 
    void power(int i);
    point get_point(int j);
    bool in_ship(point a);
    bool is_alive(); 
    char* print_ship(char* msg);
};
#endif //SHIP_H
