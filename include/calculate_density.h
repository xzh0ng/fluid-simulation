#ifndef __CALCULATE_DENSITY_H__
#define __CALCULATE_DENSITY_H__
#include <vector>
#include <Eigen/Core>
#include <memory>
#include "particle.h"
#include "constants.h"

void calculate_density(std::vector<std::shared_ptr<Particle>> &particles);

#endif