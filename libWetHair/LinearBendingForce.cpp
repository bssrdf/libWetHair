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


#include "LinearBendingForce.h"
#include "TwoDScene.h"

template<>
LinearBendingForce<2>::LinearBendingForce( TwoDScene<2>* parent, int idx1, int idx2, int idx3, const scalar& alpha, const scalar& beta, const Vectors<1>& theta0, const scalar& eb1n, const scalar& eb2n )
: Force()
, m_idx1(idx1)
, m_idx2(idx2)
, m_idx3(idx3)
, m_alpha(alpha)
, m_theta0(theta0)
, m_eb1n(eb1n)
, m_eb2n(eb2n)
, m_beta(beta)
, m_scene( parent )
{
  m_lambda.setZero();
  m_lambda_v.setZero();
  
  assert( idx1 >= 0 );
  assert( idx2 >= 0 );
  assert( idx3 >= 0 );
  assert( idx1 != idx2 );
  assert( idx1 != idx3 );
  assert( idx2 != idx3 );
  assert( m_alpha >= 0.0 );
  assert( m_eb1n >= 0.0 );
  assert( m_eb2n >= 0.0 );
  
  m_c1 = (m_eb1n + m_eb2n) / m_eb1n * 0.5;
  m_c2 = (m_eb1n + m_eb2n) / m_eb2n * 0.5;
  
  m_x2.setZero();
  
  m_x1 = Vectors<2>(0, -m_eb1n);
  m_x3 = Vectors<2>(sin(m_theta0(0))*m_eb2n, cos(m_theta0(0))*m_eb2n);
  
  Vectors<2> center = (m_x1 + m_x2 + m_x3) / 3.0;
  m_x1 -= center;
  m_x2 -= center;
  m_x3 -= center;
  
  m_L0 = m_x2 * (m_c1 + m_c2) - m_x1 * m_c1 - m_x3 * m_c2;
}

template<>
LinearBendingForce<3>::LinearBendingForce( TwoDScene<3>* parent, int idx1, int idx2, int idx3, const scalar& alpha, const scalar& beta, const Vectors<2>& theta0, const scalar& eb1n, const scalar& eb2n )
: Force( )
, m_idx1(idx1)
, m_idx2(idx2)
, m_idx3(idx3)
, m_alpha(alpha)
, m_theta0(theta0)
, m_eb1n(eb1n)
, m_eb2n(eb2n)
, m_beta(beta)
, m_scene( parent )
{
  m_lambda.setZero();
  m_lambda_v.setZero();
  
  assert( idx1 >= 0 );
  assert( idx2 >= 0 );
  assert( idx3 >= 0 );
  assert( idx1 != idx2 );
  assert( idx1 != idx3 );
  assert( idx2 != idx3 );
  assert( m_alpha >= 0.0 );
  assert( m_eb1n >= 0.0 );
  assert( m_eb2n >= 0.0 );
  
  m_c1 = (m_eb1n + m_eb2n) / m_eb1n * 0.5;
  m_c2 = (m_eb1n + m_eb2n) / m_eb2n * 0.5;
  
  m_x2.setZero();
  
  m_x1 = Vectors<3>(0, -m_eb1n, 0);
  m_x3 = Vectors<3>(sin(m_theta0(0))*cos(m_theta0(1))*m_eb2n, cos(m_theta0(0))*cos(m_theta0(1))*m_eb2n, sin(m_theta0(1))*m_eb2n);

  Vectors<3> center = (m_x1 + m_x2 + m_x3) / 3.0;
  m_x1 -= center;
  m_x2 -= center;
  m_x3 -= center;
  
  m_L0 = m_x2 * (m_c1 + m_c2) - m_x1 * m_c1 - m_x3 * m_c2;
}

template<int DIM>
void LinearBendingForce<DIM>::preCompute( const VectorXs& x, const VectorXs& v, const VectorXs& m, const scalar& dt )
{
  const Vectors<DIM>& q1 = x.segment<DIM>( m_scene->getDof(m_idx1) );
  const Vectors<DIM>& q2 = x.segment<DIM>( m_scene->getDof(m_idx2) );
  const Vectors<DIM>& q3 = x.segment<DIM>( m_scene->getDof(m_idx3) );
  
  // compute COM
  Vectors<DIM> com = (q1 + q2 + q3) / 3.0;
  
  // compute COV Matrix
  Matrixs<DIM> A = (
  (q1 - com) * m_x1.transpose() +
  (q2 - com) * m_x2.transpose() +
  (q3 - com) * m_x3.transpose()) / 3.0;
  
  if(DIM == 2) {
    scalar theta = atan2(A(1,0)-A(0,1),A(0,0)+A(1,1));
    m_R(0, 0) = m_R(1, 1) = cos(theta);
    m_R(1, 0) = sin(theta);
    m_R(0, 1) = -m_R(1, 0);
  } else if(DIM == 3) {
    const Eigen::JacobiSVD<Matrixs<DIM> >& svd = A.jacobiSvd(Eigen::ComputeFullU|Eigen::ComputeFullV);
    m_R = svd.matrixU() * svd.matrixV().transpose();
  }
  
  m_RL0 = m_R * m_L0;
}

