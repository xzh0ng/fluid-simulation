#include <algorithm>
#include "construct_grid.h"
#include "trilinear_interpolation_weights.h"

using namespace Eigen;

void construct_grid(Eigen::VectorXd &q, Eigen::VectorXd &qdot, double h, int nx, int ny, int nz,
    Eigen::VectorXd &Wu, Eigen::VectorXd &Wv, Eigen::VectorXd &Ww,
    Eigen::Tensor<double, 3> &u, Eigen::Tensor<double, 3> &v, Eigen::Tensor<double, 3> &w, Eigen::Tensor<int, 3> &cell_state) {
    
    u.resize(nx - 1, ny, nz);
    v.resize(nx, ny - 1, nz);
    w.resize(nx, ny, nz - 1);
    Wu.resize((q.size() / 3) * 8);
    Wv.resize((q.size() / 3) * 8);
    Ww.resize((q.size() / 3) * 8);
    cell_state.resize(nx, ny, nz);
    u.setZero();
    v.setZero();
    w.setZero();
    Wu.setZero();
    Wv.setZero();
    Ww.setZero();
    cell_state.setZero();

    auto get_cell_indices = [&h](Vector3d position) -> Vector3i {
        return Vector3i(position(0) / h, position(1) / h, position(2) / h);
    };

    auto clamp = [](double x, double l, double h) -> double {
        return std::max(std::min(x, h), l);
    };

    Tensor<double, 3> u_sum, v_sum, w_sum;
    u_sum.resize(nx - 1, ny, nz);
    v_sum.resize(nx, ny - 1, nz);
    w_sum.resize(nx, ny, nz - 1);
    u_sum.setConstant(1e-5);
    v_sum.setConstant(1e-5);
    w_sum.setConstant(1e-5);

    Vector3d u_corner(0.5 * h, 0, 0);
    Vector3d v_corner(0, 0.5 * h, 0);
    Vector3d w_corner(0, 0, 0.5 * h);

    for (int i = 0; i < q.size() / 3; i++) {
        Vector3d p = q.segment<3>(3 * i);
        Vector3d velocity = qdot.segment<3>(3 * i);

        Vector3i position_u = get_cell_indices(p - Vector3d(0.5 * h, 0, 0));
        Vector3i position_v = get_cell_indices(p - Vector3d(0, 0.5 * h, 0));
        Vector3i position_w = get_cell_indices(p - Vector3d(0, 0, 0.5 * h));
        Vector3i position_p = get_cell_indices(p);

        // set to liquid cell
        cell_state(clamp(position_p(0), 0, nx - 1), clamp(position_p(1), 0, ny - 1), clamp(position_p(2), 0, nz - 1)) = 1;

        VectorXd Wu_i(8), Wv_i(8), Ww_i(8);
        trilinear_interpolation_weights(h, u_corner, p, Wu_i);
        trilinear_interpolation_weights(h, v_corner, p, Wv_i);
        trilinear_interpolation_weights(h, w_corner, p, Ww_i);

        // save interpolation weights
        Wu.segment<8>(8 * i) = Wu_i;
        Wv.segment<8>(8 * i) = Wv_i;
        Ww.segment<8>(8 * i) = Ww_i;

        for (int a = 0; a < 2; a++) {
            for (int b = 0; b < 2; b++) {
                for (int c = 0; c < 2; c++) {
                    int w_idx = c + b * 2 + a * 4;
                    u_sum(clamp(position_u(0) + a, 0, nx - 2), clamp(position_u(1) + b, 0, ny - 1), clamp(position_u(2) + c, 0, nz - 1)) += Wu_i(w_idx);
                    v_sum(clamp(position_v(0) + a, 0, nx - 1), clamp(position_v(1) + b, 0, ny - 2), clamp(position_v(2) + c, 0, nz - 1)) += Wv_i(w_idx);
                    w_sum(clamp(position_w(0) + a, 0, nx - 1), clamp(position_w(1) + b, 0, ny - 1), clamp(position_w(2) + c, 0, nz - 2)) += Ww_i(w_idx);

                    u(clamp(position_u(0) + a, 0, nx - 2), clamp(position_u(1) + b, 0, ny - 1), clamp(position_u(2) + c, 0, nz - 1)) += Wu_i(w_idx) * velocity(0);
                    v(clamp(position_v(0) + a, 0, nx - 1), clamp(position_v(1) + b, 0, ny - 2), clamp(position_v(2) + c, 0, nz - 1)) += Wv_i(w_idx) * velocity(1);
                    w(clamp(position_w(0) + a, 0, nx - 1), clamp(position_w(1) + b, 0, ny - 1), clamp(position_w(2) + c, 0, nz - 2)) += Ww_i(w_idx) * velocity(2);
                }
            }
        }
    }
    u /= u_sum;
    v /= v_sum;
    w /= w_sum;
}