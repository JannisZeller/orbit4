#include "vec3D.h"

// Constructor:
vec3D::vec3D(double x1, double x2, double x3) {
    x = x1;
    y = x2;
    z = x3;
}

// Overloading << operator
std::ostream& operator<<(std::ostream& os, const vec3D& vec) {
    os << vec.x << ',' << vec.y << ',' << vec.z << std::endl;
    return os;
}

// Overloading * operator
vec3D vec3D::operator*(const vec3D& other) {
    vec3D res(x * other.x, y * other.y, z * other.z);
    return (res);
}
vec3D operator*(const vec3D& vec, const double scalar) {
    return vec3D(scalar * vec.x, scalar * vec.y, scalar * vec.z);
}
vec3D operator*(const double scalar, const vec3D& vec) {
    return vec3D(scalar * vec.x, scalar * vec.y, scalar * vec.z);
}

// Overloading / operator
vec3D vec3D::operator/(const vec3D& other) {
    vec3D res(x / other.x, y / other.y, z / other.z);
    return (res);
}
vec3D operator/(const vec3D& vec, const double scalar) {
    return vec3D(vec.x / scalar, vec.y / scalar, vec.z / scalar);
}
vec3D operator/(const double scalar, const vec3D& vec) {
    return vec3D(scalar / vec.x, scalar / vec.y, scalar / vec.z);
}

// Overloading + operator
vec3D vec3D::operator+(const vec3D& other) {
    return vec3D(x + other.x, y + other.y, z + other.z);
}
vec3D operator+(const vec3D& vec, const double scalar) {
    return vec3D(scalar + vec.x, scalar + vec.y, scalar + vec.z);
}
vec3D operator+(const double scalar, const vec3D& vec) {
    return vec3D(scalar + vec.x, scalar + vec.y, scalar + vec.z);
}

// Overloading - operator
vec3D vec3D::operator-(const vec3D& other) {
    return vec3D(x - other.x, y - other.y, z - other.z);
}
vec3D operator-(const vec3D& vec, const double scalar) {
    return vec3D(vec.x - scalar, vec.y - scalar, vec.z - scalar);
}
vec3D operator-(const double scalar, const vec3D& vec) {
    return vec3D(scalar - vec.x, scalar - vec.y, scalar - vec.z);
}

// Norm for length of vector
double vec3D::norm() {
    return (sqrt(x * x + y * y + z * z));
}

// Dot product
double vec3D::dot(vec3D& other) {
    return (x * other.x + y * other.y + z * other.z);
}