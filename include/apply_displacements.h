#ifndef __APPLY_DISPLACEMENTS_H__
#define __APPLY_DISPLACEMENTS_H__
#include <vector>
#include <Eigen/Core>
#include <memory>
#include <omp.h>
#include "particle.h"
#include "constants.h"

void apply_displacements(std::vector<std::shared_ptr<Particle>> &particles);

#endif

