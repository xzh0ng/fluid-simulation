#include "trilinear_interpolation_weights.h"

void trilinear_interpolation_weights(
  const double h,
  const Eigen::VectorXd corner,
  const Eigen::Vector3d & P,
  Eigen::VectorXd & W)
{
    Eigen::Vector3d diffs = P - corner;
    Eigen::Vector3d idx = (diffs / h).array().floor();
    int x = idx(0), y = idx(1), z = idx(2);
    Eigen::Vector3d ds = (diffs / h) - idx;

    // x, y, z
    W(0) = (1 - ds(0)) * (1 - ds(1)) * (1 - ds(2));
    // x, y, z + 1
    W(1) = (1 - ds(0)) * (1 - ds(1)) * ds(2);
    // x, y + 1, z
    W(2) = (1 - ds(0)) * ds(1) * (1 - ds(2));
    // x, y + 1, z + 1
    W(3) = (1 - ds(0)) * ds(1) * ds(2);
    // x + 1, y, z
    W(4) = ds(0) * (1 - ds(1)) * (1 - ds(2));
    // x + 1, y, z + 1
    W(5) = ds(0) * (1 - ds(1)) * ds(2);
    // x + 1, y + 1, z
    W(6) = ds(0) * ds(1) * (1 - ds(2));
    // x + 1, y + 1, z + 1
    W(7) = ds(0) * ds(1) * ds(2);
}
