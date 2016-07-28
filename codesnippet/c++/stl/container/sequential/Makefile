CC=g++
OBJ=list_example.o string_example.o vector_example.o
EXE=list_example string_example vector_example

# By default, make starts with the first target (not targets whose names start with ‘.’)
# The recompilation must be done if the source file, or any of the header files named as prerequisites, 
# is more recent than the object file, or if the object file does not exist.
all : $(EXE)
	./list_example
	./string_example
	./vector_example

# implicit rules are so convenient

list_example : list_example.o
#	$(CC) list_example.o -o list_example
string_example : string_example.o
#	$(CC) string_example.o -o string_example
vector_example : vector_example.o
#	$(CC) vector_example.o -o vector_example

# 
list_example.o : 
#	$(CC) -c list_example.cpp
string_example.o : 
#	$(CC) -c string_example.cpp
vector_example.o : 
#	$(CC) -c vector_example.cpp

#.PHONY : clean
# ignore the error for clean, alternative method is "rm -f $(OBJ) $(EXE)

# %.o : %.c

clean :
	-rm -f $(OBJ) $(EXE)
