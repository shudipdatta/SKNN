#include <iostream>
#include <time.h>
#include <gmpxx.h>

using namespace std;

mpz_t  g, N_pk;

void init_pk() {
	mpz_init(N_pk);
	mpz_init(g);
}

void read_pk () {
	
	FILE* file;
	if(!(file = fopen("Key/pk_file", "r"))) {
		fputs("not possible to open file!\n", stderr);
		exit(1);
	}
	
	mpz_inp_str (g, file, 10);
	mpz_inp_str (N_pk, file, 10);
		
	fclose(file);
}

void clear_pk() {
	mpz_clear(N_pk);
	mpz_clear(g);
}

void print_pk() {
	gmp_printf ("%Zd\n", g);
	gmp_printf ("%Zd\n", N_pk);
}