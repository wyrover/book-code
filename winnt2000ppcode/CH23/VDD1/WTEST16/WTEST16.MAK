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
PROJPATH = 
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC =             
FIRSTCPP =             
RC = rc
CFLAGS_D_WEXE = /nologo /W3 /FR /G2 /Zi /D_DEBUG /Od /AM /GA /Fd"WTEST16.PDB"
CFLAGS_R_WEXE = /nologo /W3 /FR /O1 /DNDEBUG /AM /GA
LFLAGS_D_WEXE = /NOLOGO /ONERROR:NOEXE /NOD /PACKC:61440 /CO /ALIGN:16 /STACK:10240
LFLAGS_R_WEXE = /NOLOGO /ONERROR:NOEXE /NOD /PACKC:61440 /ALIGN:16 /STACK:10240
LIBS_D_WEXE = oldnames libw commdlg shell olecli olesvr mlibcew
LIBS_R_WEXE = oldnames libw commdlg shell olecli olesvr mlibcew
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
DEFFILE = WTEST16.DEF
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
SBRS = WTEST16.SBR


# edit paths!
WTEST16_DEP = c:\ \
	c:\ \
	c:\isvbopx.h


WTEST16_RCDEP = c:\wtest16.h


all:    $(PROJ).EXE $(PROJ).BSC

WTEST16.OBJ:    WTEST16.C $(WTEST16_DEP)
	$(CC) $(CFLAGS) $(CUSEPCHFLAG) /c WTEST16.C

WTEST16.RES:    WTEST16.RC $(WTEST16_RCDEP)
	$(RC) $(RCFLAGS) $(RCDEFINES) -r WTEST16.RC


$(PROJ).EXE::   WTEST16.RES

$(PROJ).EXE::   WTEST16.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
WTEST16.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
# edit paths!
c:\dev\msvc\lib\+
c:\dev\msvc\mfc\lib\+
c:\dev\ddk\lib\+
c:\hendrix\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) WTEST16.RES $@
	@copy $(PROJ).CRF MSVC.BND

$(PROJ).EXE::   WTEST16.RES
	if not exist MSVC.BND   $(RC) $(RESFLAGS) WTEST16.RES $@

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
