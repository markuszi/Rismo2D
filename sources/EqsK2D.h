// /////////////////////////////////////////////////////////////////////////////////////////////////
//
// E Q S _ K 2 D
//
// /////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILES
//
// EqsK2D.h     : definition file of the class.
// EqsK2D.cpp   : implementation file of the class.
//
// CoefsK2D.cpp : methods EQS_K2D::Coefs()
//                        EQS_K2D::Region()
//
// -------------------------------------------------------------------------------------------------
//
// DESCRIPTION
//
// This class implements a differential equation system for bottom evolution.
//
// -------------------------------------------------------------------------------------------------
//
// COPYRIGHT (C) 2011 - 2014  by  P.M. SCHROEDER  (sc)
//
// This program is free software; you can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this program; if
// not, write to the
//
// Free Software Foundation, Inc.
// 59 Temple Place
// Suite 330
// Boston
// MA 02111-1307 USA
//
// -------------------------------------------------------------------------------------------------
//
// P.M. Schroeder
// Walzbachtal / Germany
// michael.schroeder@hnware.de
//
// -------------------------------------------------------------------------------------------------
//
// HISTORY
//
//    date              changes
// ------------  ----  -----------------------------------------------------------------------------
//  09.04.2010    sc    first implementation / first concept
//
// /////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef EQS_K2D_INCL
#define EQS_K2D_INCL

#include "Eqs.h"


class  DRYREW;
class  ELEM;
class  MODEL;
class  NODE;
class  PROJECT;
class  TIMEINT;


class EQS_K2D : public EQS
{
  protected:
    double  relaxThdt_KD;

  public:
    EQS_K2D();
    ~EQS_K2D();

    void Execute( PROJECT*, int, int );
    int  Coefs( ELEM*, PROJECT*, double**, double* );

  protected:
    void Region( ELEM*, PROJECT*, double**, double* );

    void Validate( int, NODE**, PROJECT* );
};

#endif
