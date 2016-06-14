#ifndef GRAPHICS_TEXT_H
#define GRAPHICS_TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H


struct ft_char {
    int size_x;
    float size_y;
    int bearing_x;
    int bearing_y;
    int advance;
    float texcoord_x;
};
extern ft_char test[128];
extern unsigned int ft_vbo;
/*
extern unsigned int ft_font_size;
extern unsigned int prog_ft;
extern unsigned int prog_ft_attr_v_coord;
extern unsigned int prog_ft_unif_mat_p, prog_ft_attr_texture, prog_ft_attr_color;
*/
void init_ft();
/*
void render_text(int x, int y, const char *str);
void free_ft(void);
*/
#endif // GRAPHICS_TEXT_H
