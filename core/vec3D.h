#ifndef VEC3D
#define VEC3D

#include <math.h>

#include <iostream>

class vec3D {
   public:
    double x;
    double y;
    double z;

    // Default Construcor
    vec3D();

    // Custom Constructor:
    vec3D(double x1, double x2, double x3);

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

    // Dot product
    double dot(vec3D& other);
};

#endif