#include "suffix_array.hpp"
using namespace std;

// Funções para comparação de tuplas em ordem lexicográfica.

bool cmp_dupla(int a1, int a2, int b1, int b2){ 
	return(a1 < b1 or a1 == b1 and a2 <= b2); 
} 
bool cmp_tripla(int a1, int a2, int a3, int b1, int b2, int b3){
	return(a1 < b1 or a1 == b1 and cmp_dupla(a2,a3, b2,b3)); 
} 

// Um passo do Counting Sort para ordenar os índices "indexes" do array.
void counting_sort(int* indexes, int* sorted, int* text, int text_size, int alpha_size){
	int* count = (int*) calloc(alpha_size + 1,sizeof(int)); 									
	for (int i = 0; i <= alpha_size; i++) count[i] = 0; 						
	for (int i = 0; i < text_size; i++) count[text[indexes[i]]] += 1; 					
	
	for (int i = 0, sum = 0; i <= alpha_size; i++){ 						
		int t = count[i]; 
		count[i] = sum; 
		sum += t; 
	}
	for (int i = 0; i < text_size; i++){
		sorted[count[text[indexes[i]]]] = indexes[i]; 							
		count[text[indexes[i]]] += 1;
	}
	free(count);
}

// Encontra o suffix array de text[0..text_size - 1].
// text[text_size] = text[text_size+1] = text[text_size+2] = 0, len(text) >= 2

void skew_sa(int* text, int* SA, int text_size, int alpha_size) {
	if(text_size == 1){
		SA[0] = 0;
		return;
	}
	int n0 = (text_size+2)/3, n1 = (text_size+1)/3, n2 = text_size/3;
    int n02 = n0 + n2;
	
	int* s12 = (int*) calloc(n02 + 3, sizeof(int));                 // Array com concatenação de s1 e s2.
	int* SA12 = (int*) calloc(n02 + 3, sizeof(int));                // Array de sufixos para s12.

	int* s0 = (int*) calloc(n0, sizeof(int));                       // Array com as posições divisíveis por 3
	int* SA0 = (int*) calloc(n0, sizeof(int));                      // Array de sufixos para s0.

	// Gerando array com as posições não divisíveis por 3
	for (int i = 0, j = 0; i < text_size + (n0-n1); i++){
		if(i%3 != 0){
			s12[j] = i;
			j += 1;
		}
	}
	
	// Ordenando SA12
	counting_sort(s12 , SA12, text+2, n02, alpha_size);
	counting_sort(SA12, s12 , text+1, n02, alpha_size);
	counting_sort(s12 , SA12, text , n02, alpha_size);

	// Gerando um nome para as triplas do array s12
	int name = 0, c0 = -1, c1 = -1, c2 = -1;
	for (int i = 0; i < n02; i++) {
		if(text[SA12[i]] != c0 or text[SA12[i]+1] != c1 or text[SA12[i]+2] != c2){  // Se o nome não for único
			name += 1; 
			c0 = text[SA12[i]]; 
			c1 = text[SA12[i]+1]; 
			c2 = text[SA12[i]+2]; 
		}
		if (SA12[i] % 3 == 1) s12[SA12[i]/3] = name;    // Se pertence ao primeiro grupo
		else s12[SA12[i]/3 + n0] = name;		        // Se pertence ao segundo grupo
	}
	
	if (name < n02) {				
		skew_sa(s12, SA12, n02, name);	    // Se os nomes ainda não forem únicos, gera o array de sufixos SA12
		for(int i = 0; i < n02; i++){		// Salva os nomes atualizados das tuplas em s12, para ordenação
			s12[SA12[i]] = i + 1;
		}
	} 
	else{ 									// Gera o array de sufixos para os nomes automaticamente
		for(int i = 0; i < n02; i++){
			SA12[s12[i] - 1] = i;
		}
	}
	
	for (int i = 0, j = 0; i < n02; i++){	// Gerando array com as posições divisíveis por 3
		if(SA12[i] < n0){
			s0[j] = 3*SA12[i];
            j += 1;
		}
	}
	counting_sort(s0, SA0, text, n0, alpha_size);	// Ordenando o array de sufixos de S0
	
	for (int p = 0, t = n0 - n1, k = 0; k < text_size; k++) {	// Unindo o vetor de sufixos S0 com S12 para gerar SA
		#define get_pos() (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2) // Convertendo a posição das triplas para o texto original
		int p12 = get_pos();
		int p0 = SA0[p]; 							

		if(SA12[t] < n0 ? cmp_dupla(text[p12], s12[SA12[t] + n0], text[p0], s12[p0/3]) : cmp_tripla(text[p12], text[p12+1],s12[SA12[t]-n0+1], text[p0], text[p0+1],s12[p0/3+n0])){ // Se o sufixo de SA12 for menor que o sufixo de SA0
			
            SA[k] = p12; t++;
			if (t == n02){ 							// Sobraram apenas sufixos de SA0
				for(k++; p < n0; p++, k++){
					SA[k] = SA0[p];
				}
			}
		} 
		else{										
			SA[k] = p0; p++;
			if (p == n0){							// Sobraram apenas sufixos de SA12
				for(k++; t < n02; t++, k++){
					SA[k] = get_pos();
				}
			}
		}
	}
	free(s12); free(SA12); free(s0); free(SA0);
}