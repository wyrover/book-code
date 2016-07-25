##	IO16.MAK - Make file for IO16.EXE 

all: io16.exe

io16.obj : io16.c
	cl -AM -c -D -Gs -Zi -Ox -W2 io16.c

io16.exe : io16.obj
	link io16,io16.exe,,mlibce;

## end of file
