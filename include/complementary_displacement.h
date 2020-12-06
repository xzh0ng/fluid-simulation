#include <Eigen/Core>
#include <Eigen/Sparse>
#include<Eigen/SparseCholesky>
#include <igl/slice.h>
using namespace Eigen;
// Assume the input animation and simulation use the same tetrahedron mesh
// 
// Given the rest mesh (V,T), at the frame t, we find the complementary displacement Uc
// to add dynamics secondary effects to the input rig displacement Ur.
//
// Inputs:
//   V         #V by 3 list of rest mesh vertex positions
//   T         #T by 4 list tetrahedra indices into rows of V
//   M         #V*3 by #V*3 sparse mass matrix
//   Ur        #V by 3 rig displacement at the current frame
//   U_prev    #V by 3 final displacements at the last frame
//   Udot_prev #V by 3 final speed at the last frame
//   Uc_prev   #V by 3 complementary displacement at the previous frame
//   J         #V*3 by m dense rig jacobian at the current frame
//   g         #V*3 by 1 gradient of the elasticity potential
//   H         #V*3 by #V*3 hessian of the elasticity potential
//   dt        time step
//   f         #V*3 by 1 external force
//   energy(V,T,Ur,Uc,dt) a function that computes the total energy of the FEM system (for doing line search)
// Outputs:
//   Uc #V by 3 complementary displacement at the current frame
template<typename ENERGY>
void complementary_displacement(
  const Eigen::MatrixXd & V, 
  const Eigen::MatrixXi & T, 
  const Eigen::SparseMatrix<double> & M,
  const Eigen::MatrixXd & Ur,
  const Eigen::MatrixXd & U_prev,
  const Eigen::MatrixXd & Udot_prev,
  const Eigen::MatrixXd & Uc_prev,
  const Eigen::MatrixXd & J,
  const Eigen::MatrixXd & g,
  const Eigen::SparseMatrix<double> & H,
  const double & dt,
  const Eigen::MatrixXd & f,
  ENERGY & energy,
  Eigen::MatrixXd & Uc) {
    SparseMatrix<double> Q = H + dt * dt * M;
    MatrixXd temp = ((Ur - U_prev) / dt - Udot_prev);
    VectorXd temp_v = Map<VectorXd>(temp.data(), temp.size());
    MatrixXd l = -g + ((M / dt) * temp_v) + f;
    MatrixXd C = J.transpose() * M;

    int m = C.rows(), dn = Q.rows();
    MatrixXd A(dn + m, dn + m);
    A << MatrixXd(Q), C.transpose(), C, MatrixXd::Zero(m, m);
    VectorXd b(dn + m);
    b << l, VectorXd::Zero(m);
    SimplicialLDLT<SparseMatrix<double>> solver;
    VectorXd x_and_lambda = solver.compute(A.sparseView()).solve(b);
    VectorXd x = x_and_lambda.head(dn);

    // line search
    Uc = Uc_prev;
    double alpha = 1.0, p = 0.5, c = 1e-8, atol = 1e-8;
    double curr_energy = energy(V, T, Ur, Uc, dt);
    while (energy(V, T, Ur, Uc + alpha * x, dt) > curr_energy && alpha > atol) {
      alpha *= p;
    }
    Uc = Uc + alpha * (x - Uc);
  }

// Assume the input animation and simulation use the same tetrahedron mesh
// 
// Given the rest mesh (V,T), at the frame t, we find the complementary displacement Uc
// to add dynamics secondary effects to the input rig displacement Ur.
//
// Inputs:
//   V  #V by 3 list of rest mesh vertex positions
//   T  #T by 4 list tetrahedra indices into rows of V
//   M  #V*3 by #V*3 sparse mass matrix
//   Ur #V by 3 rig displacement at the current frame
//   J  #V*3 by m dense rig jacobian at the current frame
//   g  #V*3 by 1 gradient of the elasticity potential
//   H  #V*3 by #V*3 hessian of the elasticity potential
//   dt time step
//   energy(V,T,Ur,Uc,dt) a function that computes the total energy of the FEM system (for doing line search)
// Outputs:
//   Uc #V by 3 complementary displacement at the current frame
template<typename ENERGY>
void complementary_displacement(
  const Eigen::MatrixXd & V, 
  const Eigen::MatrixXi & T, 
  const Eigen::SparseMatrix<double> & M,
  const Eigen::MatrixXd & Ur,
  const Eigen::MatrixXd & J,
  const Eigen::MatrixXd & g,
  const Eigen::SparseMatrix<double> & H,
  const double & dt,
  ENERGY & energy,
  Eigen::MatrixXd & Uc) {
      MatrixXd U_prev = MatrixXd::Zero(Ur.rows(), Ur.cols());
      MatrixXd Udot_prev = MatrixXd::Zero(Ur.rows(), Ur.cols());
      MatrixXd Uc_prev = MatrixXd::Zero(Ur.rows(), Ur.cols());;
      // no external forces for now
      MatrixXd f = MatrixXd::Zero(Ur.rows(), Ur.cols());
      complementary_displacement(V, T, M, Ur, U_prev, Udot_prev, Uc_prev, J, g, H, dt, f, energy, Uc);
  }