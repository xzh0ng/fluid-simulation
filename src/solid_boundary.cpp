#include "solid_boundary.h"

void set_boundary(Eigen::Tensor<double, 3> &t) {
    unsigned x = t.dimension(0);
    unsigned y = t.dimension(1);
    unsigned z = t.dimension(2);

    for (unsigned i = 0; i < x; ++i) {
        for (unsigned j = 0; j < y; ++j) {
            t(i, j, 0) = 0;
            t(i, j, 1) = 0;
            t(i, j, z - 1) = 0;
            t(i, j, z - 2) = 0;
        }
    }

    for (unsigned i = 0; i < x; ++i) {
        for (unsigned k = 0; k < z; ++k) {
            t(i, 0, k) = 0;
            t(i, 1, k) = 0;
            t(i, y - 1, k) = 0;
            t(i, y - 2, k) = 0;
        }
    }

    for (unsigned j = 0; j < y; ++j) {
        for (unsigned k = 0; k < z; ++k) {
            t(0, j, k) = 0;
            t(1, j, k) = 0;
            t(x - 1, j, k) = 0;
            t(x - 2, j, k) = 0;
        }
    }
}

void solid_boundary(Eigen::Tensor<double, 3> &u, Eigen::Tensor<double, 3> &v,Eigen::Tensor<double, 3> &w) {
    set_boundary(u);
    set_boundary(v);
    set_boundary(w);
}
