#include <gl.h>
#include <ship.h>
#include <vector>
#include <field.h>
#include <engine.h>

using namespace std;

unsigned int ship_vbo, ship_ibo;
unsigned int ship_ibo_size;
float ship_vbo_data[7 * 5][4];
vector<unsigned int> ship_ibo_data;
float matrixes[6][4];

char* ship::print_ship(char* msg) const {
     void *new_p = memcpy(msg, (void *)&pos.m[0], sizeof(pos) + sizeof(int) + 2 * sizeof(char));
     return (char *)new_p + sizeof(pos) + sizeof(int) + 2 * sizeof(char);
;
}

char* ship::write_ship(char* msg) {
    memcpy((void *)&pos.m[0], msg, sizeof(pos) + sizeof(int) + 2 * sizeof(char));
    update();
    return msg + sizeof(pos) + sizeof(int) + 2 * sizeof(char);
;
}



void init_ship_object() {
    
    vector<unsigned int> ibo;
    vector<polygon> me = gen_ship(4); // this func in field.h

    for (int i = 0; i < 4; i++) {
        draw_polygon(me[i], ship_vbo_data, 7 * i, ibo);
    }
    ship_ibo_data = ibo;
    ship_ibo_size = ibo.size();
}

bool ship::in_ship(point p) const {
    p.x -= pos.m[2];
    p.y -= pos.m[5];
    swap(matrixes[rot][1], matrixes[rot][2]);
    p = vec(p).rotate(matrixes[rot]).get_point();
    for (int i = 0; i < ibo_size; i += 3) {
        vector<point> triangle;
        get_triangle(i, triangle);
        if (in_triangle(p, triangle)) {
            swap(matrixes[rot][1], matrixes[rot][2]);
            return true;
        }
    }
    swap(matrixes[rot][1], matrixes[rot][2]);
    return false;
}

point ship::get_point(int j) const {
    return point(pos.m[2], pos.m[5]) + (vec(0, sqrt(3) * CELL_RAD) * j).rotate(matrixes[rot]);

}

void create_ships() {
    ships.resize(10);
    amount_of_ships = 10;
    ships[0].power(4);
    ships[0].pos.m[2] = 500;
    ships[0].pos.m[5] = 300;
    for (int i = 1; i < 3; i++) {
        ships[i].power(3);
        ships[i].pos.m[2] = 600;
        ships[i].pos.m[5] = 300 + (i - 1) * 180;
    }

    for (int i = 3; i < 6; i++) {
        ships[i].power(2);
        ships[i].pos.m[2] = 700;
        ships[i].pos.m[5] = 300 + (i - 3) * 120;
    }
    for (int i = 6; i < 10; i++) {
        ships[i].power(1);
        ships[i].pos.m[2] = 800;
        ships[i].pos.m[5] = 300 + (i - 6) * 60;
    }
}
