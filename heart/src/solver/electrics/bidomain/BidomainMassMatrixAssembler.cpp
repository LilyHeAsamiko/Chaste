/*

Copyright (c) 2005-2012, University of Oxford.
All rights reserved.

University of Oxford means the Chancellor, Masters and Scholars of the
University of Oxford, having an administrative office at Wellington
Square, Oxford OX1 2JD, UK.

This file is part of Chaste.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
 * Neither the name of the University of Oxford nor the names of its
   contributors may be used to endorse or promote products derived from this
   software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/



#include "BidomainMassMatrixAssembler.hpp"
#include "HeartRegionCodes.hpp"

template<unsigned DIM>
c_matrix<double,2*(DIM+1),2*(DIM+1)> BidomainMassMatrixAssembler<DIM>::ComputeMatrixTerm(
        c_vector<double, DIM+1> &rPhi,
        c_matrix<double, DIM, DIM+1> &rGradPhi,
        ChastePoint<DIM> &rX,
        c_vector<double,2> &rU,
        c_matrix<double,2,DIM> &rGradU /* not used */,
        Element<DIM,DIM>* pElement)
{
    c_matrix<double,2*(DIM+1),2*(DIM+1)> ret = zero_matrix<double>(2*(DIM+1), 2*(DIM+1));

    if (!HeartRegionCode::IsRegionBath( pElement->GetUnsignedAttribute() ))
    {
        c_matrix<double, DIM+1, DIM+1> basis_outer_prod = outer_prod(rPhi, rPhi);

        // even rows, even columns
        matrix_slice<c_matrix<double, 2*DIM+2, 2*DIM+2> >
        slice00(ret, slice (0, 2, DIM+1), slice (0, 2, DIM+1));
        slice00 =  basis_outer_prod;

        // odd rows, even columns: are zero
        // even rows, odd columns: are zero
        // odd rows, odd columns: are zero
    }

    return ret;
}



///////////////////////////////////////////////////////
// explicit instantiation
///////////////////////////////////////////////////////

template class BidomainMassMatrixAssembler<1>;
template class BidomainMassMatrixAssembler<2>;
template class BidomainMassMatrixAssembler<3>;
