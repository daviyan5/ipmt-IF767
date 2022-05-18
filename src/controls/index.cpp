#include "index.hpp"

// Salva o index em um arquivo ".idx"
void save_index(int* SA, int* counts, char* index_name,char* text, int text_size, int save_text){
    
    FILE* index_file = fopen(index_name, "wb");
    fwrite(&text_size, sizeof(int), 1, index_file);
    fwrite(&save_text, sizeof(int), 1, index_file);
    if(save_text == 1) fwrite(text,sizeof(char),text_size,index_file);
    else fwrite(counts, sizeof(int), alpha_size, index_file);
    fwrite(SA, sizeof(int), text_size, index_file);
    fclose(index_file);
}

void index(Args &ipmt){
    for(int tn = 0; tn < ipmt.num_txt; tn++){               // Iteração pelos arquivos de texto
        FILE *fp = fopen(ipmt.text_files[tn],"r");
        if(!fp){
            printf("Error: File %s not found\n",ipmt.text_files[tn]);
            continue;
        }
        fseek(fp,0,SEEK_END);
        int size = ftell(fp);
        fseek(fp,0,SEEK_SET);                               // Calcula o tamanho do arquivo, para leitura em um ponteiro
        char *buffer = (char*) malloc((size+3) * sizeof(char));
        ipmt.texts_sizes.push_back(size);
        int er = fread(buffer,1,size,fp);
        int *text = (int*) malloc((size+5) * sizeof(int));  // Converte o texto para inteiro, para execução dos algoritmos
        for(int i = 0; i < size; i++){
            text[i] = int(buffer[i]);
        }
        text[size] = text[size+1] = text[size+2] = 0;       // Sentinelas
        int *counts = (int*) calloc(alpha_size,sizeof(int)); // Array com a contagem de cada caractere    
        if(!ipmt.save_text){                                // Se não for salvar o texto, é necessário calcular a contagem de cada caractere
            for(int i = 0; i < size; i++){
                counts[text[i]] ++;
            }
        }
        int *SA = (int*) malloc(size * sizeof(int));
        switch(ipmt.alg){                                   // Chama o algoritmo escolhido
            case ALG_SORT:
                sort_sa(text,SA,size,alpha_size);
                break;
            case ALG_DC3:
                DC3_sa(text,SA,size,alpha_size);
                break;
            default:
                DC3_sa(text,SA,size,alpha_size);
                break;
        }
        char* index_name = (char*)malloc(sizeof(char)*(strlen(ipmt.text_files[tn]) + 5));
        strcpy(index_name, ipmt.text_files[tn]);
        int pont = strlen(ipmt.text_files[tn]) - 1;
        while(index_name[pont] != '.' and pont > 0){
            pont--;
        }
        if(pont > 0) index_name[pont] = '\0';
        strcat(index_name, ".idx");                 // Cria o nome do arquivo
        save_index(SA,counts,index_name, buffer, size,ipmt.save_text);
        printf("Index of %s saved in %s\n",ipmt.text_files[tn],index_name);
        fclose(fp);
        free(text); free(buffer); free(SA); free(counts); free(index_name);
    }
}