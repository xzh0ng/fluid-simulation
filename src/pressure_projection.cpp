#include "pressure_projection.h"

#include <unsupported/Eigen/CXX11/Tensor>

#include "construct_grid.h"
#include "PIC_transfer.h"
#include "solid_boundary.h"
#include "soft_boundary.h"

void pressure_projection(Eigen::VectorXd &q, Eigen::VectorXd &qdot, double h, int nx, int ny, int nz, double dt, double density) {

    Eigen::VectorXd Wu;
    Eigen::VectorXd Wv;
    Eigen::VectorXd Ww;
    Eigen::Tensor<double, 3> u;
    Eigen::Tensor<double, 3> v;
    Eigen::Tensor<double, 3> w;
    Eigen::Tensor<int, 3> cell_state;

    construct_grid(q, qdot, h, nx, ny, nz, Wu, Wv, Ww, u, v, w, cell_state);

    // construct pressure
    Eigen::Tensor<double, 3> p(nx, ny, nz);
    p.setZero();
    for (unsigned i = 1; i < nx - 1; ++i) {
        for (unsigned j = 0; j < ny; ++j) {
            for (unsigned k = 0; k < nz; ++k) {
                p(i, j, k) += (u(i, j, k) - u(i - 1, j, k)) / h;
            }
        }
    }
    for (unsigned i = 0; i < nx; ++i) {
        for (unsigned j = 1; j < ny - 1; ++j) {
            for (unsigned k = 0; k < nz; ++k) {
                p(i, j, k) += (v(i, j, k) - v(i, j - 1, k)) / h;
            }
        }
    }
    for (unsigned i = 0; i < nx; ++i) {
        for (unsigned j = 0; j < ny; ++j) {
            for (unsigned k = 1; k < nz - 1; ++k) {
                p(i, j, k) += (w(i, j, k) - w(i, j, k - 1)) / h;
            }
        }
    }

    soft_boundary(p, cell_state);

    // apply projection
    for (unsigned i = 0; i < nx - 1; ++i) {
        for (unsigned j = 0; j < ny; ++j) {
            for (unsigned k = 0; k < nz; ++k) {
                u(i, j, k) -= (p(i + 1, j, k) - p(i, j, k)) * dt / density;
            }
        }
    }
    for (unsigned i = 0; i < nx; ++i) {
        for (unsigned j = 0; j < ny - 1; ++j) {
            for (unsigned k = 0; k < nz; ++k) {
                v(i, j, k) -= (p(i, j + 1, k) - p(i, j, k)) * dt / density;
            }
        }
    }
    for (unsigned i = 0; i < nx; ++i) {
        for (unsigned j = 0; j < ny; ++j) {
            for (unsigned k = 0; k < nz - 1; ++k) {
                w(i, j, k) -= (p(i, j, k + 1) - p(i, j, k)) * dt / density;
            }
        }
    }

    solid_boundary(u, v, w);

    PIC_transfer(q, qdot, h, nx, ny, nz, Wu, Wv, Ww, u, v, w);
}

