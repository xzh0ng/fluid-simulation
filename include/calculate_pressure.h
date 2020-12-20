#ifndef __CALCULATE_PRESSURE_H__
#define __CALCULATE_PRESSURE_H__
#include <Eigen/Core>
#include <vector>
#include <memory>
#include <omp.h>
#include "constants.h"
#include "particle.h"

void calculate_pressure(std::vector<std::shared_ptr<Particle>> &particles);

#endif