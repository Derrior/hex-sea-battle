#ifndef POLYGON_H_
#define POLYGON_H_

#include <vector>

#include <geom.h>
#define DEFAULT_RAD 29
struct polygon
{
    point centre;
    std::vector<point> points;
};

polygon gen_cell(point centre, float rad=DEFAULT_RAD);
bool in_polygon(point a, polygon& b);
#endif //POLYGON_H_
