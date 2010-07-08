/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2010 Sebastian Pancratz

******************************************************************************/

#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_vec.h"
#include "fmpq_poly.h"

// Assumes that rpoly and poly are either equal or disjoint blocks of memory.  
// rden and den may be aliased, but none of them may be in rpoly.
// Assumes that c is non-zero.  (The case c == 0 is not handled separately in 
// this method.)
// c may also be aliased with rden or den.
void _fmpq_poly_scalar_div_fmpz(fmpz * rpoly, fmpz_t rden, 
                              const fmpz * poly, const fmpz_t den, ulong len, 
                              const fmpz_t c)
{
    if (*c == 1L)
    {
        if (rpoly != poly)
            _fmpz_vec_copy(rpoly, poly, len);
        fmpz_set(rden, den);
    }
    else if (*c == -1L)
    {
        _fmpz_vec_neg(rpoly, poly, len);
        fmpz_set(rden, den);
    }
    else
    {
        fmpz_t d;
        fmpz_init(d);
        _fmpz_vec_content(d, poly, len);
        fmpz_gcd(d, d, c);
        if (*d == 1L)
        {
            if (rpoly != poly)
                _fmpz_vec_copy(rpoly, poly, len);
            fmpz_mul(rden, den, c);
        }
        else
        {
            _fmpz_vec_scalar_divexact(rpoly, poly, len, d);
            fmpz_divexact(d, c, d);
            fmpz_mul(rden, den, d);
        }
        fmpz_clear(d);
    }
}

// rop and op may be aliased, but c may not be among the coefficients of rop, but it is allowed to be the denominator
void fmpq_poly_scalar_div_fmpz(fmpq_poly_t rop, const fmpq_poly_t op, const fmpz_t c)
{
    if (*c == 0L)
    {
        printf("ERROR (fmpq_poly_scalar_div_fmpz).  Division by 0.\n");
        abort();
    }
    
    if (rop == op)
        _fmpq_poly_scalar_div_fmpz(rop->coeffs, rop->den, op->coeffs, op->den, op->length, c);
    else
    {
        fmpq_poly_fit_length(rop, op->length);
        _fmpq_poly_scalar_div_fmpz(rop->coeffs, rop->den, op->coeffs, op->den, op->length, c);
        _fmpq_poly_set_length(rop, op->length);
    }
}
