CC=g++
FLAGS=-std=c++11

MAIN=main.cpp
EXECUTABLE=diff

SRC=include
SOURCES=$(SRC)/differentiation.cpp $(SRC)/makeOutPutFile.cpp $(SRC)/transformDiffTree.cpp $(SRC)/diff.cpp $(SRC)/makeTree.cpp $(SRC)/Node/Node.cpp  

HEADERS=$(SRC)/diff.h $(SRC)/Node/Node.h


all: build

build:  $(SOURSES) $(HEADERS) 
	$(CC) $(FLAGS) $(MAIN) $(SOURCES) -o $(EXECUTABLE)

