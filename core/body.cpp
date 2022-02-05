#include "body.h"

std::vector<body*> body::Bodies;

body::body(vec3D Pos, vec3D Vel, double m, double r, std::string str /*= "Default"*/, std::string uSys /*= "SI"*/, bool bool_massive /*= true*/, bool bool_movable /*= true*/) {
    // if (Bodies.size() == 0) {
    //     Bodies.reserve(100);
    // }
    name = str;
    massive = bool_massive;
    movable = bool_movable;
    Bodies.push_back(this);
    // count.push_back(1);
    // If unit system is already generic ([L]=AU, [T]=EarthDays, [M]=SunMasses), just take them:
    if (uSys == "generic") {
        position = Pos;
        velocity = Vel;
        mass = m;
        radius = r;
    }
    // Otherwise convert to generic unit system:
    else if (uSys == "SI") {
        position = convSys<vec3D>::lenConv(Pos);
        velocity = convSys<vec3D>::velConv(Vel);
        mass = convSys<double>::massConv(m);
        radius = convSys<double>::lenConv(r);
    }
}

vec3D body::compute_acceleration(vec3D X) {
    vec3D diff;
    diff = X - position;
    double dist = diff.norm();
    double distPow = pow(dist, 3);
    return -G * mass / distPow * diff;
}

// void body::sum_acceleration(vec3D& A) {
//     A << 0., 0., 0.;
//     vec3D temp;
//     for (std::vector<body*>::iterator p = Bodies.begin(); p != Bodies.end(); ++p) {
//         if ((*p)->massive && (*p) != this) {
//             double num;
//             (*p)->compute_acceleration(position, temp);
//             A += temp;
//         }
//     }
// }

void body::step(double dt, body other, std::string algo /*= "rkf2"*/) {
    auto temp = std::bind(&body::compute_acceleration, other, std::placeholders::_1);
    if (algo == "rk4") {
        solver::runge_kutta_4(this->position, this->velocity, dt, temp);
    } else if (algo == "rkf1") {
        solver::runge_kutta_fehlberg1(this->position, this->velocity, dt, temp);
    } else if (algo == "rkf2") {
        solver::runge_kutta_fehlberg2(this->position, this->velocity, dt, temp);
    } else {
        throw std::invalid_argument("Algorithm for update rule in body.step not defined/available!");
    }
}

