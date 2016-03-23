#ifndef MATH_3D_H
#define	MATH_3D_H

struct Matrix3f {
    float m[9];
    Matrix3f() {
        m[0] = m[4] = m[8] = 1;
    }
};

struct Vector3f
{
    float x;
    float y;
    float z;

    Vector3f()
    {
    }

    Vector3f(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
};

#endif	/* MATH_3D_H */
