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

std::ostream& operator<<(std::ostream& a, const point& b) {
    a << "(" << b.x << "; " << b.y << ") ";
    return a;
}

bool point:: operator<(const point& other)
{
    return x < other.x or (x == other.x and y < other.y);
}
bool point:: operator==(const point& other)
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

vec::vec(point a) {
    x = a.x;
    y = a.y;
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

point vec::get_point() {
    return point(x, y);
}

double vec::cross(const vec& other)
{
    return x * other.y - y * other.x;
}
    
double vec::dot(const vec& other)
{
    return x * other.x + y * other.y;
}

bool vec::operator == (const vec& other)
{
    return x == other.x and y == other.y;
}

bool vec::operator < (const vec& other)
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

vec vec::rotate(float* mat)
{
    return vec(x * mat[0] + y * mat[1], x * mat[2] + y * mat[3]);
}

point operator+ (const point& a, const vec& move)
{
    return point(a.x + move.x, a.y + move.y);
}

point operator- (const point&a, const vec& move)
{
    return point(a.x - move.x, a.y - move.y);
}

point rotate(const point& centre, const point& p, float angle)
{
    return centre + vec(centre, p).rotate(angle);
}

bool in_triangle(point a, point* triangle) {
    vec ab(triangle[0], triangle[1]), bc(triangle[1], triangle[2]), ac(triangle[0], triangle[2]);
    return ((ab.cross(vec(triangle[0], a)) * vec(triangle[0], a).cross(ac) > 0 and 
        bc.cross(vec(triangle[1], a)) * vec(triangle[1], a).cross(vec(triangle[1], triangle[0])) > 0)) or a == triangle[0] or a == triangle[1] or a == triangle[2];
    return true; 
}
