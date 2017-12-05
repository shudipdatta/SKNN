#include <iostream>
#include <gmpxx.h>

using namespace std;

int main (void)
{
	mpz_t integ;
	mpz_init (integ);
	gmp_printf ("%Zd\n", integ);
	mpz_clear (integ);
	return 0;
}
