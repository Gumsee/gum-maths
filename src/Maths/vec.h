#pragma once
#include "GumMathsClass.h"
#include <string>
#include <cstring>
#include <cmath>
#include <array>
#include <type_traits>
#include <utility>
#include <iostream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

//VS:
//#pragma warning(disable: 4201)
//#pragma warning( push )
#define VEC_TEMPLATE_CONSTRUCTORS(size, t) \
    tvec()                  { for(unsigned int i = 0; i < size; i++) vals[i] = 0; } \
    tvec(const T& f)        { for(unsigned int i = 0; i < size; i++) vals[i] = (T)f; } \
    \
    template<typename TT, unsigned int SS> \
    tvec(tvec<TT, SS, t> vvec) { for(unsigned int i = 0; i < (size < SS ? size : SS); i++) vals[i] = (T)vvec[i]; } \
    \
    template <typename... Args, typename = typename std::enable_if<sizeof...(Args) == size>::type> \
    tvec(Args&&... values)  { T args[] = { (T)values... }; memcpy(&vals[0], &args[0], size * sizeof(T)); } \
    \
    template <typename TT, unsigned int SS, typename... Args, typename = typename std::enable_if<sizeof...(Args) == size - SS>::type> \
    tvec(tvec<TT, SS, t> vvec, Args&&... values)  \
    { \
        static_assert(SS < size, "Passed vector is too large"); \
        for(unsigned int i = 0; i < SS; i++) vals[i] = (T)vvec[i]; \
    \
        T args[] = { (T)values... }; \
        memcpy(&vals[SS], &args[0], (size - SS) * sizeof(T)); \
    }

#define VEC_TEMPLATE_OPERATORS(size, type) \
    template<typename TT> void operator+=(const tvec<TT, size, type>& vvec)                      { for(unsigned int i = 0; i < size; i++) vals[i] += vvec.vals[i]; } \
    template<typename TT> void operator-=(const tvec<TT, size, type>& vvec)                      { for(unsigned int i = 0; i < size; i++) vals[i] -= vvec.vals[i]; } \
    template<typename TT> void operator/=(const tvec<TT, size, type>& vvec)                      { for(unsigned int i = 0; i < size; i++) vals[i] /= vvec.vals[i]; } \
    template<typename TT> void operator*=(const tvec<TT, size, type>& vvec)                      { for(unsigned int i = 0; i < size; i++) vals[i] *= vvec.vals[i]; } \
    template<typename TT> void operator*=(const TT& f)                                           { for(unsigned int i = 0; i < size; i++) vals[i] *= f; } \
    template<typename TT> bool operator!=(const tvec<TT, size, type>& vvec) const                { for(unsigned int i = 0; i < size; i++) if(vals[i] != vvec.vals[i]) { return true; } return false; } \
    template<typename TT> bool operator==(const tvec<TT, size, type>& vvec) const                { for(unsigned int i = 0; i < size; i++) if(vals[i] == vvec.vals[i]) { return true; } return false; } \
    template<typename TT> bool operator==(const TT& f) const                                     { for(unsigned int i = 0; i < size; i++) if(vals[i] == f)            { return true; } return false; } \
    \
    template<typename TT> tvec<T, size, type>  operator+(const tvec<TT, size, type>& vvec) const { tvec<T, size, type>  nvec; for(unsigned int i = 0; i < size; i++) nvec.vals[i] = vals[i] + vvec.vals[i]; return nvec; } \
    template<typename TT> tvec<T, size, type>  operator-(const tvec<TT, size, type>& vvec) const { tvec<T, size, type>  nvec; for(unsigned int i = 0; i < size; i++) nvec.vals[i] = vals[i] - vvec.vals[i]; return nvec; } \
    template<typename TT> tvec<T, size, type>  operator/(const tvec<TT, size, type>& vvec) const { tvec<T, size, type>  nvec; for(unsigned int i = 0; i < size; i++) nvec.vals[i] = vals[i] / vvec.vals[i]; return nvec; } \
    template<typename TT> tvec<T, size, type>  operator*(const tvec<TT, size, type>& vvec) const { tvec<T, size, type>  nvec; for(unsigned int i = 0; i < size; i++) nvec.vals[i] = vals[i] * vvec.vals[i]; return nvec; } \
    template<typename TT> tvec<T, size, type>  operator/(const TT& f) const                      { tvec<T, size, type>  nvec; for(unsigned int i = 0; i < size; i++) nvec.vals[i] = vals[i] / f;            return nvec; } \
    template<typename TT> tvec<TT, size, type> operator*(const TT& f) const                      { tvec<TT, size, type> nvec; for(unsigned int i = 0; i < size; i++) nvec.vals[i] = vals[i] * f;            return nvec; } \
    template<typename TT, unsigned int SS>  \
                          void           operator=(const tvec<TT, SS, type>& vvec)         { for(unsigned int i = 0; i < (size < SS ? size : SS); i++) vals[i] = (T)vvec.vals[i]; } \
                          /*void    operator=(tvec<T, size, type> vvec)        { for(unsigned int i = 0; i < size; i++) vals[i] = vvec.vals[i]; }*/ \
    \
    T& operator[](unsigned int& index)             { return vals[index]; } \
    const T& operator[](unsigned int& index) const { return vals[index]; } \
    const T& at(unsigned int& index) const         { return vals[index]; }
    /*std::ostream& operator<<(std::ostream& os, const T& obj) { return os << obj.toString(); }*/

