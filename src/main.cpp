#include "./parse_utils/parse.hpp"
#include "./controls/index.hpp"
#include "./controls/search.hpp"
#include "./controls/zip.hpp"
#include "./controls/unzip.hpp"
using namespace std;

// A main serve de intermediário entre o parsing dos comandos e o manejamento dos algoritmos.
int main(int argc,char *argv[]){
    // Os comandos e os argumentos são gerados
    Args ipmt = parse_commands(argc,argv);

    // Se houve alguma falha na etapa anterior, o programa fecha
    if(ipmt.failed or ipmt.only_help) exit(1);
    
    switch (ipmt.type){ // Chama a função responsável pelo tipo informado no parsing
    case INDEX:
        index(ipmt);
        break;
    case SEARCH:
        search(ipmt);
        break;
    case ZIP:
        zip(ipmt);
        break;
    case UNZIP:
        unzip(ipmt);
        break;
    }
    return 0;
}