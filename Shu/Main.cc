#include <iostream>
#include <time.h>
#include <gmpxx.h>

using namespace std;

void read_file(mpz_t* inp, FILE* file) {
	mpz_t size_, val;
	mpz_init(size_);
	mpz_init(val);
	
	int size=1;
	while(mpz_inp_str (size_, file, 10)) {
		size++;
	}
/*
	inp[size];
	for(int i=0; i<size; i++) {
		mpz_inp_str (val, file, 10);
		mpz_init2(inp[i], 10);
		mpz_set(inp[i], val);
	}
*/
}


int main (int argc, char *argv[])
{
	mp_bitcnt_t k;
	FILE* u_file;
	FILE* v_file;
	FILE* pk_file;
	FILE* sk_file;
	FILE* eu_file;
	FILE* ev_file;
	FILE* sdp_file;

	
	mpz_t* u, v;
	/*
	if(argc != 8){
		printf("need 8 parameters");
		exit(0);
	}
	*/
	for(int i = 0; i<argc; i++){
		if(strcmp(argv[i], "-k")==0) {
			k=atoi(argv[i+1]);
		}else if(strcmp(argv[i], "-u")==0) {
			if(!(u_file = fopen(argv[i+1], "r"))) {
				fputs("not possible to open U file!\n", stderr);
				exit(1);
			}
			read_file(u, u_file);
			fclose(u_file);
			//printf("u = %s", argv[i+1]);
		}else if(strcmp(argv[i], "-v")==0) {
			//printf("u = %s", argv[i+1]);
			if(!(v_file = fopen(argv[i+1], "r"))) {
				fputs("not possible to open v file!\n", stderr);
				exit(1);
			}
		}else if(strcmp(argv[i], "-pk")==0) {
			//printf("u = %s", argv[i+1]);
			if(!(pk_file = fopen(argv[i+1], "w"))) {
				fputs("not possible to open pk file!\n", stderr);
				exit(1);
			}
		}else if(strcmp(argv[i], "-sk")==0) {
			//printf("u = %s", argv[i+1]);
			if(!(sk_file = fopen(argv[i+1], "w"))) {
				fputs("not possible to open sk file!\n", stderr);
				exit(1);
			}
		}else if(strcmp(argv[i], "-eu")==0) {
			//printf("u = %s", argv[i+1]);
			if(!(eu_file = fopen(argv[i+1], "w"))) {
				fputs("not possible to open eu file!\n", stderr);
				exit(1);
			}
		}else if(strcmp(argv[i], "-ev")==0) {
			//printf("u = %s", argv[i+1]);
			if(!(ev_file = fopen(argv[i+1], "w"))) {
				fputs("not possible to open ev file!\n", stderr);
				exit(1);
			}
		}else if(strcmp(argv[i], "-sdp")==0) {
			//printf("u = %s", argv[i+1]);
			if(!(sdp_file = fopen(argv[i+1], "w"))) {
				fputs("not possible to open sdp file!\n", stderr);
				exit(1);
			}
		}
	}

	return 0;
}
