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
            res[i * h + j] = gen_cell(point(j * 40, i * 40 + (j & 1) * 20));
        }
    }
    return res;
}
