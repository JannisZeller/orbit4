#include "state.h"


// Default Constructor:
State::State() {
    this->position = vec3D();
    this->velocity = vec3D();
}


// Custom Constructor:
State::State(long double x1, long double x2, long double x3, 
             long double v1, long double v2, long double v3) {
    this->position = vec3D(x1, x2, x3);
    this->velocity = vec3D(v1, v2, v3);
}
State::State(vec3D x, vec3D v) {
    this->position = x;
    this->velocity = v;
}


// Setting fixed numbers
void State::set_values(long double x1, long double x2, long double x3, 
                        long double v1, long double v2, long double v3) {
    this->position = vec3D(x1, x2, x3);
    this->velocity = vec3D(v1, v2, v3);
}
void State::set_values(vec3D x, vec3D v) {
    this->position = x;
    this->velocity = v;
}


// Overloading << operator for std::ostream
std::ostream& operator<<(std::ostream& os, const State& state) {
    os  << "State: pos " 
        << state.position.x1 << "," << state.position.x2 << "," 
        << state.position.x3 
        << "; vel " 
        << state.velocity.x1 << "," << state.velocity.x2 << "," 
        << state.velocity.x3 << std::endl;
    return os;
}


// Overloading * operator
State State::operator*(const State& other) {
    State res(position * other.position, velocity * other.velocity);
    return (res);
}
State operator*(const State& vec, const long double scalar) {
    return State(scalar * vec.position, scalar * vec.velocity);
}
State operator*(const long double scalar, const State& vec) {
    return State(scalar * vec.position, scalar * vec.velocity);
}


// Overloading / operator
State State::operator/(const State& other) {
    State res(position / other.position, velocity / other.velocity);
    return (res);
}
State operator/(const State& vec, const long double scalar) {
    return State(vec.position / scalar, vec.velocity / scalar);
}
State operator/(const long double scalar, const State& vec) {
    return State(scalar / vec.position, scalar / vec.velocity);
}


// Overloading + operator
State State::operator+(const State& other) {
    return State(position + other.position, velocity + other.velocity);
}
State operator+(const State& vec, const long double scalar) {
    return State(scalar + vec.position, scalar + vec.velocity);
}
State operator+(const long double scalar, const State& vec) {
    return State(scalar + vec.position, scalar + vec.velocity);
}


// Overloading - operator
State State::operator-(const State& other) {
    return State(position - other.position, velocity - other.velocity);
}
State operator-(const State& vec, const long double scalar) {
    return State(scalar - vec.position, scalar - vec.velocity);
}
State operator-(const long double scalar, const State& vec) {
    return State(scalar - vec.position, scalar - vec.velocity);
}


