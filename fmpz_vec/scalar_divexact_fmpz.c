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

    Copyright (C) 2010 William Hart
    Copyright (C) 2010 Sebastian Pancratz

******************************************************************************/

#include <gmp.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_vec.h"

void
_fmpz_vec_scalar_divexact_fmpz(fmpz * vec1, const fmpz * vec2,
                               len_t len2, const fmpz_t x)
{
    fmpz c = *x;

    if (!COEFF_IS_MPZ(c))
    {
        if (c == 1)
            _fmpz_vec_set(vec1, vec2, len2);
        else if (c == -1)
            _fmpz_vec_neg(vec1, vec2, len2);
        else
            _fmpz_vec_scalar_divexact_si(vec1, vec2, len2, c);
    }
    else
    {
        len_t i;
        for (i = 0; i < len2; i++)
            fmpz_divexact(vec1 + i, vec2 + i, x);
    }
}
