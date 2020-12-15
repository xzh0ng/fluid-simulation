#include "soft_boundary.h"

void soft_boundary(Eigen::Tensor<double, 3> &p, Eigen::Tensor<int, 3> cell_state) {
    for (unsigned i = 0; i < p.dimension(0); ++i) {
        for (unsigned j = 0; j < p.dimension(1); ++j) {
            for (unsigned k = 0; k < p.dimension(2); ++k) {
                if (cell_state(i, j, k) == 1 || p(i, j, k) == 0) {
                    continue;
                }

                // Check the adjacent fluid cells
                if (i >= 1 && cell_state(i - 1, j, k) == 1) {
                    p(i - 1, j, k) -= 0.5 * p(i, j, k);
                }
                if (i + 1 < p.dimension((0)) && cell_state(i + 1, j, k) == 1) {
                    p(i + 1, j, k) -= 0.5 * p(i, j, k);
                }
                if (j >= 1 && cell_state(i, j - 1, k) == 1) {
                    p(i, j - 1, k) -= 0.5 * p(i, j, k);
                }
                if (j + 1 < p.dimension(1) && cell_state(i, j + 1, k) == 1) {
                    p(i, j + 1, k) -= 0.5 * p(i, j, k);
                }
                if (k >= 1 && cell_state(i, j, k - 1) == 1) {
                    p(i, j, k - 1) -= 0.5 * p(i, j, k);
                }
                if (k + 1 < p.dimension(2) && cell_state(i, j, k + 1) == 1) {
                    p(i, j, k + 1) -= 0.5 * p(i, j, k);
                }
            }
        }
    }
}
