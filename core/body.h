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

// vec3D
// -----
// vec3D is a class for 3-dimensional vector arithmetics.

class Body {
  public:
    std::string name;
    vec3D position;
    vec3D velocity;
    double mass;
    double radius;
    bool massive;
    bool movable;
    double G = ConversionSystem<double>::G_generic;
    
    static std::vector<Body*> all_bodies; 
    static int n_bodies;

    // Constructor defining initial position, velocity, mass, radius, name
    Body(vec3D x, vec3D v, double m, double r, std::string name = "Default", std::string input_units = "SI", bool massive = true, bool movable = true);

    // Disable the body so it does not play a role in the mortion anymore
    void disable();

    // Compute the gravity force that this body introduces at space-point x
    vec3D compute_gravity_at(vec3D x);
};

#endif