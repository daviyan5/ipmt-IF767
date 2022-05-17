SHELL:=/bin/bash
IPMT=../bin/ipmt
DNA=../tests/files/dna/dna*.txt
ENG=../tests/files/eng/eng*.txt
help:
	@echo "Atenção: os arquivos de teste devem ser adicionados previamente.";
	@echo "Para executar os testes, use "make Ti", onde i é o número do teste.";
	@echo "Além das dependências de execução do comando (GCC), para executar os testes também é necessário python."
	@echo "Os resultados de cada teste ficarão na pasta ./results";

T1:
	for file in $(DNA); do\
		basename $$file .txt;\
		NAME=$(basename $$file .txt); \
		echo $$NAME; \
	done; \
	echo T1 finished;
T2:
	
T3:

T4:

clean:
	-rm $(DNA)dna*.idx
	-rm $(ENG)english*.idx
