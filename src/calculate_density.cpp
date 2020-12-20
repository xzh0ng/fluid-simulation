#include "calculate_density.h"

using namespace Eigen;

void calculate_density(std::vector<std::shared_ptr<Particle>> &particles) {
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < particles.size(); i++) {
        double i_rho = 0;
        double i_rho_near = 0;
        for (int j = 0; j < particles.size(); j++) {
            if (i >= j) {
                continue;
            }
            Vector3d r_ij = particles[j]->pos - particles[i]->pos;
            double dist = r_ij.norm();
            double q = dist / h;
            if (q < 1) {
                #pragma omp atomic update
                particles[j]->rho += pow(1 - q, 2);
                #pragma omp atomic update
                particles[j]->rho_near += pow(1 - q, 3);
                particles[i]->neighbors.push_back(particles[j]);
                i_rho += pow(1 - q, 2);
                i_rho_near += pow(1 - q, 3);
            }
        }
        #pragma omp atomic update
        particles[i]->rho += i_rho;
        #pragma omp atomic update
        particles[i]->rho_near += i_rho_near;
    }
}
