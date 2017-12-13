#ifndef PRIVATE_KEY_H_
#define PRIVATE_KEY_H_

#include <iostream>
#include <time.h>
#include <gmpxx.h>

using namespace std;

extern mpz_t lambda, N_sk, mu;

void init_sk();

void read_sk ();

void clear_sk();

void print_sk();

#endif // PRIVATE_KEY_H_
