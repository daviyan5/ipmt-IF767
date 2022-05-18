#include "zip.hpp"
using namespace std;

void zip(Args &ipmt){
    for(int tn = 0; tn < ipmt.num_txt; tn++){
        //Abrindo o arquivo e lendo o texto para poder codifica-lo
        FILE *fp = fopen(ipmt.text_files[tn], "r");
        if(!fp){
            printf("Error: File %s not found\n",ipmt.text_files[tn]);
            continue;
        }
        fseek(fp, 0, SEEK_END);
        int size = ftell(fp);   // tamanho do texto
        fseek(fp, 0, SEEK_SET);
        char *buffer = (char*) malloc((size + 3) * sizeof(char)); //texto

        int er = fread(buffer, sizeof(char), size, fp);
        fclose(fp);

        //Mudando o nome do arquivo para nomear o .myz
        char* zip_name = (char*)malloc(sizeof(char)*(strlen(ipmt.text_files[tn]) + 5));
        strcpy(zip_name, ipmt.text_files[tn]);
        int pont = strlen(ipmt.text_files[tn]) - 1;
        while(zip_name[pont] != '.' and pont > 0){
            pont--;
        }
        if(pont > 0) zip_name[pont] = '\0';
        strcat(zip_name, ".myz");
        
        //Chamar a funcao de encode, que tem como argumentos o texto, tamanho do texto e nome do arquivo de zip
        encode(buffer, size, zip_name);
        printf("Arquivo %s zipado.\n", ipmt.text_files[tn]);
        
        free(buffer);
        free(zip_name);
    }
}
