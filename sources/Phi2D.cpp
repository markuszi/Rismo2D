// /////////////////////////////////////////////////////////////////////////////////////////////////
//
// class MODEL
//
// /////////////////////////////////////////////////////////////////////////////////////////////////
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
// /////////////////////////////////////////////////////////////////////////////////////////////////

// ---------------------------------------------------------------------------------------
// Compute product of flow gradients (phi).
//

#include "Defs.h"
#include "Report.h"
#include "Shape.h"
#include "Memory.h"
#include "Type.h"
#include "Vars.h"
#include "Node.h"
#include "Elem.h"
#include "Model.h"
#include "Subdom.h"


double* MODEL::Phi2D()
{
  int np = region->Getnp();
  int ne = region->Getne();


  // -------------------------------------------------------------------------------------
  // allocate memory arrays
  // -------------------------------------------------------------------------------------

  double* phi = (double*) MEMORY::memo.Array_nd( np );
  double* wgt = (double*) MEMORY::memo.Array_nd( np );

  for( int n=0; n<np; n++ )  phi[n] = wgt[n] = 0.0;


  // -------------------------------------------------------------------------------------
  // loop on elements
  // -------------------------------------------------------------------------------------

  for( int e=0; e<ne; e++ )
  {
    ELEM* elem = region->Getelem(e);

    SHAPE* shape = elem->GetQShape();

    int ngp = shape->ngp;          // number of GAUSS points
    int nnd = shape->nnd;          // number of corner nodes


    // -----------------------------------------------------------------------------------
    // compute coordinates relative to first node

    double x[kMaxNodes2D], y[kMaxNodes2D];

    x[0] = elem->nd[0]->x;
    y[0] = elem->nd[0]->y;

    for( int i=1; i<nnd; i++ )
    {
      x[i] = elem->nd[i]->x - *x;
      y[i] = elem->nd[i]->y - *y;
    }
    x[0] = y[0] = 0.0;


    // -----------------------------------------------------------------------------------
    // GAUSS point integration

    for( int g=0; g<ngp; g++ )
    {
      // form JACOBIAN transformation matrix ---------------------------------------------

      double  trafo[2][2];

      double* dfdxPtr = shape->dfdx[g];
      double* dfdyPtr = shape->dfdy[g];

      double detj   = shape->jacobi2D( nnd, dfdxPtr, dfdyPtr, x, y, trafo );
      double weight = detj * shape->weight[g];

      // compute values of shape functions at GP g ---------------------------------------

      double* n = shape->f[g];

      double dndx[kMaxNodes2D], dndy[kMaxNodes2D];

      for( int i=0; i<nnd; i++ )
      {
        dndx[i] = trafo[0][0] * dfdxPtr[i] + trafo[0][1] * dfdyPtr[i];
        dndy[i] = trafo[1][0] * dfdxPtr[i] + trafo[1][1] * dfdyPtr[i];
      }

      // compute flow parameters and their derivatives -----------------------------------

      double dUdx = 0.0;
      double dUdy = 0.0;

      double dVdx = 0.0;
      double dVdy = 0.0;

      for( int i=0; i<nnd; i++ )
      {
        NODE* node = elem->nd[i];

        double ndU = node->v.U;
        dUdx += dndx[i] * ndU;
        dUdy += dndy[i] * ndU;

        double ndV = node->v.V;
        dVdx += dndx[i] * ndV;
        dVdy += dndy[i] * ndV;
      }

      // compute product of gradients ----------------------------------------------------

      double f = weight * ( 2.0*dUdx*dUdx + 2.0*dVdy*dVdy + (dUdy+dVdx)*(dUdy+dVdx) );

      for( int i=0; i<nnd; i++ )
      {
        NODE* nd = elem->nd[i];
        int   no = nd->Getno();

        phi[no] += f;
        wgt[no] += weight;
      }
    }
  }

  subdom->Mpi_assemble( phi );
  subdom->Mpi_assemble( wgt );

  
  // -------------------------------------------------------------------------------------
  // solve for product of gradients

  for( int n=0; n<np; n++ )
  {
    phi[n] /= wgt[n];
  }

  // -------------------------------------------------------------------------------------

  MEMORY::memo.Detach( wgt );

  return phi;
}
