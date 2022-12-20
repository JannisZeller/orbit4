#include "solver.h"


// Runge-Kutta 4th Order functional:
void Solver::runge_kutta_4(State& state, long double dt, std::function<State(State)> f) {
    // Iteravly applying RK4 algorithm for x and v:
    State k1 = dt * f(state);
    State k2 = dt * f(state + 1./2. * k1);
    State k3 = dt * f(state              + 1./2. * k2);
    State k4 = dt * f(state                           +         k3);
    state =           state + 1./6. * k1 + 1./3. * k2 + 1./3. * k3 + 1./6. * k4;
}


// Runge-Kutta-Fehlberg 4(5)-th order method (Fehlberg, 1969, p. 12)
void Solver::runge_kutta_fehlberg1(State& state, long double dt, std::function<State(State)> f) {
    State k1 = dt * f(state);
    State k2 = dt * f(state +  2./9.   * k1);
    State k3 = dt * f(state +  1./12.  * k1 +   1./ 4.  * k2);
    State k4 = dt * f(state + 69./128. * k1 - 243./128. * k2 + 135./64. * k3);
    State k5 = dt * f(state - 17./12.  * k1 +  27./4.   * k2 -  27./5.  * k3 + 16./15.  * k4);
    State k6 = dt * f(state + 65./432. * k1 -   5./16.  * k2 +  13./16. * k3 +  4./27.  * k4 + 5./144. * k5);
    state =           state + 47./450. * k1                  +  12./25. * k3 + 32./225. * k4 + 1./30.  * k5 + 6./25. * k6;
}


// Runge-Kutta-Fehlberg 4(5)-th order method with coefficients from D. Sarafyan
void Solver::runge_kutta_fehlberg2(State& state, long double dt, std::function<State(State)> f) {
    State k1 = dt * f(state);
    State k2 = dt * f(state +    1./4.    * k1);
    State k3 = dt * f(state +    3./32.   * k1 +    9./ 32.  * k2         );
    State k4 = dt * f(state + 1932./2197. * k1 - 7200./2197. * k2 + 7296./2197.  * k3);
    State k5 = dt * f(state +  439./216.  * k1 -    8.       * k2 + 3680./513.   * k3 -   845./4104.  * k4);
    State k6 = dt * f(state -    8./27.   * k1 +    2.       * k2 - 3544./2565.  * k3 +  1859./4104.  * k4 - 11./40. * k5);
    state =           state +   16./135.  * k1                    + 6656./12825. * k3 + 28561./56430. * k4 -  9./50. * k5 + 2./55. * k6;
}