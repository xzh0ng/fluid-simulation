#include <Eigen/Core>
#include <stdlib.h>
#include <iostream>
#include <igl/opengl/glfw/Viewer.h>
#include "advection.h"
#include "external_force.h"
#include "pressure_projection.h"
#define h 0.01
#define dt 0.1
//1000 kg/m^3
#define density 1000.0
using namespace Eigen;
using namespace std;


int main(int argc, char** argv) {
    // initialization
    igl::opengl::glfw::Viewer v;
    int nx = 20, ny = 20, nz = 20;
    int n = 10;

    auto my_rand = [] (double fMin, double fMax) -> double{
        double f = (double)rand() / RAND_MAX;
        return fMin + f * (fMax - fMin); 
    };
    VectorXd q(3 * n), qdot(3 * n);
    RowVector3d color(0,0,0);

    //-9.8m/s^2
    VectorXd g(3);
    g << 0, 0, -9.8;
    srand (1);

    // fix height at 5 * h for now
    for (int i = 0; i < n; i++) {
        q.segment<3>(3 * i) = Vector3d(my_rand(0, h * nx), my_rand(0, h * ny), 5 * h);
    }
    qdot.setZero();


    v.callback_pre_draw = [&](igl::opengl::glfw::Viewer & )->bool
    {   
        advection(q, qdot, dt);
        external_force(q, qdot, g, dt);
        pressure_projection(q, qdot, h, nx, ny, nz, dt, density);
        v.data().set_points(Map<MatrixXd>(q.data(), 3, n).transpose(), color);
        return false;
    };

    v.data().point_size = 10;
    v.core().is_animating = true;
    v.launch();
    return 0;
}