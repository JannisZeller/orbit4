#include "solver.h"

#include <math.h>

#include <iostream>

// Runge-Kutta 4th Order functional:
void solver::runge_kutta_4(vec3D& x, vec3D& v, double dt, std::function<vec3D(vec3D)> f) {
    // Iteravly applying RK4 algorithm for x and v:
    vec3D k1v = f(x);
    vec3D k1x = v;
    vec3D k2v = f(x + dt * k1x / 2.);
    vec3D k2x = v + dt / 2. * k1v;
    vec3D k3v = f(x + dt * k2x / 2.);
    vec3D k3x = v + dt / 2. * k2v;
    vec3D k4v = f(x + dt * k3x);
    vec3D k4x = v + dt * k3v;
    // Updating x and v:
    x = x + dt * (k1x + 2. * k2x + 2. * k3x + k4x) / 6.;
    v = v + dt * (k1v + 2. * k2v + 2. * k3v + k4v) / 6.;
}

// Runge-Kutta-Fehlberg 4(5)-th order with alpha2=1/3 (Fehlberg, 1969, p. 12)
void solver::runge_kutta_fehlberg1(vec3D& x, vec3D& v, double dt, std::function<vec3D(vec3D)> f) {
    vec3D k1v = f(x);
    vec3D k1x = v;
    vec3D k2v = f(x + dt * 2. * k1x / 9.);
    vec3D k2x = v + dt * 2. * k1v / 9.;
    vec3D k3v = f(x + dt * k1x / 12. + dt * k2x / 4.);
    vec3D k3x = v + dt * k1v / 12. + dt * k2v / 4.;
    vec3D k4v = f(x + dt * k1x * 69. / 128. - dt * k2x * 243. / 128. + dt * k3x * 135. / 64.);
    vec3D k4x = v + dt * k1v * 69. / 128. - dt * k2v * 243. / 128. + dt * k3v * 135. / 64.;
    vec3D k5v = f(x - dt * k1x * 17. / 12. + dt * k2x * 27. / 4. - dt * k3x * 27. / 5. + dt * k4x * 16. / 15.);
    vec3D k5x = v - dt * k1v * 17. / 12. + dt * k2v * 27. / 4. - dt * k3v * 27. / 5. + dt * k4v * 16. / 15.;
    vec3D k6v = f(x + dt * k1x * 65. / 432. - dt * k2x * 5. / 16. + dt * k3x * 13. / 16. + dt * k4x * 4. / 27. + dt * k5x * 5. / 144.);
    vec3D k6x = v + dt * k1v * 65. / 432. - dt * k2v * 5. / 16. + dt * k3v * 13. / 16. + dt * k4v * 4. / 27. + dt * k5v * 5. / 144.;
    x = x + dt * (47. / 450. * k1x + 0. * k2x + 12. / 25. * k3x + 32. / 255. * k4x + 1. / 30. * k5x + 6. / 25. * k6x);
    v = v + dt * (47. / 450. * k1v + 0. * k2v + 12. / 25. * k3v + 32. / 255. * k4v + 1. / 30. * k5v + 6. / 25. * k6v);
}

// Runge-Kutta-Fehlberg 4(5)-th order with alpha2=3/8 (Fehlberg, 1969, p. 13)
void solver::runge_kutta_fehlberg2(vec3D& x, vec3D& v, double dt, std::function<vec3D(vec3D)> f) {
    vec3D k1v = f(x);
    vec3D k1x = v;
    vec3D k2v = f(x + dt * k1v / 4.);
    vec3D k2x = f(x + dt * k1x / 4.);
    vec3D k3v = f(x + dt * k1x * 3. / 32. + dt * k2x * 9. / 32.);
    vec3D k3x = v + dt * k1v * 3. / 32. + dt * k2v * 9. / 32.;
    vec3D k4v = f(x + dt * k1x * 1932. / 2197. - dt * k2x * 7200. / 2197. + dt * k3x * 7296. / 2197.);
    vec3D k4x = v + dt * k1v * 1932. / 2197. - dt * k2v * 7200. / 2197. + dt * k3v * 7296. / 2197.;
    vec3D k5v = f(x + dt * k1x * 439. / 216. - dt * k2x * 8. + dt * k3x * 3680. / 513 - dt * k4x * 845. / 4104.);
    vec3D k5x = v + dt * k1v * 439. / 216. - dt * k2v * 8. + dt * k3v * 3680. / 513 - dt * k4v * 845. / 4104.;
    vec3D k6v = f(x - dt * k1x * 8. / 27. + dt * k2x * 2. - dt * k3x * 3544. / 2565. + dt * k4x * 1859. / 4104. - dt * k5x * 11. / 40.);
    vec3D k6x = v - dt * k1v * 8. / 27. + dt * k2v * 2. - dt * k3v * 3544. / 2565. + dt * k4v * 1859. / 4104. - dt * k5v * 11. / 40.;
    x = x + dt * (16. / 135. * k1x + 6656. / 12825. * k3x + 28561. / 56430. * k4x - 9. / 50. * k5x + 2. / 55. * k6x);
    v = v + dt * (16. / 135. * k1v + 6656. / 12825. * k3v + 28561. / 56430. * k4v - 9. / 50. * k5v + 2. / 55. * k6v);
}