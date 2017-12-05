#include <iostream>
#include "../Utility/Random_Generator.cc"
#include <gmpxx.h>

using namespace std;

mpz_t one;
mpz_t N, N_sqr;

//file pointer variables
FILE* pk_file;
FILE* sk_file;

//input variables
mp_bitcnt_t k;
mpz_t *u, *v;

//output variables
mpz_t lambda, g, mu;
mpz_t p, q;

/*
*generate a  random prime number with bit length 'bit_len' [=k/2]. 
*/
void generate_prime(mpz_t prime, const mp_bitcnt_t bit_len) {
	mpz_t rand;
	mpz_init(rand);
	generate_randomb(rand, bit_len);

	mpz_setbit(rand, bit_len-1);
	mpz_nextprime(prime, rand);
	mpz_clear(rand);
}

/*
*generate 'g' and 'mu' value
*/
void generate_g_mu(const mp_bitcnt_t bit_len) {
	mpz_t temp, gcd;
	mpz_init(temp);
	mpz_init(gcd);
	
	// generating a random value of 'g' with bit lenght 'bit_len'
	// such that, GCD(N, (g^lambda mod N^2  -  1) /  N) = 1
	do{
		generate_randomb(g, bit_len);
		mpz_powm(temp, g, lambda, N_sqr);
		mpz_sub(temp, temp, one);
		mpz_cdiv_q(temp, temp, N);

		mpz_gcd(gcd, N, temp);
		
	}while(mpz_cmp(gcd, one)); // repeating untill the above condition met
	
	mpz_invert(mu, temp, N); // calculating mu = ((g^lambda mod N^2  -  1) /  N)^-1 mod N
	
	mpz_clear(temp);
	mpz_clear(gcd);
}

/*
*generating all the required values for public and private 
*keys (p, q, N, N_sqr, g, mu, lambda)
*/
void generate_key(const mp_bitcnt_t bit_len) {

	//generate two prime numbers p & q with exactly k/2 bit length
	generate_prime(p, bit_len/2);
	generate_prime(q, bit_len/2);

	//generating N = p*q and N^2
	mpz_mul(N, p, q);
	mpz_mul(N_sqr, N, N);

	//generating lambda = lcm((p-1),(q-1))
	mpz_t p_minus_1, q_minus_1;
	mpz_init_set_str(p_minus_1, "1", 10);
	mpz_init_set_str(q_minus_1, "1", 10);

	mpz_sub(p_minus_1, p, p_minus_1);
	mpz_sub(q_minus_1, q, q_minus_1);
	mpz_lcm(lambda, p_minus_1, q_minus_1);

	//calling function to generate g and mu
	generate_g_mu(bit_len);

	mpz_clear(p_minus_1);
	mpz_clear(q_minus_1);
	
	//printing public keys (p, q, g)
	mpz_out_str(pk_file, 10, g);
	fprintf(pk_file,"\n");
	mpz_out_str(pk_file, 10, N);
	fprintf(pk_file,"\n\n\\\\ g, N");
	fclose(pk_file);
	
	//printing private keys (lambda and mu)
	mpz_out_str(sk_file, 10, lambda);
	fprintf(sk_file,"\n");
	mpz_out_str(sk_file, 10, mu);
	fprintf(sk_file,"\n");
	mpz_out_str(sk_file, 10, N);
	fprintf(sk_file,"\n\n\\\\ lambda, mu, N");
	fclose(sk_file);
}

/*
*initializizing valriables
*/
void init_keys() {
	mpz_init(N);
	mpz_init(N_sqr);
	mpz_init(lambda);
	mpz_init(g);
	mpz_init(mu);
	mpz_init(p);
	mpz_init(q);
	mpz_init_set_str(one, "1", 10);
}
/*
*clearing valriables
*/
void clear_keys() {
	mpz_clear(N);
	mpz_clear(N_sqr);
	mpz_clear(lambda);
	mpz_clear(g);
	mpz_clear(mu);
	mpz_clear(p);
	mpz_clear(q);
	mpz_clear(one);
}

/*
*main function of this project. It takes the input from terminal.
*/
int main (int argc, char *argv[])
{
	for(int i = 0; i<argc; i++){
		if(strcmp(argv[i], "-k")==0) {
			k=atoi(argv[i+1]);
		}else if(strcmp(argv[i], "-pk")==0) {
			if(!(pk_file = fopen(argv[i+1], "w"))) {
				fputs("not possible to open pk file!\n", stderr);
				exit(1);
			}
		}
		else if(strcmp(argv[i], "-sk")==0) {
			if(!(sk_file = fopen(argv[i+1], "w"))) {
				fputs("not possible to open sk file!\n", stderr);
				exit(1);
			}
		}
	}
	
	init_keys();
	generate_key(k); //generating key
	clear_keys();
	
	return 0;
	
}
