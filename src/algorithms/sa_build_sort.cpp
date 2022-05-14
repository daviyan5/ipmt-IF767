#include "suffix_array.hpp"
using namespace std;

void count_sort(int **v, int **aux,int *count, int text_size, int i, int alpha_size, int offset){
	memset(count,0,sizeof(int) * (alpha_size+offset));
	for(int j = 0; j < text_size; j++){
		count[v[j][i] + offset]++;
	}
	for(int j = 1; j < alpha_size + offset; j++){
		count[j] += count[j-1];
	}
	for(int j = text_size-1; j >= 0; j--){
		aux[count[v[j][i] + offset]-1][0] = v[j][0];
		aux[count[v[j][i] + offset]-1][1] = v[j][1];
		aux[count[v[j][i] + offset]-1][2] = v[j][2];
		count[v[j][i] + offset]--;
	}
	for(int j = 0; j < text_size; j++){
		v[j][0] = aux[j][0];
		v[j][1] = aux[j][1];
		v[j][2] = aux[j][2];
	}
}
void radix_sort(int **v,int **aux,int *count, int text_size, int m, int alpha_size){
	for(int i = m-1; i >= 0; i--){
        count_sort(v, aux, count, text_size, i, alpha_size, i == 1? 1: 0);
    }
}

void sort_sa(int* text, int* SA, int text_size, int alpha_size){
	if(text_size == 1){
		SA[0] = 0;
		return;
	}
	int pont = 1;
	int lim = int(ceil(log2(text_size)));
	int sz = max(text_size,alpha_size);
	int *P = (int*) calloc(sz+1, sizeof(int));
	for(int i = 0; i < text_size; i++){
		P[i] = text[i];
	}

	int **pairs = (int**) malloc((text_size+1) * sizeof(int*));
	int **aux = (int**) malloc((text_size+1) * sizeof(int*));
	int *count = (int*) calloc(sz + 2,sizeof(int));

	for(int i = 0; i < text_size; i++){
		pairs[i] = (int*) malloc(3 * sizeof(int));
		aux[i] = (int*) malloc(3 * sizeof(int));
	}
	for(int pont = 1, l = 1; l >> 1 <= text_size; pont++, l <<= 1){
		for(int i = 0; i < text_size; i++){
			pairs[i][0] = P[i];
			pairs[i][1] = i+l < text_size? P[i+l]: -1;
			pairs[i][2] = i;
		}
        radix_sort(pairs,aux,count, text_size, 2, sz);
		int order = 0;
		memset(P, 0, sizeof(int) * sz);
		for(int i = 1; i < text_size; i++){
			P[pairs[i][2]] = i > 0 and pairs[i][0] == pairs[i-1][0] and pairs[i][1] == pairs[i-1][1]? P[pairs[i-1][2]]: i;
		}
	}
	for(int i = 0; i < text_size; i++){
		SA[P[i]] = i;
		free(pairs[i]); free(aux[i]);
	}
	free(P); free(count); free(pairs); free(aux);
}