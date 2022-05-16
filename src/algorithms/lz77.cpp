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

char convert_first(int p){
    return (char) (p >> 4);
}

char convert_second(int p, int l){
    return (char) (((0x000F & p) << 4) | l);
}

vector<char> encode(char *txt, int sz){
    int idx = 0;
    string searchBuffer, lookAheadBuffer;
    searchBuffer.assign(bufferSize, ' ');

    int m = min(lookAheadBufferSize, sz);

    for(int i = 0; i < m; i++){
        lookAheadBuffer += txt[idx];
        idx++;
    }

    vector<char> encoded;

    while(m > 0 && idx < sz){
        int p = 0, l = 0;
        prefix_match(searchBuffer, lookAheadBuffer, p, l);
        char c = lookAheadBuffer[l];
    
        // char first = convert_first(p);
        // char second = convert_second(p, l);

        char first = (reinterpret_cast<const char *>(&p), sizeof(p));
        char second = (reinterpret_cast<const char *>(&l), sizeof(l));

        // cout << first << ' ' << second << ' ' << c << '\n';

        encoded.push_back(first);
        encoded.push_back(second);
        encoded.push_back(c);

        int sbsize = searchBuffer.length();
        int lasize = lookAheadBuffer.length();

        string aux = "";
        for(int i = 0; i + l + 1 < sbsize; i++){
            aux += searchBuffer[i + l + 1];
        }

        searchBuffer = aux;

        for(int i = 0; i + l + 1 < lasize; i++){
            searchBuffer += lookAheadBuffer[i + l + 1];
        }

        aux = "";
        for(int i = 0; i + l + 1 < lasize; i++){
            aux += lookAheadBuffer[i + l + 1];
        }

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

    return encoded;
}

int decodeC(int pos, int first, int second){
    return pos - (((unsigned char) first << 4) | ((unsigned char) second >> 4));
}

int decodeSize(int x){
    return (x & 0x000F);
}

vector<char> decode(vector<char> &encoded, int sz){
    vector<char> decoded;
    int pos = 0;
    for(int i = 0; i < sz; i += 3){
        int first = (unsigned char) encoded[i];
        int second = (unsigned char) encoded[i + 1];

        int size = decodeSize(second);
        int start = decodeC(pos, first, second);
        int end = start + size;

        for(int j = start; j < end; j++){
            decoded.push_back(decoded[j]);
        }
        
        decoded.push_back(encoded[i + 2]);
        pos += size + 1;
    }
 
    return decoded;
}