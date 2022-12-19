#include "vec3D.h"


// Default Constructor:
vec3D::vec3D() {
    this->x1 = 0.0;
    this->x2 = 0.0;
    this->x3 = 0.0;
}


// Custom Constructor:
vec3D::vec3D(double x1, double x2, double x3) {
    this->x1 = x1;
    this->x2 = x2;
    this->x3 = x3;
}


// Overloading << operator for std::ostream
std::ostream& operator<<(std::ostream& os, const vec3D& vec) {
    os << vec.x1 << ',' << vec.x2 << ',' << vec.x3 << std::endl;
    return os;
}


// Setting fixed numbers
void vec3D::set_values(double x1, double x2, double x3) {
    this->x1 = x1;
    this->x2 = x2;
    this->x3 = x3;
}


// Overloading * operator
vec3D vec3D::operator*(const vec3D& other) {
    vec3D res(x1 * other.x1, x2 * other.x2, x3 * other.x3);
    return (res);
}
vec3D operator*(const vec3D& vec, const double scalar) {
    return vec3D(scalar * vec.x1, scalar * vec.x2, scalar * vec.x3);
}
vec3D operator*(const double scalar, const vec3D& vec) {
    return vec3D(scalar * vec.x1, scalar * vec.x2, scalar * vec.x3);
}


// Overloading / operator
vec3D vec3D::operator/(const vec3D& other) {
    vec3D res(x1 / other.x1, x2 / other.x2, x3 / other.x3);
    return (res);
}
vec3D operator/(const vec3D& vec, const double scalar) {
    return vec3D(vec.x1 / scalar, vec.x2 / scalar, vec.x3 / scalar);
}
vec3D operator/(const double scalar, const vec3D& vec) {
    return vec3D(scalar / vec.x1, scalar / vec.x2, scalar / vec.x3);
}


// Overloading + operator
vec3D vec3D::operator+(const vec3D& other) {
    return vec3D(x1 + other.x1, x2 + other.x2, x3 + other.x3);
}
vec3D operator+(const vec3D& vec, const double scalar) {
    return vec3D(scalar + vec.x1, scalar + vec.x2, scalar + vec.x3);
}
vec3D operator+(const double scalar, const vec3D& vec) {
    return vec3D(scalar + vec.x1, scalar + vec.x2, scalar + vec.x3);
}


// Overloading - operator
vec3D vec3D::operator-(const vec3D& other) {
    return vec3D(x1 - other.x1, x2 - other.x2, x3 - other.x3);
}
vec3D operator-(const vec3D& vec, const double scalar) {
    return vec3D(vec.x1 - scalar, vec.x2 - scalar, vec.x3 - scalar);
}
vec3D operator-(const double scalar, const vec3D& vec) {
    return vec3D(scalar - vec.x1, scalar - vec.x2, scalar - vec.x3);
}


// Norm for length of vector
double vec3D::norm_sq() {
    return (
        pow(this->x1, (int)2) + 
        pow(this->x2, (int)2) + 
        pow(this->x3, (int)2));
}


// Dot product
double vec3D::dot(vec3D& other) {
    return (x1 * other.x1 + x2 * other.x2 + x3 * other.x3);
}