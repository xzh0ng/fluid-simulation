#ifndef __PARTICLE_H__
#define __PARTICLE_H__
#include <Eigen/Core>
#include <vector>
#include <memory>

class Particle {
    public:
    Eigen::Vector3d prev_pos, pos, v;
    std::vector<std::shared_ptr<Particle>> neighbors;
    double rho, rho_near, p, p_near;
    Particle(double x, double y, double z);
};


#endif