# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=print - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to print - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "print - Win32 Release" && "$(CFG)" != "print - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "print.mak" CFG="print - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "print - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "print - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "print - Win32 Debug"
RSC=rc.exe
MTL=mktyplib.exe
CPP=cl.exe

!IF  "$(CFG)" == "print - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\print.dll" "$(OUTDIR)\PRINTMC.rc" "$(OUTDIR)\PRINTMC.h"

CLEAN : 
	-@erase "$(INTDIR)\center.obj"
	-@erase "$(INTDIR)\printit.obj"
	-@erase "$(INTDIR)\PRINTMC.h"
	-@erase "$(INTDIR)\PRINTMC.rc"
	-@erase "$(OUTDIR)\print.dll"
	-@erase "$(OUTDIR)\print.exp"
	-@erase "$(OUTDIR)\print.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "print___" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "print___" /D "WIN32" /D "NDEBUG" /D\
 "_WINDOWS" /Fp"$(INTDIR)/print.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "print___" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/print.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/print.pdb" /machine:I386 /out:"$(OUTDIR)/print.dll"\
 /implib:"$(OUTDIR)/print.lib" 
LINK32_OBJS= \
	"$(INTDIR)\center.obj" \
	"$(INTDIR)\printit.obj"

"$(OUTDIR)\print.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "print - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "print___"
# PROP BASE Intermediate_Dir "print___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "print___"
# PROP Intermediate_Dir "print___"
# PROP Target_Dir ""
OUTDIR=.\print___
INTDIR=.\print___

ALL : "$(OUTDIR)\print.dll" "$(OUTDIR)\PRINTMC.rc"

CLEAN : 
	-@erase "$(INTDIR)\center.obj"
	-@erase "$(INTDIR)\printit.obj"
	-@erase "$(INTDIR)\PRINTMC.h"
	-@erase "$(INTDIR)\PRINTMC.rc"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\print.dll"
	-@erase "$(OUTDIR)\print.exp"
	-@erase "$(OUTDIR)\print.ilk"
	-@erase "$(OUTDIR)\print.lib"
	-@erase "$(OUTDIR)\print.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "print___" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /I "print___" /D "WIN32" /D "_DEBUG"\
 /D "_WINDOWS" /Fp"$(INTDIR)/print.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\print___/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "print___" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/print.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/print.pdb" /debug /machine:I386 /out:"$(OUTDIR)/print.dll"\
 /implib:"$(OUTDIR)/print.lib" 
LINK32_OBJS= \
	"$(INTDIR)\center.obj" \
	"$(INTDIR)\printit.obj"

"$(OUTDIR)\print.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "print - Win32 Release"
# Name "print - Win32 Debug"

!IF  "$(CFG)" == "print - Win32 Release"

!ELSEIF  "$(CFG)" == "print - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\resource.h

!IF  "$(CFG)" == "print - Win32 Release"

!ELSEIF  "$(CFG)" == "print - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\printmc.h

!IF  "$(CFG)" == "print - Win32 Release"

!ELSEIF  "$(CFG)" == "print - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PRINTIT.H

!IF  "$(CFG)" == "print - Win32 Release"

!ELSEIF  "$(CFG)" == "print - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\printit.c
DEP_CPP_PRINT=\
	".\CENTER.H"\
	".\print___\printmc.h"\
	".\PRINTIT.H"\
	

"$(INTDIR)\printit.obj" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\print.rc
# PROP Exclude_From_Build 1
# End Source File
################################################################################
# Begin Source File

SOURCE=.\CENTER.H

!IF  "$(CFG)" == "print - Win32 Release"

!ELSEIF  "$(CFG)" == "print - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\center.c

"$(INTDIR)\center.obj" : $(SOURCE) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PRINTMC.MC

!IF  "$(CFG)" == "print - Win32 Release"

# Begin Custom Build - Compiling message file
IntDir=.\Release
InputPath=.\PRINTMC.MC
InputName=PRINTMC

BuildCmds= \
	mc -h $(IntDir) -r $(IntDir) $(InputName) \
	

"$(IntDir)\$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(IntDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "print - Win32 Debug"

# Begin Custom Build - Compiling message file
IntDir=.\print___
InputPath=.\PRINTMC.MC
InputName=PRINTMC

BuildCmds= \
	mc -h $(IntDir) -r $(IntDir) $(InputName) \
	

"$(IntDir)\$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(IntDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
