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
#include "core/state.h"

// Using Pi
const long double PI = 3.141592653589793238463;

int main() {

    // Placeholder variables
    vec3D x;
    vec3D v;
    ConversionSystem<long double> consys;
    const long double mass_ratio = consys.moon_mass/consys.earth_mass;
    long double tilt = 5.1 * PI / 180.; // Ecliptic tilt
    int variant;
    double step_size;
    double n_year;
    double smooth = 1e-20;

    // Recieving Settings
    std::cout << "\nobit4 - n-Body-Simulation" << std::endl;
    std::cout << "=========================\n" << std::endl;
    std::cout << "Pass 0 for Earth-Moon-Sun Config or" << std::endl;
    std::cout << "     1 for Mars-Earth-Sun Config or" << std::endl;
    std::cout << "     2 for Tatooine Config: ";
    std::cin >> variant; 
    if (variant == 1) {
        std::cout << "\nHow many earth-years?: ";
        std::cin >> n_year; 
    }

    // Configuring Setup
    if (variant == 0) {
        // Earth
        x.set_values(consys.earth_x0, 0, 0);
        v.set_values(
            0, 
            consys.earth_v0 - mass_ratio * consys.moon_v0*cos(tilt), 
            - mass_ratio * consys.moon_v0*sin(tilt));
        Body earth(
            x, v, 
            consys.earth_mass,
            "Earth", "SI", true, true);
        // Moon
        x.set_values(consys.earth_x0 + consys.moon_x0, 0, 0);
        v.set_values(0, consys.earth_v0 + consys.moon_v0*cos(tilt), consys.moon_v0*sin(tilt));
        Body moon(
            x, v, 
            consys.moon_mass,
            "Moon", "SI", true, true);
        // Sun
        x.set_values(0, 0, 0);
        v.set_values(0, 0, 0);
        Body sun(
            x, v, 
            consys.sun_mass, 
            "Sun", "SI", true, true);
        // Simulation Settings
        step_size = 0.01;
        n_year = 1;
    } else if (variant == 1) {
        // Earth
        x.set_values(consys.earth_x0, 0, 0);
        v.set_values(0, consys.earth_v0, 0);
        Body earth(
            x, v, 
            consys.earth_mass,
            "Earth", "SI", true, true);
        // Mars
        x.set_values(consys.mars_x0, 0, 0);
        v.set_values(0, consys.mars_v0, 0);
        Body mars(
            x, v, 
            consys.mars_mass,
            "Mars", "SI", true, true); 
        // Sun
        x.set_values(0, 0, 0);
        v.set_values(0, 0, 0);
        Body sun(
            x, v, 
            consys.sun_mass, 
            "Sun", "SI", true, true);
        // Simulation Settings
        step_size = 1.;
    } else {const char *msg = 
        "\nDouble-Star systems are incredibly unstable. Therefore a very large " 
        "smoothing term is used to prevent the planet to be boosted out of the "
        "system.\n";
        std::cout << msg << std::endl;
        // Sun 1
        x.set_values(-1, 0, 0);
        v.set_values(0, -0.01, 0);
        Body sun1(
            x, v, 
            1,
            "Sun 1", "generic", true, true);
        // Sun 2
        x.set_values(1, 0, 0);
        v.set_values(0, 0.01, 0);
        Body sun2(
            x, v, 
            1,
            "Sun 2", "generic", true, true);
        // Tatooine
        x.set_values(7, 0, 0);
        v.set_values(0, 0.01, 0);
        Body tatooine(
            x, v, 
            1e-5,
            "Tatooine", "generic", true, true);
        step_size = 1;
        n_year = 20;
        smooth = 1e-1;
    }

       
    // Initialize System
    System sys;


    // Running Simulation
    sys.simulate(step_size, n_year, "../data/data.csv", "rk4", smooth);


    // Final Info and Prints
    std::cout << "---------------" << std::endl;
    std::cout << "Version 0.1.1" << std::endl;
    std::cout << "---------------" << std::endl;
    std::cin.ignore();
    std::cout << "\nPress any key to close the application." << std::endl;
    std::cin.get();

    return 0;
}



