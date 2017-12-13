#include "SSED.h"

void ssed_step_1(mpz_t** e_uv, mpz_t* e_u, mpz_t* e_v, int size, mpz_t g, mpz_t N) {
	mpz_t N_, N_sqr;
	mpz_init_set_str(N_, "1", 10);	
	mpz_sub(N_, N, N_);
	
	mpz_init(N_sqr);
	mpz_mul(N_sqr, N, N);
	
	mpz_t* local_e_uv;
	local_e_uv = new mpz_t[size];
	for(int i=0; i<size; i++) {
		mpz_init2(local_e_uv[i], 10);
	}
	
	for(int i = 0; i < size; i++){
		mpz_powm(local_e_uv[i], e_v[i], N_, N_sqr);
		mpz_mul(local_e_uv[i], e_u[i], local_e_uv[i]);
	}
	*e_uv = local_e_uv;
	mpz_clear(N_);
}

void ssed_step_2(mpz_t ** e_uv2 , mpz_t*  e_uv, int size, mpz_t g,  mpz_t N, mpz_t mu, mpz_t lambda) {
	mpz_t* local_e_uv2;
	local_e_uv2 = new mpz_t[size];
	for(int i=0; i<size; i++) {
		mpz_init2(local_e_uv2[i], 10);
	}
	
	for(int i = 0; i<size; i++) {
		secure_multiplication(local_e_uv2[i], e_uv[i], e_uv[i], g, N, mu, lambda);
	}
	*e_uv2 = local_e_uv2;
}

void ssed_step_3(mpz_t ssed , mpz_t*  e_uv2, int size) {
	mpz_init_set_str(ssed, "1", 10); 
	for(int i = 0; i<size; i++){
		mpz_mul(ssed, ssed, e_uv2[i]);
	}
}


void SSED_tester()
{
	init_pk();
	init_sk();
	
	read_pk();
	read_sk();
	
	
	FILE* file;
	if(!(file = fopen("SubProtocol/SSED", "r"))) {
		fputs("not possible to open file!\n", stderr);
		exit(1);
	}	
	mpz_t ssed, *e_uv2, *e_uv, *e_u, *e_v;
	mpz_init(ssed);
	int size=0;	
	
	while(mpz_inp_str (ssed, file, 10)) {
		++size;
	}
	size /= 2;
	fclose(file);
	
	e_u = new mpz_t[size];
	e_v = new mpz_t[size];
	e_uv = new mpz_t[size];
	e_uv2 = new mpz_t[size];
	
	for(int i=0; i<size; i++) {
		mpz_init2(e_u[i], 10);
		mpz_init2(e_v[i], 10);
		mpz_init2(e_uv[i], 10);
		mpz_init2(e_uv2[i], 10);
	}
	
	if(!(file = fopen("SubProtocol/SSED", "r"))) {
		fputs("not possible to open file!\n", stderr);
		exit(1);
	}	
	
	for(int i=0; i<size; i++) {
		mpz_inp_str (e_u[i], file, 10);
		encrypt(e_u[i], e_u[i], g, N_sk);
	}
	for(int i=0; i<size; i++) {
		mpz_inp_str (e_v[i], file, 10);
		encrypt(e_v[i], e_v[i], g, N_sk);
	}
	
	ssed_step_1(&e_uv, e_u, e_v, size, g, N_pk);		
	ssed_step_2(&e_uv2, e_uv, size, g, N_sk, mu, lambda);
    ssed_step_3(ssed, e_uv2, size);
	decrypt(ssed, ssed, N_sk, mu, lambda);
	gmp_printf ("%Zd\n", ssed);
	
	fclose(file);
	
	clear_pk();
	clear_sk();
}
