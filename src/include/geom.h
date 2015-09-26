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
    vec(point begin, point end);

    vec(const vec& a);

    double cross(vec& other);
    
    double dot(vec& other);

    bool operator == (vec& other);

    bool operator < (vec& other);
    
    float len();

    vec operator* (float coeff);
    
    vec resize(float len);

    vec rotate(float angle);
};

point operator+ (const point& a, const vec& move);

point operator- (point&a, vec& move);

point rotate(point& centre, point& p, float angle);

#endif //GEOM
