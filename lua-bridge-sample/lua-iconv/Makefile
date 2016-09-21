# luaiconv - Performs character set conversions in Lua
# (c) 2005-08 Alexandre Erwin Ittner <aittner@gmail.com>
#

CC = gcc
AR = ar
# RM = rm
RM = del


# Gives a nice speedup, but also spoils debugging on x86. Comment out this
# line when debugging.
# OMIT_FRAME_POINTER = -fomit-frame-pointer

CFLAGS = -O3 -I. -I../../src/scite/lua/include

# comma separated list (e.g. "iconv.dll,libiconv.dll")
# DEFAULT_LIBICONV_DLL ?= \"\"
# CFLAGS += -pedantic -Wall
# CFLAGS += -DUSE_LIBICONV_DLL
# CFLAGS += -DDEFAULT_LIBICONV_DLL=$(DEFAULT_LIBICONV_DLL)

LFLAGS = -shared $(OMIT_FRAME_POINTER)

LIBS = -L../lib/ -lscite

all: iconv.dll

rebuild: clean all

luaiconv.o: luaiconv.c
	$(CC) -o luaiconv.o -c $(CFLAGS) luaiconv.c

iconv.dll: luaiconv.o libiconv.a
	$(CC) -s -o iconv.dll $(LFLAGS) luaiconv.o libiconv.a $(LIBS)

libiconv.a: win_iconv.c
	$(CC) $(CFLAGS) -c win_iconv.c
	$(AR) rcs libiconv.a win_iconv.o
	ranlib libiconv.a

clean:
	-$(RM) *.o
	-$(RM) libiconv.a
