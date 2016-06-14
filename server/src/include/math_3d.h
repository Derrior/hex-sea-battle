#ifndef MATH_3D_H
#define	MATH_3D_H
#include <cstring>

struct Matrix3f {
    float m[9];
    Matrix3f() {
        memset(m, 0, sizeof(m)); 
        m[0] = m[4] = m[8] = 1;
    }
    void operator *=(const float& mod) {
        for (int i = 0; i < 9; i++) {
            m[i] *= mod;
        }
    }
    void operator /=(const float& mod) {
        if (mod == 0) {
            return;
        }
        for (int i = 0; i < 9; i++) {
            m[i] /= mod;
        }
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
