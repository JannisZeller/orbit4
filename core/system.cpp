#include "system.h"

std::vector<Body*> System::bodies;
int System::n_bodies;

System::System() {
    n_bodies = 0;
    std::cout << "Initialized a System." << std::endl;
}

// System::load_from_csv();

void System::add_body(Body body) {
    n_bodies += 1;
    bodies.push_back(body);
}

// System::step(double dt, std::string algo = "rkf2");

// System::print_bodies();

// System::simulate(double stepSize, double nYear, std::string fileName = "data.csv");