#include <Eigen/Core>

// Inputs:
//   nx  number of grid steps along the x-direction
//   ny  number of grid steps along the y-direction
//   nz  number of grid steps along the z-direction
//   h   grid step size
//   P   query point
// Outputs:
//   W  8d vector contains weights for trilinear interpolation
//   W(0) = weights for cell point(x, y, z)
//   W(1) = weights for cell point(x, y, z + 1)
//   W(2) = weights for cell point(x, y + 1, z)
//   W(3) = weights for cell point(x, y + 1, z + 1)
//   W(4) = weights for cell point(x + 1, y, z)
//   W(5) = weights for cell point(x + 1, y, z + 1)
//   W(6) = weights for cell point(x + 1, y + 1, z)
//   W(7) = weights for cell point(x + 1, y + 1, z + 1)
void trilinear_interpolation_weights(
  const int nx,
  const int ny,
  const int nz,
  const double h,
  const Eigen::Vector3d & P,
  Eigen::VectorXd & W);