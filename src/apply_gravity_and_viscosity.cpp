#include "apply_gravity_and_viscosity.h"
using namespace Eigen;

void apply_gravity_and_viscosity(std::vector<std::shared_ptr<Particle>> &particles) {
    #pragma omp parallel for
    for (int a = 0; a < particles.size(); a++) {
        std::shared_ptr<Particle> &i = particles[a];
        i->v = i->v + dt * Eigen::Vector3d(0, g, 0);
        for (auto & j : i->neighbors) {
            Vector3d r_ij = j->pos - i->pos;
            double dist = r_ij.norm();
            double q = dist / h;
            double u = (i->v - j->v).dot(r_ij / dist);
            if (u > 0) {
                Vector3d I = dt * (1 - q) * (sigma * u + beta * u * u) * (r_ij / dist);
                I /= 2;
                #pragma omp atomic update
                i->v(0) -= I(0);
                #pragma omp atomic update
                i->v(1) -= I(1);
                #pragma omp atomic update
                i->v(2) -= I(2);
                #pragma omp atomic update
                j->v(0) += I(0);
                #pragma omp atomic update
                j->v(1) += I(1);
                #pragma omp atomic update
                j->v(2) += I(2);
            }
        }
    }
}