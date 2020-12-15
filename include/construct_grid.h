#include <Eigen/Core>
#include <Eigen/Sparse>
#include <unsupported/Eigen/CXX11/Tensor>

//Input:
//  q - the current generalized coordinates for fluid particles
//  qdot - the current generalized coordinates for fluid particles
//  h - grid step size
//  nx - number of grid steps along the x-direction
//  ny - number of grid steps along the y-direction
//  nz - number of grid steps along the z-direction
//Output:
//  Wu - n * 8 trilinear interpolation weight vector for u
//  Wv - n * 8 trilinear interpolation weight vector for v
//  Ww - n * 8 trilinear interpolation weight vector for w
//  u - x components of qdot
//  v - y components of qdot
//  w - z components of qdot
//  cell_state - 1 means fluid cell, 0 means air cell
void construct_grid(Eigen::VectorXd &q, Eigen::VectorXd &qdot, double h, int nx, int ny, int nz,
    Eigen::VectorXd &Wu, Eigen::VectorXd &Wv, Eigen::VectorXd &Ww,
    Eigen::Tensor<double, 3> &u, Eigen::Tensor<double, 3> &v, Eigen::Tensor<double, 3> &w, Eigen::Tensor<int, 3> &cell_state);