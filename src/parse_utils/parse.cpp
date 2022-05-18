#include "parse.hpp"
#include "getopt.h"
using namespace std;

const int PATT_MAX_SIZE = 100000;


struct option long_options[] = {
	{"algorithm", required_argument, NULL, 'a'},
    {"count", no_argument, NULL, 'c'},
    {"pattern", required_argument, NULL, 'p'},
    {"save", no_argument, NULL, 's'},
    {"per-pattern", no_argument, NULL, 'P'},
	{ 0, 0, 0, 0 }
};

void print_option(const char command[],const char description[]){
    printf("   %-28s %s\n",command,description);
}
void print_helper(){
    printf("IPMT: Indexação, Busca, Compressão e Descompressão.\n");
    printf("types: index, search, zip, unzip, help\n");
    printf("usage: ./ipmt type [options]\n\n");
    
    printf("index options:\n");
    print_option("-a, --algorithm ALG:","Especifica o algoritmo");
    print_option("","que deve ser utilizado para o array de sufixos.");

    string op = "DC3, sort";
    char f_temp[500];
    sprintf(f_temp,"ALG pode ser: {%s}\n",op.c_str());
    print_option("",f_temp);
    print_option("-s, --save:","Salva o texto no arquivo de index.");
    print_option("","Essa opção diminui consideravelmente o tempo de busca,");
    print_option("","mas pode aumentar o tamanho do arquivo de índice.");

    printf("search options:\n");
    print_option("-c, --count","Apenas conta o número de ocorrências do padrão.\n");
    print_option("-p, --pattern PATTERNFILE","Arquivo de padrões a serem buscados.\n");
    print_option("-e, --per-pattern","Imprime o número de ocorrências por padrão.\n");

}
void init_args(Args& args){
    args.failed = false;
    args.only_count = false;
    args.only_help = false;
    args.is_patt_file = false;
    args.per_pattern = false;
    args.type = -1;
    args.save_text = 0;
    args.num_patt = 0;
    args.num_txt = 0;
    args.alg = -1;
}
int get_alg_id(char* alg){
    return strcmp(alg,"sort") == 0? ALG_SORT : ALG_DC3;
}
void read_patt_file(Args& ipmt){
    FILE *file;
    file = fopen(ipmt.patt_file.c_str(), "r");
    if(!file){
        printf("Erro ao abrir o arquivo de padrões!\n");
        exit(1);
    }
    char temp[PATT_MAX_SIZE];
    while(fgets(temp, PATT_MAX_SIZE,file)){ 
        int len = strlen(temp);
        if(temp[len - 1] == '\n'){
            temp[len - 1] = '\0';
            len -= 1;
        }
        char *c = (char*) malloc((len + 1) * sizeof(char));
        strcpy(c, temp);
        ipmt.patterns.push_back(c);
        ipmt.num_patt += 1;
    }
    fclose(file);
}
Args parse_commands(int argc,char *argv[]){
    Args ipmt; init_args(ipmt);
    
    if(argc < 2 or strcmp(argv[1],"help") == 0){
        ipmt.only_help = true;
        print_helper();
    }
    else if(strcmp(argv[1],"index") == 0){
        strcpy(argv[1],"-z");
        ipmt.type = INDEX;
        int opt;
        while((opt = getopt_long(argc, argv, "a:zs",long_options,NULL)) != -1){
            switch(opt){
                case 'a':
                    ipmt.alg = get_alg_id(optarg);
                    if(ipmt.alg != ALG_SORT and ipmt.alg != ALG_DC3 and ipmt.alg != -1){
                        ipmt.failed = true;
                        printf("Algoritmo inválido.\n");
                    }
                    break;
                case 's':
                    ipmt.save_text = 1;
                    break;
                case 'z':
                    break;
            }
        }
        if(optind == argc){
            ipmt.failed = true;
            printf("Nenhum arquivo de texto foi especificado.\n");
            return ipmt;
        }
        for(int idx = optind; idx < argc; idx++){
            ipmt.text_files.push_back(argv[idx]);
            ipmt.num_txt++;
        }
    }
    else if(strcmp(argv[1],"search") == 0){
        strcpy(argv[1],"-z");
        ipmt.type = SEARCH;
        int opt;
        while((opt = getopt_long(argc, argv, "p:cze",long_options,NULL)) != -1){
            switch(opt){
                case 'c':
                    ipmt.only_count = true;
                    break;
                case 'p':
                    ipmt.is_patt_file = true;
                    ipmt.patt_file = optarg;
                    break;
                case 'e':
                    ipmt.per_pattern = true;
                    break;
                case 'z':
                    break;
            }
        }
        int idx = optind;
        if(idx == argc and !ipmt.is_patt_file){
            ipmt.failed = true;
            printf("Nenhum padrão foi especificado.\n");
            return ipmt;
        }
        else if(ipmt.is_patt_file){
            read_patt_file(ipmt);

        }
        else{
            ipmt.patterns.push_back(argv[idx]);
            ipmt.num_patt++;
            idx++;
        }
        if(idx == argc){
            ipmt.failed = true;
            printf("Nenhum arquivo de índice foi especificado.\n");
            return ipmt;
        }
        ipmt.index_file = argv[idx];
        for(auto &u:ipmt.patterns){
            ipmt.patt_size.push_back(strlen(u));
        }
    }
    else if(strcmp(argv[1],"zip") == 0){
    
        strcpy(argv[1],"-z");
        ipmt.type = ZIP;
        if(argc == 2){
            ipmt.failed = true;
            printf("Nenhum arquivo de texto foi especificado.\n");
            return ipmt;
        }
        for(int idx = 2; idx < argc; idx++){
            ipmt.text_files.push_back(argv[idx]);
            ipmt.num_txt++;
        }
    }
    else if(strcmp(argv[1],"unzip") == 0){
        strcpy(argv[1],"-z");
        ipmt.type = UNZIP;
        if(argc == 2){
            ipmt.failed = true;
            printf("Nenhum arquivo foi especificado.\n");
            return ipmt;
        }
        for(int idx = 2; idx < argc; idx++){
            ipmt.text_files.push_back(argv[idx]);
            ipmt.num_txt++;
        }
    }
    else{
        ipmt.failed = true;
        printf("Opção '%s' inválida. Tente ./ipmt help.\n",argv[1]);
    }
    return ipmt;
}
