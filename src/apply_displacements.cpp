#include "apply_displacements.h"
using namespace Eigen;

void apply_displacements(std::vector<std::shared_ptr<Particle>> &particles) {
    #pragma omp parallel for schedule(dynamic)
    for (int a = 0; a < particles.size(); a++) {
        std::shared_ptr<Particle> &i = particles[a];
        Vector3d dx = Vector3d::Zero();
        for (auto & j : i->neighbors) {
            Vector3d r_ij = j->pos - i->pos;
            double dist = r_ij.norm();
            double q = dist / h;
            if (q < 1) {
                Vector3d D_ij = (dt * dt) * (i->p * (1 - q) + i->p_near * pow(1 - q, 2)) * (r_ij / dist);
                D_ij /= 2;
                #pragma omp atomic update
                j->pos(0) += D_ij(0);
                #pragma omp atomic update
                j->pos(1) += D_ij(1);
                #pragma omp atomic update
                j->pos(2) += D_ij(2);
                dx = dx - D_ij;
            }
        }
        #pragma omp atomic update
        i->pos(0) += dx(0);
        #pragma omp atomic update
        i->pos(1) += dx(1);
        #pragma omp atomic update
        i->pos(2) += dx(2);
    }
}