template<int DIM>
LinearBendingForce<DIM>::~LinearBendingForce()
{}

template<int DIM>
void LinearBendingForce<DIM>::addEnergyToTotal( const VectorXs& x, const VectorXs& v, const VectorXs& m, scalar& E )
{
  assert( x.size() == v.size() );
  assert( m_idx1 >= 0 );
  assert( m_idx2 >= 0 );
  assert( m_idx3 >= 0 );
  
  Vectors<DIM> x1 = x.segment<DIM>( m_scene->getDof(m_idx1) );
  Vectors<DIM> x2 = x.segment<DIM>( m_scene->getDof(m_idx2) );
  Vectors<DIM> x3 = x.segment<DIM>( m_scene->getDof(m_idx3) );
  
  Vectors<DIM> v1 = v.segment<DIM>( m_scene->getDof(m_idx1) );
  Vectors<DIM> v2 = v.segment<DIM>( m_scene->getDof(m_idx2) );
  Vectors<DIM> v3 = v.segment<DIM>( m_scene->getDof(m_idx3) );
  
  Vectors<DIM> L = x2 * (m_c1 + m_c2) - x1 * m_c1 - x3 * m_c2;
  Vectors<DIM> dL = L - m_RL0;
  Vectors<DIM> Ldot = v2 * (m_c1 + m_c2) - v1 * m_c1 - v3 * m_c2;
  
  E += 0.5*m_alpha*(L - m_RL0).squaredNorm() + 0.5 * m_beta * Ldot.squaredNorm();
}

template<int DIM>
void LinearBendingForce<DIM>::addGradEToTotal( const VectorXs& x, const VectorXs& v, const VectorXs& m, VectorXs& gradE )
{
  assert( x.size() == v.size() );
  assert( x.size() == gradE.size() );
  assert( m_idx1 >= 0 );
  assert( m_idx2 >= 0 );
  assert( m_idx3 >= 0 );

  Vectors<DIM> x1 = x.segment<DIM>( m_scene->getDof(m_idx1) );
  Vectors<DIM> x2 = x.segment<DIM>( m_scene->getDof(m_idx2) );
  Vectors<DIM> x3 = x.segment<DIM>( m_scene->getDof(m_idx3) );
  
  Vectors<DIM> v1 = v.segment<DIM>( m_scene->getDof(m_idx1) );
  Vectors<DIM> v2 = v.segment<DIM>( m_scene->getDof(m_idx2) );
  Vectors<DIM> v3 = v.segment<DIM>( m_scene->getDof(m_idx3) );
  
  Vectors<DIM> L = x2 * (m_c1 + m_c2) - x1 * m_c1 - x3 * m_c2;
  Vectors<DIM> dL = L - m_RL0;
  Vectors<DIM> Ldot = v2 * (m_c1 + m_c2) - v1 * m_c1 - v3 * m_c2;
  
  gradE.segment<DIM>( m_scene->getDof(m_idx1) ) += dL * (-m_c1) * m_alpha;
  gradE.segment<DIM>( m_scene->getDof(m_idx2) ) += dL * (m_c1 + m_c2) * m_alpha;
  gradE.segment<DIM>( m_scene->getDof(m_idx3) ) += dL * (-m_c2) * m_alpha;
  
  gradE.segment<DIM>( m_scene->getDof(m_idx1) ) += Ldot * (-m_c1) * m_beta;
  gradE.segment<DIM>( m_scene->getDof(m_idx2) ) += Ldot * (m_c1 + m_c2) * m_beta;
  gradE.segment<DIM>( m_scene->getDof(m_idx3) ) += Ldot * (-m_c2) * m_beta;
}

