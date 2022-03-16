#include "body.h"

std::vector<body*> body::Bodies;

body::body(vec3D Pos, vec3D Vel, double m, double r, std::string str /*= "Default"*/, std::string uSys /*= "SI"*/, bool bool_massive /*= true*/, bool bool_movable /*= true*/) {
    // if (Bodies.size() == 0) {
    //     Bodies.reserve(100);
    // }
    name = str;
    massive = bool_massive;
    movable = bool_movable;
    Bodies.push_back(this);
    // count.push_back(1);
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

// Calculates the acceleration of the body it is called from at any given
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
vec3D body::sum_acceleration() {
    vec3D A(0.0, 0.0, 0.0);
    for (std::vector<body*>::iterator p = Bodies.begin(); p != Bodies.end(); ++p) {
        if ((*p)->massive && (*p) != this) {
            double num;
            vec3D temp = (*p)->compute_acceleration(position);
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
    auto temp = std::bind(&body::sum_acceleration, this);  // TODO: Probably to be regarded and changed...
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

// void body::sys_step(double dt, std::string algo = "rkf2") {
//     for (std::vector<body*>::iterator p = Bodies.begin(); p != Bodies.end(); ++p) {
//         if ((*p)->movable) {
//             vec3D Pos_temp(0., 0., 0.);
//             vec3D Pos_n;
//             Pos_n = (*p)->position;

//             if (algo == "rk4") {
//             }

//             else if (algo == "rkf45a13") {
//             }

//             else if (algo == "rkf45a38") {
//             }

//             else {
//                 throw std::invalid_argument("Algorithm for update rule in body.step not defined/available!");
//             }
//         }
//     }
// }

void body::print_bodies() {
    std::cout << "----------" << std::endl;
    std::cout << "ALL BODIES" << std::endl;
    std::cout << "----------" << std::endl;
    for (std::vector<body*>::iterator p = Bodies.begin(); p != Bodies.end(); ++p) {
        std::cout << (*p)->name << ", "
                  << "is movable: " << (*p)->movable << std::endl;
    };
    std::cout << "Amount of Bodies: " << Bodies.size() << std::endl;
}
