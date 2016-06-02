#include <button.h>
#include <time.h>
#include <gl.h>
using namespace std;

button::button() {
    place = point(0, 0);
    name = "";
    p = gen_cell(point(), 40);
    callback = NULL;
    registered_any_callbacks = false;
}

button::button(int x, int y) {
    place = point(x, y);
    name = "";
    p = gen_cell(point(), 40);
    callback = NULL;
    registered_any_callbacks = false;
}

button::button(int x, int y, string &_name) {
    place = point(x, y);
    name = _name;
    p = gen_cell(point(), 40);
    callback = NULL;
    registered_any_callbacks = false;
}

bool button::is_pressed(point click) {
    bool res = in_polygon(vec(place, click).get_point(), p);
    cout << "nearly success" << ' ' << res << endl; 
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
    last_time_activated = time(NULL);
}

void button::register_callback(function<void()> t) {
    callback = t;
    registered_any_callbacks = true;
}

void button::unregister_callback() {
    registered_any_callbacks = false;
    callback = NULL;
}