template<int DIM>
void LinearBendingForce<DIM>::addHessXToTotal( const VectorXs& x, const VectorXs& v, const VectorXs& m, TripletXs& hessE )
{
  assert( x.size() == v.size() );
  assert( x.size() == m.size() );
  assert( m_idx1 >= 0 );
  assert( m_idx2 >= 0 );
  assert( m_idx3 >= 0 );
  
  Matrixs<DIM> J00 = m_c1 * m_c1 * Matrixs<DIM>::Identity() * m_alpha;
  Matrixs<DIM> J01 = -m_c1 * (m_c1 + m_c2) * Matrixs<DIM>::Identity() * m_alpha;
  Matrixs<DIM> J02 = m_c1 * m_c2 * Matrixs<DIM>::Identity() * m_alpha;
  Matrixs<DIM> J10 = (m_c1 + m_c2) * (-m_c1) * Matrixs<DIM>::Identity() * m_alpha;
  Matrixs<DIM> J11 = (m_c1 + m_c2) * (m_c1 + m_c2) * Matrixs<DIM>::Identity() * m_alpha;
  Matrixs<DIM> J12 = (m_c1 + m_c2) * (-m_c2) * Matrixs<DIM>::Identity() * m_alpha;
  Matrixs<DIM> J20 = m_c2 * m_c1 * Matrixs<DIM>::Identity() * m_alpha;
  Matrixs<DIM> J21 = -m_c2 * (m_c1 + m_c2) * Matrixs<DIM>::Identity() * m_alpha;
  Matrixs<DIM> J22 = m_c2 * m_c2 * Matrixs<DIM>::Identity() * m_alpha;
  
  for(int r = 0; r < DIM; ++r) for(int s = 0; s < DIM; ++s) hessE.push_back(Triplets(m_scene->getDof(m_idx1) + r, m_scene->getDof(m_idx1) + s, J00(r, s)));
  for(int r = 0; r < DIM; ++r) for(int s = 0; s < DIM; ++s) hessE.push_back(Triplets(m_scene->getDof(m_idx1) + r, m_scene->getDof(m_idx2) + s, J01(r, s)));
  for(int r = 0; r < DIM; ++r) for(int s = 0; s < DIM; ++s) hessE.push_back(Triplets(m_scene->getDof(m_idx1) + r, m_scene->getDof(m_idx3) + s, J02(r, s)));
  for(int r = 0; r < DIM; ++r) for(int s = 0; s < DIM; ++s) hessE.push_back(Triplets(m_scene->getDof(m_idx2) + r, m_scene->getDof(m_idx1) + s, J10(r, s)));
  for(int r = 0; r < DIM; ++r) for(int s = 0; s < DIM; ++s) hessE.push_back(Triplets(m_scene->getDof(m_idx2) + r, m_scene->getDof(m_idx2) + s, J11(r, s)));
  for(int r = 0; r < DIM; ++r) for(int s = 0; s < DIM; ++s) hessE.push_back(Triplets(m_scene->getDof(m_idx2) + r, m_scene->getDof(m_idx3) + s, J12(r, s)));
  for(int r = 0; r < DIM; ++r) for(int s = 0; s < DIM; ++s) hessE.push_back(Triplets(m_scene->getDof(m_idx3) + r, m_scene->getDof(m_idx1) + s, J20(r, s)));
  for(int r = 0; r < DIM; ++r) for(int s = 0; s < DIM; ++s) hessE.push_back(Triplets(m_scene->getDof(m_idx3) + r, m_scene->getDof(m_idx2) + s, J21(r, s)));
  for(int r = 0; r < DIM; ++r) for(int s = 0; s < DIM; ++s) hessE.push_back(Triplets(m_scene->getDof(m_idx3) + r, m_scene->getDof(m_idx3) + s, J22(r, s)));
}

