#include "unzip.hpp"
using namespace std;

vector<char> load_zip(char* zip_name){
    ifstream zip_file(zip_name);
    vector<char> txt((istreambuf_iterator<char>(zip_file)), istreambuf_iterator<char>());
    //for(auto c: txt) cout << c;
    //FILE* zip_file = fopen(zip_name, "rb");
    if(!zip_file.is_open()){
        printf("Zip file '%s' not found.\n",zip_name);
        exit(1);
    }
    zip_file.close();
    return txt;
    // int ert = fread(&text_size, sizeof(int), 1, zip_file);
    // buffer = (char*) malloc(text_size * sizeof(char));
    // int ers = fread(buffer, sizeof(char), text_size, zip_file);
    // if(ert != 1 or ers != text_size){
    //     printf("Error reading zip file '%s'.\n", zip_name);
    //     exit(1);
    // }
    // fclose(zip_file);
}

void save_txt(char* txt_name, vector<char> &decoded, int size){
    FILE* txt_file = fopen(txt_name, "wt");
    if(!txt_file){
        printf("Unable to create text file \n");
        exit(1);
    }
    for(int i = 0; i < size; i++){
        fwrite(&decoded[i], sizeof(char), 1, txt_file);
    }
    fclose(txt_file);
}

void unzip(Args &ipmt){
    for(int tn = 0; tn < ipmt.num_txt; tn++){
        vector<char> txt = load_zip(ipmt.text_files[tn]);
        vector<char> decoded = decode(txt, txt.size());

        char* text_name = (char*)malloc(sizeof(char)*(strlen(ipmt.text_files[tn]) + 5));
        strcpy(text_name, ipmt.text_files[tn]);
        int pont = strlen(ipmt.text_files[tn]) - 1;
        while(text_name[pont] != '.' and pont > 0){
            pont--;
        }
        if(pont > 0) text_name[pont] = '\0';
        strcat(text_name, ".txt");
        save_txt(text_name, decoded, decoded.size());
    }
}