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

    Copyright (C) 2010 Fredrik Johansson

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "fmpz_vec.h"
#include "fmpz_mat.h"
#include "ulong_extras.h"


int
main(void)
{
    fmpz_mat_t A, B, C, I;
    fmpz_t den;
    fmpz_randstate_t rnd;
    long i, j, m, r;

    printf("inv....");
    fflush(stdout);

    fmpz_randinit(rnd);

    for (i = 0; i < 10000; i++)
    {
        m = n_randint(10);

        fmpz_mat_init(A, m, m);
        fmpz_mat_init(B, m, m);
        fmpz_mat_init(C, m, m);
        fmpz_mat_init(I, m, m);
        fmpz_init(den);

        for (j = 0; j < m; j++)
            fmpz_set_ui(&I->rows[j][j], 1UL);

        /* Verify that A * A^-1 = I for random matrices */

        fmpz_mat_randrank(A, rnd, m, 1+n_randint(2)*n_randint(100));
        /* Dense or sparse? */
        if (n_randint(2))
            fmpz_mat_randops(A, rnd, 1+n_randint(1+m*m));

        fmpz_mat_inv(B, den, A);
        fmpz_mat_mul(C, A, B);

        _fmpz_vec_scalar_divexact_fmpz(C->entries, C->entries, m*m, den);

        if (!_fmpz_vec_equal(C->entries, I->entries, m*m))
        {
            printf("FAIL:\n");
            printf("A * A^-1 != I!\n");
            printf("A:\n");
            fmpz_mat_print_pretty(A);
            printf("A^-1: (den = "); fmpz_print(den); printf(")\n");
            fmpz_mat_print_pretty(B);
            printf("A * A^-1:\n");
            fmpz_mat_print_pretty(C);
            printf("\n");
            abort();
        }

        /* Test aliasing */
        fmpz_mat_copy(C, A);
        fmpz_mat_inv(A, den, A);
        fmpz_mat_mul(B, A, C);
        _fmpz_vec_scalar_divexact_fmpz(B->entries, B->entries, m*m, den);

        if (!_fmpz_vec_equal(B->entries, I->entries, m*m))
        {
            printf("FAIL:\n");
            printf("aliasing failed!\n");
            fmpz_mat_print(C);
            abort();
        }

        fmpz_mat_clear(A);
        fmpz_mat_clear(B);
        fmpz_mat_clear(C);
        fmpz_mat_clear(I);
        fmpz_clear(den);
    }

    /* Test singular matrices */
    /* Test singular systems */
    for (i = 0; i < 10000; i++)
    {
        m = 1 + n_randint(10);
        r = n_randint(m);

        fmpz_mat_init(A, m, m);
        fmpz_mat_init(B, m, m);
        fmpz_init(den);

        fmpz_mat_randrank(A, rnd, r, 1+n_randint(2)*n_randint(100));

        /* Dense */
        if (n_randint(2))
            fmpz_mat_randops(A, rnd, 1+n_randint(1+m*m));

        fmpz_mat_inv(B, den, A);
        if (!fmpz_is_zero(den))
        {
            printf("FAIL:\n");
            printf("singular system gave nonzero denominator\n");
            abort();
        }

        /* Aliasing */
        fmpz_mat_inv(A, den, A);
        if (!fmpz_is_zero(den))
        {
            printf("FAIL:\n");
            printf("singular system gave nonzero denominator\n");
            abort();
        }

        fmpz_mat_clear(A);
        fmpz_mat_clear(B);
        fmpz_clear(den);
    }

    fmpz_mat_randclear(rnd);
    _fmpz_cleanup();
    printf("PASS\n");
    return 0;
}
