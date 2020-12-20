#include "calculate_pressure.h"

void calculate_pressure(std::vector<std::shared_ptr<Particle>> &particles) {
    for (auto & p : particles) {
        p->p = k * (p->rho - rho_0);
        p->p_near = k_near * p->rho_near;
    }
}
