// Main CPP-File for creating orbital simulations of multibody systems.
// Currently leads to compiler problems with g++ as updated via MSYS2 because of the vector.push_back problem: https://stackoverflow.com/questions/69773877/getting-throw-bad-array-new-lengthv-when-trying-to-use-stdvectorpush-back
// Using g++ 8.3.0, gdb 10.1 from Rtools mingw64 works (set up January 29, 2022).
#include <math.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//#include "core/Body.h"
#include "core/body.h"
#include "core/convSys.h"
#include "core/solver.h"
#include "core/vec3D.h"

using namespace std;

int main() {
    //--------------------------------------------------------------------------------
    // Declaring some SI-Constants (deprecated):
    const double SUN_MASS = 1.989e30;
    const double SUN_RADIUS = 695700000.0;
    const double EARTH_X0 = -147095000000.0;
    const double EARTH_Y0 = 0.0;
    const double EARTH_VX0 = 0.0;
    const double EARTH_VY0 = -30300.0;
    const double EARTH_MASS = 5.972e24;
    const double EARTH_RADIUS = 6371000.0;
    // const double dt = 1. / (double)stepsPerDay;  // 1 earth day = 86400.0 seconds

    //--------------------------------------------------------------------------------
    // Initializing the bodies of the system:
    // Sun 1
    vec3D Pos(0.0, 0.0, 0.0);
    vec3D Vel(0.0, 0.0, 0.0);
    body sun(Pos, Vel, 1, SUN_RADIUS, "Sun", "generic", true, true);

    // Sun 2
    vec3D Pos1(1.0, 0.0, 0.0);
    vec3D Vel1(0.0, 0.01, 0.0);
    body sun2(Pos1, Vel1, 0.01, SUN_RADIUS, "Sun", "generic", true, true);

    // Mecury
    // Pos << -46000000000.0, 0.0, 0.0;
    // Vel << 0.0, -58980.0, 0;
    // Body mercury(Pos, Vel, 0.33011e24, 2439700., "Mercury", "SI", true, true);

    // Earth
    vec3D Pos2(-147095000000., 0., 0.);
    vec3D Vel2(0., -30300., 0.);
    body earth(Pos2, Vel2, 5.972e24, 6371000.0, "Earth");

    //--------------------------------------------------------------------------------
    // Running Simulation
    double stepSize = 0.01;
    double nYear = 1;
    body::simulation(0.1, 2);

    //--------------------------------------------------------------------------------
    // Print Bodies
    body::print_bodies();

    // Debugging info
    cout << "-----------" << endl;
    cout << "Ver. 0.0023" << endl;
    cout << "-----------" << endl;
    // cout << "Press any key to close the application." << endl;
    // cin.ignore();
    // Test
    return 0;
}