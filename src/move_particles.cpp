#include "move_particles.h"

void move_particles(std::vector<std::shared_ptr<Particle>> &particles) {
    for (auto & p : particles) {
        p->prev_pos = p->pos;
        p->pos = p->pos + dt * p->v;
    }
}