#include <Eigen/Core>

#include "advection.h"


void advection(Eigen::VectorXd &q, Eigen::VectorXd &qdot, double dt, double h, int nx, int ny, int nz) {
    q += qdot * dt;

    for (unsigned i = 0; i < q.size() / 3; ++i) {
        if (q(3 * i) < 0) {
            q(3 * 1) = 0;
        }
        if (q(3 * i) > h * nx) {
            q(3 * i) = h * nx;
        }
        if (q(3 * i + 1) < 0) {
            q(3 * 1 + 1) = 0;
        }
        if (q(3 * i + 1) > h * ny) {
            q(3 * i + 1) = h * ny;
        }
        if (q(3 * i + 2) < 0) {
            q(3 * 1 + 2) = 0;
        }
        if (q(3 * i + 2) > h * nz) {
            q(3 * i + 2) = h * nz;
        }
    }
}
