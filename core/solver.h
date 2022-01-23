#ifndef SOLVER
#define SOLVER

#include "vec3D.h"

typedef double (*DoubleCall)(double args);
typedef vec3D (*Vec3DCall)(vec3D args);

// The different algorithms implemented in this class assume dx/dt=f(x) with no
// explicit time dependency of f.
// Coefficents used can be found in:
// Fehlberg, E. (1969). Low-Order Calssical Runge-Kutta Formulas with Stepsize Control
//     and their Application to some Heat Transfer Problems. Nasa Technical Report.
// which can be downloaded for free: https://ntrs.nasa.gov/citations/19690021375,
// The classical Runge-Kutta method can be found in standard textbooks on numerical
// methods for ODEs. Also e.g. under https://www.spektrum.de/lexikon/mathematik/runge-kutta-methode/8698.

// Be careful, the returns are updated velocities, not updated positions already!

class solver {
   public:
    // Test functional:
    static void test_solver(double x, DoubleCall f);

    // Runge-Kutta 4th Order functional:
    static vec3D runge_kutta_4(vec3D x, double dt, Vec3DCall f);

    // Runge-Kutta-Fehlberg 5-th order method (Fehlberg, 1969, p. 12)
    static vec3D runge_kutta_fehlberg1(vec3D x, double dt, Vec3DCall f);

    // Runge-Kutta-Fehlberg 5-th order method with coefficients from D. Sarafyan
    static vec3D runge_kutta_fehlberg2(vec3D x, double dt, Vec3DCall f);
};

#endif