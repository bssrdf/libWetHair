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


#ifndef FLUIDSIM_H
#define FLUIDSIM_H

#include "MathUtilities.h"
#include "array2.h"
#include "pcgsolver/pcg_solver.h"
#include "HairFlow.h"
#include "Force.h"

#include <vector>
#include <string>

enum ParticleType
{
  PT_LIQUID,
  PT_HAIR,
  PT_SOLID
};

enum ParticleDrawMode
{
  PDM_REGULAR,
  PDM_PRESSURE
};


template<int DIM>
struct Particle
{
  Particle(const Vectors<DIM>& x_, const Vectors<DIM>& v_, const scalar& radii_, ParticleType type_);
  Particle(const Vectors<DIM>& x_, const Vectors<DIM>& v_, const scalar& radii_, ParticleType type_, int edge_idx_, const scalar& edge_alpha_);
  Particle();
  Particle(const Particle<DIM>&);
  
  void write(std::vector<scalar>&) const;
  
  void read(const scalar* data);
  
  static size_t size();
  
  Vectors<DIM> x;
  Vectors<DIM> v;
  
  Vectors<DIM> buf0;
  Vectors<DIM> buf1;
  Matrixs<DIM> buf2;
  
  Matrixs<DIM> c;
  
  std::vector<int> bridges;
  
  scalar radii;
  scalar fresh;
  scalar pressure;
  scalar edge_alpha;
  
  ParticleType type;
  
  // for hair particles
  int edge_idx;
  bool deceased;

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

template<int DIM>
struct EdgeVelDragIntersection
{
  Vectori<DIM> coord;
  scalar vel_weighted;
  scalar drag_weighted;
  scalar vol_weighted;
  scalar weight;
  scalar linear_weight;
  
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

template<int DIM>
struct EdgePhiIntersection
{
  Vectori<DIM> coord;
  scalar phi;
  
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

class FluidSim
{
  
public:
  enum BOUNDARY_TYPE
  {
    BT_CIRCLE,
    BT_BOX,
    BT_CAPSULE,
    
    BT_UNION,
    BT_INTERSECT,
    
    BT_COUNT
  };
  
  template<int DIM>
  struct Boundary
  {
    Boundary(BOUNDARY_TYPE type_, int parent_ = -1);
    
    virtual void advance(const scalar& dt) = 0;
    virtual scalar compute_phi_vel(const Vectors<DIM>& pos, Vectors<DIM>& vel) const = 0;
    virtual void write(std::vector<scalar>&) const = 0;
    virtual void read(const scalar* data) = 0;
    virtual size_t size() const = 0;
    
    bool is_root() const
    {
      return (parent == -1);
    }
    
    BOUNDARY_TYPE type;
    int parent;
  };
  
  template<int DIM>
  struct OperatorBoundary : public Boundary<DIM>
  {
    OperatorBoundary(BOUNDARY_TYPE type_);
    
    virtual void advance(const scalar& dt);
    virtual scalar compute_phi_vel(const Vectors<DIM>& pos, Vectors<DIM>& vel) const;
    virtual void write(std::vector<scalar>&) const;
    virtual void read(const scalar* data);
    virtual size_t size() const;
    
    std::vector< Boundary<DIM>* > children;
  };
  
  template<int DIM>
  struct SolidBoundary : public Boundary<DIM>
  {
    SolidBoundary(const Vectors<DIM>& center_, const VectorXs& parameter_, BOUNDARY_TYPE type_, bool inside, const Vector3s& raxis, const scalar& rangle);
    virtual void advance(const scalar& dt);
    virtual scalar compute_phi_vel(const Vectors<DIM>& pos, Vectors<DIM>& vel) const;
    
    Vectors<DIM> center;
    VectorXs parameter;
    
    Eigen::Quaternion<scalar> rot;
    
    Vectors<DIM> future_center;
    Eigen::Quaternion<scalar> future_rot;
    
