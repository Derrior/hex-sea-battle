#ifndef GEOM
#define GEOM

#include <cmath>

struct point 
{
    float x, y;
    point()
    {
        x = y = 0;
    }
    point(float _x, float _y)
    {
        x = _x;
        y = _y;
    }
    bool operator<(point& other)
    {
        return x < other.x or (x == other.x and y < other.y);
    }
    bool operator==(point& other)
    {
        return x == other.x and y == other.y;
    }
};

struct vec
{
    float x, y;
    vec()
    {
        x = y = 0;
    }
    vec(float _x, float _y)
    {   
        x = _x;
        y = _y;
    }
    vec(point begin, point end)
    {
        x = end.x - begin.x;
        y = end.y - begin.y;
    }

    vec(vec& a)
    {
        x = a.x;
        y = a.y;
    }

    double cross(vec& other)
    {
        return x * other.y - y * other.x;
    }
    
    double dot(vec& other)
    {
        return x * other.x + y * other.y;
    }

    bool operator == (vec& other)
    {
        return x == other.x and y == other.y;
    }

    bool operator < (vec& other)
    {
        return this->cross(other) > 0;
    }
    
    float len()
    {
        return sqrt(x * x + y * y);
    }

    vec operator* (float coeff)
    {
        vec res(*this);
        res.x *= coeff;
        res.y *= coeff;
    }
    
    vec resize(float len)
    {
        return (*this) * (len / (this->len()));
    }

    vec rotate(float angle)
    {
        float cos = cos(angle), sin = sin(angle);
        return vec(x * cos - y * sin, x * sin + y * cos);
    }
}

point operator+ (point&a, vec& move)
{
    return point(a.x + move.x, a.y + move.y);
}

point operator- (point&a, vec& move)
{
    return point(a.x - move.x, a.y - move.y);
}

point rotate(point& centre, point& p, float angle)
{
    return centre + vec(centre, p).rotate(angle);
}

#endif //GEOM
