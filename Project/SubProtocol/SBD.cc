#include <iostream>
#include "../Key/Public_Key.cc"
#include "../Key/Private_Key.cc"
//#include "../Utility/Random_Generator.cc"
#include "Paillier.cc"
#include <gmpxx.h>

using namespace std;

void Binary(mpz_t **e_xi, mpz_t x, size_t m) {
	mpz_t one, two;	
	mpz_init_set_str(one, "1", 10);
	mpz_init_set_str(two, "2", 10);
	mpz_t* le_xi;
	le_xi = new mpz_t[m];
	int i=0;
	while(i!=m) {
		//gmp_printf ("i & x: %d & %Zd\n", i, x);
		mpz_init2(le_xi[i], 10);
		mpz_mod(le_xi[i], x, two);
		if(mpz_odd_p(x)) {
			mpz_sub(x, x, one);
		}
		mpz_cdiv_q(x, x, two);
		i = i+1;
	}
	*e_xi = le_xi;
}

void Encrypted_LSB(mpz_t e_xi, mpz_t T, mpz_t g, mpz_t N) {
	
	mpz_t r, Y, y, N_sqr, temp, alp, zero, one, N_;
	mpz_init(r);
	mpz_init(Y);
	mpz_init(y);
	mpz_init(alp);
	mpz_init(temp);
	
	mpz_init(N_sqr);
	mpz_mul(N_sqr, N, N);
	
	mpz_init_set_str(zero, "0", 10);
	mpz_init_set_str(one, "1", 10);
	
	mpz_init_set_str(N_, "1", 10);	
	mpz_sub(N_, N, N_);
	
	//step 1
	generate_randomm(r, N);
	encrypt(temp, r, g, N);
	mpz_mul(Y, T, temp);
	mpz_mod(Y, Y, N_sqr);
	
	//step 2
	decrypt(y, Y, N, mu, lambda);
	if(mpz_even_p(y)) {
		encrypt(alp, zero, g, N);
	}
	else{
		encrypt(alp, one, g, N);
	}
	
	//step 3
	if(mpz_even_p(r)) {
		mpz_set(e_xi, alp);
	}
	else {
		mpz_powm(alp, alp, N_, N_sqr);
		encrypt(temp, one, g, N);
		mpz_mul(e_xi, temp, alp);
	}
}

void SVR(mpz_t lamb, mpz_t e_x, mpz_t *e_xi,  size_t m, mpz_t g, mpz_t N, mpz_t mu, mpz_t lambda) {
	mpz_t temp, two, N_sqr, U, V, W, r_, N_;
	mpz_init(temp);
	mpz_init(V);
	mpz_init(W);
	mpz_init(r_);
	
	mpz_init(N_sqr);
	mpz_mul(N_sqr, N, N);
	mpz_init_set_str(N_, "1", 10);	
	mpz_sub(N_, N, N_);
	mpz_init(U);
	mpz_init_set_str(U, "1", 10);	
	mpz_init_set_str(two, "2", 10);
	
	//step 1
	for(int i=0; i<m; i++) {
		mpz_set_ui(temp, i);
		mpz_powm(temp, two, temp, N); //gmp_printf ("i & x: %d & %Zd\n", i, e_xi[i]);
		mpz_powm(temp, e_xi[i], temp, N_sqr);
		mpz_mul(U, U, temp);
	}
	mpz_powm(temp, e_x, N_, N_sqr);
	mpz_mul(V, U, temp);
	
	generate_randomm(r_, N);
	mpz_powm(W, V, r_, N_sqr);
	
	//step 2
	decrypt(temp, W, N, mu, lambda);
	if(!temp) {
		mpz_init_set_str (lamb, "0", 10);
	}
	else {
		mpz_init_set_str (lamb, "1", 10);
	}
}

void SBD(mpz_t** e_xi, mpz_t e_x, size_t m, mpz_t g, mpz_t N, mpz_t mu, mpz_t lambda) {	
	
	mpz_t T, l, two, temp, Z, lamb;
	mpz_init(T);
	mpz_init(l);
	mpz_init(Z);
	mpz_init(lamb);
	mpz_init(temp);
	mpz_init_set_str(two, "2", 10);
	
	mpz_t* le_xi;
	le_xi = new mpz_t[m];
	
	mpz_t N_, N_sqr;
	mpz_init_set_str(N_, "1", 10);	
	mpz_sub(N_, N, N_);
	mpz_init(N_sqr);
	mpz_mul(N_sqr, N, N);
	
	do{
		mpz_invert(l, two, N);
		mpz_set(T, e_x);
		for(int i=0; i<m; i++) {
			mpz_init2(le_xi[i], 10);
			Encrypted_LSB(le_xi[i], T, g, N); 
			mpz_powm(temp, le_xi[i], N_, N_sqr);
			mpz_mul(Z, T, temp);
			mpz_powm(T, Z, l, N_sqr);
		}
		SVR(lamb, e_x, le_xi, m, g, N, mu, lambda);
	} while (!lamb);
	*e_xi = le_xi;
}

int main (int argc, char *argv[])
{
	init_pk();
	init_sk();
	
	read_pk();
	read_sk();
	
	mpz_t* e_xi;
	mpz_t e_x, x;
	mpz_init(x);
	mpz_init(e_x);
	mpz_init_set_str(x, "9", 10);
	encrypt(e_x, x, g, N_pk);
	
	size_t m = mpz_sizeinbase(x, 2);
	e_xi = new mpz_t[m];
	for(int i=0; i<m-1; i++) {
			mpz_init2(e_xi[i], 10);
	}
	/*
	Binary(&e_xi, x, m);
	for(int i=0; i<m; i++) {
		gmp_printf ("i & x: %d & %Zd\n", i, e_xi[i]);
		//encrypt(e_xi[i], e_xi[i], g, N_pk);
		//gmp_printf ("i & x: %d & %Zd\n", i, e_xi[i]);
	}
	*/
	SBD(&e_xi, e_x, m, g, N_pk, mu, lambda);
	///*
	//testing
	for(int i=0; i<m; i++) {
		//gmp_printf ("i & x: %d & %Zd\n", i, e_xi[i]);
		decrypt(e_xi[i], e_xi[i], N_sk, mu, lambda);
		gmp_printf ("i & x: %d & %Zd\n", i, e_xi[i]);
	}
	//*/
	clear_pk();
	clear_sk();
	
	return 0;
}