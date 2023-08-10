#pragma once
#include "mat.h"
#include "quat.h"

namespace Gum {
namespace Maths
{
    extern mat4 createTransformationMatrix(vec3 translation, vec3 rotation, vec3 scale);
    extern mat4 createTransformationMatrix(vec3 translation, quat rotation, vec3 scale);
    extern mat3 createTransformationMatrix(vec2 translation, float rotation, vec2 scale);


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

    extern mat4 translateMatrix(vec3 transVector);
    extern mat3 translateMatrix(vec2 transVector);
    extern mat4 scaleMatrix(vec3 scaleVector);
    extern mat3 scaleMatrix(vec2 scaleVector);
    extern mat4 rotateMatrix(vec3 deg);
    extern mat4 rotateMatrix(quat q);
    extern mat3 rotateMatrix(float deg);
}}