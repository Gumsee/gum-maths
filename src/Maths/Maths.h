#pragma once
#include "vec.h"
#include "mat.h"
#include "ColorFunctions.h"
#include "MatrixFunctions.h"
#include <vector>

#define PI 3.14159265358979

namespace Gum {
namespace Maths
{
	extern float barryCentric(vec3 p1, vec3 p2, vec3 p3, vec2 pos);

    //Conversions
    extern float toRadians(float deg);
    extern float toDegree(float rad);
    extern float sindeg(float deg);
    extern float cosdeg(float deg);
    extern float tandeg(float deg);

    //Math Functions
    extern float clamp(float &var, float min, float max);
    extern float mix(float a, float b, float f);
    extern float randf(float from, float to);
    extern float noise(int seed);
    extern double lerp(double t, double a, double b);
    extern double fade(double t);
    extern float distance(float a, float b);

    //Vector Operations

    //Sorting
    template<typename T> 
    static T max(std::vector<T> vars)
    {
        T maxval = T(0);
        for(T t : vars)
        {
            if(t > maxval) 
                maxval = t;
        }
        return maxval;
    }
}}