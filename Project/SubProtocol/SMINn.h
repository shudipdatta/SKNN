#ifndef SMINn_H_
#define SMINn_H_

#include <iostream>
#include "../Key/Public_Key.h"
#include "../Key/Private_Key.h"
#include "../Utility/Random_Generator.h"
#include "Paillier.h"
#include "SM.h"
#include "SBD.h"
#include "SMIN.h"
#include <gmpxx.h>
#include <cmath>

using namespace std;

void SMINn(mpz_t **dmin, mpz_t **di, size_t n, size_t m, mpz_t g, mpz_t N, mpz_t mu, mpz_t lambda);

void SMINn_tester();

#endif
