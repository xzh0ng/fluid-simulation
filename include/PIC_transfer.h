#include <Eigen/Core>
#include <unsupported/Eigen/CXX11/Tensor>

//Input:
//  q - the current generalized coordinates for fluid particles
//  qdot - the current velocity coordinates for fluid particles
//  h - grid step size
//  nx - number of grid steps along the x-direction
//  ny - number of grid steps along the y-direction
//  nz - number of grid steps along the z-direction
//  u - x components of qdot
//  v - y components of qdot
//  w - z components of qdot
//Output:
//  qdot - updated generalized velocity for fluid particles
void PIC_transfer(Eigen::VectorXd &q, Eigen::VectorXd &qdot, double h, int nx, int ny, int nz,
    Eigen::Tensor<double, 3> &u, Eigen::Tensor<double, 3> &v, Eigen::Tensor<double, 3> &w);