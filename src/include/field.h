#ifndef FIELD_H_
#define FIELD_H_
#include <vector>
#include <polygon.h>
polygon* gen_field(int w, int h);
void to_float(float* arr, int& idx, point c);
void draw_polygon(polygon& p, float* vbo_data, int idx, std::vector<int>& ibo);

#endif //FIELD_H_
