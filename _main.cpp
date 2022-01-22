// Main CPP-File for creating orbital simulations of multibody systems.
#include <math.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//#include "core/Body.h"
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

int main() {
    // Testing solver class:
    cout << "Testing solver Class" << endl;
    solver::test_solver(6., (DoubleCall)test);

    // Testing vec3D class:
    // vec3D v0;
    // cout << "Default constructor of vec3D" << endl;
    // cout << v0 << endl;
    // vec3D v1(1., 2., 3.);
    // vec3D v2(-1., -2., -3.);

    // cout << "Summation" << endl;
    // cout << v1 + v2 << 3. + v1 << v1 + 3. << endl;

    // cout << "Substraction" << endl;
    // cout << v1 - v2 << 3 - v1 << v1 - 3 << endl;

    // cout << "Elementwise / Scalar Multiplication" << endl;
    // cout << v1 * v2 << 3 * v1 << v1 * 3 << endl;

    // cout << "Dot product" << endl;
    // cout << v1.dot(v2) << endl;

    // cout << "Elementwise / Scalar Division" << endl;
    // cout << v1 / v2 << 3 / v1 << v1 / 3 << endl;

    // cout << "Combination 3+v1+v1-4" << endl;
    // cout << 3 + v1 + v1 - 4 << endl;

    // cout << "Combination 3*v1/4+1" << endl;
    // cout << 3 * v1 / 4 + 1 << endl;

    // cout << "Paranthesis" << endl;
    // cout << 3 * (v1 + v2 + 1) << endl;

    // cout << "ConvSys testing" << endl;
    // double x = 1.;
    // cout << convSys<double>::lenConv(x) << endl;
    // cout << convSys<vec3D>::lenConv(v1) << endl;
    // cout << convSys<vec3D>::velConv(v1) << endl;
    // cout << convSys<double>::massConv(2000000000000) << endl;

    /*
    const double NYear = 5.;
    const int stepsPerDay = 1;
    const double SUN_MASS   = 1.989e30;
    const double SUN_RADIUS = 695700000.0;
    const double EARTH_X0   = -147095000000.0;
    const double EARTH_Y0   = 0.0;
    const double EARTH_VX0  = 0.0;
    const double EARTH_VY0  = -30300.0;
    const double EARTH_MASS = 5.972e24;
    const double EARTH_RADIUS = 6371000.0;
    const double dt = 1./(double)stepsPerDay;                  // 1 earth day = 86400.0 seconds

    // 2.1.1
    Vector3d Pos, Vel;

    Pos << 0.0, 0.0, 0.0;
    Vel << 0.0, -0.01, 0.0;
    Body sun(Pos, Vel, 1., SUN_RADIUS, "Sun", "generic", true, true);
    
    
    // Pos << -46000000000.0, 0.0, 0.0;
    // Vel << 0.0, -58980.0, 0;
    // Body mercury(Pos, Vel, 0.33011e24, 2439700., "Mercury");
    
    
    Pos << -147095000000., 0., 0.;
    Vel << 0., -30300., 0.;
    Body earth(Pos, Vel, 5.972e24, 6371000.0, "Earth");

    
    // Pos << -147095000000.-0.3844e6, 0.0, 0.0;
    // Vel << 0.0, -30300.-1022., 0.0;
    // Body moon(Pos, Vel, 0.07346e24, 1738100., "Moon", "SI", false, true);
    

    Pos << 2., 0., 0.;
    Vel << 0., 0.01, 0.;
    Body moon(Pos, Vel, 1., 1738100., "Moon", "generic", true, true);



    // 2.1.5
    ofstream outdata;
    string algorithm = "rkf45a38";

    outdata.open("data.csv"); // opens the file
    if( !outdata ) { // file couldn't be opened
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }

    for(int nstep = 0; nstep <= (int)stepsPerDay*NYear*366; nstep++) {
        // earth.step(dt, &sun, algorithm);
        // mercury.step(dt, &sun, algorithm);
        Body::sys_step(dt);
        outdata << earth.position(0) << " ; " 
                << earth.position(1) << " ; " 
                << earth.position(2) << " ; ";
        // outdata << mercury.position(0) << " ; " 
        //         << mercury.position(1) << " ; " 
        //         << mercury.position(2) << endl; 
        outdata << sun.position(0) << " ; " 
                << sun.position(1) << " ; " 
                << sun.position(2) << " ; ";
        outdata << moon.position(0) << " ; " 
                << moon.position(1) << " ; " 
                << moon.position(2) << endl; 
    }
    outdata.close();
    

    cout << "\n\n Testing sum_acceleration:" << endl;
    cout << "----------------------------"   << endl;
    Vector3d sun_acc1, sun_acc2;

    earth.sum_acceleration(sun_acc1);
    cout << sun_acc1 << endl;
    sun.compute_acceleration(earth.position, sun_acc2);
    cout << sun_acc2 << endl;

    */

    /*
    cout << "\n\n Testing sum_acceleration:" << endl;
    cout << "----------------------------"   << endl;
    Vector3d acc_EaSu_at_merc, acc_Ea_at_merc, acc_Su_at_merc;

    mercury.sum_acceleration(acc_EaSu_at_merc);
    cout << acc_EaSu_at_merc << endl;

    sun.compute_acceleration(mercury.position, acc_Su_at_merc);
    earth.compute_acceleration(mercury.position, acc_Ea_at_merc);
    cout << acc_Su_at_merc+acc_Ea_at_merc << endl;

    cout << acc_Su_at_merc << endl;
    */

    /*
    cout << "\n\n Testing pointer behavior:" << endl;
    cout << "----------------------------" << endl;
    Body::print_bodies();
    */

    cout << "Press any key to close the application. Ver. 0.0003" << endl;
    cin.ignore();
    return 0;
}