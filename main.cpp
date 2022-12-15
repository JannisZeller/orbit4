// Main CPP-File for creating orbital simulations of multibody systems.
#include <math.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// Custom files
#include "core/body.h"
#include "core/conversionsystem.h"
#include "core/solver.h"
#include "core/vec3D.h"
#include "core/system.h"

// Using the std-namespace (only in main)
using namespace std;

// Function for waiting for key input
void wait_for_key() {
    cout << "Press any key to go on." << endl;
    cin.ignore();
}


int main() {

    // Placeholder variables
    vec3D x;
    vec3D y;
    ConversionSystem<double> consys;

    // Sun
    Body sun(
        x, y, 
        consys.sun_mass, 
        consys.sun_radius, 
        "Sun", "SI", true, true);

    // Earth
    x.set_values(-consys.earth_x0, 0, 0);
    y.set_values(0, -consys.earth_v0, 0);
    Body earth(
        x, y, 
        consys.earth_mass,
        consys.earth_radius,
        "Earth", "SI", true, true);

    // Earth
    x.set_values(-consys.earth_x0 - consys.moon_x0, 0, 0);
    y.set_values(0, -consys.earth_v0 - consys.moon_v0, 0);
    Body moon(
        x, y, 
        consys.earth_mass,
        consys.earth_radius,
        "Earth", "SI", true, true);

    // Initialize System
    System sys;

    // Running Simulation
    double step_size = 0.01;
    double n_year    = 1;
    sys.simulate(step_size, n_year, "data.csv", "rk4");

    //Final Info and Prints
    cout << "" << endl;
    cout << "Ver. 0.0031" << endl;
    cout << "-----------" << endl;
    wait_for_key();

    return 0;
}