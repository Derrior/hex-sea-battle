#ifndef BUTTON_H
#define BUTTON_H
#include <functional>
#include <polygon.h>
#include <vector>
#define TIME_BETWEEN_ACTIVATIONS 0.04
struct button {
    polygon p;
    std::string name;
    std::function<void()> callback;
    point place;
    float size;
    bool registered_any_callbacks;
    long double last_time_activated;
    button();
    button(int x, int y);
    button(int x, int y, std::string &_name);
    bool is_pressed(point click);
    void call_callback();
    void register_callback(std::function<void()> t);
    void unregister_callback();
};
#endif // BUTTON_H
