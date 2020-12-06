#include "include/complementary_displacement.h"
#include <igl/read_triangle_mesh.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/parula.h>
#include <igl/massmatrix.h>
#include <igl/boundary_facets.h>
#include <Eigen/Core>
using namespace Eigen;

int main(int argc, char *argv[])
{
  Eigen::MatrixXd V;
  Eigen::MatrixXi F;
  // Load input meshes
  igl::read_triangle_mesh(
    (argc>1?argv[1]:"../data/cactus.obj"),V,F);

  SparseMatrix<double> M;
  igl::massmatrix(V, F, igl::MASSMATRIX_TYPE_DEFAULT, M);
  
  MatrixXi T;
  igl::boundary_facets(T, F);
  F = F.rowwise().reverse().eval();

  // computes the total energy of the FEM system (for doing line search)
  auto energy = [](Ref<const MatrixXd> V, Ref<const MatrixXi> T,
      Ref<const MatrixXd> Ur, Ref<const MatrixXd> Uc, const double & dt) {
      
  };

  igl::opengl::glfw::Viewer viewer;

  viewer.launch();
  return EXIT_SUCCESS;
}
