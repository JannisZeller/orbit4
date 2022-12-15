#ifndef CONVSYS_H
#define CONVSYS_H

// Standard Libraries
#include <math.h>

// Customs
#include "vec3D.h"

// ConversionSystem
// ----------------
// Class for converting SI units to generic units. Additionally storing some 
// information on planets etc. Constants marked with //! are important for the
// conversion.

template <typename T>
class ConversionSystem {
  public:
    // Set of useful constants
    // - - - - - - - - - - - -
    // Sun 
    static double constexpr sun_radius = 695700000;
    static double constexpr sun_mass   = 1.98847e30; //!
    // Earth
    static double constexpr earthday     = 86400; //!
    static double constexpr earth_x0     = 149597870700; //!
    static double constexpr earth_v0     = 30300;
    static double constexpr earth_mass   = 5.972e24;
    static double constexpr earth_radius = 6371000;
    // Mercury
    static double constexpr mercury_x0 = 46000000000;
    static double constexpr mercury_v0 = 58980;
    // Moon (Relative to Earth)
    static double constexpr moon_x0 = 383397000;
    static double constexpr moon_v0 = 1023;
    // Gravity Constant
    static double constexpr G_SI      = 6.67430e-11;
    static double constexpr G_generic = G_SI / pow(earth_x0, 3) * pow(earthday, 2) * sun_mass;

    // Convert Lengths
    static T convert_length(const T x);

    // Convert Velocities
    static T convert_velocity(const T v);

    // Convert Masses
    static T convert_mass(const T m);
};

#endif