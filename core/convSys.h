#ifndef CONVSYS
#define CONVSYS

#include "vec3D.h"

template <typename T>
class convSys {
   public:
    // Conversion of length units
    static T lenConv(const T X);

    // Conversion of velocities
    static T velConv(const T V);

    // Conversion of mass
    static double massConv(const double Mass);
};

#endif