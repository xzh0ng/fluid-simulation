#ifndef __APPLY_VISCOSITY_H__
#define __APPLY_VISCOSITY_H__
#include <Eigen/Core>
#include <Vector>
#include <memory>
#include "constants.h"
#include "particle.h"

void apply_viscosity(std::vector<std::shared_ptr<Particle>> &particles);

#endif