    Vector3s omega;
    Vectors<DIM> V;
    
    virtual void write(std::vector<scalar>&) const;
    
    virtual void read(const scalar* data);
    
    virtual size_t size() const;
    
    scalar sign;
  };
  
  template<int DIM>
  struct SourceBoundary : public SolidBoundary<DIM>
  {
    SourceBoundary(const Vectors<DIM>& center_, const VectorXs& parameter_, BOUNDARY_TYPE type_, bool inside, const Vector3s& raxis, const scalar& rangle, const Vectors<DIM>& eject_vel_, const scalar& start_, const scalar& end_, const scalar& spray_angle_, const scalar& drop_radius_prop_, const scalar& sub_activate_length, const scalar& sub_inactivate_length);
    virtual void sample(FluidSim* parent);
    virtual void advance(const scalar& dt);
    
    FluidSim* last_parent;
    std::vector< Vectors<DIM> > detectors;
    Vectors<DIM> eject_vel;
    bool activated;
    scalar start;
    scalar end;
    scalar sub_activate_length;
    scalar sub_inactivate_length;
    
    scalar spray_angle;
    scalar drop_radius_prop;
  };
  
  virtual void advect_boundary(const scalar& dt) = 0;
  
  virtual scalar cellsize() const = 0;
  virtual scalar dropvol(const scalar& radii) const = 0;
  virtual scalar dropradius(const scalar& vol) const = 0;
  
  virtual void update_boundary() = 0;
  
  virtual void advect_particles(scalar dt) = 0;
  virtual void correct(scalar dt) = 0;
  virtual void constrain_hair_particles() = 0;
  
  //fluid velocity operations
  virtual void add_drag(scalar dt) = 0;
  virtual void add_gravity(scalar dt) = 0;
  virtual void map_p2g(bool with_hair_particles) = 0;
  virtual void map_g2p_apic() = 0;
  
  virtual void combine_velocity_field() = 0;
  virtual void compute_liquid_phi() = 0;
  
  virtual void project(scalar dt) = 0;
  virtual void compute_weights() = 0;
  virtual void solve_pressure(scalar dt) = 0;
  
  virtual void constrain_velocity() = 0;
  virtual void prepare_update_from_hair() = 0;
  virtual void done_update_from_hair() = 0;
  virtual void init_hair_particles() = 0;
  
  virtual scalar cfl() = 0;
  
  virtual int num_particles() const = 0;
  virtual void sort_particles() = 0;
  virtual void save_pressure(const std::string szfn) = 0;
  virtual void save_particles_off(const std::string szfn) = 0;
  virtual void load_particles_off(const std::string szfn) = 0;
  
  virtual scalar computeTotalLiquidVol() const = 0;
  
  virtual void shareParticleWithHairs( VectorXs& x, scalar dt ) = 0;
  
  virtual void transferLiquidToGridParticle(const scalar& dt) = 0;
  
  virtual Vector3s getMinBBX() const = 0;
  
  virtual Vector3s getMaxBBX() const = 0;
  
  virtual scalar default_radius_multiplier() const = 0;
  
  virtual int default_particle_in_cell() const = 0;
  
  virtual void controlSources(const scalar& current_time, const scalar& dt) = 0;
  
  virtual void add_particle(const VectorXs& pos, const VectorXs& vel, const scalar& radii, ParticleType type) = 0;
  
  virtual void write(std::vector<scalar>& data) const = 0;
  
  virtual void writeReadable(std::vector<std::ostringstream>&) const = 0;
 
  virtual void readReadable( std::ifstream& file ) = 0;
  
  virtual void read(const scalar* data, size_t size_particles, size_t size_boundaries) = 0;
  
  virtual void preCompute( const VectorXs& x, const VectorXs& v, const VectorXs& m, const scalar& dt ) = 0;
  
  virtual void addGradEToTotal( const VectorXs& x, const VectorXs& v, const VectorXs& m, VectorXs& gradE ) = 0;
};

#endif
