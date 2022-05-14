#include "search.hpp"
using namespace std;
// Deixa a cor do texto no terminal azul 
void blue(){
    printf("\x1b[34m");
}

// Deixa a cor do texto no terminal vermelho
void red(){
    printf("\x1b[31m");
}

// Reseta a cor do terminal
void default_colour(){
    printf("\x1b[0m");
}

void load_index(int* &SA, int* counts, char* index_name, int &text_size){
    FILE* index_file = fopen(index_name, "rb");
    if(!index_file){
        printf("Index file '%s' not found.\n",index_name);
        exit(1);
    }
    int ert = fread(&text_size, sizeof(int), 1, index_file);
    int erc = fread(counts, sizeof(int), alpha_size, index_file);
    SA = (int*) malloc(text_size * sizeof(int));
    int ers = fread(SA, sizeof(int), text_size+1, index_file);
    if(ert != 1 or erc != alpha_size or ers != text_size){
        printf("Error reading index file '%s'.\n",index_name);
        exit(1);
    }
    fclose(index_file);
}
void rebuild_text(char *text, int *SA, int *counts){
    int pont = 0;
    for(int i = 0; i < alpha_size; i++){
        for(int j = 0; j < counts[i]; j++){
            text[SA[pont]] = char(i);
            pont++;
        }
    }
}
void print_occ_mul(char *text, int total_occ, int text_size, int num_patt, int **occ, 
                 vector<int> &patt_size, vector<int> &num_occ)
{
    blue();
    printf("Total Occurrences: %d\n",total_occ);
    default_colour();
    bool *paint = (bool*) calloc(text_size,sizeof(bool));
    int *counter = (int*) calloc(num_patt,sizeof(int));
    for(int i = 0; i < text_size; i++){
        for(int j = 0; j < num_patt; j++){
            if(counter[j] <  num_occ[j] and occ[j][counter[j]] == i){
                int pos = occ[j][counter[j]];
                counter[j]++;
                for(int k = 0; k < patt_size[j] and k + pos < text_size; k++) paint[k+pos] = true;
            }
        }
        if(paint[i]) red();
        else default_colour();
        printf("%c",text[i]);
        
    }
    free(paint);
    free(counter);
    default_colour();
    printf("\n");
}
int compare_int(const void *a, const void *b){
    return *(int*)a - *(int*)b;
}
void search(Args &ipmt){
    int text_size = 0;
    int *SA, *counts = (int*) calloc(alpha_size,sizeof(int));
    char *index_name = (char*)malloc(sizeof(char)*(ipmt.index_file.size() + 5));

    strcpy(index_name, ipmt.index_file.c_str());
    load_index(SA,counts,index_name,text_size);

    char *text = (char*) malloc((text_size+5) * sizeof(char));
    rebuild_text(text,SA,counts);

    int **occ = (int**) malloc(ipmt.num_patt * sizeof(int*));
    vector<int> num_occs;
    int total_occ = 0;
    for(int i = 0; i < ipmt.num_patt; i++){
        occ[i] = nullptr;
        int num_occ = sa_search(text,text_size,ipmt.patterns[i],ipmt.patt_size[i],SA,occ[i],ipmt.only_count);
        total_occ += num_occ;
        if(ipmt.only_count) printf("%s in %s: %d\n",ipmt.patterns[i],index_name, num_occ);
        else{
            qsort(occ[i],num_occ,sizeof(int),compare_int);
            num_occs.push_back(num_occ);
        }
    }
    if(!ipmt.only_count){
        print_occ_mul(text,total_occ,text_size,ipmt.num_patt,occ,ipmt.patt_size,num_occs);
    }
    free(text); free(SA); free(counts); free(index_name);
    for(int i = 0; i < ipmt.num_patt; i++){
        free(occ[i]);
        if(ipmt.is_patt_file) free(ipmt.patterns[i]);
    }
    free(occ);
}