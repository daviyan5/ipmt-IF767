#include "index.hpp"

void save_index(int* SA, int* counts, char* index_name, int text_size){
    
    FILE* index_file = fopen(index_name, "wb");
    fwrite(&text_size, sizeof(int), 1, index_file);
    fwrite(counts, sizeof(int), alpha_size, index_file);
    fwrite(SA, sizeof(int), text_size, index_file);
    fclose(index_file);
}

void index(Args &ipmt){
    for(int tn = 0; tn < ipmt.num_txt; tn++){
        FILE *fp = fopen(ipmt.text_files[tn],"r");
        if(!fp){
            printf("Error: File %s not found\n",ipmt.text_files[tn]);
            continue;
        }
        fseek(fp,0,SEEK_END);
        int size = ftell(fp);
        fseek(fp,0,SEEK_SET);
        char *buffer = (char*) malloc((size+3) * sizeof(char));
        ipmt.texts_sizes.push_back(size);
        int er = fread(buffer,1,size,fp);
        int *text = (int*) malloc((size+5) * sizeof(int));
        for(int i = 0; i < size; i++){
            text[i] = int(buffer[i]);
        }
        text[size] = text[size+1] = text[size+2] = 0;
        int *counts = (int*) calloc(alpha_size,sizeof(int));
        for(int i = 0; i < size; i++){
            counts[text[i]] += 1;
        }
        int *SA = (int*) malloc(size * sizeof(int));
        switch(ipmt.alg){
            case ALG_SORT:
                sort_sa(text,SA,size,alpha_size);
                break;
            case ALG_SKEW:
                skew_sa(text,SA,size,alpha_size);
                break;
            default:
                skew_sa(text,SA,size,alpha_size);
                break;
        }
        char* index_name = (char*)malloc(sizeof(char)*(strlen(ipmt.text_files[tn]) + 5));
        strcpy(index_name, ipmt.text_files[tn]);
        int pont = strlen(ipmt.text_files[tn]) - 1;
        while(index_name[pont] != '.' and pont > 0){
            pont--;
        }
        if(pont > 0) index_name[pont] = '\0';
        strcat(index_name, ".idx");
        save_index(SA,counts,index_name,size);
        printf("Index of %s saved in %s\n",ipmt.text_files[tn],index_name);
        fclose(fp);
        free(text); free(buffer); free(SA); free(counts); free(index_name);
    }
}