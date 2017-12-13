#include "SMINn.h"

void SMINn(mpz_t **dmin, mpz_t **di, size_t n, size_t m, mpz_t g, mpz_t N, mpz_t mu, mpz_t lambda) {
	size_t num;
	num = n;
	int lg_ceil = (int)ceil(log2(n));
	
	for(int i=1; i<=lg_ceil; i++) {
		for(int j=1; j<=num/2; j++) {
			if(i==1) {
				//di[2*j-2] = SMIN(di[2*j-2], di[2*j-1]);
				//di[2*j-1] = 0;
				SMIN(&di[2*j-2], m, di[2*j-2], di[2*j-1], g, N_sk, mu, lambda);
				//for(int k=0; k<m; k++)
				//	mpz_init_set_str(di[2*j-1][k], "0", 10);
			}
			else {
				//di[2*i*(j-1)] = SMIN(di[2*i*(j-1)], di[2*i*j-2]);
				//di[2*i*j-2] = 0;
				SMIN(&di[2*i*(j-1)], m, di[2*i*(j-1)], di[2*i*j-2], g, N_sk, mu, lambda);
				//for(int k=0; k<m; k++)
				//	mpz_init_set_str(di[2*i*j-2][k], "0", 10);
			}
		}
		num = (int)ceil(num/2.0);
	}
	*dmin = di[0];
}

void SMINn_tester() {
	init_pk();
	init_sk();
	
	read_pk();
	read_sk();
	
	
	FILE* file;
	if(!(file = fopen("SubProtocol/SMINn", "r"))) {
		fputs("not possible to open file!\n", stderr);
		exit(1);
	}
	
	size_t n = 0; //number of input
	size_t m = 0; //number of bit
	
	mpz_t temp;
	mpz_init(temp);
	while(mpz_inp_str (temp, file, 10)) {
		++n;
	}
	fclose(file);
	
	mpz_t *inp, *e_inp;
	inp = new mpz_t[n];
	e_inp = new mpz_t[n];
	
	if(!(file = fopen("SubProtocol/SMINn", "r"))) {
		fputs("not possible to open file!\n", stderr);
		exit(1);
	}
	for(int i=0; i<n; i++) {
		mpz_init2(inp[i], 10);
		mpz_inp_str (inp[i], file, 10);
		if(mpz_sizeinbase(inp[i], 2) > m){
			m = mpz_sizeinbase(inp[i], 2);
		}
		mpz_init2(e_inp[i], 10);
		encrypt(e_inp[i], inp[i], g, N_pk);	
	}	
	fclose(file);

	mpz_t **di;
	di = new mpz_t*[n];
	for(int i=0; i<n; i++) {
		di[i] = new mpz_t[m];
		
		for(int j=0; j<m; j++) {
			mpz_init2(di[i][j], 10);
		}
	}
/*
	mpz_t *inp, *e_inp;
	inp = new mpz_t[n];
	e_inp = new mpz_t[n];
	
	for(int i=0; i<n; i++) {
		mpz_init2(inp[i], 10);
	}	
	mpz_init_set_str(inp[0], "18", 10);
	mpz_init_set_str(inp[1], "15", 10);
	mpz_init_set_str(inp[2], "14", 10);
	
	for(int i=0; i<n; i++) {
		mpz_init2(e_inp[i], 10);
		encrypt(e_inp[i], inp[i], g, N_pk);	
	}
*/	
	for(int i=0; i<n; i++) {
		SBD(&di[i], e_inp[i], m, g, N_pk, mu, lambda);
	}
	
	mpz_t *dmin;
	dmin= new mpz_t[m];
	for(int j=0; j<m; j++) {
		mpz_init2(dmin[j], 10);
	}
	SMINn(&dmin, di, n, m, g, N_sk, mu, lambda);
	
	//testing
	for(int i=0; i<m; i++) {
		decrypt(dmin[i], dmin[i], N_sk, mu, lambda);
		gmp_printf ("%Zd\n", dmin[i]);
	}
		
	clear_pk();
	clear_sk();
}
