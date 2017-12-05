#include <iostream>
#include <time.h>
#include <gmpxx.h>

using namespace std;

int vector_size;
mpz_t one;
mpz_t N, N_sqr;

//file pointer variables
FILE* pk_file;
FILE* sk_file;
FILE* eu_file;
FILE* ev_file;
FILE* sdp_file;

//input variables
mp_bitcnt_t k;
mpz_t *u, *v;

//output variables
mpz_t lambda, g, mu;
mpz_t *eu, *ev;
mpz_t euv, uv;
mpz_t p, q;

/*
*generate a random number with bit length 'bit_len'. 
*The random number 'rand' will hold a value in range [0, 2^(bit_len) - 1]
*/
void generate_randomb(mpz_t rand, const mp_bitcnt_t bit_len){
	unsigned long seed = clock();
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	gmp_randseed_ui(state, seed);
	mpz_urandomb(rand, state, bit_len);
	gmp_randclear(state);
}

/*
*generate a random number with value < 'N'. 
*The random number 'rand' will hold a value in range [0, N - 1]
*/
void generate_randomm(mpz_t rand){
	unsigned long seed = clock();
	gmp_randstate_t state;
	gmp_randinit_mt(state);
	gmp_randseed_ui(state, seed);
	mpz_urandomm(rand, state, N);
	gmp_randclear(state);
}

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
	fprintf(pk_file,"p = ");
	mpz_out_str(pk_file, 10, p);
	fprintf(pk_file,"\nq = ");
	mpz_out_str(pk_file, 10, q);
	fprintf(pk_file,"\ng = ");
	mpz_out_str(pk_file, 10, g);
	fclose(pk_file);
	
	//printing private keys (lambda and mu)
	fprintf(sk_file,"lambda = ");
	mpz_out_str(sk_file, 10, lambda);
	fprintf(sk_file,"\nmu = ");
	mpz_out_str(sk_file, 10, mu);
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
*encrypting 'msg' into 'cipher' using paillier homomorphic encryption
*/
void encrypt(mpz_t cipher, mpz_t msg) {
	mpz_t r, a, b;
	mpz_init(r);
	mpz_init(a);
	mpz_init(b);

	//generating a random number r such that 0<= r <N
	generate_randomm(r);
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
}
/*
*decrypting 'cipher' into 'msg' using paillier homomorphic decryption
*/
void decrypt(mpz_t msg, mpz_t cipher) {
	//calculating x = ((c^lambda mod N^2) - 1) / N
	mpz_powm(msg, cipher, lambda, N_sqr);
	mpz_sub(msg, msg, one);
	mpz_cdiv_q(msg, msg, N);
	
	//calculating m = x *mu mod N
	mpz_mul(msg, msg, mu);
	mpz_mod(msg, msg, N);
}

/*
*testing homomorphic addition
*/
void check_add_homo(mpz_t msg3, mpz_t msg1, mpz_t msg2) {
	encrypt(msg1, msg1);
	encrypt(msg2, msg2);

	mpz_mul(msg3, msg1, msg2);
	decrypt(msg3, msg3);	
}
/*
*testing homomorphic multiplication
*/
void check_mul_homo(mpz_t msg3, mpz_t msg1, mpz_t msg2) {
	encrypt(msg1, msg1);
	mpz_powm(msg3, msg1, msg2, N_sqr);
	decrypt(msg3, msg3);	
}

/*
*calculating secure dot product using paillier homomorphic cryptosystem
*/
void sdp(mpz_t esdp) {
	mpz_t temp, print;
	mpz_init(temp);
	mpz_init(print);
	
	for(int i=0; i<vector_size; i++) {	
		//calculating E(u_i)
		encrypt(temp, u[i]);
		//calculating E(u_i * v_i) = E(u_i) ^ v_i
		mpz_powm(temp, temp, v[i], N_sqr);	
		//multiplying E(u_i * v_i) to the privious esdp. 
		//note that, esdp is initialized to 1 when passed to the function 'sdp'
		mpz_mul(esdp, esdp, temp);
		
		//printing E(u)
		encrypt(print, u[i]);
		mpz_out_str(eu_file, 10, print);
		fprintf(eu_file,"\n");
		//printing E(v)
		encrypt(print, v[i]);
		mpz_out_str(ev_file, 10, print);
		fprintf(ev_file,"\n");
	}
	
	fclose(eu_file);
	fclose(ev_file);
}

/*
*after taking all the input value and file names, 
*this function will generate the key and calculate secure dot product
*/

void run_program() {
	init_keys();
	generate_key(k); //generating key
	mpz_t esdp;
	mpz_init_set_str (esdp, "1", 10);
	sdp(esdp); //calculating secure dot product
	
	//printing E(u.v) and u.v
	fprintf(sdp_file,"E(u.v) = ");
	mpz_out_str(sdp_file, 10, esdp);
	fprintf(sdp_file,"\nu.v = ");
	decrypt(esdp,esdp);
	mpz_out_str(sdp_file, 10, esdp);
	fclose(sdp_file);
	clear_keys();
}

/*
*reading from file 'file_name' to a vector 'inp'
*/
void read_file(mpz_t **inp, char file_name[]) {
	mpz_t size_, val, *local_inp;
	mpz_init(size_);
	mpz_init(val);
	
	FILE* file;
	if(!(file = fopen(file_name, "r"))) {
		fputs("not possible to open file!\n", stderr);
		exit(1);
	}
	vector_size=0;
	while(mpz_inp_str (size_, file, 10)) {
		++vector_size;
	}
	fclose(file);

	file = fopen(file_name, "r");
	local_inp = new mpz_t[vector_size];
	for(int i=0; i<vector_size; i++) {
		mpz_inp_str (val, file, 10);
		mpz_init2(local_inp[i], 10);
		mpz_set(local_inp[i], val);
	}
	fclose(file);
	
	mpz_clear(size_);
	mpz_clear(val);
	
	*inp = local_inp;
}

/*
*main function of this project. It takes the input from terminal.
*/
int main (int argc, char *argv[])
{
	
	/*
	*command line options:
	* -k : bitlength of N
	* -u : file name of u vector
	* -v : file name of v vector
	* -pk : file name of public key (p,q,g)
	* -sk : file name of private key (lambda, mu)
	* -eu : file name of E(u) vector
	* -ev : file name of E(v) vector
	* -sdp : file name of E(u.v) and (u.v)
	*/
	for(int i = 0; i<argc; i++){
		if(strcmp(argv[i], "-k")==0) {
			k=atoi(argv[i+1]);
		}else if(strcmp(argv[i], "-u")==0) {
			read_file(&u, argv[i+1]);
		}else if(strcmp(argv[i], "-v")==0) {
			read_file(&v, argv[i+1]);
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
		}else if(strcmp(argv[i], "-eu")==0) {
			if(!(eu_file = fopen(argv[i+1], "w"))) {
				fputs("not possible to open eu file!\n", stderr);
				exit(1);
			}
		}else if(strcmp(argv[i], "-ev")==0) {
			if(!(ev_file = fopen(argv[i+1], "w"))) {
				fputs("not possible to open ev file!\n", stderr);
				exit(1);
			}
		}else if(strcmp(argv[i], "-sdp")==0) {
			if(!(sdp_file = fopen(argv[i+1], "w"))) {
				fputs("not possible to open sdp file!\n", stderr);
				exit(1);
			}
		}
	}

	run_program();

	return 0;
}
