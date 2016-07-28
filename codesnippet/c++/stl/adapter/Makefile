EXE = queue_example stack_example
OBJ = queue_example.o stack_example.o
CC = g++

all : queue_example  stack_example
	./queue_example
	./stack_example

queue_example : queue_example.o

queue_example.o : 

stack_example.o :

.PHONY : clean

clean : 
	-rm  -f $(EXE) $(OBJ)
