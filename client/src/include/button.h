#ifndef BUTTON_H
#define BUTTON_H
#include <functional>
#include <polygon.h>
#include <vector>

#define BUTTON_HEX 0
#define BUTTON_RECT 1

#define TIME_BETWEEN_ACTIVATIONS 0.04

extern unsigned int rect_vbo, rect_ibo, rect_ibo_size, rect_ibo_data[12];
extern float rect_vbo_data[5][4];

struct button {
    polygon p;
    std::string name;
    std::function<void()> callback;
    point place;
    float size;
    bool registered_any_callbacks;
    long double last_time_activated;
    button(int type = BUTTON_HEX);
    button(int x, int y, int type = BUTTON_HEX);
    button(int x, int y, const std::string &_name, int type = BUTTON_HEX);
    bool is_pressed(point click);
    void call_callback();
    void register_callback(std::function<void()> t);
    void unregister_callback();
};

void init_rect_object();
#endif // BUTTON_H
