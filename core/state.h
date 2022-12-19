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
    State(double x1, double x2, double x3, 
          double v1, double v2, double v3);
    State(vec3D x, vec3D v);
    
    // Setting fixed numbers
    void set_values(double x1, double x2, double x3, 
                    double v1, double v2, double v3);
    void set_values(vec3D x, vec3D v);

    // Overloading << operator for std::ostream
    friend std::ostream& operator<<(std::ostream& os, const State& state); 

    // Overloading * operator
    State operator*(const State& other);
    friend State operator*(const State& vec, double scalar);
    friend State operator*(double scalar, const State& vec);

    // Overloading / operator
    State operator/(const State& other);
    friend State operator/(const State& vec, double scalar);
    friend State operator/(double scalar, const State& vec);

    // Overloading + operator
    State operator+(const State& other);
    friend State operator+(const State& vec, const double scalar);
    friend State operator+(const double scalar, const State& vec);

    // Overloading - operator
    State operator-(const State& other);
    friend State operator-(const State& vec, const double scalar);
    friend State operator-(const double scalar, const State& vec);  
};

#endif