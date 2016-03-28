#ifndef FIELD_H_
#define FIELD_H_
#include <vector>
#include <polygon.h>
#include <math_3d.h>

polygon* gen_field(int w, int h, int rad=DEFAULT_RAD);
polygon* gen_ship(int w);
void to_float(float* arr, int& idx, point c);
void draw_polygon(polygon& p, float* vbo_data, int idx, std::vector<int>& ibo);
struct field {
    Matrix3f move;
    std::vector<int> bombs;
    std::vector<int> aqua;
    bool* used;
    field() {
        used = NULL;
    }

    field(int am) {
        used = new bool[am];
    }
};
#endif //FIELD_H_
