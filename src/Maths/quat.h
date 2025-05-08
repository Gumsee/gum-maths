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
    quat(mat<float, 3, 3> mat)
    {
        w = std::sqrt(1.0 + mat[0][0] + mat[1][1] + mat[2][2]) / 2.0;
        float w4 = (4.0 * w);
        x = (mat[1][2] - mat[2][1]) / w4;
        y = (mat[2][0] - mat[0][2]) / w4;
        z = (mat[0][1] - mat[1][0]) / w4;
    }

    void operator+=(const quat& q);
    void operator-=(const quat& q);
    void operator*=(const float& f);
    bool operator==(const quat& q);
    bool operator!=(const quat& q);

    quat operator+(quat q);
    quat operator-(quat q);
    quat operator*(quat q);
    //quat operator*(const float& f) const;
    quat operator/(const float& f) const;
    float operator[](int index);

    static float dot(quat a, quat b);
    static quat slerp(quat a, quat b, float f);
    static quat normalize(quat q);
    static quat rotateAround(float angle, vec3 up);
    static quat toQuaternion(vec3 anglesDeg);
    static vec3 toEuler(quat q);
    static quat rotateTowards(quat from, quat to, const float& speed);

    std::string toString(bool oneline = true, std::string prefix = "quat(", std::string suffix = ")", std::string delimiter = ", ") const;
    operator std::string() const { return toString(); }
};