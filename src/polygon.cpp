#include <geom.h>
#include <polygon.h>

using namespace std;

polygon gen_cell(point centre)
{
    polygon res;
    point curr = centre + vec(-1, 0);
    for (int i = 0; i < 6; i++)
    {
        curr = rotate(centre, curr, M_PI / 3);
        res.points.push_back(curr);
    }
    return res;
}
