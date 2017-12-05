#include <iostream>
#include <time.h>
#include <gmpxx.h>

using namespace std;

mpz_t lambda, N_sk, mu;

void init_sk() {
	mpz_init(N_sk);
	mpz_init(lambda);
	mpz_init(mu);
}

void read_sk () {
	
	FILE* file;
	if(!(file = fopen("Key/sk_file", "r"))) {
		fputs("not possible to open file!\n", stderr);
		exit(1);
	}
	
	mpz_inp_str (lambda, file, 10);
	mpz_inp_str (mu, file, 10);
	mpz_inp_str (N_sk, file, 10);
		
	fclose(file);
}

void clear_sk() {
	mpz_clear(N_sk);
	mpz_clear(mu);
	mpz_clear(lambda);
}

void print_sk() {
	gmp_printf ("%Zd\n", lambda);
	gmp_printf ("%Zd\n", mu);
	gmp_printf ("%Zd\n", N_sk);
}