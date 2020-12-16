#include "solid_boundary.h"

void solid_boundary(Eigen::Tensor<double, 3> &u, Eigen::Tensor<double, 3> &v,Eigen::Tensor<double, 3> &w) {

    for (unsigned i = 0; i < w.dimension(0); ++i) {
        for (unsigned j = 0; j < w.dimension(1); ++j) {
            if (w(i, j, 0) < 0) {
                w(i, j, 0) = 0;
            }
            if (w(i, j, w.dimension(2) - 1) > 0) {
                w(i, j, w.dimension(2) - 1) = 0;
            }
        }
    }

    for (unsigned i = 0; i < v.dimension(0); ++i) {
        for (unsigned k = 0; k < v.dimension(2); ++k) {
            if (v(i, 0, k) < 0) {
                v(i, 0, k) = 0;
            }
            if (v(i, v.dimension(1) - 1, k) > 0) {
                v(i, v.dimension(1) - 1, k) = 0;
            }
        }
    }

    for (unsigned j = 0; j < u.dimension(1); ++j) {
        for (unsigned k = 0; k < u.dimension(2); ++k) {
            if (u(0, j, k) < 0) {
                u(0, j, k) = 0;
            }
            if (u(u.dimension(0) - 1, j, k) > 0) {
                u(u.dimension(0) - 1, j, k) = 0;
            }
        }
    }
}
