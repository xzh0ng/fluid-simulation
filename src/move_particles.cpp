#include "move_particles.h"

void move_particles(std::vector<std::shared_ptr<Particle>> &particles) {
    #pragma omp parallel for
    for (int i = 0; i < particles.size(); i++) {
        std::shared_ptr<Particle> &p = particles[i];
        p->prev_pos = p->pos;
        p->pos = p->pos + dt * p->v;
    }
}