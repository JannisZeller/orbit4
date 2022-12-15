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
#include "convSys.h"
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

    static std::vector<Body*> all_bodies; 
    static int n_bodies;

    // conversion to [L] = AU, [T] = EarthDay and [m] = Sunmass
    double G = 6.67430e-11 / pow(149597870700.0, 3) * pow(86400.0, 2) * 1.98847e30;
    // For the computations carried out when simulating the system, it is sensefull to not use the "really small" and "rally big" values which we would have when using SI-units.

    // Constructor defining initial position, velocity, mass, radius, name
    Body(vec3D x, vec3D v, double m, double r, std::string name = "Default", std::string unitsystem = "SI", bool massive = true, bool movable = true);

    // Disable the body so it does not play a role in the mortion anymore
    void disable();

    // Compute the gravity force that this body introduces at space-point x
    vec3D compute_gravity_at(vec3D x);
};

#endif