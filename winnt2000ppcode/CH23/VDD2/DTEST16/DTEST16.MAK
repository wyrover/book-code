##	DTEST16.MAK - Make file for DTEST16.EXE 

all: dtest16.exe

dtest16.obj : dtest16.c
	cl -AM -c -D -Gs -Zi -Ox -W2 dtest16.c

dtest16.exe : dtest16.obj
	link dtest16,dtest16.exe,,mlibce;

## end of file
