#include "search.hpp"

void load_index(int *SA, int* counts, char* index_name, int &text_size){
    FILE* index_file = fopen(index_name, "rb");
    if(!index_file){
        printf("Index file '%s' not found.\n",index_name);
        exit(1);
    }
    int ert = fread(&text_size, sizeof(int), 1, index_file);
    int erc = fread(counts, sizeof(int), alpha_size, index_file);
    int ers = fread(SA, sizeof(int), text_size, index_file);
    fclose(index_file);
}

void search(Args &ipmt){
    
}