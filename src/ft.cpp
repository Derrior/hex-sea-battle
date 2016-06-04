#include <ft.h>
#include <gl.h>
#include <iostream>

using namespace std;

FT_Library library;
FT_Face face;
ft_char test[128];
unsigned int ft_vbo;

void init_ft() {
    int error = FT_Init_FreeType(&library);
    if (error) {
        cout << "Error while loading freetype" << endl;
    }
    error = FT_New_Face(library, "fonts/inconsolata.ttf", 0, &face);
    if (error) {
        cout << "Errors with fonts, exit" << endl;
        return;
    }
//    error = FT_Set_Pixel_Sizes(face, 0, 14);
    error = FT_Set_Char_Size(face, 0, 16 * 64, 150, 150);
    if (error) {
        cout << "Errors with set size" << endl;
        return;
    }
    
    FT_GlyphSlot slot = face->glyph;
    glGenTextures(128, tex_a);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glActiveTexture(GL_TEXTURE0);
    for (int i = 32; i < 128; i++) {
        int glyph_index = FT_Get_Char_Index(face, i);
        error = FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER);
        if (error) {
            cout << "Errors with letter " << char(i) << endl;
            return;
        }

        glBindTexture(GL_TEXTURE_2D, tex_a[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, slot->bitmap.width, slot->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, slot->bitmap.buffer);
        test[i].size_x = slot->bitmap.width;
        test[i].size_y = slot->bitmap.rows;
        test[i].bearing_x = slot->bitmap_left;
        test[i].bearing_y = slot->bitmap_top;
        test[i].advance = slot->advance.x >> 6;
        test[i].texcoord_x = 0;
    }
    float size_of_char = 16;
    float points[] = {0, 0, 0, 1,
                      size_of_char, 0, 1, 1,
                      size_of_char, size_of_char, 1, 0,
                      0, size_of_char, 0, 0};
    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glGenBuffers(1, &ft_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, ft_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

