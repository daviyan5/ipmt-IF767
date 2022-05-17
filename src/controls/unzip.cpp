#include "unzip.hpp"
using namespace std;

void save_txt(char* txt_name, string &txt){
    ofstream txt_file;
    txt_file.open(txt_name);
 
    if(!txt_file.is_open()){
        printf("Unable to create text file \n");
        exit(1);
    }
    int size = txt.length();
    txt_file.write(txt, size);
    
    txt_file.close();
}

void unzip(Args &ipmt){
    for(int tn = 0; tn < ipmt.num_txt; tn++){
        //zip name = ipmt.text_files[tn] ? 
        string txt = "";
        decode(ipmt.text_files[tn], txt); 

        char* text_name = (char*)malloc(sizeof(char)*(strlen(ipmt.text_files[tn]) + 5));
        strcpy(text_name, ipmt.text_files[tn]);
        int pont = strlen(ipmt.text_files[tn]) - 1;
        while(text_name[pont] != '.' and pont > 0){
            pont--;
        }
        if(pont > 0) text_name[pont] = '\0';
        strcat(text_name, ".txt");
        save_txt(text_name, txt);
    }
}
