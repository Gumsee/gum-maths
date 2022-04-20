#pragma once
#include "vec.h"
#include "mat.h"
#include "GumMathsClass.h"

struct quat
{
    float x, y, z, w;
    
    quat();
    quat(float f);
    quat(float sx, float sy, float sz, float sw);
    quat(vec3 vec, float sw);
    template<unsigned int N, unsigned int M>
    quat(mat<float, N, M> mat)
    {
        w = std::sqrt(1.0 + mat[0][0] + mat[1][1] + mat[2][2]) / 2.0;
        float w4 = (4.0 * w);
        x = (mat[2][1] - mat[1][2]) / w4;
        y = (mat[0][2] - mat[2][0]) / w4;
        z = (mat[1][0] - mat[0][1]) / w4;
    }

    void operator+=(quat q);
    void operator-=(quat q);

    quat operator+(quat q);
    quat operator-(quat q);
    quat operator*(quat q);
    quat operator/(float f);
    float operator[](int index);

    static float dot(quat a, quat b);
    static quat slerp(quat a, quat b, float f);
    static quat normalize(quat q);
    static quat rotateAround(float angle, vec3 up);
    static quat toQuaternion(vec3 anglesDeg);
    static vec3 toEuler(quat q);
    static quat rotateTowards(quat from, quat to, float speed);

    std::string toString(bool oneline = true, std::string prefix = "quat(", std::string suffix = ")", std::string delimiter = ", ");
};