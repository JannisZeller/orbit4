#include "convSys.h"

template <typename T>
T convSys<T>::lenConv(const T X) {
    // Conversion from meters to AU
    double AU = 149597870700.0;
    T XConv = X / AU;
    return XConv;
}

template <typename T>
T convSys<T>::velConv(const T V) {
    // Conversion from meters to AU
    double AU = 149597870700.0;
    // Conversion from seconds to earth days
    double ED = 86400.0;
    T VConv = V / AU * ED;
    return VConv;
}

template <typename T>
double convSys<T>::massConv(const double Mass) {
    // Conversion to Sun masses
    double sunMass = 1.98847e30;
    double MConv = Mass / sunMass;
    return MConv;
}

template class convSys<int>;
template class convSys<double>;
template class convSys<vec3D>;