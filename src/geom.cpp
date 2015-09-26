#include <geom.h>

point::point()
    {
        x = y = 0;
    }
point::point(float _x, float _y)
    {
        x = _x;
        y = _y;
    }
point::point(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
bool point:: operator<(point& other)
{
    return x < other.x or (x == other.x and y < other.y);
}
bool point:: operator==(point& other)
    {
        return x == other.x and y == other.y;
    }

vec::vec()
    {
        x = y = 0;
    }
vec::vec(float _x, float _y)
    {   
        x = _x;
        y = _y;
    }
vec::vec(point begin, point end)
    {
        x = end.x - begin.x;
        y = end.y - begin.y;
    }

vec::vec(const vec& a)
    {
        x = a.x;
        y = a.y;
    }

double vec::cross(vec& other)
    {
        return x * other.y - y * other.x;
    }
    
double vec::dot(vec& other)
    {
        return x * other.x + y * other.y;
    }

bool vec::operator == (vec& other)
    {
        return x == other.x and y == other.y;
    }

bool vec::operator < (vec& other)
    {
        return this->cross(other) > 0;
    }
    
float vec::len()
    {
        return sqrt(x * x + y * y);
    }

vec vec::operator* (float coeff)
    {
        vec res(*this);
        res.x *= coeff;
        res.y *= coeff;
        return res;
    }
vec vec::resize(float len)
    {
        vec res(*this);
        return res * (len / (res.len()));
    }

vec vec::rotate(float angle)
    {
        float _cos = cos(angle), _sin = sin(angle);
        return vec(x * _cos - y * _sin, x * _sin + y * _cos);
    }

point operator+ (const point& a, const vec& move)
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

