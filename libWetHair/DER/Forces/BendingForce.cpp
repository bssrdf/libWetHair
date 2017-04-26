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


#include "BendingForce.h"
#include "ViscousOrNotViscous.h"
#include "../Dependencies/ElasticStrandUtils.h"

template<typename ViscousT>
scalar BendingForce<ViscousT>::localEnergy( const StrandForce& strand, const IndexType vtx )
{
    const Mat2& B = ViscousT::bendingMatrix( strand, vtx );
    const Vec2& kappaBar = ViscousT::kappaBar( strand, vtx );
    const scalar ilen = strand.m_invVoronoiLengths[vtx];
    const Vec2& kappa = strand.m_strandState->m_kappas[vtx];

    return 0.5 * ilen * ( kappa - kappaBar ).dot( Vec2( B * ( kappa - kappaBar ) ) );
}

template<typename ViscousT>
void BendingForce<ViscousT>::computeLocal( Eigen::Matrix<scalar, 11, 1>& localF,
        const StrandForce& strand, const IndexType vtx )
{
    const Mat2& B = ViscousT::bendingMatrix( strand, vtx );
    const Vec2& kappaBar = ViscousT::kappaBar( strand, vtx );
    const scalar ilen = strand.m_invVoronoiLengths[vtx];
    const Vec2& kappa = strand.m_strandState->m_kappas[vtx];
    const GradKType& gradKappa = strand.m_strandState->m_gradKappas[vtx];

    localF = -ilen * gradKappa * B * ( kappa - kappaBar );
}

template<typename ViscousT>
void BendingForce<ViscousT>::computeLocal(Eigen::Matrix<scalar, 11, 11>& localJ,
        const StrandForce& strand, const IndexType vtx )
{
    localJ = strand.m_strandState->m_bendingProducts[vtx];

    if( strand.m_requiresExactForceJacobian )
    {
        const Mat2& bendingMatrixBase = strand.m_strandParams->bendingMatrixBase();
        const Vec2& kappaBar = ViscousT::kappaBar( strand, vtx );
        const Vec2& kappa = strand.m_strandState->m_kappas[vtx];
        const std::pair<LocalJacobianType, LocalJacobianType>& hessKappa = strand.m_strandState->m_hessKappas[vtx];
        const Vec2& temp = bendingMatrixBase * ( kappa - kappaBar );

        localJ += temp( 0 ) * hessKappa.first + temp( 1 ) * hessKappa.second;
    }

    const scalar ilen = strand.m_invVoronoiLengths[vtx];
    localJ *= -ilen * ViscousT::bendingCoefficient( strand, vtx );
}

template<typename ViscousT>
void BendingForce<ViscousT>::addInPosition( VecX& globalForce, const IndexType vtx,
        const LocalForceType& localForce )
{
    globalForce.segment<11>( 4 * ( vtx - 1 ) ) += localForce;
}

template<typename ViscousT>
void BendingForce<ViscousT>::accumulateCurrentE( scalar& energy, StrandForce& strand )
{
    for( IndexType vtx = s_first; vtx < strand.getNumVertices() - s_last; ++vtx )
    {
        energy += localEnergy( strand, vtx );
    }
}

template<typename ViscousT>
void BendingForce<ViscousT>::accumulateCurrentF( VecX& force, StrandForce& strand )
{
    for( IndexType vtx = s_first; vtx < strand.getNumVertices() - s_last; ++vtx )
    {
        LocalForceType localF;
        computeLocal( localF, strand, vtx );
        addInPosition( force, vtx, localF );
    }
}

template<typename ViscousT>
void BendingForce<ViscousT>::accumulateIntegrationVars( 
    const unsigned& pos_start, const unsigned& j_start, const unsigned& tildek_start, const unsigned& global_start_dof, 
    StrandForce& strand, VectorXs& lambda, TripletXs& J, TripletXs& tildeK, TripletXs& stiffness, VectorXs& Phi, const int& lambda_start )
{

    const Mat2& B = ViscousT::bendingMatrix( strand, 1 );
    scalar b = B(0,0); // assumes circular rods

    for( IndexType vtx = s_first; vtx < strand.getNumVertices() - s_last; ++vtx )
    {
        unsigned dfirst = global_start_dof + 4 * (vtx-1);
        unsigned dsecond = global_start_dof + 4 * vtx;
        unsigned dthird = global_start_dof + 4 * (vtx+1);

        const Vec2& kappaBar = ViscousT::kappaBar( strand, vtx );
        const Vec2& kappa = strand.m_strandState->m_kappas[vtx];
        const scalar ilen = strand.m_invVoronoiLengths[vtx];

        unsigned idx_pos = pos_start + 2 * (vtx - 1);
        Phi.segment<2>( idx_pos ) = kappa - kappaBar;
        stiffness[ idx_pos ] = Triplets( idx_pos, idx_pos, b * ilen );
        stiffness[ idx_pos + 1 ] = Triplets( idx_pos + 1, idx_pos + 1, b * ilen );

        Eigen::Matrix<scalar, 2, 11> utgk = (strand.m_strandState->m_gradKappas[vtx]).transpose();
        unsigned idx_j = j_start + 2*(11 * (vtx - 1));
        for( int r = 0; r < 4; ++r ){
            J[ idx_j + r ] = Triplets( idx_pos, dfirst + r, utgk(0,r));
            J[ idx_j + 4 + r ] = Triplets( idx_pos, dsecond + r, utgk(0,r + 4));
            if( r < 3 ) J[ idx_j + 8 + r ] = Triplets( idx_pos, dthird + r, utgk(0,r + 8));

            J[ idx_j + r + 11 ] = Triplets( idx_pos + 1, dfirst + r, utgk(1,r));
            J[ idx_j + 4 + r + 11] = Triplets( idx_pos + 1, dsecond + r, utgk(1,r + 4));
            if( r < 3 ) J[ idx_j + 8 + r + 11 ] = Triplets( idx_pos + 1, dthird + r, utgk(1,r + 8));            
        }

        if( std::is_same< ViscousT, NonViscous >::value ){
            int lidx = lambda_start + ( 2 * ( vtx - 1) );
            scalar weight0 = -lambda[lidx];
            scalar weight1 = -lambda[lidx + 1];

            const std::pair<LocalJacobianType, LocalJacobianType>& hessKappa = strand.m_strandState->m_hessKappas[vtx];
            unsigned idx_tildek = tildek_start + 2 * (121 * (vtx - 1));
            for(int r = 0; r < 11; ++r) {
                for(int s = 0; s < 11; ++s){
                    tildeK[ idx_tildek + r * 11 + s] = Triplets( dfirst + r, dfirst + s, hessKappa.first(r, s) * weight0 );
                    tildeK[ idx_tildek + r * 11 + s + 121] = Triplets( dfirst + r, dfirst + s, hessKappa.second(r, s) * weight1 );
                }
            }
        }
    }
}

template class BendingForce<NonViscous>;
template class BendingForce<Viscous>;
