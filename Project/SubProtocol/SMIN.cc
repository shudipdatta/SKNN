#include <iostream>
//#include "../Key/Public_Key.cc"
//#include "../Key/Private_Key.cc"
//#include "../Utility/Random_Generator.cc"
//#include "Paillier.cc"
#include "SM.cc"
#include "SBD.cc"
#include <gmpxx.h>

using namespace std;


void smin_step_1(mpz_t** L , mpz_t**  Gam, mpz_t** r, mpz_t f, size_t m, mpz_t* e_ui, mpz_t* e_vi, mpz_t g, mpz_t N, mpz_t mu, mpz_t lambda) {
	mpz_t e_uvi, H, G, phi, W, N_, N__, N_sqr, _one, temp;
	mpz_init(e_uvi);
	mpz_init(G);
	mpz_init(phi);
	mpz_init(W);
	mpz_init(temp);
	
	mpz_t *l_L, *l_Gam, *l_r;
	l_L = new mpz_t[m];
	l_Gam = new mpz_t[m];
	l_r = new mpz_t[m];
	for(int i=0; i<m; i++) {
			mpz_init2(l_L[i], 10);
			mpz_init2(l_Gam[i], 10);
			mpz_init2(l_r[i], 10);
	}
	
	mpz_init_set_str(H, "0", 10);
	encrypt(H, H, g, N);
	
	mpz_init_set_str(_one, "-1", 10);
	encrypt(_one, _one, g, N);
	
	mpz_init_set_str(N_, "1", 10);	
	mpz_sub(N_, N, N_);
	mpz_init_set_str(N__, "2", 10);	
	mpz_sub(N__, N, N__);
	
	mpz_init(N_sqr);
	mpz_mul(N_sqr, N, N);
	
		
		
	for(int i=0; i<m; i++) {
		secure_multiplication(e_uvi, e_ui[i], e_vi[i], g, N, mu, lambda);
		
		if(mpz_odd_p(f)) {
			mpz_powm(temp, e_uvi, N_, N_sqr);                                                                                                  
			mpz_mul(W, e_ui[i], temp);
			
			generate_randomm(l_r[i], N);
			mpz_powm(temp, e_ui[i], N_, N_sqr);
			mpz_mul(l_Gam[i], e_vi[i], temp);
			encrypt(temp, l_r[i], g, N);
			mpz_mul(l_Gam[i], l_Gam[i], temp);
		}
		else {
			mpz_powm(temp, e_uvi, N_, N_sqr);
			mpz_mul(W, e_vi[i], temp);
			
			generate_randomm(l_r[i], N);
			mpz_powm(temp, e_vi[i], N_, N_sqr);
			mpz_mul(l_Gam[i], e_ui[i], temp);
			encrypt(temp, l_r[i], g, N);
			mpz_mul(l_Gam[i], l_Gam[i], temp);
		}
		
		//decrypt(temp, W, N, mu, lambda);
		//gmp_printf ("i & W: %d & %Zd\n", i, temp);
		
		//calculating G
		mpz_powm(temp, e_uvi, N__, N_sqr);
		mpz_mul(temp, temp, e_ui[i]);
		mpz_mul(G, temp, e_vi[i]);
		
		
		//decrypt(temp, G, N, mu, lambda);
		//gmp_printf ("i & G: %d & %Zd\n", i, temp);
		
		//calculating H
		generate_randomm(temp, N);
		mpz_powm(temp, H, temp, N_sqr);
		mpz_mul(H, temp, G);
		
		//decrypt(temp, H, N, mu, lambda);
		//gmp_printf ("i & H: %d & %Zd\n", i, temp);
		
		//calculating phi
		mpz_mul(phi, _one, H);
		
		//decrypt(temp, phi, N, mu, lambda);
		//gmp_printf ("i & phi: %d & %Zd\n", i, temp);
		
		//calculating L
		generate_randomm(temp, N);
		mpz_powm(temp, phi, temp, N_sqr);
		mpz_mul(l_L[i], W, temp);
		
		//decrypt(temp, l_L[i], N, mu, lambda);
		//gmp_printf ("i & L: %d & %Zd\n", i, temp);
	}
	
	*L = l_L;
	*Gam = l_Gam;
	*r = l_r;
}

