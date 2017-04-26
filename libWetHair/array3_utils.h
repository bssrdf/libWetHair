//
// This file is part of the libWetHair open source project
//
// Copyright 2017 Yun (Raymond) Fei, Henrique Teles Maia, Christopher Batty,
// Changxi Zheng, and Eitan Grinspun
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#ifndef ARRAY3_UTILS_H
#define ARRAY3_UTILS_H

#include "array3.h"
#include "MathUtilities.h"

template<class S, class T>
T interpolate_value(const Eigen::Matrix<S, 3, 1>& point, const Array3<T, Array1<T> >& grid) {
   int i,j,k;
   S fi,fj,fk;

   get_barycentric(point[0], i, fi, 0, grid.ni);
   get_barycentric(point[1], j, fj, 0, grid.nj);
   get_barycentric(point[2], k, fk, 0, grid.nk);

   return trilerp(
         grid(i,j,k), grid(i+1,j,k), grid(i,j+1,k), grid(i+1,j+1,k), 
         grid(i,j,k+1), grid(i+1,j,k+1), grid(i,j+1,k+1), grid(i+1,j+1,k+1), 
         fi,fj,fk);
}

template<class S, class T>
T interpolate_value(const Eigen::Matrix<S, 3, 1>& point, const std::vector<T>& grid, int ni, int nj, int nk) {
  int i,j,k;
  S fi,fj,fk;
  
  get_barycentric(point[0], i, fi, 0, ni);
  get_barycentric(point[1], j, fj, 0, nj);
  get_barycentric(point[2], k, fk, 0, nk);
  
  return trilerp(
                 grid[k * ni * nj + j * ni + i],
                 grid[k * ni * nj + j * ni + i + 1],
                 grid[k * ni * nj + (j + 1) * ni + i],
                 grid[k * ni * nj + (j + 1) * ni + i + 1],
                 grid[(k + 1) * ni * nj + j * ni + i],
                 grid[(k + 1) * ni * nj + j * ni + i + 1],
                 grid[(k + 1) * ni * nj + (j + 1) * ni + i],
                 grid[(k + 1) * ni * nj + (j + 1) * ni + i + 1],
                 fi,fj,fk);
}

template<class T>
Eigen::Matrix<T, 3, 1> affine_interpolate_value(const Eigen::Matrix<T, 3, 1>& point, const Array3<T, Array1<T> >& grid) {
  int i,j,k;
  T fx,fy,fz;
  
  get_barycentric(point[0], i, fx, 0, grid.ni);
  get_barycentric(point[1], j, fy, 0, grid.nj);
  get_barycentric(point[2], k, fz, 0, grid.nk);
  
  return grad_trilerp(
                     grid(i,j,k), grid(i+1,j,k),
                     grid(i,j+1,k), grid(i+1,j+1,k),
                     grid(i,j,k+1), grid(i+1,j,k+1),
                     grid(i,j+1,k+1), grid(i+1,j+1,k+1),
                     fx, fy, fz);
}

template<class S,class T>
T interpolate_gradient(Eigen::Matrix<T, 3, 1>& gradient, const Eigen::Matrix<S, 3, 1>& point, const Array3<T, Array1<T> >& grid) {
   int i,j,k;
   S fx,fy,fz;
   
   get_barycentric(point[0], i, fx, 0, grid.ni);
   get_barycentric(point[1], j, fy, 0, grid.nj);
   get_barycentric(point[2], k, fz, 0, grid.nk);
   
   T v000 = grid(i,j,k);
   T v001 = grid(i,j,k+1);
   T v010 = grid(i,j+1,k);
   T v011 = grid(i,j+1,k+1);
   T v100 = grid(i+1,j,k);
   T v101 = grid(i+1,j,k+1);
   T v110 = grid(i+1,j+1,k);
   T v111 = grid(i+1,j+1,k+1);

   T ddx00 = (v100 - v000);
   T ddx10 = (v110 - v010);
   T ddx01 = (v101 - v001);
   T ddx11 = (v111 - v011);
   T dv_dx = bilerp(ddx00,ddx10,ddx01,ddx11, fy,fz);

   T ddy00 = (v010 - v000);
   T ddy10 = (v110 - v100);
   T ddy01 = (v011 - v001);
   T ddy11 = (v111 - v101);
   T dv_dy = bilerp(ddy00,ddy10,ddy01,ddy11, fx,fz);

   T ddz00 = (v001 - v000);
   T ddz10 = (v101 - v100);
   T ddz01 = (v011 - v010);
   T ddz11 = (v111 - v110);
   T dv_dz = bilerp(ddz00,ddz10,ddz01,ddz11, fx,fy);

   gradient[0] = dv_dx;
   gradient[1] = dv_dy;
   gradient[2] = dv_dz;
   
   //return value for good measure.
   return trilerp(
      v000, v100,
      v010, v110, 
      v001, v101,
      v011, v111,
      fx, fy, fz);
}


