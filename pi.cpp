
#include <iostream>
#include <gmpxx.h>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <cmath>
#include <cstdio>
using namespace std;

mpf_class BBP(int k){
    mpf_class z;
    z = 8*k;
    //mpf_class p,f;
    //mpf_pow_ui(p.get_mpf_t(), f.get_mpf_t() ,k);
    //cout<<"z = "<< z<<endl;
    //cout<<"p = "<< p<<endl;
  
    mpf_class a, b, c, d, e,x;
    a = 1 / pow(16,k);
    //cout<<"a = "<< a<<endl;
    b = 4 / (z+1);
    //cout<<"b = "<< b<<endl;
    c = 2 / (z+4);
    //cout<<"c = "<< c<<endl;
    d = 1 / (z+5);    
    //cout<<"d = "<< d<<endl;
    e = 1 / (z+6);
    //cout<<"e = "<< e<<endl;
    x = a * (b - c - d - e);
    return x; 
  

 //   return (4/(z+1) - 2/(z+4) - 1/(z+5) - 1/(z+6)) / p ;
}

void loop_BBP(int step, int begin, int kmax, mpf_class ret[]){
    mpf_class result;
    mpf_init(result.get_mpf_t());

    for (int i = begin; i <= kmax; i+= step)
    {
        result += BBP(i);
        //mpf_add(result.get_mpf_t(), result.get_mpf_t(), BBP(i).get_mpf_t());
    }

    ret[begin] = result;

}





int main(int argc, char *argv[]){
    if( argc != 3){
    cout << "Usage:./exec nb_thread precision" << endl;
    return 0;
    }
    int nb_thread = stoi(argv[1]);
    int precision = stoi(argv[2]);
    if(nb_thread<=0 || precision <=0){
        cout<<"nb_thread and precision must be over 0"<<endl;
        return 0;
    } 
    cout<<"nb_thread = "<< nb_thread<<endl;
    cout<<"precision = "<< precision <<endl;

    mpf_set_default_prec(precision * 4);

    auto t_start = chrono::steady_clock::now();
    vector<thread> v;
    mpf_class *tab = new mpf_class[nb_thread];
    FILE * result_file;

    for (auto i = 0; i < nb_thread; i++){
        v.push_back(thread(loop_BBP, nb_thread, i, precision, tab));
    }
    
    for ( auto& th : v ) {
        th.join();
    }
    
    for (int i = 1; i < nb_thread; i++){
        //cout<<tab[i]<<endl;
        mpf_add(tab[0].get_mpf_t(), tab[0].get_mpf_t(), tab[i].get_mpf_t());
    }

    //gmp_printf("result = %.*Ff \n", precision, tab[0].get_mpf_t());
    //gmp_printf("result = %.*Ff \n", precision*1.5, tab[0].get_mpf_t());
    //cout<<"result = "<<tab[0].get_mpf_t()<<endl;
    //gmp_printf("%Ff \n", tab[0].get_mpf_t());
    
    result_file = fopen("resultat","w");
    gmp_fprintf(result_file,"%.*Ff", precision, tab[0].get_mpf_t());
    fclose(result_file);

    auto t_end = chrono::steady_clock::now();
    auto t_diff = t_end - t_start;
    cout << "Time used: " << chrono::duration<double, milli> (t_diff).count()<<"ms"<< '\n';
    return 0;
}