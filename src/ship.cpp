#include <gl.h>
#include <ship.h>
#include <field.h>
using namespace std;

unsigned int ship_vbo, ship_ibo;
unsigned int ship_ibo_size;
float ship_vbo_data[SHIP_SIZE];
unsigned int* ship_ibo_data;
void init_ship_object() {
    
    vector<int> ibo;
    polygon* me = gen_ship(4);

    for (int i = 0; i < 4; i++) {
        draw_polygon(me[i], ship_vbo_data, 14 * i, ibo);
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

