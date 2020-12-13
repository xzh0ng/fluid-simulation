#include <Eigen/Core>

#include "external_force.h"

void external_force(Eigen::VectorXd &q, Eigen::VectorXd &qdot, Eigen::VectorXd &force, double dt) {
    if (force.size() == qdot.size()) {
        qdot += force * dt;
        return;
    }

    for (unsigned i = 0; i < qdot.size() / 3; ++i) {
        qdot.segment<3>(3 * i) += force * dt;
    }
}

