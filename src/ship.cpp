#include <gl.h>
#include <ship.h>
#include <field.h>
#include <engine.h>
using namespace std;

unsigned int ship_vbo, ship_ibo;
unsigned int ship_ibo_size;
float ship_vbo_data[7 * 5][4];
unsigned int* ship_ibo_data;
float matrixes[6][4];

void init_ship_object() {
    
    vector<int> ibo;
    polygon* me = gen_ship(4);

    for (int i = 0; i < 4; i++) {
        draw_polygon(me[i], ship_vbo_data, 7 * i, ibo);
    }
    glGenBuffers(1, &ship_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, ship_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ship_vbo_data), ship_vbo_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    ship_ibo_data = new unsigned int[ibo.size()];
    for (int i = 0; i < (int)ibo.size(); i++) {
        ship_ibo_data[i] = ibo[i];
        cout << ibo[i] << ' ';
    }
    cout << endl;
    ship_ibo_size = ibo.size();
    glGenBuffers(1, &ship_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ship_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ship_ibo_size * sizeof(unsigned int), ship_ibo_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
bool ship::in_ship(point p) {
    p.x -= pos.m[2];
    p.y -= pos.m[5];
    swap(matrixes[rot][1], matrixes[rot][2]);
    p = vec(p).rotate(matrixes[rot]).get_point();
    for (int i = 0; i < ibo_size; i += 3) {
        point* triangle;
        get_triangle(i, triangle);
        if (in_triangle(p, triangle)) {
            swap(matrixes[rot][1], matrixes[rot][2]);
            delete[] triangle;
            return true;
        }
        delete[] triangle;
    }
    swap(matrixes[rot][1], matrixes[rot][2]);
    return false;
}

point ship::get_point(int j) {
    return point(pos.m[2], pos.m[5]) + (vec(0, sqrt(3) * CELL_RAD) * j).rotate(matrixes[rot]);

}

void create_ships() {
    ships = new ship[10];
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
