#include<iostream>
#include<gmp.h>

using namespace std;

int main (int argc, char **argv) {

    mpz_t a,b,c;
    mpz_inits(a,b,c,NULL);

    mpz_set_str(a, "1234", 10);
    mpz_set_str(b,"-5678", 10); //Decimal base

    mpz_add(c,a,b);

    cout<<"\nThe exact result is:";
    mpz_out_str(stdout, 10, c); //Stream, numerical base, var
    cout<<endl;

    mpz_abs(c, c);
    cout<<"The absolute value result is:";
    mpz_out_str(stdout, 10, c);
    cout<<endl;

    cin.get();

    return 0;
}
