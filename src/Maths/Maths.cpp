#include "Maths.h"
#include <math.h>

namespace Gum {
namespace Maths
{
    float toRadians(float deg)                { return deg * PI / 180; }
    float toDegree(float rad)                 { return rad * 180 / PI; }
    float sindeg(float deg)                   { return (float)sin(toRadians(deg)); }
    float cosdeg(float deg)                   { return (float)cos(toRadians(deg)); }
    float tandeg(float deg)                   { return (float)tan(toRadians(deg)); }
    double lerp(double t, double a, double b) { return a + t * (b - a); }
    double fade(double t)                     { return ((6*t - 15)*t + 10)*t*t*t; }
    float distance(float a, float b)          { return std::abs(a - b); }
    float mix(float a, float b, float f)      { return a * (1 - f) + b * f; }
    long factorial(int n)                     
    {
        long ret = 1;
        for(long i = 1; i <= n; i++)
            ret *= i;
        return ret;
    }
    double binomialCoeff(int n, int k) 
    { 
        return (double)factorial(n) / (double)(factorial(k) * factorial(n-k)); 
    }

    float randf(float from, float to)
    {
        float randval = (float)rand() / (float)RAND_MAX;
        return from + randval * (to - from);
    }
    //float noise(int seed)                     { randomGenerator.setSeed(seed); return randomGenerator.nextFloat(); }
}}