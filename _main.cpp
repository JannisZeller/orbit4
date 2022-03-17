// Main CPP-File for creating orbital simulations of multibody systems.
// Currently leads to compiler problems with g++ as updated via MSYS2 because of the vector.push_back problem: https://stackoverflow.com/questions/69773877/getting-throw-bad-array-new-lengthv-when-trying-to-use-stdvectorpush-back
// Using g++ 8.3.0, gdb 10.1 from Rtools mingw64 works (29.01.2022).
#include <math.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//#include "core/Body.h"
#include "core/body.h"
#include "core/convSys.h"
#include "core/solver.h"
#include "core/vec3D.h"

using namespace std;

/*
class convSys {
   public:
    template <class T>
    static T lenConv(T X) {
        // Conversion from meters to AU
        const double AU = 149597870700.0;
        T XConv = X / AU;
        return XConv;
    }

    static Vector3d velConv(Vector3d V) {
        // Conversion from meters to AU
        const double AU = 149597870700.0;
        // Conversion from seconds to earth days
        const double ED = 86400.0;
        Vector3d VConv = V / AU * ED;
        return VConv;
    }

    static double massConv(double Mass) {
        // Conversion to Sun masses
        const double sunMass = 1.98847e30;
        double MConv = Mass / sunMass;
        return MConv;
    }
};
*/

/*
class vec3D {
   private:
    double x;
    double y;
    double z;

   public:
    // Default constructor of the vec3D class:
    vec3D(double x1, double x2, double x3) {
        x = x1;
        y = x2;
        z = x3;
    }

    // Overloading << operator
    friend ostream& operator<<(ostream& os, const vec3D& vec) {
        os << vec.x << ',' << vec.y << ',' << vec.z << endl;
        return os;
    };

    // Overloading * operator for dot product
    double operator*(vec3D& other) {
        return (x * other.x + y * other.y + z * other.z);
    }
    // Overloading scalar multiplication
    friend vec3D operator*(vec3D& vec, double scalar);
    friend vec3D operator*(double scalar, vec3D& vec);

    // Overloading + operator
    vec3D operator+(vec3D& other) {
        return vec3D(x + other.x, y + other.y, z + other.z);
    }
    // Overloading scalar addition
    friend vec3D operator+(vec3D& vec, double scalar);
    friend vec3D operator+(double scalar, vec3D& vec);

    // Overloading - operator
    vec3D operator-(vec3D& other) {
        return vec3D(x - other.x, y - other.y, z - other.z);
    }
    // Overloading scalar substraction
    friend vec3D operator-(vec3D& vec, double scalar);
    friend vec3D operator-(double scalar, vec3D& vec);

    // Norm for length of vector
    double norm() {
        return (sqrt(x * x + y * y + z * z));
    }
};

// Overloading scalar multiplication
vec3D operator*(vec3D& vec, double scalar) {
    return vec3D(scalar * vec.x, scalar * vec.y, scalar * vec.z);
};
vec3D operator*(double scalar, vec3D& vec) {
    return vec3D(scalar * vec.x, scalar * vec.y, scalar * vec.z);
};

// Overloading scalar addition
vec3D operator+(vec3D& vec, double scalar) {
    return vec3D(scalar + vec.x, scalar + vec.y, scalar + vec.z);
};
vec3D operator+(double scalar, vec3D& vec) {
    return vec3D(scalar + vec.x, scalar + vec.y, scalar + vec.z);
};

// Overloading scalar substraction
vec3D operator-(vec3D& vec, double scalar) {
    return vec3D(scalar - vec.x, scalar - vec.y, scalar - vec.z);
};
vec3D operator-(double scalar, vec3D& vec) {
    return vec3D(scalar - vec.x, scalar - vec.y, scalar - vec.z);
};
*/

