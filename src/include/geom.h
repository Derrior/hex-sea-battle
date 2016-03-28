#ifndef GEOM
#define GEOM

#include <math.h>
#include <iostream>

struct point 
{
    float x, y;
    point();
    point(float _x, float _y);
    point(int _x, int _y);
    bool operator<(const point& other);
    bool operator==(const point& other);
};

std::ostream& operator<<(std::ostream& a, const point& b); 

struct vec
{
    float x, y;
    vec();
    vec(float _x, float _y);
    vec(point a);
    vec(point begin, point end);

    vec(const vec& a);

    double cross(const vec& other);
    
    double dot(const vec& other);

    bool operator == (const vec& other);

    bool operator < (const vec& other);
    
    float len();

    point get_point();

    vec operator* (float coeff);
    
    vec resize(float len);

    vec rotate(float angle);

    vec rotate(float* mat);
};

point operator+ (const point& a, const vec& move);

point operator- (const point&a, const vec& move);

point rotate(const point& centre, const point& p, float angle);

bool in_triangle(point a, point* triangle);
#endif //GEOM
