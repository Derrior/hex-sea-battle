#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <field.h>
#include <polygon.h>
#define BACKGROUND_CELL_RAD 5
struct color {
    float c[4];
    color();
    color(float _a, float _b, float _c, float _d);
};

extern color* colors;

void init_colors();

struct background {
    std::vector<polygon> arr;
    std::vector<int> color_idx;
    background();
};

#endif //BACKGROUND_H
