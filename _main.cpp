// Main CPP-File for creating orbital simulations of multibody systems.
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Custom files
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

    //--------------------------------------------------------------------------------
    // Initializing the bodies of the system:
    // Sun 1
    vec3D pos1(0.0, 0.0, 0.0);
    vec3D vel1(0.0, 0.0, 0.0);
    Body sun(pos1, vel1, 1, SUN_RADIUS, "Sun", "generic", true, true);

    // Sun 2
    // vec3D Pos1(1.0, 0.0, 0.0);
    // vec3D Vel1(0.0, 0.01, 0.0);
    // body sun2(Pos1, Vel1, 0.01, SUN_RADIUS, "Sun", "generic", true, true);

    // Mecury
    // Pos << -46000000000.0, 0.0, 0.0;
    // Vel << 0.0, -58980.0, 0;
    // Body mercury(Pos, Vel, 0.33011e24, 2439700., "Mercury", "SI", true, true);

    // Earth
    vec3D Pos2(-149598022960., 0., 0.);
    vec3D Vel2(0., -29290., 0.);
    Body earth(Pos2, Vel2, EARTH_MASS, 6371000.0, "Earth", "SI", true, true);

    // Moon
    vec3D Pos3(-149598022960. - 383397000., 0., 0.);
    vec3D Vel3(0., -29290. - 1023., 0.);
    Body moon(Pos3, Vel3, EARTH_MASS / 81.3, 1., "Moon", "SI", true, true);

    //--------------------------------------------------------------------------------
    // Running Simulation
    double stepSize = 0.005;
    double nYear = 2;
    Body::simulation(stepSize, nYear);

    //--------------------------------------------------------------------------------
    // Print Bodies
    Body::print_bodies();

    //Final Info and Prints
    cout << "-----------" << endl;
    cout << "Ver. 0.0027" << endl;
    cout << "-----------" << endl;
    cout << "Press any key to close the application." << endl;
    cin.ignore();

    return 0;
}