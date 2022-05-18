# ipmt-IF767
*Projeto da disciplina de Processamento de Cadeia de Caracteres (IF767)*
IPMT é uma ferramenta para busca indexada e compressão de cadeias de caracteres.

## Dependências
_GNU Compiler Collection (gcc)_ é requisitado para a compilação e execução.
O comando "make" é necessário para compilação.

## Compilação
Na pasta principal da ferramenta, o seguinte comando deve ser executado no terminal:

```bash
make
```
Um arquivo "_ipmt_" deverá ser criado em ./bin.

## Execução
A ferramenta é executada na pasta ./bin, de acordo com:

```bash
./pmt type
```
Mais detalhes sobre os tipos e suas opções:

```
types: index, search, zip, unzip, help
usage: ./ipmt type [options]

index options:
   -a, --algorithm ALG:         Especifica o algoritmo
                                que deve ser utilizado para o array de sufixos.
                                ALG pode ser: {skew, sort}

   -s, --save:                  Salva o texto no arquivo de index.
                                Essa opção diminui consideravelmente o tempo de busca,
                                mas pode aumentar o tamanho do arquivo de índice.
search options:
   -c, --count                  Apenas conta o número de ocorrências do padrão.

   -p, --pattern PATTERNFILE    Arquivo de padrões a serem buscados.

   -e, --per-pattern            Imprime o número de ocorrências por padrão.
```

Para limpeza do executável do programa:
```bash
make clean
```