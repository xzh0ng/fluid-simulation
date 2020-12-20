#ifndef __MOVE_PARTICLES_H__
#define __MOVE_PARTICLES_H__
#include <Eigen/Core>
#include <Vector>
#include <memory>
#include "constants.h"
#include "particle.h"

void move_particles(std::vector<std::shared_ptr<Particle>> &particles);

#endif