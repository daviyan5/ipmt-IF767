CPP_FLAGS = -std=c++17 -O3
SOURCE = ./src/main.cpp
DEPENDENCIAS = 
ALGORITMOS = 
INDEXES = ./bin/indexes
ZIPPED = ./bin/zipped 
MAIN = ./bin/ipmt

all: config

config:
	g++ $(CPP_FLAGS) $(SOURCE) $(DEPENDENCIAS) $(ALGORITMOS) -o $(MAIN)

clean:
	rm $(INDEXES)
	rm $(ZIPPED)
	rm $(MAIN)

