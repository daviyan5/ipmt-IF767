#include <iostream>
#include <string.h>
using namespace std;

int main(int argc, char *argv[]){
	for(int tn = 1; tn < argc; tn++){
		FILE *fp = fopen(argv[tn],"r");
		if(!fp){
			printf("Error: File %s not found\n",argv[tn]);
			continue;
		}
		fseek(fp,0,SEEK_END);
		int size = ftell(fp);
		fseek(fp,0,SEEK_SET);
		printf("Text loaded of size = %d\n",size);
		int x = 50000000/11111;
		for(int i = x; i < size; i *= 10){
			char *buffer = (char*) malloc((i + 2) * sizeof(char));
			int er = fread(buffer,sizeof(char),i,fp);
			if(er != i){
				printf("Unable to load file %s\n",argv[tn]);
				continue;
			}
			string fname(argv[tn]);
			while(fname.back() != '.'){
				fname.pop_back();
			}
			if(fname == "") fname = "temp";
			else fname.pop_back();
			fname += to_string(i/1000);
			fname += "KB.txt";
			FILE *fp2 = fopen(fname.c_str(),"w");
			fwrite(buffer,sizeof(char),i,fp2);
			fclose(fp2); 
			free(buffer);
			
		}
		fclose(fp);
	}
	
	return 0;
}

