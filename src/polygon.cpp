#include <geom.h>
#include <polygon.h>

using namespace std;

polygon gen_cell(point centre)
{
    polygon res;
    res.centre = centre;
    point curr = centre + vec(-29, 0);
    for (int i = 0; i < 6; i++)
    {
        curr = rotate(centre, curr, M_PI / 3);
        res.points.push_back(curr);
    }
    return res;
}

bool in_triangle(point a, point* triangle) {
    vec ab(triangle[0], triangle[1]), bc(triangle[1], triangle[2]), ac(triangle[0], triangle[2]);
    return (ab.cross(vec(triangle[0], a)) * vec(triangle[0], a).cross(ac) > 0 and 
        bc.cross(vec(triangle[1], a)) * vec(triangle[1], a).cross(vec(triangle[1], triangle[0])) > 0);
    return true; 
}

bool in_polygon(point a, polygon& b) {

    for (int l = 0; l < 6; l++) {
        int r = (l + 1) % 6;
        point triangle[] = {b.centre, b.points[l], b.points[r]};
        if (in_triangle(a, triangle))
            return true;
    }
    return false;
}
