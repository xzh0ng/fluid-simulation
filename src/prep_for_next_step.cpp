#include "prep_for_next_step.h"

void prep_for_next_step(std::vector<std::shared_ptr<Particle>> &particles) {
    #pragma omp parallel for
    for (int i = 0; i < particles.size(); i++) {
        std::shared_ptr<Particle> &p = particles[i];
        p->v = (p->pos - p->prev_pos) / dt;

        if (p->pos(0) < -canvas_x) {
            p->pos(0) = -2 * canvas_x - p->pos(0);
            if (p->v(0) < 0) p->v(0) *= -1 * damp;
        }
        if (p->pos(0) > canvas_x) {
            p->pos(0) = 2 * canvas_x - p->pos(0);
            if (p->v(0) > 0) p->v(0) *= -1 * damp;
        }

        if (p->pos(1) < 0) {
            p->pos(1) = -p->pos(1);
            if (p->v(1) < 0) p->v(1) *= -1 * damp;
        }
        if (p->pos(1) > canvas_y) {
            p->pos(1) = canvas_y;
            if (p->v(1) > 0) p->v(1) *= -1 * damp;
        }

        if (p->pos(2) < -canvas_z) {
            p->pos(2) = -2 * canvas_z - p->pos(2);
            if (p->v(2) < 0) p->v(2) *= -1 * damp;
        }
        if (p->pos(2) > canvas_z) {
            p->pos(2) = 2 * canvas_z - p->pos(2);
            if (p->v(2) > 0) p->v(2) *= -1 * damp;
        }
        
    }
}