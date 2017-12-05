#include <iostream>
#include <time.h>
#include <gmpxx.h>

using namespace std;
/*
*generate a random number with bit length 'bit_len'. 
*The random number 'rand' will hold a value in range [0, 2^(bit_len) - 1]
*/
void generate_randomb(mpz_t rand, const mp_bitcnt_t bit_len){
	unsigned long seed = clock();
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	gmp_randseed_ui(state, seed);
	mpz_urandomb(rand, state, bit_len);
	gmp_randclear(state);
}

/*
*generate a random number with value < 'N'. 
*The random number 'rand' will hold a value in range [0, N - 1]
*/
void generate_randomm(mpz_t rand, mpz_t N){
	unsigned long seed = clock();
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	gmp_randseed_ui(state, seed);
	mpz_urandomm(rand, state, N);
	gmp_randclear(state);
}
