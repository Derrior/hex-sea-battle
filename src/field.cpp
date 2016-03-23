#include <geom.h>
#include <polygon.h>

using namespace std;


polygon* gen_field(int w, int h)
{
    polygon* res = new polygon[w * h];
    for (int i = 0; i < w; i++)
    {
        for (int j = 0; j < h; j++)
        {
            res[i * h + j] = gen_cell(point(j * 46 + 150, i * 52 + (j & 1) * 26 + 200));
        }
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
