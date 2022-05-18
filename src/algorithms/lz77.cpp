#include <bits/stdc++.h>
#include "lz77.hpp"
using namespace std;

typedef pair<int, char> ic;
typedef tuple<int, int, char> iic;
 
const int ALF_SIZE = 128;   //tamanho do alfabeto (ascii)

int bufferBits = 12;
int bufferBytes = ceil(bufferBits / 8.0);
int lookAheadBits = 4;
int lookAheadBytes = ceil(lookAheadBits / 8.0);
int bufferSize = (1 << bufferBits);
int lookAheadBufferSize = (1 << lookAheadBits);

void build_fsm(string &pat, int m, map<pair<int, char>, int>& fsm){     //construcao da maquina de estados
    for(int i = 0; i < ALF_SIZE; i++) fsm[ic(0, i)] = 0;
    
    fsm[ic(0, pat[0])] = 1;

    int brd = 0;
    for(int c = 1; c < m; c++){
        for(int j = 0; j < ALF_SIZE; j++){
            fsm[ic(c, j)] = fsm[ic(brd, j)];
        }
        fsm[ic(c, pat[c])] = c + 1;
        brd = fsm[ic(brd, pat[c])];
    }
 
    for(int j = 0; j < ALF_SIZE; j++){
        fsm[ic(m, j)] = fsm[ic(brd, j)];
    }
}
 
void prefix_match(string &txt, string &pat, int &p, int &l, char &c){  
    int n = txt.length();                   //tamanho do texto
    int m = pat.length() - 1;               //tamanho do padrao

    map<pair<int, char>, int> fsm;
    build_fsm(pat, m, fsm);                 //construcao da maquina de estados

    int cur = 0, i = 0;
 
    while(i < n && l < m){
        cur = fsm[ic(cur, txt[i])];

        if(cur > l){
            l = cur;
            p = i - l + 1;
        }
 
        i++;
    }

    c = pat[l];
}

void encode(char *txt, int sz, char *zip_name){
    int idx = 0, m = 0;
    string searchBuffer, lookAheadBuffer;
    searchBuffer.assign(bufferSize, ' ');               //preencher o search buffer com ' ' inicialmente

    for(int i = 0; i < lookAheadBufferSize; i++){       //preencher o look ahead buffer com o texto
        if(idx >= sz) break;
        lookAheadBuffer += txt[idx];
        idx++;
    }

    m = lookAheadBuffer.length();

    ofstream file;                                      //abrir o arquivo em que sera escrito a codificacao
    file.open(zip_name);

    while(m > 0){
        char c;
        int p = 0, l = 0;
        prefix_match(searchBuffer, lookAheadBuffer, p, l, c);
    
        file.write(reinterpret_cast<const char *>(&p), bufferBytes);
        file.write(reinterpret_cast<const char *>(&l), lookAheadBytes);
        file.write(reinterpret_cast<const char *>(&c), 1);

        int sbsize = searchBuffer.length();
        int lasize = lookAheadBuffer.length();

        //a partir da tupla <p, l, c> podemos preencher o arquivo de codificacao,
        //e atualizar o search buffer e look ahead buffer

        string aux = "";           //string para auxiliar a atualizacao e evitar utilizar a funcao .substr() de c++
        for(int i = l + 1; i < sbsize; i++) aux += searchBuffer[i];

        for(int i = 0; i < l + 1; i++) aux += lookAheadBuffer[i];

        searchBuffer = aux;

        aux = "";
        for(int i = l + 1; i < lasize; i++) aux += lookAheadBuffer[i];

        lookAheadBuffer = aux;

        for(int i = 0; i < l + 1; i++){
            if(idx < sz){
                lookAheadBuffer += txt[idx];
                idx++;
            }
            else break;
        }

        m = lookAheadBuffer.length();
    }

    file.close();
}

void decode(char *zip_name, string &txt){
    ifstream file;                              //abrindo o arquivo que sera lido a decodificacao
    file.open(zip_name, ios::binary);

    string sbuf = "", prefix = "";               
    sbuf.assign(bufferSize, ' ');               //preencher o search buffer inicialmente com ' '

    while(!file.eof()){                         //enquanto nao atingir o final do arquivo
        char c = 0;
        int p = 0, l = 0;

        //pegar o p, l, c para poder reconstruir o texto
        file.read(reinterpret_cast<char *>(&p), bufferBytes);
        file.read(reinterpret_cast<char *>(&l), lookAheadBytes);
        file.read(reinterpret_cast<char *>(&c), 1);

        prefix = "";
        for(int i = p, j = l; i < bufferSize && j > 0; i++, j--) prefix += sbuf[i];

        txt += prefix + c;

        int j = 0, pl = prefix.length();
        for(int i = l + 1; i < sbuf.length(); i++, j++) sbuf[j] = sbuf[i];
         
        for(int i = 0; i < pl; i++) {
            sbuf[j] = prefix[i];
            j++;
        }

        sbuf[j] = c;
    }

    txt.pop_back();

    file.close();
}
