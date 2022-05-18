#ifndef _HAVE_PARSE_H

#define _HAVE_PARSE_H

#include <bits/stdc++.h>
#include <iostream>
const int alpha_size = 128;

enum TYPES{
    INDEX,
    SEARCH,
    ZIP,
    UNZIP
};

enum ALGS{
    ALG_DC3,
    ALG_SORT
};

// Argumentos da CLI
struct Args{

    bool only_count;                    // Apenas conta as ocorrências
    bool only_help;                     // Apenas ajuda
    bool is_patt_file;                  // O padrão foi dado por um arquivo
    bool per_pattern;                   // Printa o número de ocorrências por padrão
    bool failed;                        // Deu ruim

    int type;                           // Tipo de comando
    int save_text;                      // Salvar o texto
    int num_patt;                       // Número de padrões
    int num_txt;                        // Número de textos
    int alg;                            // Algoritimo usado

    std::string index_file;                  // Endereço do índice
    std::string zipped_file;                 // Endereço do compactado   
    std::string patt_file;                   // Endereço do arquivo de padrões
    std::vector<char*> patterns;        // Array de padrões
    std::vector<char*> text_files;      // Endereços dos arquivos de texto
    std::vector<int> patt_size;         // Tamanho de cada padrão, para evitar uso exaustivo de strlen()
    std::vector<long> texts_sizes;      // Tamanho de cada texto
};

Args parse_commands(int argc,char *argv[]);

#endif