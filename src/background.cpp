#include <background.h>
#include <field.h>
#include <util.h>
using namespace std;

color* colors;

color::color() {
}

color::color(float _a, float _b, float _c, float _d) {
    c[0] = _a;
    c[1] = _b;
    c[2] = _c;
    c[3] = _d;
}

void init_colors() {
    colors = new color[10];
    for (int i = 0; i < 10; i++) 
        colors[i] = color((float)get_rand(0, 127) / 127, (float)get_rand(0, 255) / 127, (float)get_rand(0, 255) / 127, 1.f);
}

background::background() {
    polygon* F = gen_field(10, 10, BACKGROUND_CELL_RAD);
    arr.resize(100);
    for (int i = 0; i < 100; i++) {
        arr[i] = F[i];
        color_idx.push_back(get_rand(0, 10));
    }
}
