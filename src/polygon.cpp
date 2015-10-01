#include <geom.h>
#include <polygon.h>

using namespace std;

polygon gen_cell(point centre)
{
    polygon res;
    res.centre = centre;
    point curr = centre + vec(-20, 0);
    for (int i = 0; i < 6; i++)
    {
        curr = rotate(centre, curr, M_PI / 3);
        res.points.push_back(curr);
    }
    return res;
}
