#ifndef BODY
#define BODY

#include <math.h>

#include <iostream>
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
    static std::vector<body*> Bodies;  // The "inline"-keyword is needed to make the static non-function member visible to the .cpp file. can be used from std=c++17
    // conversion to AU and     t=1EarthDay and   m=1Sunmass // TODO: Decide if G gets static... Think of Unit conversion
    double G = 6.67430e-11 / pow(149597870700.0, 3) * pow(86400.0, 2) * 1.98847e30;

    body(vec3D Pos, vec3D Vel, double m, double r, std::string str = "Default", std::string uSys = "SI", bool bool_massive = true, bool bool_movable = true);

    // Acceleration of b1 on b2
    static vec3D compute_acceleration(body b1, body b2);

    static void sum_acceleration(vec3D& A);

    void step(double dt, body other, std::string algo = "rkf38");

    static void sys_step(double dt, std::string algo = "rkf38");

    static void print_bodies();
};

#endif