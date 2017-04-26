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

#ifndef VISCOUS_OR_NOT_VISCOUS_H_
#define VISCOUS_OR_NOT_VISCOUS_H_

#include "../StrandForce.h"
#include "../StrandParameters.h"
// These classes are taken as template arguments for the internal forces,
// indicating whether we want the non-viscous or the viscous version.
// The forces call their ViscousT's static methods returning the appropriate
// stiffness and "rest shape" (the actual rest-shape for non-viscous or the
// shape at the beginning of time step for viscous).

class NonViscous
{
protected:
    NonViscous()
    {}

    virtual ~NonViscous()
    {}

public:
    static std::string getName()
    {
        return "";
    }

    static scalar bendingCoefficient( const StrandForce& strand, int vtx )
    {
        return strand.m_strandParams->bendingCoefficient( vtx, strand.getNumVertices() );
    }

    static Mat2 bendingMatrix( const StrandForce& strand, int vtx )
    {
        return strand.m_strandParams->bendingMatrix( vtx, strand.getNumVertices() );
    }

    static const Vec2 kappaBar( const StrandForce& strand, int vtx )
    {
        return strand.m_restKappas[vtx];
    }

    static scalar kt( const StrandForce& strand, int vtx )
    {
        return strand.m_strandParams->getKt( vtx, strand.getNumVertices() );
    }

    static scalar thetaBar( const StrandForce& strand, int vtx )
    {
        return strand.m_restTwists[vtx];
    }

    static scalar ks( const StrandForce& strand, int vtx )
    {
        return strand.m_strandParams->getKs( vtx, strand.getNumVertices() );
    }

    static scalar ellBar( const StrandForce& strand, int vtx )
    {
        return strand.m_restLengths[vtx];
    }

    class NonDissipativeForce{};
};

class Viscous
{
protected:
    Viscous()
    {}

    virtual ~Viscous()
    {}

public:
    static std::string getName()
    {
        return "viscous ";
    }

    static scalar bendingCoefficient( const StrandForce& strand, int vtx )
    {
        return strand.m_strandParams->viscousBendingCoefficient( vtx, strand.getNumVertices() );
    }

    static Mat2 bendingMatrix( const StrandForce& strand, int vtx )
    {
        return strand.m_strandParams->viscousBendingMatrix( vtx, strand.getNumVertices() );
    }

    static const Vec2 kappaBar( const StrandForce& strand, int vtx )
    {
        return strand.m_startState->m_kappas[vtx];
    }

    static scalar kt( const StrandForce& strand, int vtx )
    {
        return strand.m_strandParams->getViscousKt( vtx, strand.getNumVertices() );
    }

    static scalar thetaBar( const StrandForce& strand, int vtx )
    {
        return strand.m_startState->m_twists[vtx];
    }

    static scalar ks( const StrandForce& strand, int vtx )
    {
        return strand.m_strandParams->getViscousKs( vtx, strand.getNumVertices() );
    }

    static scalar ellBar( const StrandForce& strand, int vtx )
    {
        return strand.m_startState->m_lengths[vtx];
    }

    class DissipativeForce{};
};

#endif
