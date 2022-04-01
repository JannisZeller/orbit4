#ifndef BODY
#define BODY

#include <bits/stdc++.h>
#include <math.h>

#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "convSys.h"
#include "solver.h"
#include "vec3D.h"

class body {
   public:
    std::string name;
    vec3D position;
    vec3D velocity;
    double mass;
    double radius;
    bool massive;
    bool movable;
    static std::vector<body*> Bodies;  // FMI: The "inline"-keyword is needed to make the static non-function member visible to the .cpp file. can be used from std=c++17
    static int nBodies;
    // conversion to [L] = AU, [T] = EarthDay and [m] = Sunmass
    double G = 6.67430e-11 / pow(149597870700.0, 3) * pow(86400.0, 2) * 1.98847e30;
    // For the computations carried out when simulating the system, it is sensefull to not use the "really small" and "rally big" values which we would have when using SI-units.

    body(vec3D Pos, vec3D Vel, double m, double r, std::string str = "Default", std::string uSys = "SI", bool bool_massive = true, bool bool_movable = true);

    ~body();

    void disable();

    vec3D compute_acceleration(vec3D X);

    vec3D sum_acceleration(vec3D pos);

    void step_sgl(double dt, body other, std::string algo = "rkf2");

    void step(double dt, std::string algo = "rkf2");

    static void sys_step(double dt, std::string algo = "rkf2");

    static void print_bodies();

    static void simulation(double stepSize, double nYear, std::string fileName = "data.csv");
};

#endif