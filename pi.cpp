
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
    mpf_class a, b, c, d, e, _8k;
    _8k = 8*k;
  
    a = 1 / pow(16,k);
    b = 4 / (_8k+1);
    c = 2 / (_8k+4);
    d = 1 / (_8k+5);    
    e = 1 / (_8k+6);
    
    return a * (b - c - d - e);
}

void loop_BBP(int step, int begin, int kmax, mpf_class ret[]){
    mpf_class result;
    mpf_init(result.get_mpf_t());

    for (int i = begin; i <= kmax; i+= step)
    {
        result += BBP(i);
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
        tab[0] += tab[i];
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