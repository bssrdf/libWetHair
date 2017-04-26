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


#include "DegreesOfFreedom.h"
#include "ElasticStrandUtils.h"

void Edges::compute()
{
    m_value.resize( m_size );
    const VecX& dofs = m_dofs.get();
	for (IndexType vtx = 0; vtx < m_firstValidIndex; ++vtx)
	{
		m_value[vtx].setZero();
	}
    for( IndexType vtx = m_firstValidIndex; vtx < size(); ++vtx )
    {
        m_value[vtx] = dofs.segment<3>( 4 * ( vtx + 1 ) ) - dofs.segment<3>( 4 * vtx );
    }

    setDependentsDirty();
}

void Lengths::compute()
{
    m_value.resize( m_size );
    const Vec3Array& edges = m_edges.get();
	for (IndexType vtx = 0; vtx < m_firstValidIndex; ++vtx)
	{
		m_value[vtx] = 0.0;
	}
    for( IndexType vtx = m_firstValidIndex; vtx < size(); ++vtx )
    {
        m_value[vtx] = edges[vtx].norm();
        //assert( !isSmall(m_value[vtx]) ); // Commented-out assert, as it be may thrown while we're checking stuff
    }

    setDependentsDirty();
}

void Tangents::compute()
{
    m_value.resize( m_size );
    const Vec3Array& edges = m_edges.get();
    const std::vector<scalar>& lengths = m_lengths.get();
	for (IndexType vtx = 0; vtx < m_firstValidIndex; ++vtx)
	{
		m_value[vtx].setZero();
	}
    for( IndexType vtx = m_firstValidIndex; vtx < size(); ++vtx )
    {
        m_value[vtx] = edges[vtx] / lengths[vtx];
    }

    setDependentsDirty();
}

void CurvatureBinormals::compute()
{
    m_value.resize( m_size );
	for (IndexType vtx = 0; vtx < m_firstValidIndex; ++vtx)
	{
		m_value[vtx].setZero();
	}

    const Vec3Array& tangents = m_tangents.get();

    for( IndexType vtx = m_firstValidIndex; vtx < size(); ++vtx )
    {
        const Vec3& t1 = tangents[vtx - 1];
        const Vec3& t2 = tangents[vtx];

        scalar denominator = 1. + t1.dot( t2 );

        if( denominator <= 0. || isSmall( denominator ) )
        {
            if( denominator <= 0. )
            {
                denominator = 1. + t1.normalized().dot( t2.normalized() );
            }

            if( denominator <= 0. )
            {
                std::cerr << "CurvatureBinormals::compute() denominator == " << denominator
                        << " at vertex " << vtx << " t1 = " << t1 << " t2 = " << t2 << std::endl;

                m_value[vtx] = Vec3::Constant( std::numeric_limits<scalar>::infinity() ); // Should not be accepted.
            }
            else
            {
                m_value[vtx] = 4. * std::tan( .5 * std::acos( denominator - 1. ) )
                        * findNormal( t1 ).segment<3>(0);
            }
        }
        else
        {
            m_value[vtx] = 2.0 * t1.cross( t2 ) / denominator;
        }
    }

    setDependentsDirty();
}

// Probably not as fast as MKL, but portable.
void TrigThetas::vdSinCos( const int n, const double a[], double r1[], double r2[] )
{
    Eigen::Map<const Eigen::ArrayXd> a_map( a, n );
    Eigen::Map<Eigen::ArrayXd> r1_map( r1, n );
    Eigen::Map<Eigen::ArrayXd> r2_map( r2, n );

    r1_map = a_map.sin();
    r2_map = a_map.cos();
}

void TrigThetas::compute()
{
    const VecX& dofs = m_dofs.get();
    const IndexType numThetas = m_dofs.getNumEdges();
    m_value.first.resize( numThetas );
    m_value.second.resize( numThetas );

    // Extract thetas in their own vector for mkl_vlm
    const Eigen::Map<const VecX, Eigen::Unaligned, Eigen::InnerStride<4> > thetasMap(
            dofs.data() + 3, numThetas );
    const VecX thetaVec( thetasMap );
    // Compute their sine and cosine
    // assert( typeid(double) == typeid(VecX::scalar) );
    vdSinCos( numThetas, thetaVec.data(), m_value.first.data(), m_value.second.data() ); // FIXME this won't compile if scalar != double

    setDependentsDirty();
}
