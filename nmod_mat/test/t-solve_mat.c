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
#include "nmod_vec.h"
#include "nmod_mat.h"
#include "ulong_extras.h"


int
main(void)
{
    nmod_mat_t A, X, B, AX;
    long i, m, n, r;
    mp_limb_t mod;
    int solved;

    printf("solve_mat....");
    fflush(stdout);

    for (i = 0; i < 10000; i++)
    {
        m = n_randint(10);
        n = n_randint(10);
        mod = n_nextprime(n_randtest_not_zero() - 1, 0);

        nmod_mat_init(A, m, m, mod);
        nmod_mat_init(B, m, n, mod);
        nmod_mat_init(X, m, n, mod);
        nmod_mat_init(AX, m, n, mod);

        nmod_mat_randrank(A, m);
        nmod_mat_randtest(B);

        /* Dense */
        if (n_randint(2))
            nmod_mat_randops(A, 1+n_randint(1+m*m));

        solved = nmod_mat_solve_mat(X, A, B);

        nmod_mat_mul(AX, A, X);

        if (!nmod_mat_equal(AX, B) || !solved)
        {
            printf("FAIL:\n");
            printf("AX != B!\n");
            printf("A:\n");
            nmod_mat_print_pretty(A);
            printf("B:\n");
            nmod_mat_print_pretty(B);
            printf("X:\n");
            nmod_mat_print_pretty(X);
            printf("AX:\n");
            nmod_mat_print_pretty(AX);
            printf("\n");
            abort();
        }

        nmod_mat_clear(A);
        nmod_mat_clear(B);
        nmod_mat_clear(X);
        nmod_mat_clear(AX);
    }

    /* Test singular systems */
    for (i = 0; i < 10000; i++)
    {
        m = 1 + n_randint(10);
        n = 1 + n_randint(10);
        r = n_randint(m);
        mod = n_nextprime(n_randtest_not_zero() - 1, 0);

        nmod_mat_init(A, m, m, mod);
        nmod_mat_init(B, m, n, mod);
        nmod_mat_init(X, m, n, mod);
        nmod_mat_init(AX, m, n, mod);

        nmod_mat_randrank(A, r);
        nmod_mat_randtest(B);

        /* Dense */
        if (n_randint(2))
            nmod_mat_randops(A, 1+n_randint(1+m*m));

        solved = nmod_mat_solve_mat(X, A, B);

        if (solved)
        {
            printf("FAIL:\n");
            printf("singular system was 'solved'\n");
            nmod_mat_print_pretty(A);
            nmod_mat_print_pretty(X);
            nmod_mat_print_pretty(B);
            abort();
        }

        nmod_mat_clear(A);
        nmod_mat_clear(B);
        nmod_mat_clear(X);
        nmod_mat_clear(AX);
    }

    printf("PASS\n");
    return 0;
}