# Microsoft Visual C++ generated build script - Do not modify

PROJ = WTEST16
DEBUG = 1
PROGTYPE = 0
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = -d_DEBUG
R_RCDEFINES = -dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\ARTICLE\WVERSION\SRC\
USEMFC = 1
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = WTEST.C     
FIRSTCPP =             
RC = rc
CFLAGS_D_WEXE = /nologo /W3 /FR /G2 /Zi /D_DEBUG /Od /AM /GA /Fd"WTEST16.PDB"
CFLAGS_R_WEXE = /nologo /W3 /FR /O1 /DNDEBUG /AM /GA
LFLAGS_D_WEXE = /NOLOGO /NOD /PACKC:61440 /STACK:10240 /ALIGN:16 /ONERROR:NOEXE /CO  
LFLAGS_R_WEXE = /NOLOGO /NOD /PACKC:61440 /STACK:10240 /ALIGN:16 /ONERROR:NOEXE  
LIBS_D_WEXE = mafxcwd oldnames libw mlibcew commdlg.lib olecli.lib olesvr.lib shell.lib ver.lib 
LIBS_R_WEXE = mafxcw oldnames libw mlibcew commdlg.lib olecli.lib olesvr.lib shell.lib 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
DEFFILE = WTEST.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WEXE)
LFLAGS = $(LFLAGS_D_WEXE)
LIBS = $(LIBS_D_WEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WEXE)
LFLAGS = $(LFLAGS_R_WEXE)
LIBS = $(LIBS_R_WEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = WTEST.SBR \
		WVERSION.SBR


WTEST_DEP = c:\article\wversion\src\wtest.h \
	c:\article\wversion\src\wversion.h


WTEST_RCDEP = c:\article\wversion\src\wtest.h \
	c:\article\wversion\src\wversion.h


WVERSION_DEP = c:\article\wversion\src\wversion.h


all:	$(PROJ).EXE $(PROJ).BSC

WTEST.OBJ:	WTEST.C $(WTEST_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c WTEST.C

WTEST.RES:	WTEST.RC $(WTEST_RCDEP)
	$(RC) $(RCFLAGS) $(RCDEFINES) -r WTEST.RC

WVERSION.OBJ:	WVERSION.C $(WVERSION_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c WVERSION.C


$(PROJ).EXE::	WTEST.RES

$(PROJ).EXE::	WTEST.OBJ WVERSION.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
WTEST.OBJ +
WVERSION.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\dev\msvc\lib\+
c:\dev\msvc\mfc\lib\+
c:\dev\ddk\lib\+
c:\hendrix\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) WTEST.RES $@
	@copy $(PROJ).CRF MSVC.BND

$(PROJ).EXE::	WTEST.RES
	if not exist MSVC.BND 	$(RC) $(RESFLAGS) WTEST.RES $@

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
