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
    double x1;
    double x2;
    double x3;

    // Default Construcor
    vec3D();

    // Custom Constructor:
    vec3D(double x1, double x2, double x3);

    // Setting fixed numbers
    void set_values(double x1, double x2, double x3);

    // Overloading << operator for std::ostream
    friend std::ostream& operator<<(std::ostream& os, const vec3D& vec);

    // Overloading * operator
    vec3D operator*(const vec3D& other);
    friend vec3D operator*(const vec3D& vec, double scalar);
    friend vec3D operator*(double scalar, const vec3D& vec);

    // Overloading / operator
    vec3D operator/(const vec3D& other);
    friend vec3D operator/(const vec3D& vec, double scalar);
    friend vec3D operator/(double scalar, const vec3D& vec);

    // Overloading + operator
    vec3D operator+(const vec3D& other);
    friend vec3D operator+(const vec3D& vec, const double scalar);
    friend vec3D operator+(const double scalar, const vec3D& vec);

    // Overloading - operator
    vec3D operator-(const vec3D& other);
    friend vec3D operator-(const vec3D& vec, const double scalar);
    friend vec3D operator-(const double scalar, const vec3D& vec);

    // Norm for length of vector
    double norm();

    // Dot or scalar product
    double dot(vec3D& other);
};

#endif