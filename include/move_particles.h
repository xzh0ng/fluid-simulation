#ifndef __MOVE_PARTICLES_H__
#define __MOVE_PARTICLES_H__
#include <Eigen/Core>
#include <vector>
#include <memory>
#include <omp.h>
#include "constants.h"
#include "particle.h"

void move_particles(std::vector<std::shared_ptr<Particle>> &particles);

#endif