CXX = mpic++
CXXFLAGS = -std=c++11 -g3 -Wall 
MKDIR = mkdir -p

LIBS = -lm -lmpi 

directorios:
	$(MKDIR) build dist

main.o: directorios main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o build/main.o

all: clean main.o
	$(CXX) $(CXXFLAGS) -o dist/programa build/main.o $(LIBS)
	rm -fr build

clean:
	rm -fr *.o *.png a.out core programa dist build

.DEFAULT_GOAL := all
