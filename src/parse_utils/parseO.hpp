#ifndef _HAVE_PARSE_H

#define _HAVE_PARSE_H

#include <bits/stdc++.h>

extern int algs_number;
extern char alg_names[][30];
using namespace std;

enum ALGS_ENUM{
    ALG_BOYER_MOORE,
    ALG_BRUTE_FORCE,
    ALG_KMP,
    ALG_AHO_CORASICK,
    ALG_SELLERS,
    ALG_WU_MANBER,
};

// Argumentos da CLI
struct Args{
    bool only_count;            // Apenas conta as ocorrências
    bool show_stt;              // Mostra estatísticas
    bool is_mult_patt;          // Multiplos padrões
    bool is_out_txt;            // Saída no terminal ou em um arquivo         
    bool ignore_case;           // Ignora a diferença entre lower e uppercase
    bool show_info;             // Mostra informações da instância
    bool is_patt_file;          // O padrão foi dado por um arquivo
    bool failed;                // Deu ruim
    bool force_sg;              // Força single-pattern

    int only_help;              // Apenas ajuda?
    int dist;                   // Distancia da busca aproximada
    int num_patt;               // Número de padrões
    int num_threads;            // Número de threads
    int num_txt;                // Número de textos
    int alg;                    // Algoritimo usado
    int max_count;              // Máximo de ocorrências

    string patt_file;                   // Endereço do arquivo de padrões
    char* out_file;                     // Arquivo de saída
    vector<char*> patterns;             // Array de padrões
    vector<char*> text_files;           // Endereços dos arquivos de texto
    vector<int> patt_size;              // Tamanho de cada padrão, para evitar uso exaustivo de strlen()
};
char *get_alg_name(int pos);
Args parse_commands(int argc,char *argv[]);

#endif