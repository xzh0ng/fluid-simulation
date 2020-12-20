#ifndef __PREP_FOR_NEXT_STEP_H__
#define __PREP_FOR_NEXT_STEP_H__
#include <Eigen/Core>
#include <vector>
#include <memory>
#include <omp.h>
#include "constants.h"
#include "particle.h"

void prep_for_next_step(std::vector<std::shared_ptr<Particle>> &particles);

#endif