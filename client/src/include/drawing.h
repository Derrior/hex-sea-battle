#ifndef DRAWING_H
#define DRAWING_H

#include <field.h>
#include <string>
#include <gl.h>

void draw_text(point pos, std::string& text, float size=1);
void draw_cell(int cell_idx, const float* color, field& F);
void draw_cell(int cell_idx);
void draw_background();
void draw_ship(int ship_idx, const float* color);
void draw_field(field& F);
void draw_buttons();
void draw_bombs(field& F);
#endif //DRAWING_H