/*
void body::sys_step(double dt, std::string algo = "rkf2") {
    for (std::vector<body*>::iterator p = Bodies.begin(); p != Bodies.end(); ++p) {
        if ((*p)->movable) {
            vec3D Pos_temp(0., 0., 0.);
            vec3D Pos_n;
            Pos_n = (*p)->position;
            vec3D k1, k1v, k2, k2v, k3, k3v, k4, k4v, k5, k5v, k6, k6v;

            if (algo == "rk4") {
                // 4th order Runge-Kutta
                k1 = (*p)->velocity;
                (*p)->sum_acceleration(k1v);

                k2 = k1 + dt / 2 * k1v;
                Pos_temp = dt / 2 * k1 + Pos_n;
                (*p)->sum_acceleration(k2v);

                k3 = k1 + dt / 2 * k2v;
                Pos_temp = dt / 2 * k2 + Pos_n;
                (*p)->sum_acceleration(k3v);

                k4 = k1 + dt * k3v;
                Pos_temp = dt / 2 * k3 + Pos_n;
                (*p)->sum_acceleration(k4v);

                k5 = k1 + dt * k4v;
                Pos_temp = dt / 2 * k4 + Pos_n;
                (*p)->sum_acceleration(k5v);

                vec3D xn, vn;
                xn = Pos_n + dt / 6 * (k1 + 2 * k2 + 2 * k3 + k4);
                vn = (*p)->velocity + dt / 6 * (k1v + 2 * k2v + 2 * k3v + k4v);
                (*p)->position << xn;
                (*p)->velocity << vn;
            }

            else if (algo == "rkf45a13") {
                // 4(5)th order Runge-Kutta-Fehlberg with alpha2=1/3
                k1 = (*p)->velocity;
                (*p)->sum_acceleration(k1v);

                k2 = k1 + dt * 2. / 9. * k1v;
                Pos_temp = dt * 2. / 9. * k1 + Pos_n;
                (*p)->sum_acceleration(k2v);

                k3 = k1 + dt / 12. * k2v + dt * 1. / 4. * k1v;
                Pos_temp = dt / 12. * k2 + dt * 1. / 4. * k1 + Pos_n;
                (*p)->sum_acceleration(k3v);

                k4 = k1 + dt * 69. / 128. * k3v - dt * 243. / 128. * k2v + dt * 135. / 64. * k1v;
                Pos_temp = dt * 69. / 128. * k3 - dt * 243. / 128. * k2 + dt * 135. / 64. * k1 + Pos_n;
                (*p)->sum_acceleration(k4v);

                k5 = k1 - dt * 17. / 12. * k4v + dt * 27. / 4. * k3v - dt * 27. / 5. * k2v + dt * 16. / 15. * k1v;
                Pos_temp = -dt * 17. / 12. * k4 + dt * 27. / 4. * k3 - dt * 27. / 5. * k2 + dt * 16. / 15. * k1 + Pos_n;
                (*p)->sum_acceleration(k5v);

                k6 = k1 + dt * 65. / 432. * k5v - dt * 5. / 16. * k4v + dt * 13. / 16. * k3v + dt * 4. / 27. * k2v + dt * 5. / 144. * k1v;
                Pos_temp = dt * 65. / 432. * k5 - dt * 5. / 16. * k4 + dt * 13. / 16. * k3 + dt * 4. / 27. * k2 + dt * 5. / 144. * k1v + Pos_n;
                (*p)->sum_acceleration(k6v);

                vec3D xn, vn;
                xn = Pos_n + dt * (47. / 450. * k1 + 0. * k2 + 12. / 25. * k3 + 32. / 255. * k4 + 1. / 30. * k5 + 6. / 25. * k6);
                vn = (*p)->velocity + dt * (47. / 450. * k1v + 0. * k2v + 12. / 25. * k3v + 32. / 255. * k4v + 1. / 30. * k5v + 6. / 25. * k6v);
                (*p)->position << xn;
                (*p)->velocity << vn;
            }

            else if (algo == "rkf45a38") {
                // 4(5)th order Runge-Kutta-Fehlberg with alpha2=1/3
                k1 = (*p)->velocity;
                (*p)->sum_acceleration(k1v);

                k2 = k1 + dt * 1. / 4. * k1v;
                Pos_temp = dt * 1. / 4. * k1 + Pos_n;
                (*p)->sum_acceleration(k2v);

                k3 = k1 + dt * 3. / 32. * k2v + dt * 9. / 32. * k1v;
                Pos_temp = dt * 3. / 32. * k2 + dt * 9. / 32. * k1 + Pos_n;
                (*p)->sum_acceleration(k3v);

                k4 = k1 + dt * 1932. / 2197. * k3v - dt * 7200. / 2197. * k2v + dt * 7296. / 2197. * k1v;
                Pos_temp = dt * 1932. / 2197. * k3 - dt * 7200. / 2197. * k2 + dt * 7296. / 2197. * k1 + Pos_n;
                (*p)->sum_acceleration(k4v);

                k5 = k1 + dt * 439. / 216. * k4v - dt * 8. * k3v + dt * 3680. / 513. * k2v - dt * 845. / 4104. * k1v;
                Pos_temp = +dt * 439. / 216. * k4 - dt * 8. * k3 + dt * 3680. / 513. * k2 - dt * 845. / 4104. * k1 + Pos_n;
                (*p)->sum_acceleration(k5v);

                k6 = k1 - dt * 8. / 27. * k5v + dt * 2. * k4v - dt * 3544. / 2565. * k3v + dt * 1859. / 4104. * k2v - dt * 11. / 40. * k1v;
                Pos_temp = -dt * 8. / 27. * k5 + dt * 2. * k4 - dt * 3544. / 2565. * k3 + dt * 1859. / 4104. * k2 - dt * 11. / 40. * k1v + Pos_n;
                (*p)->sum_acceleration(k6v);

                vec3D xn, vn;
                xn = Pos_n + dt * (16. / 135. * k1 + 0. * k2 + 6656. / 12825. * k3 + 28561. / 56430. * k4 - 9. / 50. * k5 + 2. / 55. * k6);
                vn = (*p)->velocity + dt * (16. / 135. * k1v + 0. * k2v + 6656. / 12825. * k3v + 28561. / 56430. * k4v - 9. / 50. * k5v + 2. / 55. * k6v);
                (*p)->position << xn;
                (*p)->velocity << vn;
            }

            else {
                throw std::invalid_argument("Algorithm for update rule in body.step not defined/available!");
            }
        }
    }
}
*/

void body::print_bodies() {
    for (std::vector<body*>::iterator p = Bodies.begin(); p != Bodies.end(); ++p) {
        std::cout << (*p)->movable << std::endl;
    };
    std::cout << Bodies.size() << std::endl;
}
