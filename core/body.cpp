#include "body.h"


std::vector<Body*> Body::all_bodies;
int Body::n_bodies;


// Constructor defining initial position, velocity, mass, radius, name
Body::Body(vec3D x, vec3D v, double m, std::string name /*= "Default"*/, std::string input_units /*= "SI"*/, bool massive /*= true*/, bool movable /*= true*/) {
    this->name = name;
    this->massive = massive;
    this->movable = movable;

    // Updating static Members
    all_bodies.push_back(this);
    n_bodies++;

    // If unit system is already generic ([L]=AU, [T]=EarthDays, [M]=SunMasses), just take them:
    if (input_units == "generic") {
        this->state = State(x, v);
        mass = m;
    }
    // Otherwise convert to generic unit system:
    else if (input_units == "SI") {
        this->state = State(
            ConversionSystem<vec3D>::convert_length(x), 
            ConversionSystem<vec3D>::convert_velocity(v));
        mass     = ConversionSystem<double>::convert_mass(m);
    }
}


// Disable the body so it is not relevant for the system anymore
void Body::disable() {
    massive = false;
    movable = false;
}


// Compute the gravity force that this body introduces at space-point x
vec3D Body::compute_gravity_at(vec3D x, double smooth) {
    vec3D diff;
    diff = x - this->state.position;
    double dist_to_the_ = pow(diff.norm_sq() + smooth, -3./2.);
    return -G * mass * dist_to_the_ * diff;
}