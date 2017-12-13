#ifndef SBOR_H_
#define SBOR_H_

#include <iostream>
#include "../Key/Public_Key.h"
#include "../Key/Private_Key.h"
#include "../Utility/Random_Generator.h"
#include "Paillier.h"
#include "SM.h"
#include <gmpxx.h>

using namespace std;

void secure_bitor (mpz_t e_aorb, mpz_t e_a, mpz_t e_b, mpz_t g, mpz_t N, mpz_t mu, mpz_t lambda);

void SBOR_tester();

#endif
