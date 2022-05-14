#include "zip.hpp"
using namespace std;

void save_zip(vector<char> &encoded, int size, char *zip_name){
    FILE* index_file = fopen(zip_name, "wb");
    fwrite(&size, sizeof(int), 1, index_file);
    for(int i = 0; i < size; i++){
        fwrite(&encoded[i], sizeof(char), 1, index_file);
    }
    fclose(index_file);
}

void zip(Args &ipmt){
    for(int tn = 0; tn < ipmt.num_txt; tn++){
        FILE *fp = fopen(ipmt.text_files[tn],"r");
        if(!fp){
            printf("Error: File %s not found\n",ipmt.text_files[tn]);
            continue;
        }
        fseek(fp, 0, SEEK_END);
        int size = ftell(fp);   // tamanho do texto
        fseek(fp, 0, SEEK_SET);
        char *buffer = (char*) malloc((size + 3) * sizeof(char)); //texto
        ipmt.texts_sizes.push_back(size);
        int er = fread(buffer, 1, size, fp);
        fclose(fp);
        // Encodar
        //printf("Entrou para o encode\n");
        vector<char> encoded = encode(buffer, size);
        printf("Saiu do encode\n");

        char* zip_name = (char*)malloc(sizeof(char)*(strlen(ipmt.text_files[tn]) + 5));
        strcpy(zip_name, ipmt.text_files[tn]);
        int pont = strlen(ipmt.text_files[tn]) - 1;
        while(zip_name[pont] != '.' and pont > 0){
            pont--;
        }
        if(pont > 0) zip_name[pont] = '\0';
        strcat(zip_name, ".myz");

        printf("Entrou para criar o zip\n");
        // Salvar
        save_zip(encoded, encoded.size(), zip_name);

        printf("Zip file %s created.\n", zip_name);
        
    }
}