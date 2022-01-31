#ifndef SOLVER
#define SOLVER

#include "vec3D.h"

typedef double (*DoubleCall)(double args);
typedef vec3D (*Vec3DCall)(vec3D args);
typedef void (*voidCall)(vec3D args);

// The different algorithms implemented in this class assume dx/dt=f(x) with no
// explicit time dependency of f.
// Coefficents used can be found in:
// Fehlberg, E. (1969). Low-Order Calssical Runge-Kutta Formulas with Stepsize Control
//     and their Application to some Heat Transfer Problems. Nasa Technical Report.
// which can be downloaded for free: https://ntrs.nasa.gov/citations/19690021375,
// The classical Runge-Kutta method can be found in standard textbooks on numerical
// methods for ODEs. Also e.g. under https://www.spektrum.de/lexikon/mathematik/runge-kutta-methode/8698.

// The methods already implement the scheme (\dot{x}, \dot{v}) = (v, f), where f=ma. An alternative
// to implement x and v explicitely would be to use a new class with 6-dim vectors, which then can be
// used directly with the algorithms (see .cpp-files for better understanding).

class solver {
   public:
    // Runge-Kutta 4th Order functional: Vec3DCall f
    static void runge_kutta_4(vec3D& x, vec3D& v, double dt, vec3D (*f)(vec3D));

    // Runge-Kutta-Fehlberg 4(5)-th order method (Fehlberg, 1969, p. 12)
    static void runge_kutta_fehlberg1(vec3D& x, vec3D& v, double dt, Vec3DCall f);

    // Runge-Kutta-Fehlberg 4(5)-th order method with coefficients from D. Sarafyan
    static void runge_kutta_fehlberg2(vec3D& x, vec3D& v, double dt, Vec3DCall f);
};

#endif