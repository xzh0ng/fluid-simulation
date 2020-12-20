#include "calculate_density.h"

using namespace Eigen;

void calculate_density(std::vector<std::shared_ptr<Particle>> &particles) {
    for (int i = 0; i < particles.size(); i++) {
        for (int j = 0; j < particles.size(); j++) {
            if (i >= j) {
                continue;
            }
            Vector3d r_ij = particles[j]->pos - particles[i]->pos;
            double dist = r_ij.norm();
            double q = dist / h;
            if (q < 1) {
                particles[j]->rho += pow(1 - q, 2);
                particles[j]->rho_near += pow(1 - q, 3);
                particles[i]->rho += pow(1 - q, 2);
                particles[i]->rho_near += pow(1 - q, 3);
                particles[i]->neighbors.push_back(particles[j]);
            }
        } 
    } 
}
