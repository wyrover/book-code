# Nmake macros for building Windows 32-Bit apps

!include <ntwin32.mak>

all: hook.lib hook.dll

# Update the object files if necessary

hook.obj: hook.c
    $(cc) $(scall) $(cflags) $(cvarsdll) $(cdebug) hook.c

# Update the import library

hook.lib: hook.obj hook.def
    $(implib) -machine:$(CPU)     \
    -def:hook.def           \
    hook.obj                \
    -out:hook.lib

# Update the dynamic link library

hook.dll: hook.obj hook.def
    $(link) $(linkdebug) $(dlllflags)     \
    -base:0x1C000000  \
    -out:hook.dll   \
    hook.exp hook.obj $(guilibsdll)

