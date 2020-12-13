#include <Eigen/Core>

#include "advection.h"


void advection(Eigen::VectorXd &q, Eigen::VectorXd &qdot, double dt) {
    q += qdot * dt;
}
