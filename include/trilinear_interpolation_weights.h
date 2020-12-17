#include <Eigen/Core>

// Inputs:
//   h   grid step size
//   corner bottom-left-front corner position of grid
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
  const double h,
  const Eigen::VectorXd corner,
  const Eigen::Vector3d & P,
  Eigen::VectorXd & W);