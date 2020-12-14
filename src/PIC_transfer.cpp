#include "PIC_transfer.h"
#include "trilinear_interpolation_weights.h"

using namespace Eigen;

void PIC_transfer(Eigen::VectorXd &q, Eigen::VectorXd &qdot, double h, int nx, int ny, int nz,
    Eigen::Tensor<double, 3> &u, Eigen::Tensor<double, 3> &v, Eigen::Tensor<double, 3> &w) {
    qdot.setZero();

    auto get_cell_indices = [&h](Vector3d position) -> Vector3i {
        return Vector3i(position(0) / h, position(1) / h, position(2) / h);
    };


    for (int i = 0; i < q.size() / 3; i++) {
        Vector3d p = q.segment<3>(3 * i);
        Vector3d velocity(0, 0, 0);
        Vector3i position_u = get_cell_indices(p - Vector3d(0.5 * h, 0, 0));
        Vector3i position_v = get_cell_indices(p - Vector3d(0, 0.5 * h, 0));
        Vector3i position_w = get_cell_indices(p - Vector3d(0, 0, 0.5 * h));
        Vector3i position_p = get_cell_indices(p);

        // TODO: Wu, Wv, Ww can be saved when we were creating the grids.
        // No need to interpolate one more time
        VectorXd Wu(8), Wv(8), Ww(8);
        trilinear_interpolation_weights(nx - 1, ny, nz, h, p, Wu);
        trilinear_interpolation_weights(nx, ny - 1, nz, h, p, Wv);
        trilinear_interpolation_weights(nx, ny, nz - 1, h, p, Ww);
        for (int a = 0; a < 2; a++) {
            for (int b = 0; b < 2; b++) {
                for (int c = 0; c < 2; c++) {
                    velocity(0) += u(position_u(0) + a, position_u(1) + b, position_u(2) + c) * Wu(a + b + c);
                    velocity(1) += v(position_v(0) + a, position_v(1) + b, position_v(2) + c) * Wv(a + b + c);
                    velocity(2) += w(position_w(0) + a, position_w(1) + b, position_w(2) + c) * Ww(a + b + c);
                }
            }
        }
        qdot.segment<3>(3 * i) = velocity;
    }
}