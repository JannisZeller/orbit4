#ifndef SOLVER_H
#define SOLVER_H

// Standard Libraries
#include <functional>
#include <math.h>

// Customs
#include "vec3D.h"
#include "state.h"

// Solver
// ------
// Solver is a class for integrating 3 dimensional equations of motion with 
// acceleration function f.
// The different algorithms implemented in this class assume dx/dt=f(x) with no
// explicit time dependency of f.
// Coefficents used can be found in:
//      Fehlberg, E. (1969). Low-Order Calssical Runge-Kutta Formulas with Stepsize Control
//          and their Application to some Heat Transfer Problems. Nasa Technical Report.
// which can be downloaded for free: https://ntrs.nasa.gov/citations/19690021375,
// The classical Runge-Kutta method can be found in standard textbooks on numerical
// methods for ODEs.
// The methods already implement the scheme (\dot{x}, \dot{v}) = (v, f) 
// for 6-dim state vectors.

class Solver {

   public:
    // Runge-Kutta 4th Order functional:
    static void runge_kutta_4(State& state, double dt, std::function<State(State)> f);

    // Runge-Kutta-Fehlberg 4(5)-th order method (Fehlberg, 1969, p. 12)
    static void runge_kutta_fehlberg1(State& state, double dt, std::function<State(State)> f);

    // Runge-Kutta-Fehlberg 4(5)-th order method with coefficients from D. Sarafyan
    static void runge_kutta_fehlberg2(State& state, double dt, std::function<State(State)> f);
};

#endif