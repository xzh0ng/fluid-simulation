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
        uniform_real_distribution<double> x_z_distribution(-canvas_x, canvas_x);
        uniform_real_distribution<double> distribution(0, canvas_y);
        for (int i = 0; i < n; i++) {
            shared_ptr<Particle> p(new Particle(x_z_distribution(generator), distribution(generator), x_z_distribution(generator)));
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

    auto sphere_generator = [](double radius, int res) {
        MatrixXd sphere;
        Matrix<double, 1, 3> center;
        center << 0, 0.5 * canvas_y, 0;
        hedra::point_spheres(center, radius, res, sphere);
        return sphere;
    };
    
    // sphere dropping from sky
    auto scene_1 = [&]() {
        particles.clear();
        uniform_real_distribution<double> distribution(-canvas_x, canvas_x);
        MatrixXd sphere = sphere_generator(0.5 * canvas_y, 20);
        int total_sphere_pts = 0;
        for(int i = 0; i < sphere.rows(); i++) {
            shared_ptr<Particle> p(new Particle(sphere(i, 0), sphere(i, 1), sphere(i, 2)));
            particles.push_back(p);
        }
        total_sphere_pts += sphere.rows();

        sphere = sphere_generator(0.3 * canvas_y, 10);
        for(int i = 0; i < sphere.rows(); i++) {
            shared_ptr<Particle> p(new Particle(sphere(i, 0), sphere(i, 1), sphere(i, 2)));
            particles.push_back(p);
        }
        total_sphere_pts += sphere.rows();

        for (int i = total_sphere_pts; i < n; i++) {
            shared_ptr<Particle> p(new Particle(distribution(generator), 0.2 * abs(distribution(generator)), distribution(generator)));
            particles.push_back(p);
        }
        
    };

    // double dam
    auto scene_2 = [&]() {
        particles.clear();
        uniform_real_distribution<double> x_distribution(0.3 * canvas_x, canvas_x);
        uniform_real_distribution<double> y_distribution(0, canvas_y);
        uniform_real_distribution<double> z_distribution(0.3 * canvas_z, canvas_z);
        for (int i = 0; i < n / 2; i++) {
            shared_ptr<Particle> p(new Particle(x_distribution(generator), y_distribution(generator), z_distribution(generator)));
            particles.push_back(p);
        }

        uniform_real_distribution<double> x_distribution2(-0.3 * canvas_x, -canvas_x);
        uniform_real_distribution<double> y_distribution2(0, canvas_y);
        uniform_real_distribution<double> z_distribution2(-0.3 * canvas_z, - canvas_z);
        for (int i = n / 2; i < n; i++) {
            shared_ptr<Particle> p(new Particle(x_distribution2(generator), y_distribution2(generator), z_distribution2(generator)));
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
        case 'e':
        {
            scene_2();
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
    v.data().point_size = 2;
    v.core().is_animating = true;
    std::cout<< R"(
q        change to scene 0
w        change to scene 1
e        change to scene 2
r        reset particle position
)";
    v.launch();
    return 0;
}

