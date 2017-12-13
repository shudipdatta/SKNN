#include <iostream>
//#include "../Key/Public_Key.cc"
//#include "../Key/Private_Key.cc"
//#include "../Utility/Random_Generator.cc"
//#include "Paillier.cc"
#include "SM.cc"
#include "SBD.cc"
#include <gmpxx.h>
#include <cmath>

using namespace std;

void SMINn(mpz_t **dmin, mpz_t **di, size_t n, mpz_t g, mpz_t N, mpz_t mu, mpz_t lambda) {
	size_t num;
	num = n;
	int lg_ceil = (int)ceil(log2(n));
	
	for(int i=0; i<lg_ceil; i++) {
		for(int j=0; j<
	}
}

int main (int argc, char *argv[])
{
	init_pk();
	init_sk();
	
	read_pk();
	read_sk();
	
	size_t n = 3; //number of input
	size_t m = 5; // = mpz_sizeinbase(u, 2);
	
	mpz_t **di;
	di = new mpz_t*[n];
	for(int i=0; i<n; i++) {
		di[i] = new mpz_t[m];
		
		for(int j=0; j<m; j++) {
			mpz_init2(di[i][j], 10);
		}
	}

	mpz_t *inp, *e_inp;
	inp = new mpz_t[n];
	e_inp = new mpz_t[n];
	for(int i=0; i<n; i++) {
		mpz_init2(inp[i], 10);
		mpz_init2(e_inp[i], 10);
		encrypt(e_inp[i], inp[i], g, N_pk);
	}
	
	mpz_init_set_str(inp[0], "10", 10);
	mpz_init_set_str(inp[1], "15", 10);
	mpz_init_set_str(inp[2], "8", 10);
	
	for(int i=0; i<n; i++) {
		SBD(&di[i], e_inp[i], m, g, N_pk, mu, lambda);
	}
	
	mpz_t *dmin;
	dmin= new mpz_t[m];
	for(int j=0; j<m; j++) {
		mpz_init2(dmin[j], 10);
	}
	SMINn(&dmin,  di, n, g, N_sk, mu, lambda);
	
	//testing
	for(int i=0; i<m; i++) {
		decrypt(dmin[i], dmin[i], N_sk, mu, lambda);
		gmp_printf ("i & x: %d & %Zd\n", i, dmin[i]);
	}
		
	clear_pk();
	clear_sk();
	
	return 0;	
}