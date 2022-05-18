#include "suffix_array.hpp"
using namespace std;

int rangedbsearch(char *text, int text_size, char *patt, int patt_size, int* SA, bool lower = true, int l_prev = 0, int lcpl_prev = 0){
	int left = l_prev;
	int right = text_size - 1;
	int lcpl = lcpl_prev, lcpr = 0;
	if(lcpl == 0){
		for(int i = 0; i < patt_size; i++){
			if(text[SA[0] + i] == patt[i]) lcpl++;
			else break;
		}
	}
	if(lcpr == 0){
		for(int i = 0; i < patt_size; i++){
			if(text[SA[text_size - 1] + i] == patt[i]) lcpr++;
			else break;
		}
	}
	int ans = 0;
	while(left < right){
		int mid = (left+right)/2;
		int i = min(lcpl,lcpr);                         // Otimização, LCP(i,T[SA[mid]]) >= min(LCP(i,T[SA[left]]), LCP(i,T[SA[right]]))
		bool less = true;                               // Indica que é menor que o sufixo
		while(i < patt_size and SA[mid] + i < text_size){
			if(patt[i] < text[SA[mid] + i]){
				break;
			}
			else if(patt[i] > text[SA[mid] + i]){
				less = false;
				break;
			}
			i++;
		}
		if(i == patt_size){                             // Se for igual ao sufixo
			ans = mid;
			if(lower){                                  // Se a busca for pelo lower-bound
				if(mid == right) break;
				right = mid;
				lcpr = i;
			}
			else{                                       // Se a busca for pelo upper-bound
				if(mid == left) break;
				left = mid;
				lcpl = i;
			}
		}
		else if(less){
			if(mid == right) break;
			right = mid;
			lcpr = i;                          
		}
		else{
			if(mid == left) break;
			left = mid;
			lcpl = i;
		}
	}
	return ans;
}

int sa_search(char *text, int text_size,char *patt, int patt_size, int* SA, int* &occ, bool only_count){
    if(!text or !patt or !SA){
        return -1;
    }
    // Busca pelo lower e upper bound
	int lower = rangedbsearch(text, text_size, patt, patt_size, SA, true, 0, 0);
	if(lower > 0 and text[SA[lower]] != patt[0]){
		printf("lower bound error\n");
		return -1;
	}
	else if(lower == 0 and text[SA[0]] != patt[0]){	// Se não achou ninguém
		return 0;
	}
	int upper = rangedbsearch(text, text_size, patt, patt_size, SA, false, lower, patt_size);
	if(upper > 0 and text[SA[upper]] != patt[0]){
		printf("upper bound error\n");
		return -1;
	}
	if(only_count) return upper - lower + 1;
    else{
        int num = (upper - lower + 1);
        occ = (int*)malloc(sizeof(int)*num);
        for(int i = 0; i < num; i++){
            occ[i] = SA[lower + i];
        }
        return upper - lower + 1;
    }
}