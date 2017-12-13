#include <iostream>
//#include "../Key/Public_Key.cc"
//#include "../Key/Private_Key.cc"
//#include "../Utility/Random_Generator.cc"
//#include "Paillier.cc"
#include "SM.cc"
#include <gmpxx.h>

using namespace std;

void secure_bitor (mpz_t e_aorb, mpz_t e_a, mpz_t e_b, mpz_t g, mpz_t N, mpz_t mu, mpz_t lambda) {
	mpz_t e_ab, N_, N_sqr;
	mpz_init(e_ab);
	mpz_init_set_str(N_, "1", 10);	
	mpz_sub(N_, N, N_);
	mpz_init(N_sqr);
	mpz_mul(N_sqr, N, N);
	
	secure_multiplication(e_ab, e_a, e_b, g, N, mu, lambda);
	mpz_powm(e_ab, e_ab, N_, N_sqr);
	
	mpz_mul(e_aorb, e_a, e_b);
	mpz_mul(e_aorb, e_aorb, e_ab);
}

int main (int argc, char *argv[])
{
	init_pk();
	init_sk();
	
	read_pk();
	read_sk();
	
	
	mpz_t e_a, e_b, e_aorb;
	mpz_init(e_aorb);
	mpz_init_set_str(e_a, "1", 10);
	mpz_init_set_str(e_b, "0", 10);
	encrypt(e_a, e_a, g, N_pk);
	encrypt(e_b, e_b, g, N_pk);
	
	secure_bitor (e_aorb, e_a, e_b, g, N_pk, mu, lambda);
	decrypt(e_aorb, e_aorb, N_sk, mu, lambda);
	gmp_printf("a or b: %Zd\n", e_aorb);
	
	
	clear_pk();
	clear_sk();
	
	return 0;	
}