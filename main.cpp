#include <Eigen/Core>
#include <iostream>
#include <igl/opengl/glfw/Viewer.h>
#include <random>
#include "apply_displacements.h"
#include "apply_gravity.h"
#include "apply_viscosity.h"
#include "double_density_relaxation.h"
#include "move_particles.h"
#include "prep_for_next_step.h"
#include "constants.h"

using namespace Eigen;
using namespace std;


int main(int argc, char** argv) {

    vector<std::shared_ptr<Particle>> particles;

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0, canvas_x);


    RowVector3d color(0,0,0);
    for (int i = 0; i < n; i++) {
        shared_ptr<Particle> p(new Particle(distribution(generator), distribution(generator), distribution(generator)));
        particles.push_back(p);
    }



    igl::opengl::glfw::Viewer v;
    v.callback_pre_draw = [&](igl::opengl::glfw::Viewer & )->bool
    {
        apply_gravity(particles);
        apply_viscosity(particles);
        move_particles(particles);
        double_density_relaxation(particles);
        apply_displacements(particles);
        prep_for_next_step(particles);
        v.data().clear_points();
        for (int i = 0; i < n; i++) {
            v.data().add_points((particles[i]->pos).transpose(), color);
        }
        return false;
    };

    v.data().point_size = 10;
    v.core().is_animating = true;

    //add x y z line
    Matrix<double, 1, 3> x_axis, y_axis, z_axis, origin;
    origin.setZero();
    x_axis << canvas_x, 0, 0;
    y_axis << 0, canvas_y, 0;
    z_axis << 0, 0, canvas_z;
    v.data().add_edges(origin, x_axis, RowVector3d(0,0,1));
    v.data().add_edges(origin, y_axis, RowVector3d(0,1,0));
    v.data().add_edges(origin, z_axis, RowVector3d(1,0,0));
    v.launch();
    return 0;
}




// #include <Eigen/Core>
// #include <stdlib.h>
// #include <iostream>
// #include <igl/opengl/glfw/Viewer.h>
// #include "advection.h"
// #include "external_force.h"
// #include "pressure_projection.h"
// #define h 0.01
// #define dt 0.01
// //1000 kg/m^3
// #define density 1000.0
// using namespace Eigen;
// using namespace std;


// int main(int argc, char** argv) {
//     // initialization
//     igl::opengl::glfw::Viewer v;
//     int nx = 20, ny = 20, nz = 20;
//     int n = 1000;

//     auto my_rand = [] (double fMin, double fMax) -> double{
//         double f = (double)rand() / RAND_MAX;
//         return fMin + f * (fMax - fMin); 
//     };
//     VectorXd q(3 * n), qdot(3 * n);
//     RowVector3d color(0,0,0);

//     //-9.8m/s^2
//     VectorXd g(3);
//     g << 0, 0, -9.8;
//     srand (1);

//     // fix height at 5 * h for now
//     for (int i = 0; i < n; i++) {
//         q.segment<3>(3 * i) = Vector3d(my_rand(0, h * nx), my_rand(0, h * ny), 5 * h);
//     }
//     qdot.setZero();


//     v.callback_pre_draw = [&](igl::opengl::glfw::Viewer & )->bool
//     {   
//         advection(q, qdot, dt, h, nx, ny, nz);
//         external_force(q, qdot, g, dt);
//         pressure_projection(q, qdot, h, nx, ny, nz, dt, density);
//         v.data().set_points(Map<MatrixXd>(q.data(), 3, n).transpose(), color);
//         return false;
//     };

//     v.data().point_size = 10;
//     v.core().is_animating = true;

//     //add x y z line
//     Matrix<double, 1, 3> x_axis, y_axis, z_axis, origin;
//     origin.setZero();
//     x_axis << h * nx, 0, 0;
//     y_axis << 0, h * ny, 0;
//     z_axis << 0, 0, 2 * h * nz;
//     v.data().add_edges(origin, x_axis, Eigen::RowVector3d(0,0,1));
//     v.data().add_edges(origin, y_axis, Eigen::RowVector3d(0,1,0));
//     v.data().add_edges(origin, z_axis, Eigen::RowVector3d(1,0,0));
//     v.launch();
//     return 0;
// }