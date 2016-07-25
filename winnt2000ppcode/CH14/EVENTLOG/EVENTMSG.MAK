!include <win32.mak>

all: eventmsg.dll

eventmsg.obj: $*.c $*.h
  $(cc) $(cflags) $(cdebug) $(cvarsdll) $*.c

eventmsg.dll: eventmsg.obj eventmsg.res
    $(link) $(linkdebug) $(dlllflags) \
    eventmsg.obj eventmsg.res $(guilibsdll) version.lib \
    -dll                      \
    -entry:DllMain$(DLLENTRY) \
    -out:eventmsg.dll $(MAPFILE)

eventmsg.rc: $*.mc
  mc -v $*.mc

eventmsg.h: $*.rc

.rc.rbj:
    $(rc) -r -fo $*.res $*.rc