template<class S,class T>
void interpolate_gradient(Eigen::Matrix<T, 3, 1>& gradient, const Eigen::Matrix<S, 3, 1>& point, const Array3<T, Array1<T> >& grid, S cellSize) {
  int i,j,k;
  S fx,fy,fz;
  
  get_barycentric(point[0], i, fx, 0, grid.ni);
  get_barycentric(point[1], j, fy, 0, grid.nj);
  get_barycentric(point[2], k, fz, 0, grid.nk);
  
  T v000 = grid(i,j,k);
  T v001 = grid(i,j,k+1);
  T v010 = grid(i,j+1,k);
  T v011 = grid(i,j+1,k+1);
  T v100 = grid(i+1,j,k);
  T v101 = grid(i+1,j,k+1);
  T v110 = grid(i+1,j+1,k);
  T v111 = grid(i+1,j+1,k+1);
  
  T ddx00 = (v100 - v000);
  T ddx10 = (v110 - v010);
  T ddx01 = (v101 - v001);
  T ddx11 = (v111 - v011);
  T dv_dx = bilerp(ddx00,ddx10,ddx01,ddx11, fy,fz);
  
  T ddy00 = (v010 - v000);
  T ddy10 = (v110 - v100);
  T ddy01 = (v011 - v001);
  T ddy11 = (v111 - v101);
  T dv_dy = bilerp(ddy00,ddy10,ddy01,ddy11, fx,fz);
  
  T ddz00 = (v001 - v000);
  T ddz10 = (v101 - v100);
  T ddz01 = (v011 - v010);
  T ddz11 = (v111 - v110);
  T dv_dz = bilerp(ddz00,ddz10,ddz01,ddz11, fx,fy);
  
  gradient[0] = dv_dx / cellSize;
  gradient[1] = dv_dy / cellSize;
  gradient[2] = dv_dz / cellSize;
}


template<class S,class T>
void interpolate_gradient(Eigen::Matrix<T, 3, 1>& gradient, const Eigen::Matrix<S, 3, 1>& point, const std::vector<T>& grid, int ni, int nj, int nk, S cellSize) {
  int i,j,k;
  S fx,fy,fz;
  
  get_barycentric(point[0], i, fx, 0, ni);
  get_barycentric(point[1], j, fy, 0, nj);
  get_barycentric(point[2], k, fz, 0, nk);
  
  T v000 = grid[k * (ni * nj) + j * ni + i];
  T v001 = grid[(k+1) * (ni * nj) + j * ni + i];
  T v010 = grid[k * (ni * nj) + (j+1) * ni + i];
  T v011 = grid[(k+1) * (ni * nj) + (j+1) * ni + i];
  T v100 = grid[k * (ni * nj) + j * ni + i + 1];
  T v101 = grid[(k+1) * (ni * nj) + j * ni + i + 1];
  T v110 = grid[k * (ni * nj) + (j+1) * ni + i + 1];
  T v111 = grid[(k+1) * (ni * nj) + (j+1) * ni + i + 1];
  
  T ddx00 = (v100 - v000);
  T ddx10 = (v110 - v010);
  T ddx01 = (v101 - v001);
  T ddx11 = (v111 - v011);
  T dv_dx = bilerp(ddx00,ddx10,ddx01,ddx11, fy,fz);
  
  T ddy00 = (v010 - v000);
  T ddy10 = (v110 - v100);
  T ddy01 = (v011 - v001);
  T ddy11 = (v111 - v101);
  T dv_dy = bilerp(ddy00,ddy10,ddy01,ddy11, fx,fz);
  
  T ddz00 = (v001 - v000);
  T ddz10 = (v101 - v100);
  T ddz01 = (v011 - v010);
  T ddz11 = (v111 - v110);
  T dv_dz = bilerp(ddz00,ddz10,ddz01,ddz11, fx,fy);
  
  gradient[0] = dv_dx / cellSize;
  gradient[1] = dv_dy / cellSize;
  gradient[2] = dv_dz / cellSize;
}