template<int DIM>
void LinearBendingForce<DIM>::addHessVToTotal( const VectorXs& x, const VectorXs& v, const VectorXs& m, TripletXs& hessE )
{
  assert( x.size() == v.size() );
  assert( x.size() == m.size() );
  assert( m_idx1 >= 0 );
  assert( m_idx2 >= 0 );
  assert( m_idx3 >= 0 );
  
  Matrixs<DIM> J00 = m_c1 * m_c1 * Matrixs<DIM>::Identity() * m_beta;
  Matrixs<DIM> J01 = -m_c1 * (m_c1 + m_c2) * Matrixs<DIM>::Identity() * m_beta;
  Matrixs<DIM> J02 = m_c1 * m_c2 * Matrixs<DIM>::Identity() * m_beta;
  Matrixs<DIM> J10 = (m_c1 + m_c2) * (-m_c1) * Matrixs<DIM>::Identity() * m_beta;
  Matrixs<DIM> J11 = (m_c1 + m_c2) * (m_c1 + m_c2) * Matrixs<DIM>::Identity() * m_beta;
  Matrixs<DIM> J12 = (m_c1 + m_c2) * (-m_c2) * Matrixs<DIM>::Identity() * m_beta;
  Matrixs<DIM> J20 = m_c2 * m_c1 * Matrixs<DIM>::Identity() * m_beta;
  Matrixs<DIM> J21 = -m_c2 * (m_c1 + m_c2) * Matrixs<DIM>::Identity() * m_beta;
  Matrixs<DIM> J22 = m_c2 * m_c2 * Matrixs<DIM>::Identity() * m_beta;
  
  for(int r = 0; r < DIM; ++r) for(int s = 0; s < DIM; ++s) hessE.push_back(Triplets(m_scene->getDof(m_idx1) + r, m_scene->getDof(m_idx1) + s, J00(r, s)));
  for(int r = 0; r < DIM; ++r) for(int s = 0; s < DIM; ++s) hessE.push_back(Triplets(m_scene->getDof(m_idx1) + r, m_scene->getDof(m_idx2) + s, J01(r, s)));
  for(int r = 0; r < DIM; ++r) for(int s = 0; s < DIM; ++s) hessE.push_back(Triplets(m_scene->getDof(m_idx1) + r, m_scene->getDof(m_idx3) + s, J02(r, s)));
  for(int r = 0; r < DIM; ++r) for(int s = 0; s < DIM; ++s) hessE.push_back(Triplets(m_scene->getDof(m_idx2) + r, m_scene->getDof(m_idx1) + s, J10(r, s)));
  for(int r = 0; r < DIM; ++r) for(int s = 0; s < DIM; ++s) hessE.push_back(Triplets(m_scene->getDof(m_idx2) + r, m_scene->getDof(m_idx2) + s, J11(r, s)));
  for(int r = 0; r < DIM; ++r) for(int s = 0; s < DIM; ++s) hessE.push_back(Triplets(m_scene->getDof(m_idx2) + r, m_scene->getDof(m_idx3) + s, J12(r, s)));
  for(int r = 0; r < DIM; ++r) for(int s = 0; s < DIM; ++s) hessE.push_back(Triplets(m_scene->getDof(m_idx3) + r, m_scene->getDof(m_idx1) + s, J20(r, s)));
  for(int r = 0; r < DIM; ++r) for(int s = 0; s < DIM; ++s) hessE.push_back(Triplets(m_scene->getDof(m_idx3) + r, m_scene->getDof(m_idx2) + s, J21(r, s)));
  for(int r = 0; r < DIM; ++r) for(int s = 0; s < DIM; ++s) hessE.push_back(Triplets(m_scene->getDof(m_idx3) + r, m_scene->getDof(m_idx3) + s, J22(r, s)));
}


template<int DIM>
void LinearBendingForce<DIM>::addGradEToTotal( const VectorXs& x, const VectorXs& v, const VectorXs& m, VectorXs& gradE, int pidx )
{
  assert( x.size() == v.size() );
  assert( x.size() == gradE.size() );
  assert( m_idx1 >= 0 );
  assert( m_idx2 >= 0 );
  assert( m_idx3 >= 0 );
  
  const Vectors<DIM>& x1 = x.segment<DIM>( m_scene->getDof(m_idx1) );
  const Vectors<DIM>& x2 = x.segment<DIM>( m_scene->getDof(m_idx2) );
  const Vectors<DIM>& x3 = x.segment<DIM>( m_scene->getDof(m_idx3) );
  
  const Vectors<DIM>& v1 = v.segment<DIM>( m_scene->getDof(m_idx1) );
  const Vectors<DIM>& v2 = v.segment<DIM>( m_scene->getDof(m_idx2) );
  const Vectors<DIM>& v3 = v.segment<DIM>( m_scene->getDof(m_idx3) );
  
  int idir = m_scene->getComponent( pidx );
  if( idir == DIM ) return;
  int ip = m_scene->getVertFromDof( pidx );
  
  if(ip == m_idx1) {
    gradE(pidx) +=
    (x2(idir) * (m_c1 + m_c2) - x1(idir) * m_c1 - x3(idir) * m_c2 - m_RL0(idir)) * (-m_c1) * m_alpha +
    (v2(idir) * (m_c1 + m_c2) - v1(idir) * m_c1 - v3(idir) * m_c2) * (-m_c1) * m_beta;
  } else if(ip == m_idx2) {
    gradE(pidx) +=
    (x2(idir) * (m_c1 + m_c2) - x1(idir) * m_c1 - x3(idir) * m_c2 - m_RL0(idir)) * (m_c1 + m_c2) * m_alpha +
    (v2(idir) * (m_c1 + m_c2) - v1(idir) * m_c1 - v3(idir) * m_c2) * (m_c1 + m_c2) * m_beta;
  } else if(ip == m_idx3) {
    gradE(pidx) +=
    (x2(idir) * (m_c1 + m_c2) - x1(idir) * m_c1 - x3(idir) * m_c2 - m_RL0(idir)) * (-m_c2) * m_alpha +
    (v2(idir) * (m_c1 + m_c2) - v1(idir) * m_c1 - v3(idir) * m_c2) * (-m_c2) * m_beta;
  }
}

