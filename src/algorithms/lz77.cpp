#include <bits/stdc++.h>
#include "lz77.hpp"
using namespace std;

typedef pair<int, char> ic;
typedef tuple<int, int, char> iic;
 
const int ALF_SIZE = 128;

int bufferBits = 12;
int lookAheadBits = 4;
int bufferSize = (1 << bufferBits);
int lookAheadBufferSize = (1 << lookAheadBits);

void build_fsm(string pat, int m, map<pair<int, char>, int>& fsm){
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
 
void prefix_match(string txt, string pat, int &p, int &l){    // n = len(txt), m = len(pat)
    int n = txt.length();
    int m = pat.length();

    map<pair<int, char>, int> fsm;
    build_fsm(pat, m, fsm);
    
    int cur = 0, i = 0;
 
    while(i < n && l < m){
        cur = fsm[ic(cur, txt[i])];

        if(cur > l){
            l = cur;
            p = i - l + 1;
        }
 
        i++;
    }
}

void encode(char *txt, int sz, char *zip_name){
    int idx = 0;
    string searchBuffer, lookAheadBuffer;
    searchBuffer.assign(bufferSize, ' ');

    int m = min(lookAheadBufferSize, sz);

    for(int i = 0; i < m; i++){
        lookAheadBuffer += txt[idx];
        idx++;
    }

    for(int i = 0; i < lookAheadBufferSize; i++){
        if(idx >= sz) break;
        lookAheadBuffer += txt[i];
        idx++;
    }

    ofstream file;
    file.open(zip_name);

    while(idx < sz){
        int p = 0, l = 0;
        prefix_match(searchBuffer, lookAheadBuffer, p, l);
        char c = lookAheadBuffer[l];
    
        file.write(reinterpret_cast<const char *>(&p), sizeof(p));
        file.write(reinterpret_cast<const char *>(&l), sizeof(l));
        file.write(reinterpret_cast<const char *>(&c), sizeof(c));

        int sbsize = searchBuffer.length();
        int lasize = lookAheadBuffer.length();

        string aux = "";
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
    }
}

void decode(char *zip_name, string &txt){
    ifstream file;
    file.open(zip_name, ios::binary);

    string sbuf, prefix = "";
    sbuf.assign(bufferSize, ' ');

    while(!file.eof()){
        char toReadC;
        int toReadP, toReadL;

        file.read(reinterpret_cast<char *>(&toReadP), sizeof(toReadP));
        file.read(reinterpret_cast<char *>(&toReadL), sizeof(toReadL));
        file.read(reinterpret_cast<char *>(&toReadC), sizeof(toReadC));

        prefix = "";
        for(int i = toReadP, j = toReadL; i < bufferSize && j > 0; i++, j--) prefix += sbuf[i];

        txt += prefix + toReadC;

        string aux = "";
        for(int i = toReadL + 1; i < sbuf.length(); i++) aux += sbuf[i];
        aux += prefix + toReadC;

        sbuf = aux;
    }
}
