#ifndef SOLVER
#define SOLVER

class solver {
   public:
    // Conversion of length units
    static T lenConv(const T X);

    // Conversion of velocities
    static T velConv(const T V);

    // Conversion of mass
    static double massConv(const double Mass);
};

#endif