template<int DIM>
void LinearBendingForce<DIM>::addHessXToTotal( const VectorXs& x, const VectorXs& v, const VectorXs& m, VectorXs& hessE, int pidx )
{
  assert( x.size() == v.size() );
  assert( x.size() == m.size() );
  assert( m_idx1 >= 0 );
  assert( m_idx2 >= 0 );
  assert( m_idx3 >= 0 );
  
  int idir = m_scene->getComponent( pidx );
  if( idir == DIM ) return;
  int ip = m_scene->getVertFromDof( pidx );
  
  if(ip == m_idx1) {
    hessE( m_scene->getDof(m_idx1) + idir) += m_c1 * m_c1 * m_alpha;
    hessE( m_scene->getDof(m_idx2) + idir) += -m_c1 * (m_c1 + m_c2) * m_alpha;
    hessE( m_scene->getDof(m_idx3) + idir) += m_c1 * m_c2 * m_alpha;
  } else if(ip == m_idx2) {
    hessE( m_scene->getDof(m_idx1) + idir) += (m_c1 + m_c2) * (-m_c1) * m_alpha;
    hessE( m_scene->getDof(m_idx2) + idir) += (m_c1 + m_c2) * (m_c1 + m_c2) * m_alpha;
    hessE( m_scene->getDof(m_idx3) + idir) += (m_c1 + m_c2) * (-m_c2) * m_alpha;
  } else if(ip == m_idx3) {
    hessE( m_scene->getDof(m_idx1) + idir) += m_c2 * m_c1 * m_alpha;
    hessE( m_scene->getDof(m_idx2) + idir) += -m_c2 * (m_c1 + m_c2) * m_alpha;
    hessE( m_scene->getDof(m_idx3) + idir) += m_c2 * m_c2 * m_alpha;
  }
}

template<int DIM>
void LinearBendingForce<DIM>::addHessVToTotal( const VectorXs& x, const VectorXs& v, const VectorXs& m, VectorXs& hessE, int pidx )
{
  assert( x.size() == v.size() );
  assert( x.size() == m.size() );
  assert( m_idx1 >= 0 );
  assert( m_idx2 >= 0 );
  assert( m_idx3 >= 0 );
  
  int idir = m_scene->getComponent( pidx );
  if( idir == DIM ) return;
  int ip = m_scene->getVertFromDof( pidx );
  
  if(ip == m_idx1) {
    hessE( m_scene->getDof(m_idx1) + idir) += m_c1 * m_c1 * m_beta;
    hessE( m_scene->getDof(m_idx2) + idir) += -m_c1 * (m_c1 + m_c2) * m_beta;
    hessE( m_scene->getDof(m_idx3) + idir) += m_c1 * m_c2 * m_beta;
  } else if(ip == m_idx2) {
    hessE( m_scene->getDof(m_idx1) + idir) += (m_c1 + m_c2) * (-m_c1) * m_beta;
    hessE( m_scene->getDof(m_idx2) + idir) += (m_c1 + m_c2) * (m_c1 + m_c2) * m_beta;
    hessE( m_scene->getDof(m_idx3) + idir) += (m_c1 + m_c2) * (-m_c2) * m_beta;
  } else if(ip == m_idx3) {
    hessE( m_scene->getDof(m_idx1) + idir) += m_c2 * m_c1 * m_beta;
    hessE( m_scene->getDof(m_idx2) + idir) += -m_c2 * (m_c1 + m_c2) * m_beta;
    hessE( m_scene->getDof(m_idx3) + idir) += m_c2 * m_c2 * m_beta;
  }
}

