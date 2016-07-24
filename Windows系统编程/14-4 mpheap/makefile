
TARGETOS=WINNT
SEHMAP=TRUE
!include <win32.mak>

!IF "$(TARGETLANG)" == "LANG_JAPANESE"
all: WARN_MSG
!ELSE
all: mpheap.dll tmpheap.exe
!ENDIF


WARN_MSG:
 @echo "Warning:  MPHEAP sample works only on Windows NT 4.0"



# Inference rule for updating the object files
tmpheap.obj: tmpheap.c
  $(cc) $(cdebug) $(cflags) $(cvars) $*.c

mpheap.obj: mpheap.c
  $(cc) $(cdebug) $(cflags) $(cvarsdll) $*.c

# Build rule for EXE
tmpheap.exe: tmpheap.obj mpheap.lib
    $(link) $(linkdebug) $(conlflags) $(conlibs) $** -out:tmpheap.exe

# linker will build the import library as well
mpheap.dll: mpheap.obj
    $(link) $(linkdebug) $(dlllflags) $(conlibsdll) $** -out:mpheap.dll -def:mpheap.def
