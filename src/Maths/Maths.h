#pragma once
#include <iomanip>
#include <limits>
#include <string>
#include <vector>
#include <sstream>

#define PI 3.14159265358979

namespace Gum {
namespace Maths
{
    //Conversions
    extern float toRadians(float deg);
    extern float toDegree(float rad);
    extern float sindeg(float deg);
    extern float cosdeg(float deg);
    extern float tandeg(float deg);
    
    template<typename T>
    static std::string numToString(const T& num, const unsigned short& precision = std::numeric_limits<unsigned short>::max())
    {
        if(precision < std::numeric_limits<unsigned short>::max())
        {
            std::stringstream sstream;
            sstream << std::fixed << std::setprecision(precision) << num;
            return sstream.str();
        }

        return std::to_string(num);
    }

    //Math Functions
    template<typename T>
    static T clamp(const T var, const T min, const T max)
    {   
        static_assert(std::is_arithmetic<T>::value, "Maths::clamp: T must be numeric");
        
        if(var < min) { return min; }
        if(var > max) { return max; }
        return var;
    }
    extern float mix(float a, float b, float f);
    extern float randf(float from, float to);
    extern float noise(int seed);
    extern double lerp(double t, double a, double b);
    extern double fade(double t);
    extern float distance(float a, float b);
    extern long factorial(int n);
    extern double binomialCoeff(int n, int k);
    extern float fract(float f);
    extern float smoothstep (float edge0, float edge1, float x);
    extern float inversesqrt(float x);

    template<typename T> 
    static int sign(T val) 
    {
        return (T(0) < val) - (val < T(0));
    }


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

    template<typename T> 
    static T min(T x, T y)
    {
        if(x > y) 
            return y;
        return x;
    }
}}