#define VEC_TEMPLATE_LENGTH_FUNC(size, type) \
    float length() \
    { \
        T sum = 0; \
        for(unsigned int i = 0; i < size; i++) \
            sum += vals[i] * vals[i]; \
        return (float)sqrt(sum); \
    }

#define VEC_TEMPLATE_ABS_FUNC(size, type) \
    template<typename TT> \
    static tvec<T, size, type> abs(tvec<TT, size, type> vvec) \
    { \
        tvec<T, size, type> ret; \
        for(unsigned int i = 0; i < size; i++) \
            ret[i] = std::abs(vvec.vals[i]); \
        return ret; \
    }

#define VEC_TEMPLATE_DEG_FUNC(size, type) \
    template<typename TT> \
    static tvec<T, size, type> deg(tvec<TT, size, type> vvec) \
    { \
        tvec<T, size, type> ret; \
        for(unsigned int i = 0; i < size; i++) \
            ret[i] = (vvec.vals[i] * 180.f) / GUM_PI; \
        return ret; \
    }

#define VEC_TEMPLATE_RAD_FUNC(size, type) \
    template<typename TT> \
    static tvec<T, size, type> rad(tvec<TT, size, type> vvec) \
    { \
        tvec<T, size, type> ret; \
        for(unsigned int i = 0; i < size; i++) \
            ret[i] = vvec.vals[i] * GUM_PI / 180.0f; \
        return ret; \
    }

#define VEC_TEMPLATE_CLAMP_FUNC(size, type) \
    template<typename TT> \
    static tvec<T, size, type> clamp(tvec<TT, size, type> vvec, float min, float max) \
    { \
        tvec<TT, size, type> ret; \
        for(unsigned int i = 0; i < size; i++) \
        { \
            if     (vvec.vals[i] < min) { ret[i] = min; }  \
            else if(vvec.vals[i] > max) { ret[i] = max; }  \
            else                  { ret[i] = vvec.vals[i]; } \
        } \
        return ret; \
    }

#define VEC_TEMPLATE_CROSS_FUNC(size, type) \
    template<typename TT>  \
    static tvec<T, size, type> cross(tvec<TT, size, type> a, tvec<TT, size, type> b)  \
    {  \
        if(size == 3)      { return tvec<T, size, type>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); } \
        else if(size == 7) { return tvec<T, size, type>(); } /*TODO*/ \
        else               { std::cerr << "GumMaths: Crossproduct not defined for " + std::to_string(size) + "-dimensional vectors." << std::endl; return tvec<T, size, type>(); } \
    }

#define VEC_TEMPLATE_NORMALIZE_FUNC(size, type) \
    template<typename TT>  \
    static tvec<T, size, type> normalize(tvec<TT, size, type> vvec) \
    {  \
        T length_of_v = (T)vvec.length();  \
        tvec<T, size, type> ret; \
        for(unsigned int i = 0; i < size; i++) \
            ret[i] = (T)vvec.vals[i] / length_of_v; \
        return ret; \
    }

#define VEC_TEMPLATE_RANDOM_FUNC(size, type) \
    template<typename TT>  \
    static tvec<T, size, type> random(tvec<TT, size, type> from, tvec<TT, size, type> to) \
    {  \
        tvec<T, size, type> ret; \
        for(unsigned int i = 0; i < size; i++) \
        { \
            T randval = (T)rand() / (T)RAND_MAX; \
            ret[i] = from[i] + randval * (to[i] - from[i]); \
        } \
        return ret; \
    }

#define VEC_TEMPLATE_DOT_FUNC(size, type) \
    template<typename TT> \
    static float dot(tvec<TT, size, type> a, tvec<TT, size, type> b)  \
    { \
        float ret = 0.0f; \
        for(unsigned int i = 0; i < size; i++) \
            ret += a[i] * b[i]; \
        return ret;  \
    }

