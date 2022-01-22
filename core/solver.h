#ifndef SOLVER
#define SOLVER

#include "vec3D.h"

typedef double (*DoubleCall)(int args);
typedef vec3D (*Vec3DCall)(int args);

class solver {
   public:
    // Test functional:
    static void test_solver(double x, DoubleCall f);

    // Runge-Kutta 4th Order functional:
    static void runge_kutta_4(double x, DoubleCall f);
};

#endif