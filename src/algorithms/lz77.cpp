#include <bits/stdc++.h>
#include "lz77.hpp"
using namespace std;

typedef pair<int, char> ic;
typedef tuple<int, int, char> iic;
 
const int ALF_SIZE = 128;

int bufferBits = 12;
int bufferBytes = ceil(bufferBits / 8.0);
int lookAheadBits = 4;
int lookAheadBytes = ceil(lookAheadBits / 8.0);
int bufferSize = (1 << bufferBits);
int lookAheadBufferSize = (1 << lookAheadBits);

void build_fsm(string &pat, int m, map<pair<int, char>, int>& fsm){
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
 
void prefix_match(string &txt, string &pat, int &p, int &l, char &c){    // n = len(txt), m = len(pat)
    int n = txt.length();
    int m = pat.length() - 1;

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

    c = pat[l];
}

void encode(char *txt, int sz, char *zip_name){
    int idx = 0, m = 0;
    string searchBuffer(' ',bufferSize), lookAheadBuffer;

    for(int i = 0; i < lookAheadBufferSize; i++){
        if(idx >= sz) break;
        lookAheadBuffer += txt[idx];
        idx++;
    }

    m = lookAheadBuffer.length();

    ofstream file;
    file.open(zip_name);

    while(m > 0){
        char c = '\\';
        int p = 0, l = 0;
        prefix_match(searchBuffer, lookAheadBuffer, p, l, c);
        if(c == '\\') break;
    
        file.write(reinterpret_cast<const char *>(&p), bufferBytes);
        file.write(reinterpret_cast<const char *>(&l), lookAheadBytes);
        file.write(reinterpret_cast<const char *>(&c), 1);

        int sbsize = searchBuffer.length();
        int lasize = lookAheadBuffer.length();

        for (int i = l + 1; i < sbsize; i++){
            searchBuffer[i - l - 1] = searchBuffer[i];
            searchBuffer[i] = lookAheadBuffer[i - l - 1];
        }
        lookAheadBuffer = lookAheadBuffer.substr(l+1, lasize - l - 1);

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
    ifstream file;
    file.open(zip_name, ios::binary);

    string sbuf = "", prefix = "";
    sbuf.assign(bufferSize, ' ');

    while(!file.eof()){
        char c = 0;
        int p = 0, l = 0;

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

    file.close();
}
