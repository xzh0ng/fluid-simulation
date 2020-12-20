#ifndef __DOUBLE_DENSITY_RELAXATION_H__
#define __DOUBLE_DENSITY_RELAXATION_H__
#include <Eigen/Core>
#include <vector>
#include <memory>
#include "constants.h"
#include "particle.h"
#include "calculate_density.h"
#include "calculate_pressure.h"

void double_density_relaxation(std::vector<std::shared_ptr<Particle>> &particles);

#endif