template<int DIM>
void LinearBendingForce<DIM>::computeIntegrationVars( const VectorXs& x, const VectorXs& v, const VectorXs& m,
                                                     VectorXs& lambda, VectorXs& lambda_v,
                                                     TripletXs& J, TripletXs& Jv, TripletXs& Jxv, TripletXs& tildeK,
                                                     TripletXs& stiffness, TripletXs& damping, VectorXs& Phi, VectorXs& Phiv, const scalar& dt)
{
  Vectors<DIM> x1 = x.segment<DIM>( m_scene->getDof(m_idx1) );
  Vectors<DIM> x2 = x.segment<DIM>( m_scene->getDof(m_idx2) );
  Vectors<DIM> x3 = x.segment<DIM>( m_scene->getDof(m_idx3) );
  
  Vectors<DIM> L = x2 * (m_c1 + m_c2) - x1 * m_c1 - x3 * m_c2;
  Vectors<DIM> dL = L - m_RL0;
  
  scalar combined_k0 = (m_alpha + m_beta / dt);
  
  Phi.segment<DIM>(m_internal_index_pos) = dL * m_alpha / combined_k0;

  for(int r = 0; r < DIM; ++r) {
    J[m_internal_index_J + r] = Triplets(m_internal_index_pos + r, m_scene->getDof(m_idx1) + r,-m_c1);
    J[m_internal_index_J + DIM + r] = Triplets(m_internal_index_pos + r, m_scene->getDof(m_idx2) + r, m_c1 + m_c2);
    J[m_internal_index_J + DIM * 2 + r] = Triplets(m_internal_index_pos + r, m_scene->getDof(m_idx3) + r,-m_c2);
    
    stiffness[m_internal_index_pos + r] = Triplets(m_internal_index_pos + r, m_internal_index_pos + r, combined_k0);
  }
}

template<int DIM>
int LinearBendingForce<DIM>::numJ()
{
  return DIM * 3;
}
template<int DIM>
int LinearBendingForce<DIM>::numJv()
{
  return 0;//m_beta > 0.0 ? (DIM * 3) : 0;
}
template<int DIM>
int LinearBendingForce<DIM>::numJxv()
{
  return 0;
}
template<int DIM>
int LinearBendingForce<DIM>::numTildeK()
{
  return 0;
}
template<int DIM>
bool LinearBendingForce<DIM>::isParallelized()
{
  return false;   // TODO: Implementation
}

template<int DIM>
bool LinearBendingForce<DIM>::isPrecomputationParallelized()
{
  return false;
}

template<int DIM>
int LinearBendingForce<DIM>::numConstraintPos()
{
  return DIM;
}

template<int DIM>
int LinearBendingForce<DIM>::numConstraintVel()
{
  return 0;//m_beta > 0.0 ? DIM : 0;
}

template<int DIM>
Force* LinearBendingForce<DIM>::createNewCopy()
{
  return new LinearBendingForce(*this);
}

template<int DIM>
const char* LinearBendingForce<DIM>::name()
{
  return "linearbendingforce";
}

template<int DIM>
void LinearBendingForce<DIM>::getAffectedVars( int colidx, std::unordered_set<int>& vars )
{
  int idir = m_scene->getComponent( colidx );
  if( idir == DIM ) return;
  int ip = m_scene->getVertFromDof( colidx );

  if(ip == m_idx1 || ip == m_idx2 || ip == m_idx3) {
    for(int r = 0; r < DIM; ++r) {
      vars.insert( m_scene->getDof(m_idx1) + r);
      vars.insert( m_scene->getDof(m_idx2) + r);
      vars.insert( m_scene->getDof(m_idx3) + r);
    }
  }
}

template<int DIM>
int LinearBendingForce<DIM>::getAffectedHair( const std::vector<int> particle_to_hairs )
{
  return particle_to_hairs[m_idx1];
}

template<int DIM>
bool LinearBendingForce<DIM>::isContained( int colidx )
{
  int idir = m_scene->getComponent( colidx );
  if( idir == DIM ) return false;
  int ip = m_scene->getVertFromDof( colidx );

  if(ip == m_idx1 || ip == m_idx2 || ip == m_idx3) return true;
  else return false;
}

template<int DIM>
void LinearBendingForce<DIM>::storeLambda(const VectorXs& lambda, const VectorXs& lambda_v)
{
}

// explicit instantiations at bottom
template class LinearBendingForce<2>;
template class LinearBendingForce<3>;
