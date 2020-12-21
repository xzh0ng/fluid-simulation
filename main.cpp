#include <Eigen/Core>
#include <iostream>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/png/readPNG.h>
#include <igl/readOBJ.h>
#include <igl/combine.h>
#include <random>
#include "apply_displacements.h"
#include "apply_gravity_and_viscosity.h"
#include "double_density_relaxation.h"
#include "move_particles.h"
#include "prep_for_next_step.h"
#include "constants.h"


using namespace Eigen;
using namespace std;


int main(int argc, char** argv) {

    vector<shared_ptr<Particle>> particles;
    RowVector3d color(153, 255, 204);
    default_random_engine generator;
    MatrixXd bunny_v, _;
    igl::readOBJ("../data/bunny.obj", bunny_v, _);
    MatrixXd particle_v;
    MatrixXi particle_f;
    igl::readOBJ("../data/particle.obj", particle_v, particle_f);
    Matrix<unsigned char,Eigen::Dynamic,Eigen::Dynamic> R,G,B,A;
    igl::png::readPNG("../data/texture.png",R,G,B,A);
    double scale = 0.5;

    const auto update = [&]() {
        apply_gravity_and_viscosity(particles);
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

    // single dam
    auto scene_0 = [&]() {
        particles.clear();
        uniform_real_distribution<double> x_distribution(0.5 * canvas_x, canvas_x);
        uniform_real_distribution<double> y_distribution(0, canvas_y);
        uniform_real_distribution<double> z_distribution(0.5 * canvas_z, canvas_z);
        for (int i = 0; i < n; i++) {
            shared_ptr<Particle> p(new Particle(x_distribution(generator), y_distribution(generator), z_distribution(generator)));
            particles.push_back(p);
        }
    };

    
    // sphere dropping from sky
    auto scene_1 = [&]() {
        particles.clear();
        for (int i = 0; i < n; i++) {
            Vector3d curr_position = Vector3d::Random();
            while (curr_position.norm() > canvas_x) curr_position = Vector3d::Random();
            shared_ptr<Particle> p(new Particle(curr_position(0), 0.5 * canvas_y + curr_position(1), curr_position(2)));
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


    auto scene_3 = [&]() {
        particles.clear();
        for (int i = 0; i < bunny_v.rows(); i++) {
            shared_ptr<Particle> p(new Particle(bunny_v(i, 0), bunny_v(i, 1), bunny_v(i, 2)));
            particles.push_back(p);
        }
        uniform_real_distribution<double> distribution(-canvas_x, canvas_x);
        for (int i = bunny_v.rows(); i < n; i++) {
            shared_ptr<Particle> p(new Particle(distribution(generator), 0.2 * abs(distribution(generator)), distribution(generator)));
            particles.push_back(p);
        }
    };

    // small sphere dropping
    auto scene_4 = [&]() {
        particles.clear();
        for (int i = 0; i < 200; i++) {
            Vector3d curr_position = Vector3d::Random();
            while (curr_position.norm() > 0.3 *canvas_x) curr_position = Vector3d::Random();
            shared_ptr<Particle> p(new Particle(curr_position(0), 0.5 * canvas_y + curr_position(1), curr_position(2)));
            particles.push_back(p);
        }
        uniform_real_distribution<double> distribution(-canvas_x, canvas_x);
        for (int i = 200; i < n; i++) {
            shared_ptr<Particle> p(new Particle(distribution(generator), 0.4 * abs(distribution(generator)), distribution(generator)));
            particles.push_back(p);
        }
    };

    igl::opengl::glfw::Viewer v;
    v.data().show_lines = false;
    
    v.callback_key_pressed = [&](igl::opengl::glfw::Viewer &, unsigned int key, int mod)
    {
        switch(key)
        {
        case '1':
        {
            scene_0();
            update();
            break;
        }
        case '2':
        {
            scene_1();
            update();
            break;
        }
        case '3':
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
        case '4':
        {
            scene_3();
            update();
            break;
        }
        case '5':
        {
            scene_4();
            update();
            break;
        }
        case 'w':
            scale *= 2;
            particle_v *= scale;
            std::cout<<"  scale increased"<< scale << std::endl;
            update();
            break;
        case 's':
            scale *= 0.5;
            particle_v *= scale;
            std::cout<<"  scale decreased"<< scale << std::endl;
            update();
            break;
        default:
            return false;
        }
        return true;
    };
    
    v.callback_pre_draw = [&](igl::opengl::glfw::Viewer & )->bool
    {
        update();
        MatrixXd pv;
        MatrixXi pf; 
        vector<MatrixXd> vs;
        vector<MatrixXi> fs;
        for (int i = 0; i < n; i++) {
            MatrixXd curr;
            curr.resize(particle_v.rows(), particle_v.cols());
            curr << particle_v.rowwise() + (particles[i]->pos).transpose();
            vs.push_back(curr);
            fs.push_back(particle_f);
        }
        igl::combine(vs, fs, pv, pf);
        v.data().set_mesh(pv, pf);
        v.data().set_texture(R,G,B,A);
        v.data().use_matcap = true;
        return false;
    };

    reset_position();
    v.core().is_animating = true;
    v.data().use_matcap = true;
    std::cout<<
R"(
  1        single dam
  2        big sphere
  3        double dam
  4        bunny drop
  5        tension demo
  r        random reset
w,s        change scale of particle
)";
    v.launch();
    return 0;
}

