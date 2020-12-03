#include "include/complementary_displacement.h"
#include <igl/read_triangle_mesh.h>
#include <igl/opengl/glfw/Viewer.h>
#include <Eigen/Core>


int main(int argc, char *argv[])
{
  Eigen::MatrixXd V;
  Eigen::MatrixXi F;
  // Load input meshes
  igl::read_triangle_mesh(
    (argc>1?argv[1]:"../data/cactus.obj"),V,F);

  igl::opengl::glfw::Viewer viewer;

  update();
  viewer.launch();
  return EXIT_SUCCESS;
}
