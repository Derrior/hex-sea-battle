#ifndef POLYGON_H_
#define POLYGON_H_

#include <vector>

#include <geom.h>

struct polygon
{
    point centre;
    std::vector<point> points;
};

polygon gen_cell(point centre);
#endif //POLYGON_H_