void smin_step_2(mpz_t** M , mpz_t e_alp, size_t m, mpz_t* Gam, mpz_t* L, mpz_t g,  mpz_t N, mpz_t mu, mpz_t lambda) {
	mpz_t temp, one, N_sqr;
	mpz_init(temp);
	mpz_init_set_str(e_alp, "0", 10);
	mpz_init_set_str(one, "1", 10);

	mpz_init(N_sqr);
	mpz_mul(N_sqr, N, N);
	
	mpz_t *l_M;
	l_M = new mpz_t[m];
	for(int i=0; i<m; i++) {
			mpz_init2(l_M[i], 10);
	}
	
	for(int i=0; i<m; i++) {
		decrypt(temp, L[i], N, mu, lambda);
		if(!mpz_cmp(temp,one))  {
			mpz_init_set_str(e_alp, "1", 10);
			break;
		}
	}
	
	for(int i=0; i<m; i++) {
		mpz_powm(l_M[i], Gam[i], e_alp, N_sqr);
		
		//decrypt(temp, l_M[i], N, mu, lambda);
		//gmp_printf("gamma: %Zd\n", temp);
	}
	//gmp_printf("alpha: %Zd\n", e_alp);
	encrypt(e_alp, e_alp, g, N);
	
	*M = l_M;
}

void smin_step_3(mpz_t** e_mini , mpz_t*  M, mpz_t* r, mpz_t e_alp, size_t m, mpz_t f, mpz_t* e_ui, mpz_t* e_vi, mpz_t N) {
	mpz_t temp, lamb, N_sqr;
	mpz_init(temp);
	mpz_init(lamb);
	mpz_init(N_sqr);
	mpz_mul(N_sqr, N, N);
	
	mpz_t *le_mini;
	le_mini = new mpz_t[m];
	for(int i=0; i<m; i++) {
			mpz_init2(le_mini[i], 10);
	}
	
	for(int i=0; i<m; i++) {
		mpz_sub(temp, N, r[i]);
		mpz_powm(temp, e_alp, temp, N_sqr);
		mpz_mul(lamb, M[i], temp);
	
		//decrypt(temp, lamb, N, mu, lambda);
		//gmp_printf("lamb: %Zd\n", temp);
		//gmp_printf("f: %Zd\n", f);
		if(mpz_odd_p(f)) {
			mpz_mul(le_mini[i], e_ui[i], lamb);
			
			
		//decrypt(temp, le_mini[i], N, mu, lambda);
		//gmp_printf("checking : %Zd\n", temp);
		}
		else {
			mpz_mul(le_mini[i], e_vi[i], lamb);
			
			//decrypt(temp, e_vi[i], N, mu, lambda);
			//gmp_printf("e_vi: %Zd\n", temp);
		}
	}
	
	*e_mini = le_mini;
}


int main (int argc, char *argv[])
{
	init_pk();
	init_sk();
	
	read_pk();
	read_sk();
	
	mpz_t e_u, e_v, u, v;
	mpz_init(e_u);
	mpz_init(e_v);
	mpz_init_set_str(u, "10", 10);
	mpz_init_set_str(v, "8", 10);
	encrypt(e_u, u, g, N_pk);
	encrypt(e_v, v, g, N_pk);
	
	size_t m = 5; // = mpz_sizeinbase(u, 2);
	
	mpz_t *e_ui, *e_vi, *L, *Gam, *r, *M, *e_mini;
	e_ui = new mpz_t[m];
	e_vi = new mpz_t[m];
	
	L = new mpz_t[m];
	Gam = new mpz_t[m];
	r = new mpz_t[m];
	M = new mpz_t[m];
	e_mini = new mpz_t[m];
	
	for(int i=0; i<m; i++) {
			mpz_init2(e_ui[i], 10);
			mpz_init2(e_vi[i], 10);
			
			mpz_init2(L[i], 10);
			mpz_init2(Gam[i], 10);
			mpz_init2(r[i], 10);
			mpz_init2(M[i], 10);
			mpz_init2(e_mini[i], 10);
	}
	
	SBD(&e_ui, e_u, m, g, N_pk, mu, lambda);
	SBD(&e_vi, e_v, m, g, N_pk, mu, lambda);
	
	mpz_t f, two, e_alp;
	mpz_init(f);
	mpz_init(e_alp);
	mpz_init_set_str(two, "2", 10);	
	generate_randomm(f, two); gmp_printf("f: %Zd\n", f);
	
	
	smin_step_1(&L , &Gam, &r, f, m, e_ui, e_vi, g, N_sk, mu, lambda);
	smin_step_2(&M , e_alp, m, Gam, L, g,  N_sk, mu, lambda);
	smin_step_3(&e_mini , M, r, e_alp, m, f, e_ui, e_vi, N_sk);
	
	//testing
	for(int i=0; i<m; i++) {
		decrypt(e_mini[i], e_mini[i], N_sk, mu, lambda);
		gmp_printf ("i & x: %d & %Zd\n", i, e_mini[i]);
	}
	
	clear_pk();
	clear_sk();
	
	return 0;	
}
