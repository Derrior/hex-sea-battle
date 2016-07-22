#include <geom.h>
#include <field.h>
#include <polygon.h>

using namespace std;
const float CONST_X = 45 * (CELL_RAD / 29.0);
const float CONST_Y = 52.1 * (CELL_RAD / 29.0);

void field::use_cell(int cell_idx) {
    if (used[cell_idx]) {
        return;
    }
    if (containing[cell_idx] >= 0) {
        bombs.push_back(cell_idx);
    } else {
        aqua.push_back(cell_idx);
    }
    used[cell_idx] = true;
}

void field::use_all_cells() {
    for (int i = 0; i < size; i++) {
        use_cell(i);
    }
}

int field::contain_ship(int cell_idx) {
    return containing[cell_idx];
}

char* field::print_field(char* msg) {
    return (char *)memcpy(msg, &move.m[0], sizeof(move)) + sizeof(move);
}

char* field::write_field(char* msg) {
    memcpy(&move.m[0], msg, sizeof(move));
    return msg + sizeof(move);
}

polygon* gen_field(int w, int h, int scale)
{
    polygon* res = new polygon[w * h];
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            res[i * h + j] = gen_cell(point(j * CONST_X * scale, scale * (i * CONST_Y + (j & 1) * CONST_Y / 2)));
        }
    }
    return res;
}
polygon* gen_ship(int w) {
    polygon* res = new polygon[w];
    for (int i = 0; i < w; i++) {
        res[i] = gen_cell(point(0., i * CONST_Y));
    }
    return res;
    
}

void to_float(float arr[][4], int& idx, point c)
{
    arr[idx][0] = c.x;
    arr[idx][1] = c.y;
    arr[idx][2] = 0;
    arr[idx][3] = 0;
    idx += 1;
}
void draw_polygon(polygon& p, float vbo_data[][4], int idx, vector<int>& ibo)
{
    to_float(vbo_data, idx, p.centre);
    int was_idx = idx;
   
    for (int i = 0; i < (int)p.points.size(); i++)
    {
        to_float(vbo_data, idx, p.points[i]);
    }
    idx = was_idx;
    for (int i = 0; i < (int)p.points.size(); i++)
    {
        ibo.push_back(idx - 1);
        ibo.push_back(idx + i);
        ibo.push_back(idx + (i + 1) % p.points.size());
    }
}


