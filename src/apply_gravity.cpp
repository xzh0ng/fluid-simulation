#include "apply_gravity.h"

void apply_gravity(std::vector<std::shared_ptr<Particle>> &particles) {
    for (auto p : particles) {
        p->v = p->v + dt * Eigen::Vector3d(0, g, 0);
    }
}