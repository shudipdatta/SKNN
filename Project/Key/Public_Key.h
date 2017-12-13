#ifndef PUBLIC_KEY_H_
#define PUBLIC_KEY_H_

#include <iostream>
#include <time.h>
#include <gmpxx.h>

using namespace std;

extern mpz_t  g, N_pk;

void init_pk();

void read_pk ();

void clear_pk();

void print_pk();

#endif // PUBLIC_KEY_H_
