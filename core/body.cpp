#include "body.h"

std::vector<body*> body::Bodies;

int body::nBodies;

// Constructor which assigns a lot of properties of the body
body::body(vec3D Pos, vec3D Vel, double m, double r, std::string str /*= "Default"*/, std::string uSys /*= "SI"*/, bool bool_massive /*= true*/, bool bool_movable /*= true*/) {
    name = str;
    massive = bool_massive;
    movable = bool_movable;
    Bodies.push_back(this);
    nBodies++;
    // If unit system is already generic ([L]=AU, [T]=EarthDays, [M]=SunMasses), just take them:
    if (uSys == "generic") {
        position = Pos;
        velocity = Vel;
        mass = m;
        radius = r;
    }
    // Otherwise convert to generic unit system:
    else if (uSys == "SI") {
        position = convSys<vec3D>::lenConv(Pos);
        velocity = convSys<vec3D>::velConv(Vel);
        mass = convSys<double>::massConv(m);
        radius = convSys<double>::lenConv(r);
    }
}

// Destructor with changes on Bodies and nBodies.
// FMI: Can only be used directly on instances which have been initialized using the
// body* temp = new body(*args) - syntax.
body::~body() {
    nBodies--;
    auto it = std::find(Bodies.begin(), Bodies.end(), this);
    int index = it - Bodies.begin();
    // erase the it-th element
    Bodies.erase(Bodies.begin() + index);
}

void body::disable() {
    massive = false;
    movable = false;
}

// Calculates the acceleration of the "calling body" at any given
// point in space X.
vec3D body::compute_acceleration(vec3D X) {
    vec3D diff;
    diff = X - position;
    double dist = diff.norm();
    double distPow = pow(dist, 3);
    return -G * mass / distPow * diff;
}

// Sums of the acceleration of all other bodies in the system for the body
// it is called from, meaning at the position the body is located.
// Has to act on a "new" vec3D (called pos) because it is called multiple
// times with different arguments (others than just "this.position") in the
// solvers.
vec3D body::sum_acceleration(vec3D pos) {
    vec3D A(0.0, 0.0, 0.0);
    for (std::vector<body*>::iterator p = Bodies.begin(); p != Bodies.end(); ++p) {
        if ((*p)->massive && (*p) != this) {
            vec3D temp = (*p)->compute_acceleration(pos);
            A = A + temp;
        }
    }
    return A;
}

// Performs a single step for the "calling body" under the influence
// of the body "other" in the argument.
void body::step_sgl(double dt, body other, std::string algo /*= "rkf2"*/) {
    auto temp = std::bind(&body::compute_acceleration, other, std::placeholders::_1);
    if (algo == "rk4") {
        solver::runge_kutta_4(this->position, this->velocity, dt, temp);
    } else if (algo == "rkf1") {
        solver::runge_kutta_fehlberg1(this->position, this->velocity, dt, temp);
    } else if (algo == "rkf2") {
        solver::runge_kutta_fehlberg2(this->position, this->velocity, dt, temp);
    } else {
        throw std::invalid_argument("Algorithm for update rule in body.step_sgl not defined/available!");
    }
}

// Performs a single step for the "calling body" under the influence
// of ALL other bodys in the system.
void body::step(double dt, std::string algo /*= "rkf2"*/) {
    auto temp = std::bind(&body::sum_acceleration, this, std::placeholders::_1);
    if (algo == "rk4") {
        solver::runge_kutta_4(this->position, this->velocity, dt, temp);
    } else if (algo == "rkf1") {
        solver::runge_kutta_fehlberg1(this->position, this->velocity, dt, temp);
    } else if (algo == "rkf2") {
        solver::runge_kutta_fehlberg2(this->position, this->velocity, dt, temp);
    } else {
        throw std::invalid_argument("Algorithm for update rule in body.step not defined/available!");
    }
}

// Performs a whole system step obeying massive and movable boolean members of the bodies.
void body::sys_step(double dt, std::string algo /*= "rkf2"*/) {
    // Fist: Storing new positions based on body::sum_acceleration in a vector for all
    // movable bodies.
    std::vector<vec3D> posPost;
    for (std::vector<body*>::iterator p = Bodies.begin(); p != Bodies.end(); ++p) {
        if ((*p)->movable) {
            vec3D posTemp = (*p)->position;

            auto temp = std::bind(&body::sum_acceleration, (*p), std::placeholders::_1);

            if (algo == "rk4") {
                solver::runge_kutta_4(posTemp, (*p)->velocity, dt, temp);
            } else if (algo == "rkf1") {
                solver::runge_kutta_fehlberg1(posTemp, (*p)->velocity, dt, temp);
            } else if (algo == "rkf2") {
                solver::runge_kutta_fehlberg2(posTemp, (*p)->velocity, dt, temp);
            } else {
                throw std::invalid_argument("Algorithm for update rule in body::sys_step not defined/available!");
            }
            posPost.push_back(posTemp);
        }
    }
    // Second: Updating the positions of the bodies all at once.
    auto pos = posPost.begin();
    for (std::vector<body*>::iterator p = Bodies.begin(); p != Bodies.end(); ++p) {
        if ((*p)->movable) {
            (*p)->position = (*pos);
            pos = std::next(pos, 1);
        }
    }
}

// Printing an overview over the bodies currently in the system.
void body::print_bodies() {
    std::cout << "----------" << std::endl;
    std::cout << "ALL BODIES" << std::endl;
    std::cout << "----------" << std::endl;
    for (std::vector<body*>::iterator p = Bodies.begin(); p != Bodies.end(); ++p) {
        std::cout << (*p)->name << ", "
                  << "is movable: " << (*p)->movable << std::endl;
    }
    std::cout << "Amount of Bodies: " << Bodies.size() << std::endl;
}

// Simulating a given amount of years with given step size (in days).
// Directly outputting the results to a file with given name.
void body::simulation(double stepSize, double nYear, std::string fileName /*"data.csv"*/) {
    // Open file
    std::ofstream outdata;
    outdata.open(fileName);  // opens the file
    if (!outdata) {          // file couldn't be opened
        std::cerr << "Error: file could not be opened" << std::endl;
        exit(1);
    }

    // Writing positions to file and performing system step
    int nSteps = 365 * nYear / stepSize;
    std::cout << "Simulating System with " << nSteps << " Steps." << std::endl;
    for (int step = 0; step <= nSteps; step++) {
        for (std::vector<body*>::iterator p = Bodies.begin(); p != std::prev(Bodies.end()); ++p) {
            outdata << (*p)->position.x << ","
                    << (*p)->position.y << ","
                    << (*p)->position.z << ",";
        }
        // Last object has to be included without final " , " to prevent data column full of " ".
        body* last = Bodies.back();
        outdata << last->position.x << ","
                << last->position.y << ","
                << last->position.z << std::endl;
        body::sys_step(stepSize);
        // std::cout << step << std::endl; /* Debugging */
    }
};
