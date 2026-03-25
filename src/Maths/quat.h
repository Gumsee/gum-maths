#pragma once
#include "vec.h"
#include "mat.h"
#include "GumMathsClass.h"

template <typename T = float, bool ispointer = std::is_pointer<T>::value>
struct quat
{
    typedef std::remove_pointer_t<T> PureType;
    union {
        T vals[4] = {0};
        struct { T w, x, y, z; };
    };
    
    quat()                       : w((T)1.0), x((T)0.0),  y((T)0.0f),  z((T)0.0f)  {}
    quat(T f)                    : w(f),      x(f),       y(f),        z(f)     {}
    quat(T sw, T sx, T sy, T sz) : w(sw),     x(sx),      y(sy),       z(sz)    {}
    quat(T sw, tvec<T,3> vec)    : w(sw),     x(vec.x),   y(vec.y),    z(vec.z) {}
    quat(mat<PureType, 3, 3> mat)
    {
        T t = mat[0][0] + mat[1][1] + mat[2][2];
        // we protect the division by s by ensuring that s>=1
        if (t >= 0) 
        { // by w
            T s = sqrt(t + 1);
            s = (T)0.5 / s;
            w = (T)0.5 * s;                 
            x = (mat[2][1] - mat[1][2]) * s;
            y = (mat[0][2] - mat[2][0]) * s;
            z = (mat[1][0] - mat[0][1]) * s;
        } 
        else if ((mat[0][0] > mat[1][1]) && (mat[0][0] > mat[2][2])) 
        { // by x
            T s = sqrt(1 + mat[0][0] - mat[1][1] - mat[2][2]); 
            s = (T)0.5 / s;
            x = s * (T)0.5;
            y = (mat[1][0] + mat[0][1]) * s;
            z = (mat[0][2] + mat[2][0]) * s;
            w = (mat[2][1] - mat[1][2]) * s;
        } 
        else if (mat[1][1] > mat[2][2]) 
        { // by y
            T s = sqrt(1 + mat[1][1] - mat[0][0] - mat[2][2]);  
            s = (T)0.5 / s;
            y = s * (T)0.5;
            x = (mat[1][0] + mat[0][1]) * s;
            z = (mat[2][1] + mat[1][2]) * s;
            w = (mat[0][2] - mat[2][0]) * s;
        } 
        else 
        { // by z
            T s = sqrt(1 + mat[2][2] - mat[0][0] - mat[1][1]); 
            z = s * (T)0.5; 
            s = (T)0.5 / s;
            x = (mat[0][2] + mat[2][0]) * s;
            y = (mat[2][1] + mat[1][2]) * s;
            w = (mat[1][0] - mat[0][1]) * s;
        }
        //w = std::sqrt(1.0f + mat[0][0] + mat[1][1] + mat[2][2]) / 2.0f;
        //PureType w4 = ((PureType)4.0 * w);
        //std::cout << "w: " << w << std::endl;
        //x = (mat[2][1] - mat[1][2]) / w4;
        //y = (mat[0][2] - mat[2][0]) / w4;
        //z = (mat[1][0] - mat[0][1]) / w4;
    }


    template<typename TT>
    void operator+=(const quat<TT>& q)
    { 
        if      constexpr (!ispointer && !std::is_pointer<TT>::value) {  this->x +=  q.x;  this->y +=  q.y;  this->z +=  q.z;  this->w +=  q.w; }
        else if constexpr (ispointer  && !std::is_pointer<TT>::value) { *this->x +=  q.x; *this->y +=  q.y; *this->z +=  q.z; *this->w +=  q.w; }
        else if constexpr (!ispointer &&  std::is_pointer<TT>::value) {  this->x += *q.x;  this->y += *q.y;  this->z += *q.z;  this->w += *q.w; }
        else if constexpr (ispointer  &&  std::is_pointer<TT>::value) { *this->x += *q.x; *this->y += *q.y; *this->z += *q.z; *this->w += *q.w; }
    }
    template<typename TT>
    void operator-=(const quat<TT>& q)
    { 
        if      constexpr (!ispointer && !std::is_pointer<TT>::value) {  this->x -=  q.x;  this->y -=  q.y;  this->z -=  q.z;  this->w -=  q.w; }
        else if constexpr (ispointer  && !std::is_pointer<TT>::value) { *this->x -=  q.x; *this->y -=  q.y; *this->z -=  q.z; *this->w -=  q.w; }
        else if constexpr (!ispointer &&  std::is_pointer<TT>::value) {  this->x -= *q.x;  this->y -= *q.y;  this->z -= *q.z;  this->w -= *q.w; }
        else if constexpr (ispointer  &&  std::is_pointer<TT>::value) { *this->x -= *q.x; *this->y -= *q.y; *this->z -= *q.z; *this->w -= *q.w; }
    }