#define VEC_TEMPLATE_MIX_FUNC(size, type) \
    template<typename TT> \
    static tvec<TT, size, type> mix(tvec<TT, size, type> a, tvec<TT, size, type> b, float factor)  \
    {  \
        return a * (1 - factor) + b * factor;  \
    }

#define VEC_TEMPLATE_DISTANCE_FUNC(size, type) \
    template<typename TT>  \
    static float distance(tvec<TT, size, type> a, tvec<TT, size, type> b)  \
    {  \
        return (a - b).length();  \
    }

#define VEC_TEMPLATE_TO_STRING_FUNC(size, type, name) \
    std::string toString(std::string prefix = std::string(name) + "(", std::string suffix = ")", std::string delimiter = ", ") const \
    {  \
        std::string str = prefix; \
        for(unsigned int i = 0; i < size; i++) \
            str += std::to_string(vals[i]) + delimiter; \
        str = str.substr(0, str.length() - delimiter.length()); \
        str += suffix; \
        return str; \
    }

#define VEC_TEMPLATE(size, name, type, ...) \
    union { \
        T vals[size] = {0}; \
        __VA_ARGS__; \
    }; \
    \
    VEC_TEMPLATE_CONSTRUCTORS(size, type) \
    VEC_TEMPLATE_OPERATORS(size, type) \
    VEC_TEMPLATE_LENGTH_FUNC(size, type) \
    VEC_TEMPLATE_ABS_FUNC(size, type) \
    VEC_TEMPLATE_CLAMP_FUNC(size, type) \
    VEC_TEMPLATE_CROSS_FUNC(size, type) \
    VEC_TEMPLATE_NORMALIZE_FUNC(size, type) \
    VEC_TEMPLATE_DOT_FUNC(size, type) \
    VEC_TEMPLATE_DEG_FUNC(size, type) \
    VEC_TEMPLATE_RAD_FUNC(size, type) \
    VEC_TEMPLATE_MIX_FUNC(size, type) \
    VEC_TEMPLATE_DISTANCE_FUNC(size, type) \
    VEC_TEMPLATE_TO_STRING_FUNC(size, type, name) \
    VEC_TEMPLATE_RANDOM_FUNC(size, type) \
    \
    unsigned int dim() \
    { \
        return size; \
    }

template<typename T, unsigned int S, unsigned int type = 0U>
struct tvec
{
    VEC_TEMPLATE(S, "vec" + std::to_string(S), type, struct{ T x; }; struct{ T r; });
};

template<typename T>
struct tvec<T, 2, 0U>
{
    VEC_TEMPLATE(2, "vec2", 0U, struct{ T x, y; }; struct{ T r, g; });
};
template<typename T>
struct tvec<T, 3, 0U>
{
    VEC_TEMPLATE(3, "vec3", 0U, struct{ T x, y, z; }; struct{ T r, g, b; });
};
template<typename T>
struct tvec<T, 4, 0U>
{
    VEC_TEMPLATE(4, "vec4", 0U, struct{ T x, y, z, w; }; struct{ T r, g, b, a; });
};

template<typename T>
struct tvec<T, 3, 1U>
{
    VEC_TEMPLATE(3, "rgb", 1U, struct{ T r, g, b; });
};
template<typename T>
struct tvec<T, 4, 1U>
{
    VEC_TEMPLATE(4, "rgba", 1U, struct{ T r, g, b, a; });
};

template<typename T>
struct tvec<T, 3, 2U>
{
    VEC_TEMPLATE(3, "hsv", 2U, struct{ T h, s, v; });
};
template<typename T>
struct tvec<T, 4, 2U>
{
    VEC_TEMPLATE(4, "hsva", 2U, struct{ T h, s, v, a; });
};

typedef tvec<float,             2>   vec2;
typedef tvec<int,               2>  ivec2;
typedef tvec<unsigned int,      2> uivec2;
typedef tvec<double,            2>  dvec2;
typedef tvec<bool,              2>  bvec2;
typedef tvec<float,             3>   vec3;
typedef tvec<int,               3>  ivec3;
typedef tvec<unsigned int,      3> uivec3;
typedef tvec<double,            3>  dvec3;
typedef tvec<bool,              3>  bvec3;
typedef tvec<float,             4>   vec4;
typedef tvec<int,               4>  ivec4;
typedef tvec<unsigned int,      4> uivec4;
typedef tvec<double,            4>  dvec4;
typedef tvec<bool,              4>  bvec4;
typedef tvec<unsigned short int, 3, 1>    rgb;
typedef tvec<unsigned short int, 4, 1>   rgba;
typedef tvec<unsigned short int, 3, 2>    hsv;
typedef tvec<unsigned short int, 4, 2>   hsva;

//#pragma warning( pop )
#pragma GCC diagnostic pop