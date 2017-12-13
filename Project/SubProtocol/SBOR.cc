#include "SBOR.h"

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

void SBOR_tester() {
	init_pk();
	init_sk();
	
	read_pk();
	read_sk();
	
	
	FILE* file;
	if(!(file = fopen("SubProtocol/SBOR", "r"))) {
		fputs("not possible to open file!\n", stderr);
		exit(1);
	}
	mpz_t e_a, e_b, e_aorb;
	mpz_init(e_aorb);
	mpz_init(e_a);
	mpz_init(e_b);
	mpz_inp_str (e_a, file, 10);
	mpz_inp_str (e_b, file, 10);
	encrypt(e_a, e_a, g, N_pk);
	encrypt(e_b, e_b, g, N_pk);
	
	secure_bitor (e_aorb, e_a, e_b, g, N_pk, mu, lambda);
	decrypt(e_aorb, e_aorb, N_sk, mu, lambda);
	gmp_printf("a or b: %Zd\n", e_aorb);
	
	fclose(file);
	
	clear_pk();
	clear_sk();
}
