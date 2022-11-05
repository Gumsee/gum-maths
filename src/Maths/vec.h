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
#define VEC_TEMPLATE_CONSTRUCTORS(size) \
    tvec()                 { for(unsigned int i = 0; i < size; i++) v[i] = 0; } \
    tvec(const T& f)            { for(unsigned int i = 0; i < size; i++) v[i] = (T)f; } \
    \
    template<typename TT, unsigned int SS> \
    tvec(tvec<TT, SS> vvec) { for(unsigned int i = 0; i < (size < SS ? size : SS); i++) v[i] = (T)vvec[i]; } \
    \
    template <typename... Args, typename = typename std::enable_if<sizeof...(Args) == size>::type> \
    tvec(Args&&... values) { T args[] = { (T)values... }; memcpy(&v[0], &args[0], size * sizeof(T)); } \
    \
    template <typename TT, unsigned int SS, typename... Args, typename = typename std::enable_if<sizeof...(Args) == size - SS>::type> \
    tvec(tvec<TT, SS> vvec, Args&&... values)  \
    { \
        static_assert(SS < size, "Passed vector is too large"); \
        for(unsigned int i = 0; i < SS; i++) v[i] = (T)vvec[i]; \
    \
        T args[] = { (T)values... }; \
        memcpy(&v[SS], &args[0], (size - SS) * sizeof(T)); \
    }

#define VEC_TEMPLATE_OPERATORS(size) \
    template<typename TT> void operator+=(const tvec<TT, size>& vvec)        { for(unsigned int i = 0; i < size; i++) v[i] += vvec.v[i]; } \
    template<typename TT> void operator-=(const tvec<TT, size>& vvec)        { for(unsigned int i = 0; i < size; i++) v[i] -= vvec.v[i]; } \
    template<typename TT> void operator/=(const tvec<TT, size>& vvec)        { for(unsigned int i = 0; i < size; i++) v[i] /= vvec.v[i]; } \
    template<typename TT> void operator*=(const tvec<TT, size>& vvec)        { for(unsigned int i = 0; i < size; i++) v[i] *= vvec.v[i]; } \
    template<typename TT> void operator*=(const TT& f)                       { for(unsigned int i = 0; i < size; i++) v[i] *= f; } \
    template<typename TT> bool operator!=(const tvec<TT, size>& vvec) const  { for(unsigned int i = 0; i < size; i++) if(v[i] != vvec.v[i]) return true; return false; } \
    template<typename TT> bool operator==(const tvec<TT, size>& vvec) const  { for(unsigned int i = 0; i < size; i++) if(v[i] == vvec.v[i]) return true; return false; } \
    \
    template<typename TT> tvec<T, size>  operator+(const tvec<TT, size>& vvec) const { tvec<T, size>  nvec; for(unsigned int i = 0; i < size; i++) nvec.v[i] = v[i] + vvec.v[i]; return nvec; } \
    template<typename TT> tvec<T, size>  operator-(tvec<TT, size> const& vvec) const { tvec<T, size>  nvec; for(unsigned int i = 0; i < size; i++) nvec.v[i] = v[i] - vvec.v[i]; return nvec; } \
    template<typename TT> tvec<T, size>  operator/(const tvec<TT, size>& vvec) const { tvec<T, size>  nvec; for(unsigned int i = 0; i < size; i++) nvec.v[i] = v[i] / vvec.v[i]; return nvec; } \
    template<typename TT> tvec<T, size>  operator*(const tvec<TT, size>& vvec) const { tvec<T, size>  nvec; for(unsigned int i = 0; i < size; i++) nvec.v[i] = v[i] * vvec.v[i]; return nvec; } \
    template<typename TT> tvec<T, size>  operator/(const TT& f) const                { tvec<T, size>  nvec; for(unsigned int i = 0; i < size; i++) nvec.v[i] = v[i] / f;         return nvec; } \
    template<typename TT> tvec<TT, size> operator*(const TT& f) const                { tvec<TT, size> nvec; for(unsigned int i = 0; i < size; i++) nvec.v[i] = v[i] * f;         return nvec; } \
    template<typename TT> void           operator=(const TT& f)                      { for(unsigned int i = 0; i < size; i++) v[i] = (T)f; } \
    template<typename TT, unsigned int SS>  \
                          void           operator=(const tvec<TT, SS>& vvec)   { for(unsigned int i = 0; i < (size < SS ? size : SS); i++) v[i] = (T)vvec.v[i]; } \
                          /*void    operator=(tvec<T, size> vvec)        { for(unsigned int i = 0; i < size; i++) v[i] = vvec.v[i]; }*/ \
    \
    T& operator[](unsigned int& index) { return v[index]; } \
    const T& operator[](unsigned int& index) const { return v[index]; }
    /*std::ostream& operator<<(std::ostream& os, const T& obj) { return os << obj.toString(); }*/

#define VEC_TEMPLATE_LENGTH_FUNC(size) \
    float length() \
    { \
        T sum = 0; \
        for(unsigned int i = 0; i < size; i++) \
            sum += v[i] * v[i]; \
        return (float)sqrt(sum); \
    }

#define VEC_TEMPLATE_ABS_FUNC(size) \
    template<typename TT> \
    static tvec<T, size> abs(tvec<TT, size> vvec) \
    { \
        tvec<T, size> ret; \
        for(unsigned int i = 0; i < size; i++) \
            ret[i] = std::abs(vvec.v[i]); \
        return ret; \
    }

#define VEC_TEMPLATE_DEG_FUNC(size) \
    template<typename TT> \
    static tvec<T, size> deg(tvec<TT, size> vvec) \
    { \
        tvec<T, size> ret; \
        for(unsigned int i = 0; i < size; i++) \
            ret[i] = (vvec.v[i] * 180.f) / GUM_PI; \
        return ret; \
    }

