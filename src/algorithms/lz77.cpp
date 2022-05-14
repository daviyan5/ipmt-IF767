#include <bits/stdc++.h>
#include "lz77.hpp"
using namespace std;

typedef pair<int, char> ic;
typedef tuple<int, int, char> iic;
 
const int ALF_SIZE = 128;
const unsigned long bufferSize = 8;                     //2ˆ12 - 1 (12 bits)
const unsigned long lookAheadBufferSize = 8;           //2ˆ4 - 1  (4 bits)

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
 
pair<int, int> prefix_match(string txt, string pat){    // n = len(txt), m = len(pat)
    int n = txt.length();
    int m = pat.length();

    map<pair<int, char>, int> fsm;
    build_fsm(pat, m, fsm);
    
    int l = 0, p = 0, cur = 0, i = 0;
 
    while(i < n && l < m){
        cur = fsm[ic(cur, txt[i])];

        if(cur > l){
            l = cur;
            p = i - l + 1;
        }
 
        i++;
    }
 
    return pair<int, int>(p, l);
}

char convert_first(int p){
    return (char) (p >> 4);
}

char convert_second(int p, int l){
    return (char) (((0x000F & p) << 4) | l);
}

vector<char> encode(char *txt, unsigned long sz){
    unsigned long idx = 0;
    string searchBuffer, lookAheadBuffer;
    searchBuffer.assign(bufferSize, ' ');

    int m = min(lookAheadBufferSize, sz);

    for(int i = 0; i < m; i++){
        lookAheadBuffer += txt[idx];
        idx++;
    }

    //cout << lookAheadBuffer << '\n';

    vector<char> encoded;

    while(m > 0 && idx < sz){
        auto [p, l] = prefix_match(searchBuffer, lookAheadBuffer);
        char c = lookAheadBuffer[l];
    
        cout << p << ' ' << l << ' ' << c << '\n';

        char first = convert_first(p);
        char second = convert_second(p, l);

        encoded.push_back(first);
        encoded.push_back(second);
        encoded.push_back(c);

        //cout << searchBuffer << ' ' << lookAheadBuffer << '\n';
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

        //cout << searchBuffer << ' ' << lookAheadBuffer << '\n';
        cout << "idx = " << idx << '\n';
        m = lookAheadBuffer.length();
    }

    return encoded;
}

unsigned long decodeC(int pos, int first, int second){
    return pos - (((unsigned char) first << 4) | ((unsigned char) second >> 4));
}

int decodeSize(int x){
    return (x & 0x000F);
}

vector<char> decode(char *encoded, int sz){
    vector<char> decoded;
    unsigned long pos = 0;
    for(int i = 0; i < sz; i += 3){
        int first = (unsigned char) encoded[i];
        int second = (unsigned char) encoded[i + 1];

        int size = decodeSize(second);
        unsigned long start = decodeC(pos, first, second);
        unsigned long end = start + size;

        for(unsigned long j = start; j < end; j++){
            decoded.push_back(decoded[j]);
        }
        
        decoded.push_back(encoded[i + 2]);
        pos += size + 1;
    }
 
    return decoded;
}

/*
abr1cQdtaGc�
*/
 
// int main(){
//     string txt = "abracadabraabraabracadabra";

//     vector<char> encoded = encode(txt, txt.length());
//     for(auto c: encoded) cout << c;
//     cout << '\n';
//     vector<char> decoded = decode(encoded, encoded.size());
//     for(auto c: decoded) cout << c;


//     cout << '\n';
// }

/*

saida dos caras: abr1cQdtaGc�
minha saida: abrQc1daDbc

0 0 a
0 0 b
0 0 r
5 1 c
3 1 d
1 4 a
4 4 b
0 2 c
2 1 d

0 0 a
0 0 b
0 0 r
3 1 c
5 1 d
7 4 a
4 7 c
15 6 

*/

/*
0 0 a
       a bracada
0 0 b
      ab racada
0 0 r
     abr acada
5 1 c
   abrac adab
3 1 d
 abracad abr
1 3 

0 0 a
         abracada
       a bracada
0 0 b
       a bracada
      ab racada
0 0 r
      ab racada
     abr acada
5 1 c
     abr acada
   abrac ada
3 1 d
   abrac ada
 abracad a
0 0 a
 abracad a
abracada 

===========================
entrei
0 0 a
         abracada
       a bracadab
idx =  9
0 0 b
       a bracadab
      ab racadabr
idx =  10
0 0 r
      ab racadabr
     abr acadabra
idx =  11
5 1 c
     abr acadabra
   abrac adabraab
idx =  13
3 1 d
   abrac adabraab
 abracad abraabra
idx =  15
1 4 a
 abracad abraabra
cadabraa braabrac
idx =  20
4 4 b
cadabraa braabrac
raabraab racadabr
idx =  25
0 2 c
raabraab racadabr
fora do range
fora do range
braabrac adabra
idx =  26

==== meu
abracada
0 0 a
         abracada
       a bracadab
idx = 9
0 0 b
       a bracadab
      ab racadabr
idx = 10
0 0 r
      ab racadabr
     abr acadabra
idx = 11
5 1 c
     abr acadabra
   abrac adabraab
idx = 13
3 1 d
   abrac adabraab
 abracad abraabra
idx = 15
1 4 a
 abracad abraabra
cadabraa braabrac
idx = 20
4 4 b
cadabraa braabrac
raabraab racadabr
idx = 25
0 2 c
raabraab racadabr
braabrac adabra
idx = 28
*/