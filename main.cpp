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
    v.launch();
    return 0;
}

