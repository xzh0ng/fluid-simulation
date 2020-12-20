#ifndef __APPLY_GRAVITY_H__
#define __APPLY_GRAVITY_H__
#include <Eigen/Core>
#include <Vector>
#include <memory>
#include "constants.h"
#include "particle.h"

void apply_gravity(std::vector<std::shared_ptr<Particle>> &particles);

#endif