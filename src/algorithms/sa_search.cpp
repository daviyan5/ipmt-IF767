#include "suffix_array.hpp"
using namespace std;

int rangedbsearch(char *text, int text_size,char *patt, int patt_size, int* SA, bool lower = true){
	int left = 0;
	int right = text_size;
	int lcpl = 0, lcpr = 0;
	int ans = 0;
	while(left < right){
		int mid = (left+right)/2;
		int i = min(lcpl,lcpr);                         // Otimização, SA[mid] >= min(SA[left], SA[right])
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
			}
			else{                                       // Se a busca for pelo upper-bound
				if(mid == left) break;
				left = mid;
			}
			lcpl = lcpr = 0;
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

int search(char *text, int text_size,char *patt, int patt_size, int* SA, int *occ, bool only_count){
    if(!text or !patt or !SA or !occ){
        return -1;
    }
    // Busca pelo lower e upper bound
	int lower = rangedbsearch(text, text_size, patt, patt_size, SA, true);
	int upper = rangedbsearch(text, text_size, patt, patt_size, SA, false);
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