    template<typename TT>
    bool operator==(const quat<TT>& q)
    { 
        if      constexpr (!ispointer && !std::is_pointer<TT>::value) { return  this->x ==  q.x &&  this->y ==  q.y &&  this->z ==  q.z &&  this->w ==  q.w; }
        else if constexpr (ispointer  && !std::is_pointer<TT>::value) { return *this->x ==  q.x && *this->y ==  q.y && *this->z ==  q.z && *this->w ==  q.w; }
        else if constexpr (!ispointer &&  std::is_pointer<TT>::value) { return  this->x == *q.x &&  this->y == *q.y &&  this->z == *q.z &&  this->w == *q.w; }
        else if constexpr (ispointer  &&  std::is_pointer<TT>::value) { return *this->x == *q.x && *this->y == *q.y && *this->z == *q.z && *this->w == *q.w; }
    }

    template<typename TT>
    bool operator!=(const quat<TT>& q)  { return !this->operator==(q); }

    quat operator+(quat q)  { return quat(this->w + q.w, this->x + q.x, this->y + q.y, this->z + q.z); }
    quat operator-(quat q)  { return quat(this->w - q.w, this->x - q.x, this->y - q.y, this->z - q.z); }
    quat operator*(quat q)  
    {
        return quat(
            q.w * w - q.x * x - q.y * y - q.z * z,
            q.w * x + q.x * w - q.y * z + q.z * y,
            q.w * y + q.x * z + q.y * w - q.z * x, 
            q.w * z - q.x * y + q.y * x + q.z * w
        ); 
    }

    void operator*=(const PureType& f)
    { 
        if constexpr (!ispointer)
        {
            this->x *= f;
            this->y *= f;
            this->z *= f;
            this->w *= f; 
        }
        else
        {
            *this->x *= f;
            *this->y *= f;
            *this->z *= f;
            *this->w *= f; 
        }
    }

    quat<PureType> operator/(const PureType& f) const 
    { 
        if constexpr (!ispointer)
            return quat<PureType>(this->w / f, this->x / f, this->y / f, this->z / f); 
        else
            return quat<PureType>(*this->w / f, *this->x / f, *this->y / f, *this->z / f); 
    }
    
    PureType& operator[](int index)
    {
        if constexpr (!ispointer)
            return &vals[index];
        else
            return *vals[index];
    }

    quat<T> operator-()
    { 
      return quat<T>(-w, -x, -y, -z);
    }

    PureType* data()
    {
        if constexpr (!ispointer)
            return &vals[0];
        else
            return vals;
    }

    static quat<PureType> normalize(quat q)
    {
        if constexpr (!ispointer)
        {
            PureType length_of_v = std::sqrt((q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w));;
            return quat(q.w / length_of_v, q.x / length_of_v, q.y / length_of_v, q.z / length_of_v);
        }
        else
        {
            PureType length_of_v = std::sqrt((*q.x * *q.x) + (*q.y * *q.y) + (*q.z * *q.z) + (*q.w * *q.w));;
            return quat(*q.w / length_of_v, *q.x / length_of_v, *q.y / length_of_v, *q.z / length_of_v);
        }
    }

    static tvec<T, 3> toEuler(quat q)
    {
        tvec<T, 3> euler;
        // roll (x-axis rotation)
        T sinr_cosp = (T)2.0 * (q.w * q.x + q.y * q.z);
        T cosr_cosp = (T)1.0 - 2 * (q.x * q.x + q.y * q.y);
        euler.x = (T)std::atan2(sinr_cosp, cosr_cosp);

        // pitch (y-axis rotation)
        T sinp = (T)2.0 * (q.w * q.y - q.z * q.x);
        if (std::abs(sinp) >= 1.0)
            euler.y = (T)std::copysign(GUM_PI / 2, sinp); // use 90 degrees if out of range
        else
            euler.y = (T)std::asin(sinp);

        // yaw (z-axis rotation)
        T siny_cosp = (T)2.0 * (q.w * q.z + q.x * q.y);
        T cosy_cosp = (T)1.0 - 2 * (q.y * q.y + q.z * q.z);
        euler.z = (T)std::atan2(siny_cosp, cosy_cosp);

        return euler * 180 / GUM_PI;
    }


