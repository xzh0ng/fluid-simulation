#include <Eigen/Core>
#include <iostream>
#define IGL_VIEWER_VIEWER_QUIET
#include <igl/opengl/glfw/Viewer.h>
#include <random>
#include "apply_displacements.h"
#include "apply_gravity.h"
#include "apply_viscosity.h"
#include "double_density_relaxation.h"
#include "move_particles.h"
#include "prep_for_next_step.h"
#include "point_spheres.h"
#include "constants.h"


using namespace Eigen;
using namespace std;


int main(int argc, char** argv) {

    vector<shared_ptr<Particle>> particles;
    RowVector3d color(153, 255, 204);
    default_random_engine generator;

    const auto update = [&]() {
        apply_gravity(particles);
        apply_viscosity(particles);
        move_particles(particles);
        double_density_relaxation(particles);
        apply_displacements(particles);
        prep_for_next_step(particles);
    };

    auto reset_position = [&]() {
        particles.clear();
        uniform_real_distribution<double> distribution(0.0, canvas_x);
        for (int i = 0; i < n; i++) {
            shared_ptr<Particle> p(new Particle(distribution(generator), distribution(generator), distribution(generator)));
            particles.push_back(p);
        }
    };

    auto scene_0 = [&]() {
        particles.clear();
        uniform_real_distribution<double> x_distribution(0.5 * canvas_x, canvas_x);
        uniform_real_distribution<double> y_distribution(0.5 * canvas_y, canvas_y);
        uniform_real_distribution<double> z_distribution(0, canvas_z);
        for (int i = 0; i < n; i++) {
            shared_ptr<Particle> p(new Particle(x_distribution(generator), y_distribution(generator), z_distribution(generator)));
            particles.push_back(p);
        }
    };

    auto sphere_generator = []() {
        MatrixXd sphere;
        Matrix<double, 1, 3> center;
        center << 0.5 * canvas_x, 0.5 * canvas_y, 0.5 * canvas_z;
        hedra::point_spheres(center, 0.5 * canvas_z, 20, sphere);
        return sphere;
    };
    
    auto scene_1 = [&]() {
        particles.clear();
        uniform_real_distribution<double> distribution(0.0, canvas_x);
        MatrixXd sphere = sphere_generator();
        for(int i = 0; i < sphere.rows(); i++) {
            shared_ptr<Particle> p(new Particle(sphere(i, 0), sphere(i, 1), sphere(i, 2)));
            particles.push_back(p);
        }

        for (int i = sphere.rows(); i < n; i++) {
            shared_ptr<Particle> p(new Particle(distribution(generator), 0.2* distribution(generator), distribution(generator)));
            particles.push_back(p);
        }
        
    };
    

    igl::opengl::glfw::Viewer v;
    
    v.callback_key_pressed = [&](igl::opengl::glfw::Viewer &, unsigned int key, int mod)
    {
        switch(key)
        {
        case 'q':
        {
            scene_0();
            update();
            break;
        }
        case 'w':
        {
            scene_1();
            update();
            break;
        }
        case 'r':
        {
            reset_position();
            update();
            break;
        }
        default:
            return false;
        }
        return true;
    };
    
    v.callback_pre_draw = [&](igl::opengl::glfw::Viewer & )->bool
    {
        update();
        v.data().clear_points();
        for (int i = 0; i < n; i++) {
            v.data().add_points((particles[i]->pos).transpose(), color / 255);
        }
        return false;
    };

    reset_position();
    v.data().point_size = 10;
    v.core().is_animating = true;
    std::cout<< R"(
q        change to scene 0
w        change to scene 1
r        reset particle position
)";
    v.launch();
    return 0;
}

