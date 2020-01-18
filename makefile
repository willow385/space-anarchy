CPP = g++

SRC = src/main.cpp\
      src/Universe.cpp\
      src/Universe.h

TARGET = bin/space_anarchy.x86

FLAGS = -O3 -std=c++11
LD_FLAGS = -ldjf-3d-2 -lSDL2

$(TARGET): $(SRC)
	$(CPP) $(FLAGS) -c src/Universe.cpp -o objs/Universe.o
