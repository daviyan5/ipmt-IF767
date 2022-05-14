CPP_FLAGS = -std=c++17 -O3
SOURCE = ./src/main.cpp
DEPENDENCIAS = ./src/parse_utils/parse.cpp ./src/controls/index.cpp ./src/controls/search.cpp ./src/controls/zip.cpp ./src/controls/unzip.cpp
ALGORITMOS = ./src/algorithms/sa_build_skew.cpp ./src/algorithms/sa_build_sort.cpp ./src/algorithms/sa_search.cpp
MAIN = ./bin/ipmt

all: config

config:
	g++ $(CPP_FLAGS) $(SOURCE) $(DEPENDENCIAS) $(ALGORITMOS) -o $(MAIN)

clean:

	rm $(MAIN)

