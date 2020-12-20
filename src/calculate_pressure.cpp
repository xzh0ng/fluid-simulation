#include "calculate_pressure.h"

void calculate_pressure(std::vector<std::shared_ptr<Particle>> &particles) {
    #pragma omp parallel for
    for (int i = 0; i < particles.size(); i++) {
        std::shared_ptr<Particle> &p = particles[i];
        p->p = k * (p->rho - rho_0);
        p->p_near = k_near * p->rho_near;
    }
}
