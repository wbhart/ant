/*============================================================================

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

===============================================================================*/
/****************************************************************************

   Copyright (C) 2009 William Hart

*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <mpir.h>
#include "flint.h"
#include "ulong_extras.h"
#include "fmpz.h"

int main(void)
{
   int result;
   printf("init/init2/clear....");
   fflush(stdout);
   
   for (ulong i = 0; i < 100000UL; i++) 
   {
      fmpz_t a;

      fmpz_init2(a, n_randint(100));
      fmpz_clear(a);      
   }

   for (ulong i = 0; i < 100000UL; i++) 
   {
      fmpz_t a;

      fmpz_init(a);
      fmpz_randtest(a, FLINT_BITS-2);
      
      _fmpz_promote_val(a);
      _fmpz_demote_val(a);

      fmpz_clear(a);
   }

   _fmpz_cleanup();
   printf("PASS\n");
   return 0;
}