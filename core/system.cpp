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
void System::step(long double dt, std::string algo /*= "rk4"*/, long double smooth) {

    // 1. Initializing new-states containerr
    std::vector<State> new_states;


    // 2. Calculating the updates
    for (std::vector<Body*>::iterator p = bodies.begin(); p != bodies.end(); ++p) {
        if ((*p)->movable) {
            State new_state = (*p)->state;
            auto gradient = [this, p, smooth](State curr_state) {
                vec3D a(0., 0., 0.);
                for (std::vector<Body*>::iterator other = bodies.begin(); other != bodies.end(); ++other) {
                    if ((*other)->massive && other != p) {
                        vec3D da = (*other)->compute_gravity_at(curr_state.position, smooth); // Smoothing factor = minimal distance (in AU) between bodies assumed. 
                        a = a + da;
                    }
                }
                vec3D v = curr_state.velocity;
                State grad(v, a);
                return grad;
            };
            if      (algo ==  "rk4") {Solver::runge_kutta_4(        new_state, dt, gradient);} 
            else if (algo == "rkf1") {Solver::runge_kutta_fehlberg1(new_state, dt, gradient);}
            else if (algo == "rkf2") {Solver::runge_kutta_fehlberg2(new_state, dt, gradient);}
            else {throw std::invalid_argument("Algorithm for update rule in body::sys_step not defined/available!");}

            new_states.push_back(new_state);
        }
    }

    // 3. Actually updating the states
    auto current_state = new_states.begin();
    for (std::vector<Body*>::iterator p = bodies.begin(); p != bodies.end(); ++p) {
        if ((*p)->movable) {
            (*p)->state = (*current_state);
            current_state = std::next(current_state, 1);
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
    std::cout << "Number of Bodies: " << bodies.size() << "\n" << std::endl;
}


// Perform a simulation
void System::simulate(long double step_size, long double n_year, std::string of_name /*"data.csv"*/, std::string algo /*"rk4"*/, long double smooth /*=0*/) {

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
            outdata << (*p)->state.position.x1 << ","
                    << (*p)->state.position.x2 << ","
                    << (*p)->state.position.x3 << ",";
        }
        // Last body has to be included without final " , " to prevent data column full of " ".
        Body* last = bodies.back();
        outdata << last->state.position.x1 << ","
                << last->state.position.x2 << ","
                << last->state.position.x3 << std::endl;

        // Performing one integration step
        this->step(step_size, algo, smooth);

        // Displaying Progress
        // std::cout << iter << std::endl; /* Debugging */
        if ((long double)iter / (long double)n_steps >= (long double)progress * 0.1) {
            std::cout << "#";
            progress++;
        }
    }
    std::cout << "] 100%" << std::endl;
};
