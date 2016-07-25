!include <ntwin32.mak>

all: cons32.exe

cons32.obj: cons32.c
	$(cc) $(cflags) $(cvars) cons32.c
	cvtomf cons32.obj

cons32.exe: cons32.obj hpscan32.lib
	link $(conflags) \
	-out:cons32.exe cons32.obj hpscan32.lib $(conlibs)