/*
class Body {
   public:
    string name;
    Vector3d position;
    Vector3d velocity;
    double x1;
    double x2;
    double x3;
    double v1;
    double v2;
    double v3;
    double mass;
    double radius;
    bool massive;
    bool movable;
    static vector<Body*> Bodies;
    // conversion to AU and     t=1EarthDay and   m=1Sunmass
    double G = 6.67430e-11 / pow(149597870700.0, 3) * pow(86400.0, 2) * 1.98847e30;

    Body(Vector3d Pos, Vector3d Vel, double m, double r, string str = "Default", string uSys = "SI", bool bool_massive = true, bool bool_movable = true) {
        name = str;
        massive = bool_massive;
        movable = bool_movable;
        Bodies.push_back(this);
        // If unit system is already generic ([L]=AU, [T]=EarthDays, [M]=SunMasses), just take them:
        if (uSys == "generic") {
            position = Pos;
            velocity = Vel;
            mass = m;
            radius = r;
        }
        // Otherwise convert to generic unit system:
        else if (uSys == "SI") {
            position = convSys::lenConv(Pos);
            velocity = convSys::velConv(Vel);
            mass = convSys::massConv(m);
            radius = convSys::lenConv(r);
        }
    }

    void compute_acceleration(Vector3d X, Vector3d& A) {
        Vector3d diff;
        diff = X - position;
        double dist = diff.norm();
        double distPow = pow(dist, 3);
        A = -G * mass / distPow * diff;
    }

    void sum_acceleration(Vector3d& A) {
        A << 0., 0., 0.;
        Vector3d temp;
        for (vector<Body*>::iterator p = Bodies.begin(); p != Bodies.end(); ++p) {
            if ((*p)->massive && (*p) != this) {
                double num;
                (*p)->compute_acceleration(position, temp);
                A += temp;
            }
        }
    }

    void step(double dt, Body* other, string algo = "rkf45a38") {
        Vector3d A_temp, Pos_temp;
        Vector3d k1, k1v, k2, k2v, k3, k3v, k4, k4v, k5, k5v, k6, k6v;

        if (algo == "rk4") {
            // 4th order Runge-Kutta
            k1 = velocity;
            other->compute_acceleration(position, k1v);

            k2 = k1 + dt / 2 * k1v;
            Pos_temp = dt / 2 * k1 + position;
            other->compute_acceleration(Pos_temp, k2v);

            k3 = k1 + dt / 2 * k2v;
            Pos_temp = dt / 2 * k2 + position;
            other->compute_acceleration(Pos_temp, k3v);

            k4 = k1 + dt * k3v;
            Pos_temp = dt / 2 * k3 + position;
            other->compute_acceleration(Pos_temp, k4v);

            k5 = k1 + dt * k4v;
            Pos_temp = dt / 2 * k4 + position;
            other->compute_acceleration(Pos_temp, k5v);

            Vector3d xn, vn;
            xn = position + dt / 6 * (k1 + 2 * k2 + 2 * k3 + k4);
            vn = velocity + dt / 6 * (k1v + 2 * k2v + 2 * k3v + k4v);
            position << xn;
            velocity << vn;
        }

        else if (algo == "rkf45a13") {
            // 4(5)th order Runge-Kutta-Fehlberg with alpha2=1/3
            k1 = velocity;
            other->compute_acceleration(position, k1v);

            k2 = k1 + dt * 2. / 9. * k1v;
            Pos_temp = dt * 2. / 9. * k1 + position;
            other->compute_acceleration(Pos_temp, k2v);

            k3 = k1 + dt / 12. * k2v + dt * 1. / 4. * k1v;
            Pos_temp = dt / 12. * k2 + dt * 1. / 4. * k1 + position;
            other->compute_acceleration(Pos_temp, k3v);

            k4 = k1 + dt * 69. / 128. * k3v - dt * 243. / 128. * k2v + dt * 135. / 64. * k1v;
            Pos_temp = dt * 69. / 128. * k3 - dt * 243. / 128. * k2 + dt * 135. / 64. * k1 + position;
            other->compute_acceleration(Pos_temp, k4v);

            k5 = k1 - dt * 17. / 12. * k4v + dt * 27. / 4. * k3v - dt * 27. / 5. * k2v + dt * 16. / 15. * k1v;
            Pos_temp = -dt * 17. / 12. * k4 + dt * 27. / 4. * k3 - dt * 27. / 5. * k2 + dt * 16. / 15. * k1 + position;
            other->compute_acceleration(Pos_temp, k5v);

            k6 = k1 + dt * 65. / 432. * k5v - dt * 5. / 16. * k4v + dt * 13. / 16. * k3v + dt * 4. / 27. * k2v + dt * 5. / 144. * k1v;
            Pos_temp = dt * 65. / 432. * k5 - dt * 5. / 16. * k4 + dt * 13. / 16. * k3 + dt * 4. / 27. * k2 + dt * 5. / 144. * k1v + position;
            other->compute_acceleration(Pos_temp, k6v);

            Vector3d xn, vn;
            xn = position + dt * (47. / 450. * k1 + 0. * k2 + 12. / 25. * k3 + 32. / 255. * k4 + 1. / 30. * k5 + 6. / 25. * k6);
            vn = velocity + dt * (47. / 450. * k1v + 0. * k2v + 12. / 25. * k3v + 32. / 255. * k4v + 1. / 30. * k5v + 6. / 25. * k6v);
            position << xn;
            velocity << vn;
        }

        else if (algo == "rkf45a38") {
            // 4(5)th order Runge-Kutta-Fehlberg with alpha2=1/3
            k1 = velocity;
            other->compute_acceleration(position, k1v);

            k2 = k1 + dt * 1. / 4. * k1v;
            Pos_temp = dt * 1. / 4. * k1 + position;
            other->compute_acceleration(Pos_temp, k2v);

            k3 = k1 + dt * 3. / 32. * k2v + dt * 9. / 32. * k1v;
            Pos_temp = dt * 3. / 32. * k2 + dt * 9. / 32. * k1 + position;
            other->compute_acceleration(Pos_temp, k3v);

            k4 = k1 + dt * 1932. / 2197. * k3v - dt * 7200. / 2197. * k2v + dt * 7296. / 2197. * k1v;
            Pos_temp = dt * 1932. / 2197. * k3 - dt * 7200. / 2197. * k2 + dt * 7296. / 2197. * k1 + position;
            other->compute_acceleration(Pos_temp, k4v);

            k5 = k1 + dt * 439. / 216. * k4v - dt * 8. * k3v + dt * 3680. / 513. * k2v - dt * 845. / 4104. * k1v;
            Pos_temp = +dt * 439. / 216. * k4 - dt * 8. * k3 + dt * 3680. / 513. * k2 - dt * 845. / 4104. * k1 + position;
            other->compute_acceleration(Pos_temp, k5v);

            k6 = k1 - dt * 8. / 27. * k5v + dt * 2. * k4v - dt * 3544. / 2565. * k3v + dt * 1859. / 4104. * k2v - dt * 11. / 40. * k1v;
            Pos_temp = -dt * 8. / 27. * k5 + dt * 2. * k4 - dt * 3544. / 2565. * k3 + dt * 1859. / 4104. * k2 - dt * 11. / 40. * k1v + position;
            other->compute_acceleration(Pos_temp, k6v);

            Vector3d xn, vn;
            xn = position + dt * (16. / 135. * k1 + 0. * k2 + 6656. / 12825. * k3 + 28561. / 56430. * k4 - 9. / 50. * k5 + 2. / 55. * k6);
            vn = velocity + dt * (16. / 135. * k1v + 0. * k2v + 6656. / 12825. * k3v + 28561. / 56430. * k4v - 9. / 50. * k5v + 2. / 55. * k6v);
            position << xn;
            velocity << vn;
        }

        else {
            throw invalid_argument("Algorithm for update rule in Body.step not defined/available!");
        }
    }

    static void sys_step(double dt, string algo = "rkf45a38") {
        for (vector<Body*>::iterator p = Bodies.begin(); p != Bodies.end(); ++p) {
            if ((*p)->movable) {
                Vector3d Pos_temp(0., 0., 0.);
                Vector3d Pos_n;
                Pos_n = (*p)->position;
                Vector3d k1, k1v, k2, k2v, k3, k3v, k4, k4v, k5, k5v, k6, k6v;

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

                    Vector3d xn, vn;
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

                    Vector3d xn, vn;
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

                    Vector3d xn, vn;
                    xn = Pos_n + dt * (16. / 135. * k1 + 0. * k2 + 6656. / 12825. * k3 + 28561. / 56430. * k4 - 9. / 50. * k5 + 2. / 55. * k6);
                    vn = (*p)->velocity + dt * (16. / 135. * k1v + 0. * k2v + 6656. / 12825. * k3v + 28561. / 56430. * k4v - 9. / 50. * k5v + 2. / 55. * k6v);
                    (*p)->position << xn;
                    (*p)->velocity << vn;
                }

                else {
                    throw invalid_argument("Algorithm for update rule in Body.step not defined/available!");
                }
            }
        }
    }

    static void print_bodies() {
        for (vector<Body*>::iterator p = Bodies.begin(); p != Bodies.end(); ++p) {
            cout << (*p)->movable << endl;
        }
    }
};

vector<Body*> Body::Bodies;

void test(vector<Body> bodyVec) {
    for (int i = 0; i < bodyVec.size(); i++) {
        cout << "Test" << endl;
        cout << bodyVec[i].name << endl;
    }
};

*/

