#ifndef VEC3D_H
#define VEC3D_H

#include <math.h>
#include <iostream>

// vec3D
// -----
// vec3D is a class for 3-dimensional vector arithmetics.

class vec3D {

// Everything is public. Nothing needs to be hidden here.
  public:
    long double x1;
    long double x2;
    long double x3;

    // Default Construcor
    vec3D();

    // Custom Constructor:
    vec3D(long double x1, long double x2, long double x3);

    // Setting fixed numbers
    void set_values(long double x1, long double x2, long double x3);

    // Overloading << operator for std::ostream
    friend std::ostream& operator<<(std::ostream& os, const vec3D& vec);

    // Overloading * operator
    vec3D operator*(const vec3D& other);
    friend vec3D operator*(const vec3D& vec, long double scalar);
    friend vec3D operator*(long double scalar, const vec3D& vec);

    // Overloading / operator
    vec3D operator/(const vec3D& other);
    friend vec3D operator/(const vec3D& vec, long double scalar);
    friend vec3D operator/(long double scalar, const vec3D& vec);

    // Overloading + operator
    vec3D operator+(const vec3D& other);
    friend vec3D operator+(const vec3D& vec, const long double scalar);
    friend vec3D operator+(const long double scalar, const vec3D& vec);

    // Overloading - operator
    vec3D operator-(const vec3D& other);
    friend vec3D operator-(const vec3D& vec, const long double scalar);
    friend vec3D operator-(const long double scalar, const vec3D& vec);

    // Norm for length of vector
    long double norm_sq();

    // Dot or scalar product
    long double dot(vec3D& other);
};

#endif