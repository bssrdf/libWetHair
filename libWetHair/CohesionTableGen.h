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

#ifndef __COHESION_TABLE_GEN_H__
#define __COHESION_TABLE_GEN_H__

#include <Eigen/Core>

#include "MathDefs.h"

class CohesionTable
{
  //const parameters
  scalar m_sigma;
  scalar m_theta;
  scalar m_radii;
  scalar m_max_alpha;
  scalar m_max_d0;
  
  scalar m_min_d0;
  scalar m_min_d0_planar;
  
  const scalar m_ang_epsilon = 0.008;
  
  int m_discretization;
  
  MatrixXs m_A_table;
  MatrixXs m_alpha_table;
  MatrixXs m_dEdd_table;
  
  MatrixXs m_A_planar_table;
  MatrixXs m_alpha_planar_table;
  MatrixXs m_dEdd_planar_table;
  
  VectorXs m_max_As;
  VectorXs m_min_As;
  
  VectorXs m_max_A_planars;
  VectorXs m_min_A_planars;
  
  const scalar& m_radius_multiplier;
  const scalar& m_radius_multiplier_planar;
  
  const scalar& m_collision_stiffness;
  const scalar& m_collision_stiffness_planar;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  
  scalar computeR(const scalar& alpha, const scalar& d0) const;
  scalar computeA(const scalar& R, const scalar& alpha) const;
  scalar computeH(const scalar& R, const scalar& alpha) const;
  scalar computeApproxA(const scalar& alpha, const scalar& d0) const;
  scalar computeApproxdEdd(const scalar& alpha, const scalar& d0) const;
  
  scalar computeRPlanar(const scalar& alpha, const scalar& d0) const;
  scalar computeAPlanar(const scalar& R, const scalar& alpha) const;
  scalar computeHPlanar(const scalar& R, const scalar& alpha) const;
  scalar computeApproxAPlanar(const scalar& alpha, const scalar& d0) const;
  scalar computeApproxdEddPlanar(const scalar& alpha, const scalar& d0) const;
  
  scalar computedEdd(const scalar& R, const scalar& alpha) const;
  scalar computedEddPlanar(const scalar& R, const scalar& alpha) const;
  
  scalar interpolate_table(const scalar& A, const scalar& d0, const MatrixXs& mat, const scalar& dmin) const;
  scalar interpolate_table_grad(const scalar& A, const scalar& d0, const MatrixXs& mat, const scalar& dmin) const;
  
  scalar getStiffness(const scalar& d0, const scalar& A_L, const scalar& pressure_weight) const;
  scalar getStiffnessPlanar(const scalar& d0, const scalar& A_L, const scalar& pressure_weight) const;
  
  scalar computedEddAreaDist(const scalar& A_target, const scalar& d0) const;
  scalar computedEddAreaDistPlanar(const scalar& A_target, const scalar& d0) const;
  
  void print_energy_data(std::ostream& oss, bool first_time) const;
  void print_table(std::ostream& oss, const MatrixXs& mat, const scalar& dmin) const;
  void print_dEdd_table(std::ostream& oss) const;

  CohesionTable(const scalar& radius_multiplier, const scalar& m_collision_stiffness, const scalar& radius_multiplier_planar, const scalar& collision_stiffness_planar);
  
  void setParameter(const scalar& sigma, const scalar& theta, const scalar& radii, const scalar& max_d0, const int disc = 256);
  
  void construct_alpha_table();
  void construct_planar_alpha_table();
  
  scalar getRadii() const;
  
  scalar getDMin() const;
  scalar getDHat() const;
  scalar getDStar() const;
  
  scalar getDMinPlanar() const;
  scalar getDHatPlanar() const;
  scalar getDStarPlanar() const;
  
  scalar getRadiusMultiplierPlanar() const;
  scalar getCollisionStiffnessPlanar() const;
  
  scalar getRadiusMultiplier() const;
  scalar getCollisionStiffness() const;
  
  scalar interpolate_dEdd(const scalar& A, const scalar& d0) const;
  scalar interpolate_d2Edd2(const scalar& A, const scalar& d0) const;
  scalar interpolate_alpha(const scalar& A, const scalar& d0) const;
  
  scalar interpolate_dEdd_planar(const scalar& A, const scalar& d0) const;
  scalar interpolate_d2Edd2_planar(const scalar& A, const scalar& d0) const;
  scalar interpolate_alpha_planar(const scalar& A, const scalar& d0) const;
};

#endif
