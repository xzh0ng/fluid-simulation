#include "apply_gravity.h"

void apply_gravity(std::vector<std::shared_ptr<Particle>> &particles) {
    #pragma omp parallel for
    for (int i = 0; i < particles.size(); i++) {
        std::shared_ptr<Particle> &p = particles[i];
        p->v = p->v + dt * Eigen::Vector3d(0, g, 0);
    }
}