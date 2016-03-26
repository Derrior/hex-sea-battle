#ifndef DRAWING_H
#define DRAWING_H

#include <field.h>
#include <gl.h>

void draw_cell(int cell_idx, const float* color, field& F);
void draw_cell(int cell_idx);
void draw_background();
void draw_ship(int ship_idx, const float* color);
void draw_field(field& F);

void draw_bombs(field& F);
#endif //DRAWING_H
