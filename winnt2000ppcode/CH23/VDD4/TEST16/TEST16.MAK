##	TEST16.MAK - Make file for TEST16.EXE sample program

all: test16.exe

test16.obj : test16.c
	cl -AM -c -D -Gs -Zi -Ox -W2 test16.c

test16.exe : test16.obj
	link test16,test16.exe,,mlibce;
