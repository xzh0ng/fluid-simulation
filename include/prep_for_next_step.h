#ifndef __PREP_FOR_NEXT_STEP_H__
#define __PREP_FOR_NEXT_STEP_H__
#include <Eigen/Core>
#include <Vector>
#include <memory>
#include "constants.h"
#include "particle.h"

void prep_for_next_step(std::vector<std::shared_ptr<Particle>> &particles);

#endif