#include "MatrixFunctions.h"
#include "Maths.h"

namespace Gum {
namespace Maths
{
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
}}