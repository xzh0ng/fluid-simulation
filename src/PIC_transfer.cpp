#include <algorithm>
#include "PIC_transfer.h"
#include "trilinear_interpolation_weights.h"

using namespace Eigen;

void PIC_transfer(Eigen::VectorXd &q, Eigen::VectorXd &qdot, double h, int nx, int ny, int nz,
    Eigen::VectorXd &Wu, Eigen::VectorXd &Wv, Eigen::VectorXd &Ww,
    Eigen::Tensor<double, 3> &u, Eigen::Tensor<double, 3> &v, Eigen::Tensor<double, 3> &w) {
    qdot.setZero();

    auto get_cell_indices = [&h](Vector3d position) -> Vector3i {
        return Vector3i(position(0) / h, position(1) / h, position(2) / h);
    };

    auto clamp = [](double x, double l, double h) -> double {
        return std::max(std::min(x, h), l);
    };


    for (int i = 0; i < q.size() / 3; i++) {
        Vector3d p = q.segment<3>(3 * i);
        Vector3d velocity(0, 0, 0);
        Vector3i position_u = get_cell_indices(p - Vector3d(0.5 * h, 0, 0));
        Vector3i position_v = get_cell_indices(p - Vector3d(0, 0.5 * h, 0));
        Vector3i position_w = get_cell_indices(p - Vector3d(0, 0, 0.5 * h));
        Vector3i position_p = get_cell_indices(p);

        VectorXd Wu_i = Wu.segment<8>(8 * i);
        VectorXd Wv_i = Wv.segment<8>(8 * i);
        VectorXd Ww_i = Ww.segment<8>(8 * i);

        for (int a = 0; a < 2; a++) {
            for (int b = 0; b < 2; b++) {
                for (int c = 0; c < 2; c++) {
                    int w_idx = c + b * 2 + a * 4;
                    velocity(0) += u(clamp(position_u(0) + a, 0, nx - 2), clamp(position_u(1) + b, 0, ny - 1), clamp(position_u(2) + c, 0, nz - 1)) * Wu_i(w_idx);
                    velocity(1) += v(clamp(position_v(0) + a, 0, nx - 1), clamp(position_v(1) + b, 0, ny - 2), clamp(position_v(2) + c, 0, nz - 1)) * Wv_i(w_idx);
                    velocity(2) += w(clamp(position_w(0) + a, 0, nx - 1), clamp(position_w(1) + b, 0, ny - 1), clamp(position_w(2) + c, 0, nz - 2)) * Ww_i(w_idx);
                }
            }
        }
        qdot.segment<3>(3 * i) = velocity;
    }
}