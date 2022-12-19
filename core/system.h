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
#include "state.h"

// System
// ------
// The System class represents an orbit-system consisting of multiple Bodies 
// (from the "Body"-class). It wraps around these bodies and acts as an 
// interface for the simulation.

class System {
  public:
    std::vector<Body*> bodies;  
    int n_bodies;

    // Constructor
    System();

    // Load multiple Bodies from a csv
    // void load_from_csv();

    // Add a single body (needed if bodies are created after the system is
    // initialized)
    void add_body(Body& body);

    // Simulate a single step
    void step(double dt, std::string algo="rk4", double smooth=0);

    // Print all Bodies in the system
    void print_bodies();

    // Perform a simulation (step_size is in earth-days.)
    void simulate(double step_size, double n_year, std::string of_name="data.csv", std::string algo ="rk4", double smooth=0);
};

#endif