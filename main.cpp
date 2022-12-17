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

// Using Pi
const double PI = 3.141592653589793238463;

int main() {

    // Placeholder variables
    vec3D x;
    vec3D y;
    ConversionSystem<double> consys;
    const double mass_ratio = consys.moon_mass/consys.earth_mass;
    double tilt = 5.1 * PI / 180.; // Ecliptic tilt

    // Earth
    x.set_values(consys.earth_x0, 0, 0);
    // y.set_values(
    //     0, 
    //     consys.earth_v0 - mass_ratio * consys.moon_v0*cos(tilt), 
    //     - mass_ratio * consys.moon_v0*sin(tilt));
    y.set_values(0, consys.earth_v0, 0);
    Body earth(
        x, y, 
        consys.earth_mass,
        "Earth", "SI", true, true);

    // Moon
    x.set_values(consys.earth_x0 + consys.moon_x0, 0, 0);
    y.set_values(0, consys.earth_v0 + consys.moon_v0*cos(tilt), consys.moon_v0*sin(tilt));
    Body moon(
        x, y, 
        consys.moon_mass,
        "Moon", "SI", true, true);


    cout << "Testing" << endl;
    cout << earth.compute_gravity_at(moon.position, 0) << endl;
    cout << moon.position << endl;
    cout << "Testing" << endl;


    // Sun
    x.set_values(0, 0, 0);
    y.set_values(0, 0, 0);
    Body sun(
        x, y, 
        consys.sun_mass, 
        "Sun", "SI", true, true);

    // Mars
    x.set_values(consys.mars_x0, 0, 0);
    y.set_values(0, consys.mars_v0, 0);
    Body mars(
        x, y, 
        consys.mars_mass,
        "Mars", "SI", true, true);


        
    // Initialize System
    System sys;
    sys.print_bodies();

    // Running Simulation
    double step_size = 0.05;
    double n_year    = 1.;
    sys.simulate(step_size, n_year, "./data.csv", "rkf2", 1e-20);

    //Final Info and Prints
    cout << "" << endl;
    cout << "Ver. 0.0033" << endl;
    cout << "-----------" << endl;
    wait_for_key();

    return 0;
}

/* 

    // Earth
    x.set_values(0, 0, 0);
    y.set_values(0, 0, 0);
    Body earth(
        x, y, 
        consys.earth_mass,
        "Earth", "SI", true, true);

    // Moon
    double tilt = 5. * PI / 180.; // Ecliptic tilt
    x.set_values(consys.moon_x0, 0, 0);
    y.set_values(0, consys.moon_v0*cos(tilt), consys.moon_v0*sin(tilt));
    Body moon(
        x, y, 
        consys.moon_mass,
        "Moon", "SI", true, true);






    
    // Sun 1
    x.set_values(-1, 0, 0);
    y.set_values(0, -0.01, 0);
    Body sun1(
        x, y, 
        1,
        "Sun 1", "generic", true, true);

    // Sun 2
    x.set_values(1, 0, 0);
    y.set_values(0, 0.01, 0);
    Body sun2(
        x, y, 
        1,
        "Sun 2", "generic", true, true);

    // Tatooine
    x.set_values(0, 0, 0);
    y.set_values(0, 0.001, 0.001);
    Body tatooine(
        x, y, 
        1e-5,
        "Tatooine", "generic", true, true);

*/