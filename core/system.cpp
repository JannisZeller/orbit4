#include "system.h"


System::System() {
    n_bodies = 0;
    std::cout << "Initialized a System." << std::endl;
}


// System::load_from_csv();


void System::add_body(Body& body) {
    n_bodies++;
    bodies.push_back(&body);
}


void System::step(double dt, std::string algo /*= "rkf2"*/) {
    // Fist: Storing new positions of movable bodies.
    std::vector<vec3D> new_positions;
    for (std::vector<Body*>::iterator p = bodies.begin(); p != bodies.end(); ++p) {
        if ((*p)->movable) {
            vec3D new_position = (*p)->position;

            // Defining the acceleration function for the current body using c++
            // Lambda:
            auto acceleration_function = [this, p](vec3D x) {
                vec3D A(0.0, 0.0, 0.0);
                for (std::vector<Body*>::iterator other = bodies.begin(); other != bodies.end(); ++other) {
                    if ((*other)->massive && other != p) {
                        vec3D dA = (*other)->compute_acceleration_at(x);
                        A = A + dA;
                    }
                }
                return A;
            };
            // Actually calling the integration methods for the OD:
            if (algo == "rk4") {
                solver::runge_kutta_4(new_position, (*p)->velocity, dt, acceleration_function);
            } else if (algo == "rkf1") {
                solver::runge_kutta_fehlberg1(new_position, (*p)->velocity, dt, acceleration_function);
            } else if (algo == "rkf2") {
                solver::runge_kutta_fehlberg2(new_position, (*p)->velocity, dt, acceleration_function);
            } else {
                throw std::invalid_argument("Algorithm for update rule in body::sys_step not defined/available!");
            }
            new_positions.push_back(new_position);
        }
    }
    // Second: Updating the positions of the bodies all at once.
    auto current_position = new_positions.begin();
    for (std::vector<Body*>::iterator p = bodies.begin(); p != bodies.end(); ++p) {
        if ((*p)->movable) {
            (*p)->position = (*current_position);
            current_position = std::next(current_position, 1);
        }
    }
}


void System::print_bodies() {
    std::cout << "Listing bodies in System" << std::endl;
    std::cout << "------------------------" << std::endl;
    for (std::vector<Body*>::iterator p = bodies.begin(); p != bodies.end(); ++p) {
        std::cout << (*p)->name << ", "
                  << "is movable: " << (*p)->movable << std::endl;
    }
    std::cout << "Amount of Bodies: " << bodies.size() << std::endl;
}


// System::simulate(double stepSize, double nYear, std::string fileName = "data.csv");