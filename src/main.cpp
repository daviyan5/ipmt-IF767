#include "./parse_utils/parse.hpp"
using namespace std;

// A main serve de intermediário entre o parsing dos comandos e o manejamento dos algoritmos.
int main(int argc,char *argv[]){
    // Os comandos e os argumentos são gerados
    Args ipmt = parse_commands(argc,argv);

    // Se houve alguma falha na etapa anterior, o programa fecha
    if(ipmt.failed or ipmt.only_help) exit(1);
    return 0;
}