gcc -O2 -Wall -c *.c
ren lua.o lua.obj
ren luac.o luac.obj
ren print.o print.obj
gcc -shared -Wl,--export-all-symbols -o lua5.1.dll *.o
strip --strip-unneeded lua5.1.dll
gcc -o lua.exe -s lua.obj lua5.1.dll -lm
gcc -o luac.exe -s -static luac.obj print.obj *.o -lm
