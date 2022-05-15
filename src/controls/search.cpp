#include "search.hpp"
#include <chrono>
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

void load_index(int* &SA, int* counts, char* index_name,char* &text, int &text_size, int &saved_text){
    FILE* index_file = fopen(index_name, "rb");
    if(!index_file){
        printf("Index file '%s' not found.\n",index_name);
        exit(1);
    }
    int erz = fread(&text_size, sizeof(int), 1, index_file);
    text = (char*) malloc((text_size+5) * sizeof(char));
    int era = fread(&saved_text,sizeof(int), 1, index_file);
    if(saved_text == 1){
        int ert = fread(text,sizeof(char),text_size, index_file);
        if(ert != text_size){
            printf("Error reading text from file.\n");
            exit(1);
        }
    }
    else{
        int erc = fread(counts, sizeof(int), alpha_size, index_file);
        if(erc != alpha_size){
            printf("Error reading counts from file.\n");
            exit(1);
        }
    }
    
    SA = (int*) malloc((text_size+1) * sizeof(int));
    int ers = fread(SA, sizeof(int), text_size, index_file);
    if(erz != 1 or  era != 1 or ers != text_size){
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
    printf("-----Total de Ocorrências: %d-----\n",total_occ);
    default_colour();
    bool *paint = (bool*) calloc(text_size,sizeof(bool));
    int lim = 0;
    for(int j = 0; j < num_patt; j++){
        for(int o = 0; o < num_occ[j]; o++){
            int pos = occ[j][o];
            for(int k = 0; k < patt_size[j] and k + pos < text_size; k++){
                paint[k+pos] = true;
                lim = max(lim,k+pos+1);
            }
        }
        
    }
    while(lim < text_size and text[lim] != '\n') lim++;
    for(int i = 0; i < lim; i++){
        if(paint[i]) red();
        else default_colour();
        printf("%c",text[i]);
        
    }
    if(lim != text_size){
        blue();
        printf("\n-----Sem mais aparições a partir da posição %d.-----\n",lim);
        default_colour();
    }
    free(paint);
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
    char *text;
    int saved_text = 0;
    load_index(SA,counts,index_name,text,text_size,saved_text);
    if(!saved_text) rebuild_text(text,SA,counts);

    int **occ = (int**) malloc(ipmt.num_patt * sizeof(int*));
    vector<int> num_occs;
    int total_occ = 0;
    for(int i = 0; i < ipmt.num_patt; i++){
        occ[i] = nullptr;
        int num_occ = sa_search(text,text_size,ipmt.patterns[i],ipmt.patt_size[i],SA,occ[i],ipmt.only_count);
        if(num_occ == -1){
            printf("Pattern '%s' failed in search.\n",ipmt.patterns[i]);
            continue;
        }
        total_occ += num_occ;
        printf("%s in %s: %d\n",ipmt.patterns[i],index_name, num_occ);
        if(!ipmt.only_count){
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