#include <bits/stdc++.h>
 
using namespace std;
 
typedef tuple<int, int, char> iic;
 
const int ALF_SIZE = 128;
const int bufferSize = 4095;              //2ˆ12 - 1 (12 bits)
const int lookAheadBufferSize = 15;       //2ˆ4 - 1  (4 bits)

void build_fsm(char *pat, int m, map<pair<int, int>, int>& fsm){
    for(int i = 0; i < ALF_SIZE; i++) fsm[make_pair(0, i)] = 0;
    
    fsm[make_pair(0, pat[0])] = 1;

    int brd = 0;
    for(int c = 1; c < m; c++){
        for(int j = 0; j < ALF_SIZE; j++){
            fsm[make_pair(c, j)] = fsm[make_pair(brd, j)];
        }
        fsm[make_pair(c, pat[c])] = c + 1;
        brd = fsm[make_pair(brd, pat[c])];
    }
 
    for(int j = 0; j < ALF_SIZE; j++){
        fsm[make_pair(m, j)] = fsm[make_pair(brd, j)];
    }
}
 
pair<int, int> prefix_match_bf(char *txt, char *pat, int n, int m){
    m--;
    int p = 0, l = 0;
    for(int i = 0; i < n; i++){
        int j = 0;
        while(i + j < n && j < m && pat[j] == txt[i + j]) j++;
        if(j > l){
            l = j;
            p = i;
        }
    }
 
    return {p, l};
}   
 
pair<int, int> prefix_match(char *txt, char *pat){    // n = len(txt), m = len(pat)
    int n = strlen(txt);
    int m = strlen(pat) - 1;

    map<pair<int, int>, int> fsm;
    build_fsm(pat, m, fsm);
 
    m--;
    int l = 0, p = 0, cur = 0, i = 0;
 
    while(i < n && l < m){
        int idx = txt[i];
        cur = fsm[{cur, idx}];
 
        if(cur > l){
            l = cur;
            p = i - l + 1;
        }
 
        i++;
    }
 
    return make_pair(p, l);
}

char convert_first(int p){
    return (char) (p >> 4);
}

char convert_second(int p, int l){
    return (char) (((0x000F & p) << 4) | l);
}

char *getSubstring(char *sub, const char *s, int start, int end){
    return strncpy(sub, s + start, end);
}
 
vector<char> encode(char *txt, int sz){
    int idx = 0;
    // char *searchBuffer;
    // char *lookAheadBuffer;

    string searchBuffer, lookAheadBuffer;

    int m = min(lookAheadBufferSize, sz);

    //searchBuffer = (char*)malloc((bufferSize + 1)*sizeof(char));
    //lookAheadBuffer = (char*)malloc((lookAheadBufferSize+1)*sizeof(char));

    lookAheadBuffer = txt.substr(idx, m);

    idx

    for(int j = 0; j < m; j++) {
        lookAheadBuffer[j] = txt[idx];
        idx++;
    }

    vector<char> encoded;

    while(m > 0 && idx < sz){
        auto [p, l] = prefix_match(searchBuffer, lookAheadBuffer);
        char c = lookAheadBuffer[l];

        printf("p = %d || l = %d || c = %c\n", p, l, c);

        char first = convert_first(p);
        char second = convert_second(p, l);
        
        encoded.push_back(first);
        encoded.push_back(second);
        encoded.push_back(c);

        for(int i = 0; i + l + 1 < bufferSize; i++) {
            searchBuffer[i] = searchBuffer[l + 1];
        }
        for(int i = bufferSize - l - 1, j = 0; i < bufferSize; i++, j++){
            searchBuffer[i] = lookAheadBuffer[j];
        }

        for(int i = l + 1, j = 0; i < lookAheadBufferSize; i++, j++){
            lookAheadBuffer[j] = lookAheadBuffer[i];
        }

        char complement[l + 5];
        for(int j = 0; j < l + 1 && idx < sz; j++){
            complement[j] = txt[idx];
            printf("idx = %d\n", idx);
            idx++;
        }
        printf("new_size = %d\n", lookAheadBufferSize + l + 6);
        lookAheadBuffer = (char*)realloc(lookAheadBuffer, (lookAheadBufferSize + l+ 6)*sizeof(char));
        
        strcat(lookAheadBuffer, complement);
        m = strlen(lookAheadBuffer);
    }

    return encoded;
}

vector<char> decode(char *code, int sz){
    int pos = 0;
    vector<char> decoded;
    for(int i = 0; i < sz; i += 3){
        int first = (char) code[i];
        int second = (char) code[i + 1];
 
        int size = (char) (second & 0x000F);
        int start = pos - (((char) first << 4) | ((char) second >> 4));
        int end = start + size;
 
        for(int j = start; j < end; j++){
            decoded.push_back(decoded[j]);
        }
        
        decoded.push_back(code[i + 2]);
        pos += size + 1;
    }
 
    return decoded;
}
 
int main(){
    char *txt = (char*) malloc(500 * sizeof(char));
    strcpy(txt,"abracadabraabraabracadabra");
    vector<char> encoded = encode(txt, strlen(txt));
    for(auto c: encoded) printf("%c", c);
    printf("\n");
}