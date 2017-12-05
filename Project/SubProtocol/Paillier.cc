#include <iostream>
#include "../Utility/Random_Generator.cc"
#include <gmpxx.h>

using namespace std;

/*
*encrypting 'msg' into 'cipher' using paillier homomorphic encryption
*/
void encrypt(mpz_t cipher, mpz_t msg, mpz_t g, mpz_t N) {
	mpz_t r, a, b, N_sqr;
	
	mpz_init(r);
	mpz_init(a);
	mpz_init(b);
	
	mpz_init(N_sqr);
	mpz_mul(N_sqr, N, N);

	//generating a random number r such that 0<= r <N
	generate_randomm(r, N);
	//calculating g^m mod N^2
	mpz_powm(a, g, msg, N_sqr);
	//calculating r^N mod N^2
	mpz_powm(b, r, N, N_sqr);
	//calculating cipher =  (g^m * r^N) mod N^2
	mpz_mul(cipher, a, b);
	mpz_mod(cipher, cipher, N_sqr);

	mpz_clear(r);
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(N_sqr);
}
/*
*decrypting 'cipher' into 'msg' using paillier homomorphic decryption
*/
void decrypt(mpz_t msg, mpz_t cipher, mpz_t N, mpz_t mu, mpz_t lambda) {
	mpz_t N_sqr, one;
	mpz_init(N_sqr);
	mpz_mul(N_sqr, N, N);
	mpz_init_set_str(one, "1", 10);
	
	//calculating x = ((c^lambda mod N^2) - 1) / N
	mpz_powm(msg, cipher, lambda, N_sqr);
	mpz_sub(msg, msg, one);
	mpz_cdiv_q(msg, msg, N);
	
	//calculating m = x *mu mod N
	mpz_mul(msg, msg, mu);
	mpz_mod(msg, msg, N);
	
	mpz_clear(N_sqr);
	mpz_clear(one);
}


