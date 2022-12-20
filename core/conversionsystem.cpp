#include "conversionsystem.h"


// Convert Lengths
template <typename T>
T ConversionSystem<T>::convert_length(const T x) {
    return x / ConversionSystem::earth_x0;
}


// Convert Velocities
template <typename T>
T ConversionSystem<T>::convert_velocity(const T v) {
    return ConversionSystem::earthday / ConversionSystem::earth_x0 * v;
}


// Convert Masses
template <typename T>
T ConversionSystem<T>::convert_mass(const T m) {
    return m / ConversionSystem::sun_mass;
}

template class ConversionSystem<int>;
template class ConversionSystem<double>;
template class ConversionSystem<long double>;
template class ConversionSystem<vec3D>;