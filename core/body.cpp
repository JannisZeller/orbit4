#include "body.h"


std::vector<Body*> Body::all_bodies;
int Body::n_bodies;


// Constructor defining initial position, velocity, mass, radius, name
Body::Body(vec3D x, vec3D v, double m, double r, std::string name /*= "Default"*/, std::string input_units /*= "SI"*/, bool massive /*= true*/, bool movable /*= true*/) {
    this->name = name;
    this->massive = massive;
    this->movable = movable;

    // Updating static Members
    all_bodies.push_back(this);
    n_bodies++;

    // If unit system is already generic ([L]=AU, [T]=EarthDays, [M]=SunMasses), just take them:
    if (input_units == "generic") {
        position = x;
        velocity = v;
        mass = m;
        radius = r;
    }
    // Otherwise convert to generic unit system:
    else if (input_units == "SI") {
        position = ConversionSystem<vec3D>::convert_length(x);
        velocity = ConversionSystem<vec3D>::convert_velocity(v);
        mass     = ConversionSystem<double>::convert_mass(m);
        radius   = ConversionSystem<double>::convert_length(r);
    }
}


// Disable the body so it does not play a role in the mortion anymore
void Body::disable() {
    massive = false;
    movable = false;
}


// Compute the gravity force that this body introduces at space-point x
vec3D Body::compute_gravity_at(vec3D x) {
    vec3D diff;
    diff = x - position;
    double dist_to_the_3 = pow(diff.norm(), 3);
    return -G * mass / dist_to_the_3 * diff;
}