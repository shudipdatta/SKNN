#include <iostream>
//#include "../Key/Public_Key.cc"
//#include "../Key/Private_Key.cc"
//#include "../Utility/Random_Generator.cc"
//#include "Paillier.cc"
#include "SM.cc"
#include <gmpxx.h>

using namespace std;

void smin_step_1(mpz_t** e_uv, mpz_t* e_u, mpz_t* e_v, int size, mpz_t g, mpz_t N) {

}

void smin_step_2(mpz_t ** e_uv2 , mpz_t*  e_uv, int size, mpz_t g,  mpz_t N, mpz_t mu, mpz_t lambda) {

}

void smin_step_3(mpz_t ssed , mpz_t*  e_uv2, int size) {

}


int main (int argc, char *argv[])
{
	init_pk();
	init_sk();
	
	read_pk();
	read_sk();
	
	
	
	clear_pk();
	clear_sk();
	
	return 0;	
}
