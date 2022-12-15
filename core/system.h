#ifndef SYSTEM_H
#define SYSTEM_H

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
#include "body.h"

// System
// ------
// The System class represents an orbit-system consisting of multiple Bodies 
// (from the "Body"-class). It wraps around these bodies and acts as an 
// interface for the simulation.

class System {
  public:
    std::vector<Body*> bodies;  
    int n_bodies;
    // conversion to [L] = AU, [T] = EarthDay and [m] = Sunmass
    double G = 6.67430e-11 / pow(149597870700.0, 3) * pow(86400.0, 2) * 1.98847e30;

    // Constructor
    System();

    // Load multiple Bodies from a csv
    // void load_from_csv();

    // Add a single body (needed if bodies are created after the system is
    // initialized)
    void add_body(Body& body);

    // Simulate a single step
    void step(double dt, std::string algo = "rkf2");

    // Print all Bodies in the system
    void print_bodies();

    // Perform a simulation (step_size is in earth-days.)
    void simulate(double step_size, double n_year, std::string of_name = "data.csv", std::string algo = "rkf2");
};

#endif