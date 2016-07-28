all: sum.dll

sum.o: sum.c
	gcc	-c sum.c 

sum.dll:	sum.o
	gcc	-shared	-o sum.dll sum.o	

clean:
	@del sum.o
	@del sum.dll