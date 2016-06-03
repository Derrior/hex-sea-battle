#include <draw_text.h>

#ifndef _WIN32
void draw_text(point pos, string& text) {

    World.m[2] = pos.x;
    cout << "I tried" << endl;
    World.m[5] = pos.y;
    glUniformMatrix3fv(world_loc, 1, GL_TRUE, &World.m[0]);
    glUniform4fv(f_color_loc, 1, white_color);
    glRasterPos2f(pos.x, pos.y);
    int len = text.length();
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
    glRasterPos2f(0, 0);
}
#endif
