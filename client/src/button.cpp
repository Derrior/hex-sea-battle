#include <button.h>
#include <time.h>
#include <gl.h>
using namespace std;

unsigned int rect_vbo, rect_ibo, rect_ibo_size, rect_ibo_data[12];
float rect_vbo_data[5][4];

void init_rect_object() {
    int x[] = {0, -100, 100, 100, -100}, y[] = {0, -20, -20, 20, 20};
    rect_vbo_data[0][0] = rect_vbo_data[0][1] = 0;
    rect_vbo_data[0][2] = rect_vbo_data[0][3] = 0.5;
    for (int i = 1; i < 5; i++) {
        rect_vbo_data[i][0] = x[i];
        rect_vbo_data[i][1] = y[i];
        rect_vbo_data[i][2] = (int)(x[i] > 0);
        rect_vbo_data[i][3] = (int)(y[i] > 0);
    }
    unsigned int ibo_data_current[] = {0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 1};
    memcpy(rect_ibo_data, ibo_data_current, 12 * sizeof(int));
    rect_ibo_size = 12;
    glGenBuffers(1, &rect_vbo);
    glGenBuffers(1, &rect_ibo);
    glBindBuffer(GL_ARRAY_BUFFER, rect_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 20, rect_vbo_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rect_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 12, rect_ibo_data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

polygon gen_rect(point centre, float rad)
{
    polygon res;
    res.centre = centre;
    float dx, dy;
    dx = rad * 5;
    dy = rad;
    point curr = centre + vec(-dx, -dy);
    res.points.resize(4);
    res.points[0] = curr;
    curr.x += 2 * dx;
    res.points[1] = curr;
    curr.y += 2 * dy;
    res.points[2] = curr;
    curr.x -= 2 * dx;
    res.points[3] = curr;

    return res;
}


button::button(int type) {
    place = point(0, 0);
    name = "";
    if (type == BUTTON_HEX) {
        p = gen_cell(point(), 40);
    } else if (type == BUTTON_RECT) {
        p = gen_rect(point(), 20);
    }
    callback = NULL;
    size = font_size = 1;
    registered_any_callbacks = false;
}

button::button(int x, int y, int type) {
    place = point(x, y);
    name = "";
    if (type == BUTTON_HEX) {
        p = gen_cell(point(), 40);
    } else if (type == BUTTON_RECT) {
        p = gen_rect(point(), 20);
    }
    callback = NULL;
    size = font_size = 1;
    registered_any_callbacks = false;
}

button::button(int x, int y, const string &_name, float _size, int type) {
    size = _size;
    font_size = 1;
    place = point(x, y);
    name = _name;
    if (type == BUTTON_HEX) {
        p = gen_cell(point(), 40 * size);
    } else if (type == BUTTON_RECT) {
        p = gen_rect(point(), 20 * size);
    }
    callback = NULL;
    registered_any_callbacks = false;
}

bool button::is_pressed(point click) {
    bool res = in_polygon(vec(place, click).get_point(), p);
    cout << "nearly success " << name << ' ' << res << endl; 
    return res; 
}

void button::call_callback() {
    if (!registered_any_callbacks) {
        return;
    }
    if (curr_time - last_time_activated < TIME_BETWEEN_ACTIVATIONS) {
        return;
    }
    callback();
    last_time_activated = curr_time;
}

void button::register_callback(function<void()> t) {
    callback = t;
    registered_any_callbacks = true;
}

void button::unregister_callback() {
    registered_any_callbacks = false;
    callback = NULL;
}
