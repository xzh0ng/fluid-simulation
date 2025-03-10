#include "particle.h"
using namespace Eigen;

Particle::Particle(double x, double y, double z) {
    prev_pos = Vector3d(x, y, z);
    pos = Vector3d(x, y, z);
    v = Vector3d(0, 0, 0);
    rho = 0;
    rho_near = 0;
    p = 0;
    p_near = 0;
}
