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

// System::step(double dt, std::string algo = "rkf2");

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