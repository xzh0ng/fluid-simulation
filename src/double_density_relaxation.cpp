#include "double_density_relaxation.h"

void double_density_relaxation(std::vector<std::shared_ptr<Particle>> &particles) {
    calculate_density(particles);
    calculate_pressure(particles);
}