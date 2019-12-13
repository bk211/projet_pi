#include "pi.hpp"

/*
    calcule la somme de la formule BBP au rang k
    https://fr.wikipedia.org/wiki/Formule_BBP
*/
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

/*  calcule la somme de la formule BBP jusqu'au rang k incluse avec un pas donné.
    @param step le pas utilisé à chaque iteration de boucle
    @param begin indice de depart / numéro de thread
    @param kmax le rang maximal que la fonction doit calculer
    @param ret le tableau de mpf_class qui stock les résultat, il y en a autant qu'il y a de thread
*/
void loop_BBP(int step, int begin, int kmax, mpf_class ret[]){
    mpf_class result;
    mpf_init(result.get_mpf_t());

    for (int i = begin; i <= kmax; i+= step){
        result += BBP(i);
    }
    ret[begin] = result;

}


int main(int argc, char *argv[]){
    if( argc != 3){//affichage usage
    cout << "Usage:./exec nb_thread precision" << endl;
    return 0;
    }
    
    //conversion des paramètres
    int nb_thread = stoi(argv[1]);
    int precision = stoi(argv[2]);
    if(nb_thread<=0 || precision <=0){//test pour éviter que nb_thread ou precision soit négatif ou nul
        cout<<"nb_thread and precision must be over 0"<<endl;
        return 0;
    }
    //affichages des paramètres données
    cout<<"nb_thread = "<< nb_thread<<endl;
    cout<<"precision = "<< precision <<endl;

    //définie la précision qu'on utiliseras pour toutes les mpf_class qui suivreras,
    // *4 car la precision est définit en "bit", le multiplier par 4 assure qu'on auras certainnement la précision supérieur
    mpf_set_default_prec(precision * 4);

    auto t_start = chrono::steady_clock::now();//début du chronomètre
    vector<thread> v;
    mpf_class *tab = new mpf_class[nb_thread];
    FILE * result_file;

    for (auto i = 0; i < nb_thread; i++){//lance les threads
        v.push_back(thread(loop_BBP, nb_thread, i, precision, tab));
    }
    
    for ( auto& th : v ) {//attends la fin de tous les threads
        th.join();
    }
    
    //additionne toutes les résultats obtenues par chaque thread dans la première case du tableau
    for (int i = 1; i < nb_thread; i++){
        tab[0] += tab[i];
    }

    /*decommenter ici si vous souhaiter une affichage du résultat au précision près à l'écran*/
    //gmp_printf("result = %.*Ff \n", precision, tab[0].get_mpf_t());
    //gmp_printf("result = %.*Ff \n", precision*2, tab[0].get_mpf_t());
    //gmp_printf("result = %.*Ff \n", precision*4, tab[0].get_mpf_t());
    
    //écriture dans le fichier resultat.txt
    result_file = fopen("resultat.txt","w");
    gmp_fprintf(result_file,"%.*Ff", precision, tab[0].get_mpf_t());
    fclose(result_file);
    
    //fin du chronomètre et affichage du temps utilisé
    auto t_end = chrono::steady_clock::now();
    auto t_diff = t_end - t_start;
    cout << "Time used: " << chrono::duration<double, milli> (t_diff).count()<<"ms"<< '\n';
    return 0;
}