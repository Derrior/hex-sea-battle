#include <geom.h>
#include <field.h>
#include <polygon.h>

using namespace std;
const float CONST_X = 45 * (CELL_RAD / 29.0);
const float CONST_Y = 52.1 * (CELL_RAD / 29.0);

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

void to_float(float* arr, int& idx, point c)
{
    arr[idx] = c.x;
    arr[idx + 1] = c.y;
    idx += 2;
}
void draw_polygon(polygon& p, float* vbo_data, int idx, vector<int>& ibo)
{
    to_float(vbo_data, idx, p.centre);
    int was_idx = idx;
   
    for (int i = 0; i < (int)p.points.size(); i++)
    {
        to_float(vbo_data, idx, p.points[i]);
    }
    idx = was_idx / 2;
    for (int i = 0; i < (int)p.points.size(); i++)
    {
        ibo.push_back(idx - 1);
        ibo.push_back(idx + i);
        ibo.push_back(idx + (i + 1) % p.points.size());
    }
}


