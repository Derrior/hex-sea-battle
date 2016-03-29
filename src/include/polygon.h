#ifndef POLYGON_H_
#define POLYGON_H_

#define CELL_RAD 35
#include <vector>
#include <geom.h>
#include <ship.h>
struct polygon
{
    point centre;
    std::vector<point> points;
};

polygon gen_cell(point centre, float rad=CELL_RAD);
bool in_polygon(point a, polygon& b);
#endif //POLYGON_H_
