#include "MatrixFunctions.h"
#include "Maths.h"

namespace Gum {
namespace Maths
{
    mat4 createTransformationMatrix(vec3 translation, vec3 rotation, vec3 scale)
    {
        return translateMatrix(translation) * rotateMatrix(rotation) * scaleMatrix(scale);
    }

    mat4 createTransformationMatrix(vec3 translation, quat rotation, vec3 scale)
    {
        return translateMatrix(translation) * rotateMatrix(rotation) * scaleMatrix(scale);
    }


    mat4 perspective(float FOV, float aspectRatio, float near, float far)
    {
        mat4 perspectiveMat;
        float scale = 1.0f / tandeg(FOV * 0.5f); 
        perspectiveMat[0][0] = scale / aspectRatio;
        perspectiveMat[1][1] = scale;
        perspectiveMat[2][2] = -(far + near) / (far - near);
        perspectiveMat[3][2] = -(2 * far * near) / (far - near);
        perspectiveMat[2][3] = -1;
        perspectiveMat[3][3] = 0;
        return perspectiveMat;
    }
    
    mat4 ortho(float top, float right, float bottom, float left, float near, float far)
    {
        mat4 orthoMat;
        orthoMat[0][0] = 2.0f / (right - left);
        orthoMat[1][1] = 2.0f / (top - bottom);
        orthoMat[2][2] = -2.0f / (far - near);
        orthoMat[3][3] = 1.0f;

        orthoMat[3][0] = -(right + left) / (right - left);
        orthoMat[3][1] = -(top + bottom) / (top - bottom);
        orthoMat[3][2] = -(far + near) / (far - near);

        return orthoMat;
    }


    mat4 view(const vec3& eye, const vec3& position, const vec3& up)
    {
        vec3 zaxis = vec3::normalize(position - eye);    
        vec3 xaxis = vec3::normalize(vec3::cross(zaxis, up));
        vec3 yaxis = vec3::cross(xaxis, zaxis);

        zaxis *= -1.0f;

        mat4 viewMatrix(
            xaxis.x, xaxis.y, xaxis.z, -vec3::dot(xaxis, eye),
            yaxis.x, yaxis.y, yaxis.z, -vec3::dot(yaxis, eye),
            zaxis.x, zaxis.y, zaxis.z, -vec3::dot(zaxis, eye),
            0, 0, 0, 1
        );

        return viewMatrix;
    }


    mat4 translateMatrix(vec3 transVector)
    {
        mat4 mat;
        mat[3][0] = transVector.x;
        mat[3][1] = transVector.y;
        mat[3][2] = transVector.z;
        return mat;
    }

    mat4 scaleMatrix(vec3 scaleVector)
    {
        mat4 mat;
        mat[0][0] = scaleVector.x;
        mat[1][1] = scaleVector.y;
        mat[2][2] = scaleVector.z;
        return mat;
    }


    mat4 rotateMatrix(vec3 deg)
    {
        mat4 retmat, matx, maty, matz;
        vec3 rad = vec3::rad(deg);

        matz[0][0] =  cos(rad.z);
        matz[1][0] = -sin(rad.z);
        matz[0][1] =  sin(rad.z);
        matz[1][1] =  cos(rad.z);
        retmat *= matz;
        
        maty[0][0] =  cos(rad.y);
        maty[2][0] =  sin(rad.y);
        maty[0][2] = -sin(rad.y);
        maty[2][2] =  cos(rad.y);
        retmat *= maty;

        matx[1][1] =  cos(rad.x);
        matx[2][2] =  cos(rad.x);
        matx[2][1] = -sin(rad.x);
        matx[1][2] =  sin(rad.x);
        retmat *= matx;
        
        return retmat;
    }

    mat4 rotateMatrix(quat q)
    {
        mat4 retmat;

        float qxx = q.x * q.x;
        float qyy = q.y * q.y;
        float qzz = q.z * q.z;
        float qxz = q.x * q.z;
        float qxy = q.x * q.y;
        float qyz = q.y * q.z;
        float qwx = q.w * q.x;
        float qwy = q.w * q.y;
        float qwz = q.w * q.z;

        retmat[0][0] = 1.0f - 2.0f * (qyy +  qzz);
        retmat[0][1] = 2.0f * (qxy + qwz);
        retmat[0][2] = 2.0f * (qxz - qwy);
        retmat[1][0] = 2.0f * (qxy - qwz);
        retmat[1][1] = 1.0f - 2.0f * (qxx +  qzz);
        retmat[1][2] = 2.0f * (qyz + qwx);
        retmat[2][0] = 2.0f * (qxz + qwy);
        retmat[2][1] = 2.0f * (qyz - qwx);
        retmat[2][2] = 1.0f - 2.0f * (qxx +  qyy);
        return retmat;
    }
}}