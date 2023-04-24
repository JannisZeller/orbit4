#ifndef STATE_H
#define STATE_H

#include <math.h>
#include <iostream>
#include "vec3D.h"

// State
// -----
// State is a class for 6-dimensional (3 position, 3 velocity) states of bodies.

class State {
// Everything is public. Nothing needs to be hidden here.
  public:
    vec3D position;
    vec3D velocity;

  // Default Construcor
    State();

    // Custom Constructor:
    State(long double x1, long double x2, long double x3, 
          long double v1, long double v2, long double v3);
    State(vec3D x, vec3D v);
    
    // Setting fixed numbers
    void set_values(long double x1, long double x2, long double x3, 
                    long double v1, long double v2, long double v3);
    void set_values(vec3D x, vec3D v);

    // Overloading << operator for std::ostream
    friend std::ostream& operator<<(std::ostream& os, const State& state); 

    // Overloading * operator
    State operator*(const State& other);
    friend State operator*(const State& vec, long double scalar);
    friend State operator*(long double scalar, const State& vec);

    // Overloading / operator
    State operator/(const State& other);
    friend State operator/(const State& vec, long double scalar);
    friend State operator/(long double scalar, const State& vec);

    // Overloading + operator
    State operator+(const State& other);
    friend State operator+(const State& vec, const long double scalar);
    friend State operator+(const long double scalar, const State& vec);

    // Overloading - operator
    State operator-(const State& other);
    friend State operator-(const State& vec, const long double scalar);
    friend State operator-(const long double scalar, const State& vec);  
};

#endif