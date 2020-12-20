#include "apply_viscosity.h"
using namespace Eigen;

void apply_viscosity(std::vector<std::shared_ptr<Particle>> &particles) {
    for (auto & i : particles) {
        for (auto & j : i->neighbors) {
            Vector3d r_ij = j->pos - i->pos;
            double dist = r_ij.norm();
            double q = dist / h;
            double u = (i->v - j->v).dot(r_ij / dist);
            if (u > 0) {
                Vector3d I = dt * (1 - q) * (sigma * u + beta * u * u) * (r_ij / dist);
                i->v = i->v - I / 2;
                j->v = j->v + I / 2;
            }
        }
    }
}