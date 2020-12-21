#ifndef __APPLY_GRAVITY_AND_VISCOSITY_H__
#define __APPLY_GRAVITY_AND_VISCOSITY_H__
#include <Eigen/Core>
#include <vector>
#include <memory>
#include <omp.h>
#include "constants.h"
#include "particle.h"

void apply_gravity_and_viscosity(std::vector<std::shared_ptr<Particle>> &particles);

#endif
