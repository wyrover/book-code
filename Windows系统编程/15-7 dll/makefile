# Nmake macros for building Windows 32-Bit apps

!include <ntwin32.mak>

all: demo.exe select.dll

# Update the object files if necessary

demo.obj: demo.c
    $(cc) $(cflags) $(cvarsdll) $(cdebug) demo.c

select.obj: select.c
    $(cc) $(cflags) $(cvarsdll) $(cdebug) select.c

# Update the resources if necessary

demo.res: demo.rc demo.h
    $(rc) $(rcvars) -r demo.rc

# Update the import library

select.lib: select.obj select.def
    $(implib) -machine:$(CPU)     \
    -def:select.def	    \
    select.obj	 	    \
    -out:select.lib

# Update the dynamic link library

select.dll: select.obj select.def
    $(link) $(linkdebug) $(dlllflags)     \
    -base:0x1C000000  \
    -out:select.dll   \
    select.exp select.obj $(guilibsdll)


# Update the executable file if necessary.
# If so, add the resource back in.

demo.exe: demo.obj select.lib demo.res demo.def
    $(link) $(linkdebug) $(guiflags) -out:demo.exe demo.obj select.lib demo.res $(guilibsdll)
