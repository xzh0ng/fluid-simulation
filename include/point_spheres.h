// This file is part of libhedra, a library for polygonal mesh processing
// Copyright (C) 2018 Amir Vaxman <avaxman@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HEDRA_POINT_SPHERES_H
#define HEDRA_POINT_SPHERES_H

#include <string>
#include <vector>
#include <cmath>
#include <Eigen/Core>
#include <igl/igl_inline.h>
#include <iostream>


// modified from https://github.com/avaxman/libhedra/blob/master/include/hedra/point_spheres.h
namespace hedra
{
  // creates small spheres to visualize points on the overlay of the mesh
  // Input:
  //  P:      #P by 3 coordinates of the centers of spheres
  //  radius: radii of the spheres
  //  res:    the resolution of the sphere discretization
  // Output:
  //  V:    #V by 3 sphere mesh coordinates
  IGL_INLINE bool point_spheres(const Eigen::MatrixXd& points,
                                const double& radius,
                                const int res,
                                Eigen::MatrixXd& V)
  {
    using namespace Eigen;
    V.resize(res*res*points.rows(),3);
  

    for (int i=0;i<points.rows();i++){
      RowVector3d center=points.row(i);
    
      //creating vertices
      for (int j=0;j<res;j++){
        double z=center(2)+radius*cos(M_PI*(double)j/(double(res-1)));
        for (int kk=0;kk<res;kk++){
          double x=center(0)+radius*sin(M_PI*(double)j/(double(res-1)))*cos(2*M_PI*(double)kk/(double(res-1)));
          double y=center(1)+radius*sin(M_PI*(double)j/(double(res-1)))*sin(2*M_PI*(double)kk/(double(res-1)));
          V.row((res*res)*i+j*res+kk)<<x,y,z;
        }
      }
    }
    
    return true;
  }
}




#endif


