#include "system.h"


// Constructor
System::System() {
    this->n_bodies = Body::n_bodies;
    this->bodies   = Body::all_bodies;
    std::cout << "Initialized a System;" << std::endl;
    std::cout << "found " << this->n_bodies << " bodies" << std::endl;
}


// Load multiple Bodies from a csv
// System::load_from_csv();


// Add a single body
void System::add_body(Body& body) {
    n_bodies++;
    bodies.push_back(&body);
}


// Simulate a single step
void System::step(double dt, std::string algo /*= "rk4"*/, double smooth) {

    // Fist: STORING new positions of movable bodies.
    std::vector<vec3D> new_positions;
    std::vector<vec3D> new_velocities;
    for (std::vector<Body*>::iterator p = bodies.begin(); p != bodies.end(); ++p) {
        if ((*p)->movable) {
            vec3D new_position = (*p)->position;
            vec3D new_velocity = (*p)->velocity;

            // Computing the total force on the body introduced by all other
            // massive bodies using a lambda-function.
            auto acceleration_function = [this, p, smooth](vec3D x) {
                vec3D a(0., 0., 0.);
                for (std::vector<Body*>::iterator other = bodies.begin(); other != bodies.end(); ++other) {
                    if ((*other)->massive && other != p) {
                        vec3D da = (*other)->compute_gravity_at(x, smooth); // Smoothing factor = minimal distance (in AU) between bodies assumed. 
                        a = a + da;
                    }
                }
                return a;
            };

            // Actually calling the integration methods for the movement:
            if      (algo ==  "rk4") {Solver::runge_kutta_4(        new_position, new_velocity, dt, acceleration_function);} 
            else if (algo == "rkf1") {Solver::runge_kutta_fehlberg1(new_position, new_velocity, dt, acceleration_function);} 
            else if (algo == "rkf2") {Solver::runge_kutta_fehlberg2(new_position, new_velocity, dt, acceleration_function);}
            else {throw std::invalid_argument("Algorithm for update rule in body::sys_step not defined/available!");}
            new_positions.push_back( new_position);
            new_velocities.push_back(new_velocity);
        }
    }

    // Second: UPDATING the positions and velocities of the bodies all at once.
    auto current_position = new_positions.begin();
    auto current_velocity = new_velocities.begin();
    for (std::vector<Body*>::iterator p = bodies.begin(); p != bodies.end(); ++p) {
        if ((*p)->movable) {
            (*p)->position = (*current_position);
            current_position = std::next(current_position, 1);
            (*p)->velocity = (*current_velocity);
            current_velocity = std::next(current_velocity, 1);
        }
    }
}


// Print all Bodies in the system
void System::print_bodies() {
    std::cout << "Listing bodies in System" << std::endl;
    std::cout << "------------------------" << std::endl;
    for (std::vector<Body*>::iterator p = bodies.begin(); p != bodies.end(); ++p) {
        std::cout << (*p)->name << ", "
                  << "is movable: " << (*p)->movable << std::endl;
    }
    std::cout << "Number of Bodies: " << bodies.size() << std::endl;
}


// Perform a simulation
void System::simulate(double step_size, double n_year, std::string of_name /*"data.csv"*/, std::string algo /*"rk4"*/, double smooth /*=0*/) {

    // Open file
    std::ofstream outdata;
    outdata.open(of_name);  // opens the file
    if (!outdata) {         // if file couldn't be opened throw error
        std::cerr << "Error: file could not be opened" << std::endl;
        exit(1);
    }

    // Writing positions to file and performing system step
    int n_steps = 365 * n_year / step_size;
    int progress = 1;
    std::cout << "Simulating System with " << n_steps << " Steps." << std::endl;
    std::cout << "Progress:" << std::endl;
    std::cout << "[";
    for (int iter = 0; iter <= n_steps; iter++) {
        for (std::vector<Body*>::iterator p = bodies.begin(); p != std::prev(bodies.end()); ++p) {
            outdata << (*p)->position.x1 << ","
                    << (*p)->position.x2 << ","
                    << (*p)->position.x3 << ",";
        }
        // Last body has to be included without final " , " to prevent data column full of " ".
        Body* last = bodies.back();
        outdata << last->position.x1 << ","
                << last->position.x2 << ","
                << last->position.x3 << std::endl;

        // Performing one integration step
        this->step(step_size, algo, smooth);

        // Displaying Progress
        // std::cout << iter << std::endl; /* Debugging */
        if ((double)iter / (double)n_steps >= (double)progress * 0.1) {
            std::cout << "#";
            progress++;
        }
    }
    std::cout << "] 100%" << std::endl;
};
