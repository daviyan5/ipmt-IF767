#include "zip.hpp"
using namespace std;

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

        char* zip_name = (char*)malloc(sizeof(char)*(strlen(ipmt.text_files[tn]) + 5));
        strcpy(zip_name, ipmt.text_files[tn]);
        int pont = strlen(ipmt.text_files[tn]) - 1;
        while(zip_name[pont] != '.' and pont > 0){
            pont--;
        }
        if(pont > 0) zip_name[pont] = '\0';
        strcat(zip_name, ".myz");
        
        //Encodar 
        encode(buffer, size, zip_name);
        printf("Saiu do encode\n");
    }
}
