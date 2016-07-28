CC=g++
CPPFLAGS=-Wall -std=c++11 -ggdb

EXE=HelloWorldThread MultipleArgumentsThread SingleArgumentThread FunctionObjectThread FunctionObjectByRefThread
OBJ=HelloWorldThread.o MultipleArgumentsThread.o SingleArgumentThread.o FunctionObjectThread.o FunctionObjectByRefThread.o

all : $(EXE)
	for target in $(EXE); do \
		./$${target}; \
	done

HelloWorldThread : HelloWorldThread.o

MultipleArgumentsThread : MultipleArgumentsThread.o

SingleArgumentThread : SingleArgumentThread.o

FunctionObjectThread : FunctionObjectThread.o

FunctionObjectByRefThread : FunctionObjectByRefThread.o

HelloWorldThread.o : 

SingleArgumentThread.o : 

MultipleArgumentsThread.o : 

FunctionObjectThread.o :

FunctionObjectByRefThread.o : 

.PHONY: clean

clean:
	-rm -f $(EXE) $(OBJ)
