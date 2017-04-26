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

#ifndef KAPPAS_H_
#define KAPPAS_H_

#include "MaterialFrames.h"

/**
 * Unit: no dimension.
 */
class Kappas: public DependencyNode<Vec2Array>
{
public:
    Kappas( CurvatureBinormals& curvatureBinormals, MaterialFrames<1>& materialFrames1,
            MaterialFrames<2>& materialFrames2 ) :
            DependencyNode<Vec2Array>( 1, curvatureBinormals.size() ), m_curvatureBinormals(
                    curvatureBinormals ), m_materialFrames1( materialFrames1 ), m_materialFrames2(
                    materialFrames2 )
    {
        m_curvatureBinormals.addDependent( this );
        m_materialFrames1.addDependent( this );
        m_materialFrames2.addDependent( this );
    }

    virtual const char* name() const
    {
        return "Kappas";
    }

protected:
    virtual void compute();

    CurvatureBinormals& m_curvatureBinormals;
    MaterialFrames<1>& m_materialFrames1;
    MaterialFrames<2>& m_materialFrames2;
};

typedef Eigen::Matrix<scalar, 11, 2> GradKType;
typedef std::vector<GradKType, Eigen::aligned_allocator<GradKType>> GradKArrayType;

/**
 * Unit: cm^-1 for position derivatives, no dimension for theta derivatives
 */
class GradKappas: public DependencyNode<GradKArrayType>
{
public:
    GradKappas( Lengths& lengths, Tangents& tangents, CurvatureBinormals& curvatureBinormals,
            MaterialFrames<1>& materialFrames1, MaterialFrames<2>& materialFrames2, Kappas& kappas ) :
            DependencyNode<GradKArrayType>( 1, curvatureBinormals.size() ), //
            m_lengths( lengths ), //
            m_tangents( tangents ), //
            m_curvatureBinormals( curvatureBinormals ), //
            m_materialFrames1( materialFrames1 ), //
            m_materialFrames2( materialFrames2 ), //
            m_kappas( kappas )
    {
        m_lengths.addDependent( this );
        m_tangents.addDependent( this );
        m_curvatureBinormals.addDependent( this );
        m_materialFrames1.addDependent( this );
        m_materialFrames2.addDependent( this );
        m_kappas.addDependent( this );
    }

    virtual const char* name() const
    {
        return "GradKappas";
    }

protected:
    virtual void compute();

    Lengths& m_lengths;
    Tangents& m_tangents;
    CurvatureBinormals& m_curvatureBinormals;
    MaterialFrames<1>& m_materialFrames1;
    MaterialFrames<2>& m_materialFrames2;
    Kappas& m_kappas;
};

typedef Mat11Pair HessKType;
typedef std::vector<HessKType> HessKArrayType;

inline std::ostream& operator<<( std::ostream& os, const HessKType& HessKappa )
{
    os << "Hess kappa1: " << HessKappa.first << '\n';
    os << "Hess kappa2: " << HessKappa.second;

    return os;
}

/**
 * Unit: cm^-2 for position derivatives, no dimension for theta derivatives
 */
class HessKappas: public DependencyNode<HessKArrayType>
{
public:
    HessKappas( Lengths& lengths, Tangents& tangents, CurvatureBinormals& curvatureBinormals,
            MaterialFrames<1>& materialFrames1, MaterialFrames<2>& materialFrames2, Kappas& kappas ) :
            DependencyNode<HessKArrayType>( 1, curvatureBinormals.size() ), //
            m_lengths( lengths ), //
            m_tangents( tangents ), //
            m_curvatureBinormals( curvatureBinormals ), //
            m_materialFrames1( materialFrames1 ), //
            m_materialFrames2( materialFrames2 ), //
            m_kappas( kappas )
    {
        m_lengths.addDependent( this );
        m_tangents.addDependent( this );
        m_curvatureBinormals.addDependent( this );
        m_materialFrames1.addDependent( this );
        m_materialFrames2.addDependent( this );
        m_kappas.addDependent( this );
    }

    virtual const char* name() const
    {
        return "HessKappas";
    }

protected:
    virtual void compute();

    Lengths& m_lengths;
    Tangents& m_tangents;
    CurvatureBinormals& m_curvatureBinormals;
    MaterialFrames<1>& m_materialFrames1;
    MaterialFrames<2>& m_materialFrames2;
    Kappas& m_kappas;
};

typedef std::pair<Mat2, Mat2> ThetaHessKType;
typedef std::vector<ThetaHessKType> ThetaHessKArrayType;

inline std::ostream& operator<<( std::ostream& os, const ThetaHessKType& HessKappa )
{
    os << "ThetaHess kappa1: " << HessKappa.first << '\n';
    os << "ThetaHess kappa2: " << HessKappa.second;

    return os;
}

/**
 * Unit: no dimension
 */
class ThetaHessKappas: public DependencyNode<ThetaHessKArrayType>
{
public:
    ThetaHessKappas( CurvatureBinormals& curvatureBinormals, MaterialFrames<1>& materialFrames1,
            MaterialFrames<2>& materialFrames2 ) :
            DependencyNode<ThetaHessKArrayType>( 1, curvatureBinormals.size() ), //
            m_curvatureBinormals( curvatureBinormals ), //
            m_materialFrames1( materialFrames1 ), //
            m_materialFrames2( materialFrames2 )
    {
        m_curvatureBinormals.addDependent( this );
        m_materialFrames1.addDependent( this );
        m_materialFrames2.addDependent( this );
    }

    virtual const char* name() const
    {
        return "ThetaHessKappas";
    }

protected:
    virtual void compute();

    CurvatureBinormals& m_curvatureBinormals;
    MaterialFrames<1>& m_materialFrames1;
    MaterialFrames<2>& m_materialFrames2;
};

#endif
