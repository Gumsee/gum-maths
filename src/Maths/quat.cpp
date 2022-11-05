#include "quat.h"
#include <math.h>
#include "mat.h"
#include "Constants.h"

quat::quat()                                       : x(0),     y(0),     z(0),     w(0)  {}
quat::quat(float f)                                : x(f),     y(f),     z(f),     w(f)  {}
quat::quat(float sx, float sy, float sz, float sw) : x(sx),    y(sy),    z(sz),    w(sw) {}
quat::quat(vec3 vec, float sw)                     : x(vec.x), y(vec.y), z(vec.z), w(sw) {}


void quat::operator+=(quat q) { this->x += q.x; this->y += q.y; this->z += q.z; this->w += q.w; }
void quat::operator-=(quat q) { this->x -= q.x; this->y -= q.y; this->z -= q.z; this->w -= q.w; }

quat quat::operator+(quat q)  { return quat(this->x + q.x, this->y + q.y, this->z + q.z, this->w + q.w); }
quat quat::operator-(quat q)  { return quat(this->x - q.x, this->y - q.y, this->z - q.z, this->w - q.w); }
quat quat::operator*(quat q)  
{ 
    return quat(
        w * q.x + x * q.w + y * q.z - z * q.y, 
        w * q.y + y * q.w + z * q.x - x * q.z, 
        w * q.z + z * q.w + x * q.y - y * q.x, 
        w * q.w - x * q.x - y * q.y - z * q.z
    ); 
}
quat quat::operator/(float f)  { return quat(this->x / f, this->y / f, this->z / f, this->w / f); }

float quat::operator[](int index)
{
    if(index == 0)      return x;
    else if(index == 1) return y;
    else if(index == 2) return z;
    else if(index == 3) return w;
    else                return 0;
}

quat quat::slerp(quat a, quat b, float f)
{
    // quaternion to return
	quat qm;
	// Calculate angle between them.
	double cosHalfTheta = a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
	// if qa=qb or qa=-qb then theta = 0 and we can return qa
	if (std::abs(cosHalfTheta) >= 1.0){
		qm.x = a.x; qm.y = a.y; qm.z = a.z; qm.w = a.w;
		return qm;
	}
	// Calculate temporary values.
	double halfTheta = std::acos(cosHalfTheta);
	double sinHalfTheta = std::sqrt(1.0 - cosHalfTheta*cosHalfTheta);
	// if theta = 180 degrees then result is not fully defined
	// we could rotate around any axis normal to qa or qb
	if (std::fabs(sinHalfTheta) < 0.001){ // fabs is floating point absolute
		qm.w = (a.w * 0.5 + b.w * 0.5);
		qm.x = (a.x * 0.5 + b.x * 0.5);
		qm.y = (a.y * 0.5 + b.y * 0.5);
		qm.z = (a.z * 0.5 + b.z * 0.5);
		return qm;
	}
	double ratioA = std::sin((1 - f) * halfTheta) / sinHalfTheta;
	double ratioB = std::sin(f * halfTheta) / sinHalfTheta; 
	//calculate Quaternion.
	qm.w = (a.w * ratioA + b.w * ratioB);
	qm.x = (a.x * ratioA + b.x * ratioB);
	qm.y = (a.y * ratioA + b.y * ratioB);
	qm.z = (a.z * ratioA + b.z * ratioB);
	return qm;
}

quat quat::normalize(quat q)
{
    float length_of_v = std::sqrt((q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w));;
    return quat(q.x / length_of_v, q.y / length_of_v, q.z / length_of_v, q.w / length_of_v);
}

quat quat::rotateAround(float angle, vec3 up)
{
    float rad = angle * GUM_PI / 180.0f;
    float s = std::sin(rad * 0.5f);
    return quat(up * s, std::cos(rad * 0.5f));
}

quat quat::toQuaternion(vec3 anglesDeg)
{
    vec3 rad = anglesDeg * GUM_PI / 180.0f;
    // Abbreviations for the various angular functions
    double cy = std::cos(rad.z * 0.5);
    double sy = std::sin(rad.z * 0.5);
    double cp = std::cos(rad.y * 0.5);
    double sp = std::sin(rad.y * 0.5);
    double cr = std::cos(rad.x * 0.5);
    double sr = std::sin(rad.x * 0.5);

    quat q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return q;
}

vec3 quat::toEuler(quat q)
{
    vec3 euler;
    // roll (x-axis rotation)
    double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
    double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
    euler.x = std::atan2(sinr_cosp, cosr_cosp);

    // pitch (y-axis rotation)
    double sinp = 2 * (q.w * q.y - q.z * q.x);
    if (std::abs(sinp) >= 1)
        euler.y = std::copysign(GUM_PI / 2, sinp); // use 90 degrees if out of range
    else
        euler.y = std::asin(sinp);

    // yaw (z-axis rotation)
    double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
    double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
    euler.z = std::atan2(siny_cosp, cosy_cosp);

    return euler * 180 / GUM_PI;
}


float quat::dot(quat a, quat b)  { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

quat quat::rotateTowards(quat from, quat to, float speed)
{
    float cosTheta = dot(from, to);

    if(cosTheta > 0.9999f) { return to; } // q1 and q2 are already equal. Force q2 just to be sure

    if (cosTheta < 0) // Avoid taking the long path around the sphere
    {
        from = from*-1.0f;
        cosTheta *= -1.0f;
    }

    float angle = std::acos(cosTheta);
    if (angle < speed) { return to; }

    float fT = speed / angle;
    angle = speed;

    quat res = (from * std::sin((1.0f - fT) * angle) + to * std::sin(fT * angle)) / std::sin(angle);
    res = quat::normalize(res);
    return res;
}


std::string quat::toString(bool oneline, std::string prefix, std::string suffix, std::string delimiter)
{
    return prefix + std::to_string(this->x) + delimiter + std::to_string(this->y) + delimiter + std::to_string(this->z) + delimiter + std::to_string(this->w) + suffix;
}