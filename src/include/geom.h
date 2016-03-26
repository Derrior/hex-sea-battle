#ifndef GEOM
#define GEOM

#include <cmath>

struct point 
{
    float x, y;
    point();
    point(float _x, float _y);
    point(int _x, int _y);
    bool operator<(point& other);
    bool operator==(point& other);
};

struct vec
{
    float x, y;
    vec();
    vec(float _x, float _y);
    vec(point a);
    vec(point begin, point end);

    vec(const vec& a);

    double cross(const vec& other);
    
    double dot(vec& other);

    bool operator == (vec& other);

    bool operator < (vec& other);
    
    float len();

    point get_point();

    vec operator* (float coeff);
    
    vec resize(float len);

    vec rotate(float angle);

    vec rotate(float* mat);
};

point operator+ (const point& a, const vec& move);

point operator- (point&a, vec& move);

point rotate(point& centre, point& p, float angle);

bool in_triangle(point a, point* triangle);
#endif //GEOM
