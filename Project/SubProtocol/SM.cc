#include <iostream>
#include "../Key/Public_Key.cc"
#include "../Key/Private_Key.cc"
//#include "../Utility/Random_Generator.cc"
#include "Paillier.cc"
#include <gmpxx.h>

using namespace std;

void step_1(mpz_t a_, mpz_t b_, mpz_t e_a, mpz_t e_b, mpz_t r_a, mpz_t r_b, mpz_t g, mpz_t N) {	
	generate_randomm(r_a, N);
	generate_randomm(r_b, N);
	
	mpz_t temp;
	mpz_init(temp);
	
	encrypt(temp, r_a, g, N);
	mpz_mul(a_, e_a, temp);
	
	encrypt(temp, r_b, g, N);
	mpz_mul(b_, e_b, temp);
	
	mpz_clear(temp);
}

void step_2(mpz_t h_, mpz_t a_, mpz_t b_, mpz_t g, mpz_t mu, mpz_t lambda, mpz_t N) {
	decrypt(a_, a_, N, mu, lambda);
	decrypt(b_, b_, N, mu, lambda);
	
	mpz_mul(h_, a_, b_);
	mpz_mod(h_, h_, N);
	
	encrypt(h_, h_, g, N);
}

void step_3(mpz_t e_ab, mpz_t h_,  mpz_t e_a, mpz_t e_b, mpz_t r_a, mpz_t r_b, mpz_t g, mpz_t N ) {
	mpz_t temp, N_, one, N_sqr;
	mpz_init(temp);
	mpz_init(N_);
	mpz_init_set_str(one, "1", 10);
	
	mpz_init(N_sqr);
	mpz_mul(N_sqr, N, N);
	
	mpz_sub(N_, N, r_b);
	mpz_powm(temp, e_a, N_, N_sqr);
	mpz_mul(h_, h_, temp);
	
	mpz_sub(N_, N, r_a);
	mpz_powm(temp, e_b, N_, N_sqr);
	mpz_mul(h_, h_, temp);
	
	mpz_mul(temp, r_a, r_b);
	encrypt(temp, temp, g, N);
	mpz_sub(N_, N, one);
	mpz_powm(temp, temp, N_, N_sqr);
	mpz_mul(e_ab, h_, temp);	
	
	mpz_clear(temp);
	mpz_clear(N_);
	mpz_clear(one);
}

void secure_multiplication(mpz_t e_ab, mpz_t e_a, mpz_t e_b, mpz_t g, mpz_t N, mpz_t mu, mpz_t lambda){
	mpz_t a_, b_, r_a, r_b, h_;
	
	mpz_init(r_a);
	mpz_init(r_b);
	
	mpz_init(a_);
	mpz_init(b_);
	
	mpz_init(h_);
	
	step_1(a_, b_, e_a, e_b, r_a, r_b, g, N);
	step_2(h_, a_, b_, g, mu, lambda, N);
	step_3(e_ab, h_,  e_a, e_b, r_a, r_b, g, N);
	
	
	mpz_clear(r_a);
	mpz_clear(r_b);
	
	mpz_clear(h_);
	mpz_clear(a_);
	mpz_clear(b_);
}
/*
int main (int argc, char *argv[])
{
	init_pk();
	init_sk();
	
	read_pk();
	read_sk();
	
	mpz_t a, b, e_a, e_b, e_ab;
	mpz_init(e_ab);
	mpz_init(e_a);
	mpz_init(e_b);
	
	mpz_init_set_str(a, "16", 10);
	mpz_init_set_str(b, "500", 10);
	
	encrypt(e_a, a, g, N_pk);
	encrypt(e_b, b, g, N_pk);
	
	secure_multiplication(e_ab, e_a, e_b, g, N_sk, mu, lambda);
	
	decrypt(e_ab, e_ab, N_sk, mu, lambda);
	gmp_printf ("e_ab: %Zd\n", e_ab);

	
	clear_pk();
	clear_sk();
	
	return 0;	
}
*/