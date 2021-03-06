#include <drawing.h>
#include <field.h>
#include <ft.h>
#include <gl.h>

using namespace std;

void draw_text(point pos, string& text) {
    float size = 0.7;
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(any_texture_loc, 1);
    glUniform1f(scale_loc, size);
    glUniform1i(tex_loc, 1);
//    glUniform4fv(f_color_loc, 1, black_color);
    int len = text.length();
    int x_pos = 0, y_pos = 0;
    
    World.m[2] = pos.x;
    World.m[5] = pos.y;
    float buff_data[4][4];
    buff_data[0][2] = 0;
    buff_data[0][3] = 1;

    buff_data[1][2] = 1;
    buff_data[1][3] = 1;

    buff_data[2][2] = 1;
    buff_data[2][3] = 0;

    buff_data[3][2] = 0;
    buff_data[3][3] = 0;

    glUniformMatrix3fv(world_loc, 1, GL_TRUE, &World.m[0]);
    for (int i = 0; i < len; i++) {
        float x_pos1 = (x_pos + test[text[i]].bearing_x);// * 783;
        float y_pos1 = (y_pos - test[text[i]].size_y + test[text[i]].bearing_y);// * 386;
        float x_pos2 = (x_pos + test[text[i]].bearing_x + test[text[i]].size_x);// * 783;
        float y_pos2 = (y_pos - test[text[i]].size_y + test[text[i]].bearing_y + test[text[i]].size_y);// * 386;
        //cout << x_pos1 << ' ' << y_pos1 << ' ' << x_pos2 << ' ' << y_pos2  << ' ' << test[i].size_y << endl;

        buff_data[0][0] = x_pos1;
        buff_data[0][1] = y_pos1;
        buff_data[1][0] = x_pos2;
        buff_data[1][1] = y_pos1;
        buff_data[2][0] = x_pos2;
        buff_data[2][1] = y_pos2;
        buff_data[3][0] = x_pos1;
        buff_data[3][1] = y_pos2;
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, tex_a[(int)text[i]]);
        glBindBuffer(GL_ARRAY_BUFFER, ft_vbo);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(aa);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 16 * sizeof(float), buff_data);
        //glBufferData(GL_ARRAY_BUFFER, 0, buff_data, 16 * sizeof(float));
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glVertexAttribPointer(aa, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
        glDrawArrays(GL_QUADS, 0, 4);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(aa);
       // glActiveTexture(GL_TEXTURE0);
        x_pos = x_pos + test[text[i]].advance;
    }
    glUniform1i(tex_loc, 0);
    glUniform1i(any_texture_loc, 0);
    glActiveTexture(GL_TEXTURE0);
}

void draw_cell(int cell_idx, const float* color, field& F) {
        World.m[2] = F.move.m[2] + Field[cell_idx].centre.x;
        World.m[5] = F.move.m[5] + Field[cell_idx].centre.y;
        glUniformMatrix3fv(world_loc, 1, GL_TRUE, &World.m[0]);
        glUniformMatrix2fv(angle_loc, 1, GL_TRUE, &matrixes[0][0]);
        glUniform4fv(f_color_loc, 1, color);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, ship_vbo);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ship_ibo);
        glDrawElements(GL_TRIANGLES, SHIP_SIZE, GL_UNSIGNED_INT, 0);
        glDisableVertexAttribArray(0);
}

void draw_cell(int cell_idx) {
        World.m[2] = bg.arr[cell_idx].centre.x;
        World.m[5] = bg.arr[cell_idx].centre.y;
        glUniformMatrix3fv(world_loc, 1, GL_TRUE, &World.m[0]);
        glUniformMatrix3fv(camera_loc, 1, GL_TRUE, &Camera.m[0]);
        glUniformMatrix2fv(angle_loc, 1, GL_TRUE, &matrixes[0][0]);
        glUniform4fv(f_color_loc, 1, &colors[bg.color_idx[cell_idx]].c[0]);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, ship_vbo);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ship_ibo);
        glDrawElements(GL_TRIANGLES, SHIP_SIZE, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
    
}

void draw_background() {
    glUniform1f(scale_loc, BACKGROUND_CELL_RAD);
    Camera.m[2] /= 2;
    Camera.m[5] /= 2;
    glUniformMatrix3fv(camera_loc, 1, GL_TRUE, &Camera.m[0]);
    for (int i = 0; i < (int)bg.arr.size(); i++) {
        draw_cell(i);
        
    }
    Camera.m[2] *= 2;
    Camera.m[5] *= 2;
    glUniformMatrix3fv(camera_loc, 1, GL_TRUE, &Camera.m[0]);
    glUniform1f(scale_loc, world_scale);
}

void draw_ship(int ship_idx, const float* color) {
        glUniformMatrix3fv(world_loc, 1, GL_TRUE, &ships[ship_idx].pos.m[0]);
        glUniformMatrix2fv(angle_loc, 1, GL_TRUE, &matrixes[ships[ship_idx].rot][0]);
        glUniform4fv(f_color_loc, 1, color);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, ship_vbo);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ship_ibo);
        glDrawElements(GL_TRIANGLES, ships[ship_idx].ibo_size, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
}

void draw_field(field& F) {
    glUniformMatrix3fv(world_loc, 1, GL_TRUE, &F.move.m[0]);
    glUniformMatrix2fv(angle_loc, 1, GL_TRUE, &matrixes[0][0]);
    glUniform4fv(f_color_loc, 1, field_color + (4 * colorscheme));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_buffer);
    glDrawElements(GL_TRIANGLES, ibo_size, GL_UNSIGNED_INT, 0);
    glDisableVertexAttribArray(0);
}


void draw_bombs(field& F) {
    for (int i = 0; i < (int)F.bombs.size(); i++) {
        draw_cell(F.bombs[i], bomb_color, F);
    }
    for (int i = 0; i < (int)F.aqua.size(); i++) {
        draw_cell(F.aqua[i], aqua_color, F);
    }
}

void draw_buttons() {
    glUniformMatrix3fv(camera_loc, 1, GL_TRUE, &Empty.m[0]);
    for (int i = 0; i < (int)buttons.size(); i++) {
        World.m[2] = buttons[i].place.x;
        World.m[5] =  buttons[i].place.y;
        glUniformMatrix3fv(world_loc, 1, GL_TRUE, &World.m[0]);
        glUniformMatrix2fv(angle_loc, 1, GL_TRUE, &matrixes[0][0]);
        glUniform1f(scale_loc, 1.5);
        glUniform4fv(f_color_loc, 1, aqua_color);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, ship_vbo);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ship_ibo);
        glDrawElements(GL_TRIANGLES, SHIP_SIZE, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
        draw_text(point((buttons[i].place.x - 6.5 * buttons[i].name.length()), buttons[i].place.y - 1 * buttons[i].name.length()), buttons[i].name);
    }
    glUniformMatrix3fv(camera_loc, 1, GL_TRUE, &Camera.m[0]);
    glUniform1f(scale_loc, world_scale);
}