    static T dot(quat a, quat b)  { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
    
    static quat rotateAround(T angle, tvec<T, 3> up)
    {
        T rad = angle * GUM_PI / (T)180.0;
        T s = std::sin(rad * 0.5);
        return quat(std::cos(rad * 0.5), up * s);
    }

    static quat toQuaternion(tvec<T, 3> anglesDeg)
    {
        tvec<T, 3> rad = anglesDeg * GUM_PI / 180.0;
        // Abbreviations for the various angular functions
        T cy = (T)std::cos(rad.z * 0.5);
        T sy = (T)std::sin(rad.z * 0.5);
        T cp = (T)std::cos(rad.y * 0.5);
        T sp = (T)std::sin(rad.y * 0.5);
        T cr = (T)std::cos(rad.x * 0.5);
        T sr = (T)std::sin(rad.x * 0.5);

        quat q;
        q.w = cr * cp * cy + sr * sp * sy;
        q.x = sr * cp * cy - cr * sp * sy;
        q.y = cr * sp * cy + sr * cp * sy;
        q.z = cr * cp * sy - sr * sp * cy;

        return q;
    }

    static quat slerp(quat a, quat b, T f)
    {
        quat qm;
        T cosHalfTheta = (T)dot(a,b);

        if(cosHalfTheta > (T)0.9999) { return b; } // q1 and q2 are already equal. Force q2 just to be sure

        if (cosHalfTheta < (T)0.0) // Avoid taking the long path around the sphere
        {
            a *= -1.0f;
            cosHalfTheta *= -1.0f;
        }

        // if qa=qb or qa=-qb then theta = 0 and we can return qa
        if(std::abs(cosHalfTheta) >= 1.0)
            return a;

        // Calculate temporary values.
        T halfTheta = (T)acos(cosHalfTheta);
        T sinHalfTheta = (T)sqrt(1.0 - cosHalfTheta*cosHalfTheta);
        // if theta = 180 degrees then result is not fully defined
        // we could rotate around any axis normal to qa or qb
        if (fabs(sinHalfTheta) < 0.001)
        { // fabs is floating point absolute
            qm.w = (a.w * 0.5 + b.w * 0.5);
            qm.x = (a.x * 0.5 + b.x * 0.5);
            qm.y = (a.y * 0.5 + b.y * 0.5);
            qm.z = (a.z * 0.5 + b.z * 0.5);
            return qm;
        }

        T ratioA = (T)sin((1 - f) * halfTheta) / sinHalfTheta;
        T ratioB = (T)sin(f * halfTheta) / sinHalfTheta; 
        //calculate Quaternion.
        qm.w = (a.w * ratioA + b.w * ratioB);
        qm.x = (a.x * ratioA + b.x * ratioB);
        qm.y = (a.y * ratioA + b.y * ratioB);
        qm.z = (a.z * ratioA + b.z * ratioB);
        return qm;
    }

    static quat rotateTowards(quat from, quat to, const T& speed)
    {
        quat ffrom = from;
        float cosTheta = dot(ffrom, to);

        if(cosTheta > 0.9999f) { return to; } // q1 and q2 are already equal. Force q2 just to be sure

        if (cosTheta < 0) // Avoid taking the long path around the sphere
        {
            ffrom *= -1.0f;
            cosTheta *= -1.0f;
        }

        float angle = std::acos(cosTheta);
        if (angle < speed) { return to; }

        float fT = speed / angle;
        angle = speed;

        quat res = (ffrom * std::sin((1.0f - fT) * angle) + to * std::sin(fT * angle)) / std::sin(angle);
        res = quat::normalize(res);
        return res;
    }

    
    std::string toString(std::string prefix = "quat(", std::string suffix = ")", std::string delimiter = ",") const
    {
      return prefix + std::to_string(this->w) + delimiter + std::to_string(this->x) + delimiter + std::to_string(this->y) + delimiter + std::to_string(this->z) + suffix;
    }
    operator std::string() const { return toString(); }
};

typedef quat<float>  fquat;
typedef quat<double> dquat;