template<typename S, typename T>
void interpolate_hessian(Eigen::Matrix<S, 3, 3>& hessian, const Eigen::Matrix<S, 3, 1>& point, const Array3<T, Array1<T> >& grid, S cellSize) {
  Eigen::Matrix<S, 3, 1> gx0;
  Eigen::Matrix<S, 3, 1> gx1;
  Eigen::Matrix<S, 3, 1> gy0;
  Eigen::Matrix<S, 3, 1> gy1;
  Eigen::Matrix<S, 3, 1> gz0;
  Eigen::Matrix<S, 3, 1> gz1;
  
  Eigen::Matrix<S, 3, 1> px0 = point - Eigen::Matrix<S, 3, 1>(cellSize * (S) 0.5, (S) 0.0, (S) 0.0);
  Eigen::Matrix<S, 3, 1> px1 = point + Eigen::Matrix<S, 3, 1>(cellSize * (S) 0.5, (S) 0.0, (S) 0.0);
  Eigen::Matrix<S, 3, 1> py0 = point - Eigen::Matrix<S, 3, 1>((S) 0.0, cellSize * (S) 0.5, (S) 0.0);
  Eigen::Matrix<S, 3, 1> py1 = point + Eigen::Matrix<S, 3, 1>((S) 0.0, cellSize * (S) 0.5, (S) 0.0);
  Eigen::Matrix<S, 3, 1> pz0 = point - Eigen::Matrix<S, 3, 1>((S) 0.0, (S) 0.0, cellSize * (S) 0.5);
  Eigen::Matrix<S, 3, 1> pz1 = point + Eigen::Matrix<S, 3, 1>((S) 0.0, (S) 0.0, cellSize * (S) 0.5);
  
  interpolate_gradient(gx0, px0, grid, cellSize);
  interpolate_gradient(gx1, px1, grid, cellSize);
  interpolate_gradient(gy0, py0, grid, cellSize);
  interpolate_gradient(gy1, py1, grid, cellSize);
  interpolate_gradient(gz0, pz0, grid, cellSize);
  interpolate_gradient(gz1, pz1, grid, cellSize);
  
  hessian.col(0) = (gx1 - gx0) / cellSize;
  hessian.col(1) = (gy1 - gy0) / cellSize;
  hessian.col(2) = (gz1 - gz0) / cellSize;
  hessian = (hessian + hessian.transpose()) * 0.5;
}


template<class T>
void write_matlab_array(std::ostream &output, Array3<T, Array1<T> >&a, const char *variable_name, bool transpose=false)
{
  output<<variable_name<<"=[";
  for(int k = 0; k < a.nk; ++k) {
    for(int j = 0; j < a.nj; ++j) {
      for(int i = 0; i < a.ni; ++i)  {
        output<<a(i,j,k)<<" ";
      }
      output<<";";
    }
  }
  output<<"]";
  if(transpose)
    output<<"'";
  output<<";"<<std::endl;
}
  
#endif
