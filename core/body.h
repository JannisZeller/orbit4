#ifndef BODY_H
#define BODY_H

// Standard Libraries
#include <bits/stdc++.h>
#include <math.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

// Customs
#include "conversionsystem.h"
#include "solver.h"
#include "vec3D.h"
#include "state.h"

// vec3D
// -----
// vec3D is a class for 3-dimensional vector arithmetics.

class Body {
  public:
    std::string name;
    State state;
    double mass;
    bool massive;
    bool movable;
    double G = ConversionSystem<double>::G_generic;
    
    static std::vector<Body*> all_bodies; 
    static int n_bodies;

    // Constructor defining initial position, velocity, mass, radius, name
    Body(vec3D x, vec3D v, double m, std::string name = "Default", std::string input_units = "SI", bool massive = true, bool movable = true);

    // Disable the body so it is not relevant for the system anymore
    void disable();

    // Compute the gravity force that this body introduces at space-point x
    vec3D compute_gravity_at(vec3D x, double smooth);
};

#endif