#include <geom.h>
#include <polygon.h>

using namespace std;

polygon gen_cell(point centre, float rad)
{
    polygon res;
    res.centre = centre;
    point curr = centre + vec(-rad, 0);
    for (int i = 0; i < 6; i++)
    {
        curr = rotate(centre, curr, M_PI / 3);
        res.points.push_back(curr);
    }
    return res;
}


bool in_polygon(point a, polygon& b) {

    for (int l = 0; l < b.points.size(); l++) {
        int r = (l + 1) % b.points.size();
        point triangle[] = {b.centre, b.points[l], b.points[r]};
        if (in_triangle(a, triangle))
            return true;
    }
    return false;
}