/*
typedef double (*vFunctionCall)(int args);
void test_solver(double x, vFunctionCall f) {
    double x1 = x;
    double y = f(x);
    std::cout << y << std::endl;
}
*/

double test(double x) {
    return 2. * x;
}

// Only works, when input vector is of type (x1, 0, 0)
vec3D harmomic_potential(vec3D x) {
    double D = 1.;
    vec3D ret = -D * x;
    return (ret);
}

int main() {
    //--------------------------------------------------------------------------------
    // Testing solver class:
    /*
    cout << "Testing solver Class (RK4) with harmonic oscillator:" << endl;
    double dt = 0.05;
    vec3D x(2., 0., 0.);
    vec3D f = harmomic_potential(x);
    cout << f;

    vec3D v(0., 0., 0.);
    solver::runge_kutta_4(x, v, dt, harmomic_potential);
    cout << x;
    cout << v;


    ofstream outdata;

    outdata.open("dataTest.csv");  // opens the file
    if (!outdata) {                // file couldn't be opened
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }

    for (int nstep = 0; nstep <= (int)1000; nstep++) {
        outdata << x.x << " , "
                << x.y << " , "
                << x.z << " , ";
        outdata << v.x << " , "
                << v.y << " , "
                << v.z << " , ";
        outdata << harmomic_potential(x).x << endl;
        solver::runge_kutta_fehlberg1(x, v, dt, harmomic_potential);
    }
    outdata.close();
    */

    //--------------------------------------------------------------------------------
    // Testing body class:

    const double NYear = 5.;
    const int stepsPerDay = 1;
    const double SUN_MASS = 1.989e30;
    const double SUN_RADIUS = 695700000.0;
    const double EARTH_X0 = -147095000000.0;
    const double EARTH_Y0 = 0.0;
    const double EARTH_VX0 = 0.0;
    const double EARTH_VY0 = -30300.0;
    const double EARTH_MASS = 5.972e24;
    const double EARTH_RADIUS = 6371000.0;
    // const double dt = 1. / (double)stepsPerDay;  // 1 earth day = 86400.0 seconds

    // Sun 1
    vec3D Pos(0.0, 0.0, 0.0);
    vec3D Vel(0.0, 0.0, 0.0);

    body sun(Pos, Vel, 1, SUN_RADIUS, "Sun", "generic", true, true);

    // Sun 2
    vec3D Pos1(1.0, 0.0, 0.0);
    vec3D Vel1(0.0, 0.01, 0.0);

    body sun2(Pos1, Vel1, 0.01, SUN_RADIUS, "Sun", "generic", true, true);

    // Mecury
    // Pos << -46000000000.0, 0.0, 0.0;
    // Vel << 0.0, -58980.0, 0;
    // Body mercury(Pos, Vel, 0.33011e24, 2439700., "Mercury");

    // Earth
    vec3D Pos2(-147095000000., 0., 0.);
    vec3D Vel2(0., -30300., 0.);

    body earth(Pos2, Vel2, 5.972e24, 6371000.0, "Earth");

    // Open file
    ofstream outdata;

    outdata.open("dataTest.csv");  // opens the file
    if (!outdata) {                // file couldn't be opened
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }

    // Simulation
    double stepSize = 0.5;
    double nYear = 1;

    for (int nstep = 0; nstep <= (int)365 / stepSize * nYear; nstep++) {
        outdata << earth.position.x << " , "
                << earth.position.y << " , "
                << earth.position.z << " , ";
        outdata << earth.velocity.x << " , "
                << earth.velocity.y << " , "
                << earth.velocity.z << " , ";
        outdata << sun2.position.x << " , "
                << sun2.position.y << " , "
                << sun2.position.z << " , " << endl;
        ;
        // earth.step_sgl(1., sun, "rkf2");
        body::sys_step(stepSize);
    }
    outdata.close();

    // ---------------------------------------------------
    // TODO: Implement an ofstream handler for body class
    // ---------------------------------------------------

    // Print Bodies
    body::print_bodies();

    // Debugging info
    cout << "-----------" << endl;
    cout << "Ver. 0.0020" << endl;
    cout << "-----------" << endl;
    // cout << "Press any key to close the application." << endl;
    // cin.ignore();
    return 0;
}