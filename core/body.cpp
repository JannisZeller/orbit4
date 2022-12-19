#include "body.h"


std::vector<Body*> Body::all_bodies;
int Body::n_bodies;


// Constructor defining initial position, velocity, mass, radius, name
Body::Body(vec3D x, vec3D v, double mass, std::string name /*= "Default"*/, std::string input_units /*= "SI"*/, bool massive /*= true*/, bool movable /*= true*/) {
    this->name = name;
    this->massive = massive;
    this->movable = movable;

    // Updating static Members
    all_bodies.push_back(this);
    n_bodies++;

    // If unit system is already generic ([L]=AU, [T]=EarthDays, [M]=SunMasses), just take them:
    if (input_units == "generic") {
        this->state = State(x, v);
        this->mass = mass;
    }
    // Otherwise convert to generic unit system:
    else if (input_units == "SI") {
        this->state = State(
            ConversionSystem<vec3D>::convert_length(x), 
            ConversionSystem<vec3D>::convert_velocity(v));
        this->mass = ConversionSystem<double>::convert_mass(mass);
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
    double exp = -1.5;
    double dist_to_the_exp = pow(diff.norm_sq() + smooth, exp);
    return -G * mass * dist_to_the_exp * diff;
}