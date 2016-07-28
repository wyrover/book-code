CC=g++
OBJ=map_example.o set_example.o
EXE=map_example set_example
all : map_example set_example
	./map_example
	./set_example

map_example : map_example.o
	$(CC) map_example.o -o map_example
set_example : set_example.o
	$(CC) set_example.o -o set_example

map_example.o : map_example.cpp
	$(CC) -c map_example.cpp

set_example.o : set_example.cpp
	$(CC) -c set_example.cpp

.PHONY : clean

clean :
	-rm -f $(OBJ) $(EXE)
