#include <iostream>
#include <gmpxx.h>

using namespace std;

void foo (mpz_t result, const mpz_t param, unsigned long n)
{
	unsigned long  i;
	mpz_mul_ui (result, param, n);
	for (i = 1; i <= n; i++)
		mpz_add_ui (result, result, i*7);
	mpz_sub_ui (result, result, 1);
}

int main (void)
{
	mpz_t  r, n, test1, test2, test3;
	mpz_init (r);
	mpz_init_set_str (n, "25", 10);
	
	mpz_init_set (test1,n);	
	//mpz_init_set_str (test2, "5", 10);

	FILE* input;
	input = fopen("input","r");
	mpz_inp_str (test2, input, 10);
	fclose(input);
	
	foo (r, n, 2);

	mpz_init(test3);
	mpz_cdiv_q (test3, test1, test2);
	//mpz_cdiv_r (test3, test1, test2);

	gmp_printf ("%Zd\n", r);
	gmp_printf ("%Zd\n", test1);

	//gmp_printf ("%Zd\n", test3);
	FILE* output;
	output = fopen("output","w");
	mpz_out_str(output, 10, test3);
	fclose(output);

	return 0;
}
