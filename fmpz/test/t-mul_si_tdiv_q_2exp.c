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

    Copyright (C) 2009 William Hart
    Copyright (C) 2012 Fredrik Johansson

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "fmpz.h"
#include "long_extras.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    printf("mul_si_tdiv_q_2exp....");
    fflush(stdout);

    flint_randinit(state);

    for (i = 0; i < 10000 * flint_test_multiplier(); i++)
    {
        fmpz_t a, b;
        mpz_t d, e, f;
        long x;
        ulong exp;

        fmpz_init(a);
        fmpz_init(b);

        mpz_init(d);
        mpz_init(e);
        mpz_init(f);

        fmpz_randtest(a, state, 200);

        fmpz_get_mpz(d, a);
        x = z_randtest(state);
        exp = n_randint(state, 200);

        fmpz_mul_si_tdiv_q_2exp(b, a, x, exp);
        mpz_mul_si(e, d, x);
        mpz_tdiv_q_2exp(e, e, exp);

        fmpz_get_mpz(f, b);

        result = (mpz_cmp(e, f) == 0);
        if (!result)
        {
            printf("FAIL:\n");
            gmp_printf("d = %Zd, e = %Zd, f = %Zd, x = %ld, exp = %lu\n",
                d, e, f, x, exp);
            abort();
        }

        fmpz_clear(a);
        fmpz_clear(b);

        mpz_clear(d);
        mpz_clear(e);
        mpz_clear(f);
    }

    /* Check aliasing of a and b */
    for (i = 0; i < 10000 * flint_test_multiplier(); i++)
    {
        fmpz_t a;
        mpz_t d, e, f;
        long x;
        ulong exp;

        fmpz_init(a);

        mpz_init(d);
        mpz_init(e);
        mpz_init(f);

        fmpz_randtest(a, state, 200);

        fmpz_get_mpz(d, a);
        x = z_randtest(state);
        exp = n_randint(state, 200);

        fmpz_mul_si_tdiv_q_2exp(a, a, x, exp);
        mpz_mul_si(e, d, x);
        mpz_tdiv_q_2exp(e, e, exp);

        fmpz_get_mpz(f, a);

        result = (mpz_cmp(e, f) == 0);
        if (!result)
        {
            printf("FAIL:\n");
            gmp_printf("d = %Zd, e = %Zd, f = %Zd, x = %ld, exp = %lu\n",
                d, e, f, x, exp);
            abort();
        }

        fmpz_clear(a);

        mpz_clear(d);
        mpz_clear(e);
        mpz_clear(f);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return 0;
}