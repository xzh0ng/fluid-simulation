#include <Eigen/Core>

#include "advection.h"


void advection(Eigen::VectorXd &q, Eigen::VectorXd &qdot, double dt) {
    for (size_t i = 0; i < q.size() / 3; ++i) {
        q.segment<3>(3 * i) += qdot.segment<3>(3 * i) * dt;
    }
}
