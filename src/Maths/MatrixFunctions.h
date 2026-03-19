#pragma once
#include "mat.h"
#include "quat.h"

namespace Gum {
namespace Maths
{
    /**
     * | (1 / tan(FOV / 2)) / aspectratio          0                         0                                  0               |
     * |                  0                 1 / tan(FOV / 2)                 0                                  0               |
     * |                  0                        0           -(far + near) / (far - near)    -(2 * far * near) / (far - near) |
     * |                  0                        0                        -1                                  0               |
     * @param FOV
     * @param aspectRatio
     * @param near
     * @param far
     * @return
     */
    extern mat4 perspective(float FOV, float aspectRatio, float near, float far);
    extern mat4 ortho(float top, float right, float bottom, float left, float near, float far);
    extern mat4 view(const vec3& eye, const vec3& position, const vec3& up);

    template<typename T>
    static mat<T,4,4> translateMatrix(tvec<T, 3> transVector)
    {
        mat<T,4,4> mat;
        mat[3][0] = (T)transVector.x;
        mat[3][1] = (T)transVector.y;
        mat[3][2] = (T)transVector.z;
        return mat;
    }

    template<typename T>
    static mat<T,3,3> translateMatrix(tvec<T, 2> transVector)
    {
        mat<T,3,3> mat;
        mat[2][0] = (T)transVector.x;
        mat[2][1] = (T)transVector.y;
        return mat;
    }

    template<typename T>
    static mat<T,4,4> scaleMatrix(tvec<T, 3> scaleVector)
    {
        mat<T,4,4> mat;
        mat[0][0] = (T)scaleVector.x;
        mat[1][1] = (T)scaleVector.y;
        mat[2][2] = (T)scaleVector.z;
        return mat;
    }

    template<typename T>
    static mat<T,3,3> scaleMatrix(tvec<T, 2> scaleVector)
    {
        mat<T,3,3> mat;
        mat[0][0] = (T)scaleVector.x;
        mat[1][1] = (T)scaleVector.y;
        return mat;
    }


    template<typename T>
    static mat<T,4,4> rotateMatrix(tvec<T, 3> deg)
    {
        mat<T,4,4> retmat, matx, maty, matz;
        tvec<T, 3> rad = tvec<T, 3>::rad(deg);

        matz[0][0] = (T) cos(rad.z);
        matz[1][0] = (T)-sin(rad.z);
        matz[0][1] = (T) sin(rad.z);
        matz[1][1] = (T) cos(rad.z);
        retmat *= matz;
        
        maty[0][0] = (T) cos(rad.y);
        maty[2][0] = (T) sin(rad.y);
        maty[0][2] = (T)-sin(rad.y);
        maty[2][2] = (T) cos(rad.y);
        retmat *= maty;

        matx[1][1] = (T) cos(rad.x);
        matx[2][2] = (T) cos(rad.x);
        matx[2][1] = (T)-sin(rad.x);
        matx[1][2] = (T) sin(rad.x);
        retmat *= matx;
        
        return retmat;
    }

    template<typename T>
    static mat<T,4,4> rotateMatrix(quat<T> q)
    {
        mat<T,4,4> retmat;

        T qxx = q.x * q.x;
        T qyy = q.y * q.y;
        T qzz = q.z * q.z;
        T qxz = q.x * q.z;
        T qxy = q.x * q.y;
        T qyz = q.y * q.z;
        T qwx = q.w * q.x;
        T qwy = q.w * q.y;
        T qwz = q.w * q.z;

        retmat[0][0] = (T)1.0 - (T)2.0 * (qyy +  qzz);
        retmat[0][1] = (T)2.0 * (qxy + qwz);
        retmat[0][2] = (T)2.0 * (qxz - qwy);
        retmat[1][0] = (T)2.0 * (qxy - qwz);
        retmat[1][1] = (T)1.0 - (T)2.0 * (qxx +  qzz);
        retmat[1][2] = (T)2.0 * (qyz + qwx);
        retmat[2][0] = (T)2.0 * (qxz + qwy);
        retmat[2][1] = (T)2.0 * (qyz - qwx);
        retmat[2][2] = (T)1.0 - (T)2.0 * (qxx +  qyy);
        return retmat;
    }

    template<typename T>
    static mat<T,3,3> rotateMatrix(T q)
    {
        mat<T,3,3> retmat;
        retmat[0][0] = (T) cos(q);
        retmat[1][0] = (T)-sin(q);
        retmat[0][1] = (T) sin(q);
        retmat[1][1] = (T) cos(q);
        return retmat;
    }

    template<typename T>
    static quat<T> rotationFromMatrix(mat<T,4,4> m)
    {
        T sx = tvec<T, 3>(m[0][0], m[0][1], m[0][2]).length();
        T sy = tvec<T, 3>(m[1][0], m[1][1], m[1][2]).length();
        T sz = tvec<T, 3>(m[2][0], m[2][1], m[2][2]).length();

        mat<T,3,3U> rot {
            m[0][0] / sx, m[1][0] / sy, m[2][0] / sz,
            m[0][1] / sx, m[1][1] / sy, m[2][1] / sz,
            m[0][2] / sx, m[1][2] / sy, m[2][2] / sz
        };
        return quat<T>(rot);
    }

    template<typename T>
    static tvec<T, 3> positionFromMatrix(mat<T,4,4> m)
    {
        return tvec<T, 3>(m[3][0], m[3][1], m[3][2]);
    }

    template<typename T>
    static tvec<T, 3> scaleFromMatrix(mat<T,4,4> m)
    {
        T sx = tvec<T, 3>(m[0][0], m[0][1], m[0][2]).length();
        T sy = tvec<T, 3>(m[1][0], m[1][1], m[1][2]).length();
        T sz = tvec<T, 3>(m[2][0], m[2][1], m[2][2]).length();
        return tvec<T, 3>(sx, sy, sz);
    }


    template<typename T>
    static mat<T,4,4> createTransformationMatrix(tvec<T, 3> translation, tvec<T, 3> rotation, tvec<T, 3> scale)
    {
        return translateMatrix<T>(translation) * rotateMatrix<T>(quat<T>::toQuaternion(rotation)) * scaleMatrix<T>(scale);
    }

    template<typename T>
    static mat<T,4,4> createTransformationMatrix(tvec<T, 3> translation, quat<T> rotation, tvec<T, 3> scale)
    {
        return translateMatrix<T>(translation) * rotateMatrix<T>(rotation) * scaleMatrix<T>(scale);
    }

    template<typename T>
    static mat<T,3,3> createTransformationMatrix(tvec<T, 2> translation, T rotation, tvec<T, 2> scale)
    {
        return translateMatrix<T>(translation) * rotateMatrix<T>(rotation) * scaleMatrix<T>(scale);
    }

    template<typename T>
    static mat<T,4,4> inverseTransformationMatrix(mat<T,4,4> m)
    {
      mat<T,3,3> rotation = mat<T,3,3>::transpose(mat<T,3,3>(m));
      tvec<T,3> translation = rotation * -positionFromMatrix(m);
      tvec<T,3> scale = scaleFromMatrix(m);
      mat4 rotation44(rotation);
      rotation44[3][3] = (T)1.0;

      return translateMatrix<T>(translation) * rotation44 * scaleMatrix<T>(scale);
    }
}}