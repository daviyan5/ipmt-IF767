CPP_FLAGS = -std=c++17 -O3
SOURCE = ../src/main.cpp
DEPENDENCIAS = 
ALGORITMOS = 
INDEXES = ./indexes
ZIPPED = ./zipped 
MAIN = ./ipmt

all: config

config:
	g++ $(CPP_FLAGS) $(SOURCE) $(DEPENDENCIAS) $(ALGORITMOS) -o $(MAIN)

clean:
	rm $(INDEXES)
	rm $(ZIPPED)
	rm $(MAIN)