#define VEC_TEMPLATE_RAD_FUNC(size) \
    template<typename TT> \
    static tvec<T, size> rad(tvec<TT, size> vvec) \
    { \
        tvec<T, size> ret; \
        for(unsigned int i = 0; i < size; i++) \
            ret[i] = vvec.v[i] * GUM_PI / 180.0f; \
        return ret; \
    }

#define VEC_TEMPLATE_CLAMP_FUNC(size) \
    template<typename TT> \
    static tvec<T, size> clamp(tvec<TT, size> vvec, float min, float max) \
    { \
        tvec<TT, size> ret; \
        for(unsigned int i = 0; i < size; i++) \
        { \
            if     (vvec.v[i] < min) { ret[i] = min; }  \
            else if(vvec.v[i] > max) { ret[i] = max; }  \
            else                  { ret[i] = vvec.v[i]; } \
        } \
        return ret; \
    }

#define VEC_TEMPLATE_CROSS_FUNC(size) \
    template<typename TT>  \
    static tvec<T, size> cross(tvec<TT, size> a, tvec<TT, size> b)  \
    {  \
        if(size == 3)      { return tvec<T, size>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); } \
        else if(size == 7) { return tvec<T, size>(); } /*TODO*/ \
        else               { std::cerr << "GumMaths: Crossproduct not defined for " + std::to_string(size) + "-dimensional vectors." << std::endl; return tvec<T, size>(); } \
    }

#define VEC_TEMPLATE_NORMALIZE_FUNC(size) \
    template<typename TT>  \
    static tvec<TT, size> normalize(tvec<TT, size> vvec) \
    {  \
        float length_of_v = vvec.length();  \
        tvec<TT, size> ret; \
        for(unsigned int i = 0; i < size; i++) \
            ret[i] = vvec.v[i] / length_of_v; \
        return ret; \
    }

#define VEC_TEMPLATE_DOT_FUNC(size) \
    template<typename TT> \
    static float dot(tvec<TT, size> a, tvec<TT, size> b)  \
    { \
        float ret = 0.0f; \
        for(unsigned int i = 0; i < size; i++) \
            ret += a[i] * b[i]; \
        return ret;  \
    }

#define VEC_TEMPLATE_MIX_FUNC(size) \
    template<typename TT> \
    static tvec<TT, size> mix(tvec<TT, size> a, tvec<TT, size> b, float factor)  \
    {  \
        return a * (1 - factor) + b * factor;  \
    }

#define VEC_TEMPLATE_DISTANCE_FUNC(size) \
    template<typename TT>  \
    static float distance(tvec<TT, size> a, tvec<TT, size> b)  \
    {  \
        return (a - b).length();  \
    }

#define VEC_TEMPLATE_TO_STRING_FUNC(size) \
    std::string toString(std::string prefix = "vec"+std::to_string(size)+"(", std::string suffix = ")", std::string delimiter = ", ") const \
    {  \
        std::string str = prefix; \
        for(unsigned int i = 0; i < size; i++) \
            str += std::to_string(v[i]) + delimiter; \
        str = str.substr(0, str.length() - delimiter.length()); \
        str += suffix; \
        return str; \
    }

#define VEC_TEMPLATE(size, ...) \
    union { \
        T v[size]; \
        __VA_ARGS__; \
    }; \
    \
    VEC_TEMPLATE_CONSTRUCTORS(size) \
    VEC_TEMPLATE_OPERATORS(size) \
    VEC_TEMPLATE_LENGTH_FUNC(size) \
    VEC_TEMPLATE_ABS_FUNC(size) \
    VEC_TEMPLATE_CLAMP_FUNC(size) \
    VEC_TEMPLATE_CROSS_FUNC(size) \
    VEC_TEMPLATE_NORMALIZE_FUNC(size) \
    VEC_TEMPLATE_DOT_FUNC(size) \
    VEC_TEMPLATE_DEG_FUNC(size) \
    VEC_TEMPLATE_RAD_FUNC(size) \
    VEC_TEMPLATE_MIX_FUNC(size) \
    VEC_TEMPLATE_DISTANCE_FUNC(size) \
    VEC_TEMPLATE_TO_STRING_FUNC(size) \
    \
    unsigned int dim() \
    { \
        return size; \
    }

template<typename T, unsigned int S>
struct tvec
{
    VEC_TEMPLATE(S, struct{ T x; }; struct{ T r; });
};

template<typename T>
struct tvec<T, 2>
{
    VEC_TEMPLATE(2, struct{ T x, y; }; struct{ T r, g; });
};
template<typename T>
struct tvec<T, 3>
{
    VEC_TEMPLATE(3, struct{ T x, y, z; }; struct{ T r, g, b; });
};
template<typename T>
struct tvec<T, 4>
{
    VEC_TEMPLATE(4, struct{ T x, y, z, w; }; struct{ T r, g, b, a; });
};

typedef tvec<float,   2>   vec2;
typedef tvec<int,     2>  ivec2;
typedef tvec<unsigned int,    2> uivec2;
typedef tvec<double,  2>  dvec2;
typedef tvec<float,   3>   vec3;
typedef tvec<int,     3>  ivec3;
typedef tvec<unsigned int,    3> uivec3;
typedef tvec<double,  3>  dvec3;
typedef tvec<float,   4>   vec4;
typedef tvec<int,     4>  ivec4;
typedef tvec<unsigned int,    4> uivec4;
typedef tvec<double,  4>  dvec4;

//#pragma warning( pop )
#pragma GCC diagnostic pop