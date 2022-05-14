#ifndef _HAVE_PARSE_H

#define _HAVE_PARSE_H

#include <bits/stdc++.h>
#include <iostream>
using namespace std;

enum TYPES{
    INDEX,
    SEARCH,
    ZIP,
    UNZIP
};

// Argumentos da CLI
struct Args{

    bool only_count;                    // Apenas conta as ocorrências
    bool only_help;                     // Apenas ajuda
    bool is_patt_file;                  // O padrão foi dado por um arquivo
    bool uncompressed;                  // O arquivo deve ser salvo descompactado
    bool failed;                        // Deu ruim

    int type;                           // Tipo de comando
    int num_patt;                       // Número de padrões
    int num_txt;                        // Número de textos
    int alg;                            // Algoritimo usado

    string index_file;                  // Endereço do índice
    string zipped_file;                 // Endereço do compactado   
    string patt_file;                   // Endereço do arquivo de padrões
    std::vector<char*> patterns;        // Array de padrões
    std::vector<char*> text_files;      // Endereços dos arquivos de texto
    std::vector<int> patt_size;         // Tamanho de cada padrão, para evitar uso exaustivo de strlen()
};

Args parse_commands(int argc,char *argv[]);

#endif