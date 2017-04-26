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

#ifndef __STRAND_FORCE_H__
#define __STRAND_FORCE_H__

#include <Eigen/Core>
#include <iostream>
#include "../Force.h"

#include "Definitions.h"
#include "../TwoDScene.h"
#include "Dependencies/ElasticityParameters.h"
#include "Dependencies/DegreesOfFreedom.h"
#include "Dependencies/ReferenceFrames.h"
#include "Dependencies/MaterialFrames.h"
#include "Dependencies/Kappas.h"
#include "Dependencies/Twists.h"
#include "Dependencies/BendingProducts.h"
#include "StrandParameters.h"

struct StrandState
{ 
  StrandState( const VecX& initDofs, BendingMatrixBase& bendingMatrixBase );

  DOFs m_dofs;
  Edges m_edges;
  Lengths m_lengths;
  Tangents m_tangents;
  mutable ReferenceFrames1 m_referenceFrames1;
  mutable ReferenceFrames2 m_referenceFrames2;
  ReferenceTwists m_referenceTwists;
  Twists m_twists;
  CurvatureBinormals m_curvatureBinormals;
  TrigThetas m_trigThetas;
  mutable MaterialFrames<1> m_materialFrames1;
  mutable MaterialFrames<2> m_materialFrames2;
  Kappas m_kappas;
  GradKappas m_gradKappas;
  GradTwists m_gradTwists;
  GradTwistsSquared m_gradTwistsSquared;
  HessKappas m_hessKappas;
  HessTwists m_hessTwists;
  BendingProducts m_bendingProducts;
};

struct StartState
{ // used for Viscous updates that depend of start of step state
  StartState( const VecX& initDofs );

  DOFs m_dofs;
  Edges m_edges;
  Lengths m_lengths;
  Tangents m_tangents;
  mutable ReferenceFrames1 m_referenceFrames1;
  mutable ReferenceFrames2 m_referenceFrames2;
  ReferenceTwists m_referenceTwists;
  Twists m_twists;
  CurvatureBinormals m_curvatureBinormals;
  TrigThetas m_trigThetas;
  mutable MaterialFrames<1> m_materialFrames1;
  mutable MaterialFrames<2> m_materialFrames2;
  Kappas m_kappas;
};

class StrandForce : public Force
{
public:

  StrandForce( TwoDScene<3>* scene, const std::vector<int>& consecutiveVertices, const int& parameterIndex, int globalIndex );

  virtual ~StrandForce();
  
  virtual void addEnergyToTotal( const VecX& x, const VecX& v, const VecX& m, scalar& E );
  
  virtual void addGradEToTotal( const VecX& x, const VecX& v, const VecX& m, VecX& gradE );
  
  virtual void addHessXToTotal( const VecX& x, const VecX& v, const VecX& m, TripletXs& hessE );
  
  virtual void addHessVToTotal( const VecX& x, const VecX& v, const VecX& m, TripletXs& hessE );
  
  virtual void addGradEToTotal( const VecX& x, const VecX& v, const VecX& m, VecX& gradE, int pidx );
  
  virtual void addHessXToTotal( const VecX& x, const VecX& v, const VecX& m, VecX& hessE, int pidx );
  
  virtual void addHessVToTotal( const VecX& x, const VecX& v, const VecX& m, VecX& hessE, int pidx );
  
  virtual void computeIntegrationVars( const VectorXs& x, const VectorXs& v, const VectorXs& m,
                                      VectorXs& lambda, VectorXs& lambda_v,
                                      TripletXs& J, TripletXs& Jv, TripletXs& Jxv, TripletXs& tildeK,
                                      TripletXs& stiffness, TripletXs& damping, VectorXs& Phi, VectorXs& Phiv, const scalar& dt);
  
  virtual int numConstraintPos();
  
  virtual int numConstraintVel();
  
  virtual int numJ();
  
  virtual int numJv();
  
  virtual int numJxv();
  
  virtual int numTildeK();
  
  virtual bool isParallelized();
  
  virtual bool isPrecomputationParallelized();
  
  virtual void storeLambda(const VectorXs& lambda, const VectorXs& lambda_v);
  
  virtual int getAffectedHair( const std::vector<int> particle_to_hairs );
  
  virtual Force* createNewCopy();
  
  virtual void preCompute( const VecX& x, const VecX& v, const VecX& m, const scalar& dt );
  
  virtual void getAffectedVars( int pidx, std::unordered_set<int>& vars );
  
  virtual bool isContained( int pidx );
  
  virtual const char* name();

  int getGlobalIndex() const { return m_globalIndex; }
  int getNumVertices() const { return (int) m_verts.size(); }
  
  Vec2Array& alterRestKappas()
  {
      return m_restKappas;
  }

  void updateStartDoFs( const VecX& x_startOfStep );

// private: //todo need to make get/set calls for twodscene in order to make this private again.

  int getNumEdges() const { return (int) m_verts.size() - 1; }
  
  void resizeInternals();
  void freezeRestShape( unsigned begin, unsigned end, scalar damping = 0. );
  void updateEverythingThatDependsOnRestLengths();

  int numConstraintNonViscous();
  int numConstraintViscous();

  void recomputeGlobal();
  void clearStored();
  void getLocalAffectedVars( int colidx, std::vector< std::pair<int,int> >& vars );

  template<typename AccumulatedT>
  void accumulateQuantity( AccumulatedT& accumulated );

  //// FOSSSim related //////////////////////////////////////////////////
  std::vector< int > m_verts; // in order root to tip
  int m_globalIndex; // Global index amongst the hairs
  StrandParameters* m_strandParams;
  TwoDScene<3>* m_scene;
  bool m_requiresExactForceJacobian;

  // increase memory, reduce re-computation
  scalar m_strandEnergyUpdate;
  VecX m_strandForceUpdate;
  TripletXs m_strandHessianUpdate;
  SparseRXs m_hess;

  // Linear Compliant Implicit Euler solve
  VectorXs m_lambda;
  VectorXs m_lambda_v;

  //// Strand State (implicitly the end of timestep state, evolved from rest config) ////////////////////////
  StrandState* m_strandState; // future state
  StartState* m_startState; // current state

  //// Rest shape //////////////////////////////////////////////////////
  std::vector<scalar> m_restLengths; // The following four members depend on m_restLengths, which is why updateEverythingThatDependsOnRestLengths() must be called
  scalar m_totalRestLength;
  std::vector<scalar> m_VoronoiLengths; // rest length around each vertex
  std::vector<scalar> m_invVoronoiLengths; // their inverses
  std::vector<scalar> m_vertexMasses;
  Vec2Array m_restKappas;               
  std::vector<scalar> m_restTwists;

  //// Friends /////////////////////////////////////////////////////////
  friend class Viscous;
  friend class NonViscous;
  template<typename ViscousT> friend class StretchingForce;
  template<typename ViscousT> friend class BendingForce;
  template<typename ViscousT> friend class TwistingForce;  
};

#endif
