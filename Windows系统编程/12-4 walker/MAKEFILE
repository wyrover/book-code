# Nmake macros for building Windows 32-Bit apps

TARGETOS=WINNT
SEHMAP=TRUE

!include <ntwin32.mak>

!if "$(CPU)" == "i386"
cflags = $(cflags) -D_CRTAPI1=_cdecl -D_CRTAPI2=_cdecl
!else
cflags = $(cflags) -D_CRTAPI1= -D_CRTAPI2=
!endif

all: pwalk.exe

# object files for pwalk.exe
pstat.obj : pstat.c pwalk.h
    $(cc) $(cflags) $(cvars) $(cdebug) pstat.c

pdebug.obj : pdebug.c pwalk.h
    $(cc) $(cflags) $(cvars) $(cdebug) pdebug.c

pwalkio.obj : pwalkio.c pwalk.h
    $(cc) $(cflags) $(cvars) $(cdebug) pwalkio.c

pwalk.obj : pwalk.c pwalk.h
    $(cc) $(cflags) $(cvars) $(cdebug) pwalk.c

pview.obj : pview.c pwalk.h
    $(cc) $(cflags) $(cvars) $(cdebug) pview.c


# object files for pefile.dll
pefile.obj : pefile.c pefile.h
    $(cc) $(cflags) $(cvars) $(cdebug) pefile.c


# object files for probe.dll
probe.obj: probe.c pwalk.h
    $(cc) $(cflags) $(cvarsdll) $(cdebug) probe.c


# resrouces for pwalk.exe
pwalk.res: pwalk.rc pwalk.h pwalk.ico
    rc -r pwalk.rc

# resrouces for probe.dll
probe.res: probe.rc pwalk.h
    rc -r probe.rc

# resrouces for pefile.dll
pefile.res: pefile.rc pefile.h
    rc -r pefile.rc


# export library for probe.dll
probe.lib: probe.obj probe.def probe.res
    $(implib) -machine:$(CPU)	 \
    -def:probe.def		 \
    probe.obj			 \
    -out:probe.lib


# export library for pefile.dll
pefile.lib: pefile.obj pefile.def pefile.res
    $(implib) -machine:$(CPU)	 \
    -def:pefile.def		 \
    pefile.obj			 \
    -out:pefile.lib


# link probe.dll
probe.dll: probe.obj probe.def probe.lib pefile.lib pefile.dll
    $(link) $(linkdebug) $(dlllflags)	 \
    -base:0x1C000000		 \
    -out:probe.dll		 \
    probe.exp pefile.lib probe.obj probe.res $(guilibsdll)


# link pefile.dll
pefile.dll: pefile.obj pefile.def pefile.lib
    $(link) $(linkdebug) $(dlllflags)	 \
    -base:0x1D000000		 \
    -out:pefile.dll		 \
    pefile.exp pefile.obj pefile.res $(guilibsdll)


# link pwalk.exe
pwalk.exe: pwalk.res pwalk.obj probe.lib probe.dll pefile.lib pefile.dll \
	   pwalkio.obj pdebug.obj pstat.obj pview.obj
    $(link) $(linkdebug) $(lflags)	 \
    $(guiflags) 		 \
    -out:pwalk.exe		 \
    pwalk.obj pdebug.obj probe.lib pefile.lib pstat.obj pwalkio.obj pview.obj pwalk.res $(guilibs)
