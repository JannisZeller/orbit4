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
    static double constexpr sun_mass   = 1.98847e+30; //!
    // Earth
    static double constexpr earthday     = 86400.; //!
    static double constexpr earth_x0     = 149597870700.; //!
    static double constexpr earth_v0     = 29290.;
    static double constexpr earth_mass   = 5.972e+24;
    // Mercury
    static double constexpr mercury_x0   = 46000000000.;
    static double constexpr mercury_v0   = 58980.;
    static double constexpr mercury_mass = 3.285e+23;
    // Mars
    static double constexpr mars_x0   = 227987154947.;
    static double constexpr mars_v0   = 27070.;
    static double constexpr mars_mass = 6.39004e+23;
    // Moon (Relative to Earth)
    static double constexpr moon_x0   = 383397000.;
    static double constexpr moon_v0   = 1023.;
    static double constexpr moon_mass = 7.3456e+22;
    // Gravity Constant
    static double constexpr G_SI      = 6.67430e-11;
    static double constexpr G_generic = 0.0002959211565456235; // 8.887441836640551e-10 // G_SI / earth_x0**3 * earthday**2 * earth_mass;

    // Convert Lengths
    static T convert_length(const T x);

    // Convert Velocities
    static T convert_velocity(const T v);

    // Convert Masses
    static T convert_mass(const T m);
};

#endif