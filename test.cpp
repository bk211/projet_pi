
#include <iostream>
#include <gmpxx.h>
#include <cmath>
using namespace std;

mpf_class BBP(int k){
    mpf_class z;
    z = 8*k;
    mpz_class p;
    mpz_ui_pow_ui(p.get_mpz_t(), 16 ,k);
    //cout<<"z = "<< z<<endl;
    //cout<<"p = "<< p<<endl;
/*  
    mpf_class a, b, c, d, e;
    a = 1 / pow(16,k);
    cout<<"a = "<< a<<endl;
    b = 4 / (z+1);
    cout<<"b = "<< b<<endl;
    c = 2 / (z+4);
    cout<<"c = "<< c<<endl;
    d = 1 / (z+5);    
    cout<<"d = "<< d<<endl;
    e = 1 / (z+6);
    cout<<"e = "<< e<<endl;
    return a * (b - c - d -e);
*/  

    return (4/(z+1) - 2/(z+4) - 1/(z+5) - 1/(z+6)) / p ;
}


int main(int argc, char *argv[]){
    mpf_class a;
    for(int i; i <20; ++i){
        a = BBP(i);
        cout << ">>"<<a<<endl;
      //  result += a;
    }


    //cout<<"result ="<<result<<endl;
    return 0;
}