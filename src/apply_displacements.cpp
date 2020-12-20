#include "apply_displacements.h"
using namespace Eigen;

void apply_displacements(std::vector<std::shared_ptr<Particle>> &particles) {
    for (auto & i : particles) {
        Vector3d dx = Vector3d::Zero();
        for (auto & j : i->neighbors) {
            Vector3d r_ij = j->pos - i->pos;
            double dist = r_ij.norm();
            double q = dist / h;
            if (q < 1) {
                Vector3d D_ij = (dt * dt) * (i->p * (1 - q) + i->p_near * pow(1 - q, 2)) * (r_ij / dist);
                j->pos = j->pos + D_ij / 2;
                dx = dx - D_ij / 2;
            }
        }
        i->pos = i->pos + dx;
    }
}