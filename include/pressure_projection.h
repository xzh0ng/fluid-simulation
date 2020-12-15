#include <Eigen/Core>

void pressure_projection(Eigen::VectorXd &q, Eigen::VectorXd &qdot, double h, int nx, int ny, int nz, double dt, double density);