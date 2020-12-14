#include <trilinear_interpolation_weights.h>
using namespace Eigen;

void trilinear_interpolation_weights(
  const int nx,
  const int ny,
  const int nz,
  const double h,
  const Eigen::Vector3d & P,
  Eigen::VectorXd & W)
{
    Vector3d idx = (P / h).array().floor();
    int x = idx(0), y = idx(1), z = idx(2);
    Vector3d ds = (P / h) - idx;

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
