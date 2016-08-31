# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=CursorIconExplorer - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to CursorIconExplorer - Win32\
 Debug.
!ENDIF 

!IF "$(CFG)" != "CursorIconExplorer - Win32 Release" && "$(CFG)" !=\
 "CursorIconExplorer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "CursorIconExplorer.mak"\
 CFG="CursorIconExplorer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CursorIconExplorer - Win32 Release" (based on\
 "Win32 (x86) Application")
!MESSAGE "CursorIconExplorer - Win32 Debug" (based on\
 "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "CursorIconExplorer - Win32 Release"
MTL=mktyplib.exe
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "CursorIconExplorer - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\CursorIconExplorer.exe" "$(OUTDIR)\CursorIconExplorer.hlp"\
 ".\CursorIconMessages.rc"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\Cur.obj"
	-@erase "$(INTDIR)\CursorIconExplorer.hlp"
	-@erase "$(INTDIR)\CursorIconExplorer.obj"
	-@erase "$(INTDIR)\CursorIconExplorer.pch"
	-@erase "$(INTDIR)\CursorIconExplorer.res"
	-@erase "$(INTDIR)\CursorIconExplorerDoc.obj"
	-@erase "$(INTDIR)\CursorIconExplorerView.obj"
	-@erase "$(INTDIR)\FmtMsg.obj"
	-@erase "$(INTDIR)\Icon.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MessageTable.obj"
	-@erase "$(INTDIR)\OBM.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\CursorIconExplorer.exe"
	-@erase ".\CursorIconMessages.h"
	-@erase ".\CursorIconMessages.rc"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/CursorIconExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/CursorIconExplorer.res" /d "NDEBUG" /d\
 "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/CursorIconExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/CursorIconExplorer.pdb" /machine:I386\
 /out:"$(OUTDIR)/CursorIconExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\Cur.obj" \
	"$(INTDIR)\CursorIconExplorer.obj" \
	"$(INTDIR)\CursorIconExplorer.res" \
	"$(INTDIR)\CursorIconExplorerDoc.obj" \
	"$(INTDIR)\CursorIconExplorerView.obj" \
	"$(INTDIR)\FmtMsg.obj" \
	"$(INTDIR)\Icon.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MessageTable.obj" \
	"$(INTDIR)\OBM.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\CursorIconExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "CursorIconExplorer - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\CursorIconExplorer.exe" "$(OUTDIR)\CursorIconExplorer.hlp"\
 ".\CursorIconMessages.rc"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\Cur.obj"
	-@erase "$(INTDIR)\CursorIconExplorer.hlp"
	-@erase "$(INTDIR)\CursorIconExplorer.obj"
	-@erase "$(INTDIR)\CursorIconExplorer.pch"
	-@erase "$(INTDIR)\CursorIconExplorer.res"
	-@erase "$(INTDIR)\CursorIconExplorerDoc.obj"
	-@erase "$(INTDIR)\CursorIconExplorerView.obj"
	-@erase "$(INTDIR)\FmtMsg.obj"
	-@erase "$(INTDIR)\Icon.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MessageTable.obj"
	-@erase "$(INTDIR)\OBM.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\CursorIconExplorer.exe"
	-@erase "$(OUTDIR)\CursorIconExplorer.ilk"
	-@erase "$(OUTDIR)\CursorIconExplorer.pdb"
	-@erase ".\CursorIconMessages.h"
	-@erase ".\CursorIconMessages.rc"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/CursorIconExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/CursorIconExplorer.res" /d "_DEBUG" /d\
 "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/CursorIconExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/CursorIconExplorer.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/CursorIconExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\Cur.obj" \
	"$(INTDIR)\CursorIconExplorer.obj" \
	"$(INTDIR)\CursorIconExplorer.res" \
	"$(INTDIR)\CursorIconExplorerDoc.obj" \
	"$(INTDIR)\CursorIconExplorerView.obj" \
	"$(INTDIR)\FmtMsg.obj" \
	"$(INTDIR)\Icon.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MessageTable.obj" \
	"$(INTDIR)\OBM.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\CursorIconExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "CursorIconExplorer - Win32 Release"
# Name "CursorIconExplorer - Win32 Debug"

!IF  "$(CFG)" == "CursorIconExplorer - Win32 Release"

!ELSEIF  "$(CFG)" == "CursorIconExplorer - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "CursorIconExplorer - Win32 Release"

!ELSEIF  "$(CFG)" == "CursorIconExplorer - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CursorIconExplorer.cpp
DEP_CPP_CURSO=\
	".\About.h"\
	".\ChildFrm.h"\
	".\CursorIconExplorer.h"\
	".\CursorIconExplorerDoc.h"\
	".\CursorIconExplorerView.h"\
	".\MainFrm.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CursorIconExplorer.obj" : $(SOURCE) $(DEP_CPP_CURSO) "$(INTDIR)"\
 "$(INTDIR)\CursorIconExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "CursorIconExplorer - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/CursorIconExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\CursorIconExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "CursorIconExplorer - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/CursorIconExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\CursorIconExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\Cur.h"\
	".\CursorIconExplorer.h"\
	".\FmtMsg.h"\
	".\Icon.h"\
	".\MainFrm.h"\
	".\MessageTable.h"\
	".\OBM.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\CursorIconExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ChildFrm.cpp
DEP_CPP_CHILD=\
	".\ChildFrm.h"\
	".\CursorIconExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\CursorIconExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CursorIconExplorerDoc.cpp
DEP_CPP_CURSOR=\
	".\CursorIconExplorer.h"\
	".\CursorIconExplorerDoc.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CursorIconExplorerDoc.obj" : $(SOURCE) $(DEP_CPP_CURSOR) "$(INTDIR)"\
 "$(INTDIR)\CursorIconExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CursorIconExplorerView.cpp
DEP_CPP_CURSORI=\
	".\CursorIconExplorer.h"\
	".\CursorIconExplorerDoc.h"\
	".\CursorIconExplorerView.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CursorIconExplorerView.obj" : $(SOURCE) $(DEP_CPP_CURSORI)\
 "$(INTDIR)" "$(INTDIR)\CursorIconExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CursorIconExplorer.rc
DEP_RSC_CURSORIC=\
	".\res\CursorIconExplorer.ico"\
	".\res\CursorIconExplorer.rc2"\
	".\res\CursorIconExplorerDoc.ico"\
	".\res\icon1.ico"\
	".\res\jmnco.ico"\
	".\res\Toolbar.bmp"\
	

"$(INTDIR)\CursorIconExplorer.res" : $(SOURCE) $(DEP_RSC_CURSORIC) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\hlp\CursorIconExplorer.hpj

!IF  "$(CFG)" == "CursorIconExplorer - Win32 Release"

# Begin Custom Build - Making help file...
OutDir=.\Release
ProjDir=.
TargetName=CursorIconExplorer
InputPath=.\hlp\CursorIconExplorer.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "CursorIconExplorer - Win32 Debug"

# Begin Custom Build - Making help file...
OutDir=.\Debug
ProjDir=.
TargetName=CursorIconExplorer
InputPath=.\hlp\CursorIconExplorer.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Cur.cpp
DEP_CPP_CUR_C=\
	".\Cur.h"\
	".\CursorIconExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Cur.obj" : $(SOURCE) $(DEP_CPP_CUR_C) "$(INTDIR)"\
 "$(INTDIR)\CursorIconExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.cpp
DEP_CPP_ABOUT=\
	".\About.h"\
	".\CursorIconExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"\
 "$(INTDIR)\CursorIconExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FmtMsg.cpp
DEP_CPP_FMTMS=\
	".\CursorIconExplorer.h"\
	".\FmtMsg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FmtMsg.obj" : $(SOURCE) $(DEP_CPP_FMTMS) "$(INTDIR)"\
 "$(INTDIR)\CursorIconExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Icon.cpp
DEP_CPP_ICON_=\
	".\CursorIconExplorer.h"\
	".\Icon.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Icon.obj" : $(SOURCE) $(DEP_CPP_ICON_) "$(INTDIR)"\
 "$(INTDIR)\CursorIconExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MessageTable.cpp
DEP_CPP_MESSA=\
	".\CursorIconExplorer.h"\
	".\CursorIconMessages.h"\
	".\MessageTable.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MessageTable.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"\
 "$(INTDIR)\CursorIconExplorer.pch" ".\CursorIconMessages.h"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\OBM.cpp
DEP_CPP_OBM_C=\
	".\CursorIconExplorer.h"\
	".\OBM.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\OBM.obj" : $(SOURCE) $(DEP_CPP_OBM_C) "$(INTDIR)"\
 "$(INTDIR)\CursorIconExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CursorIconMessages.mc

!IF  "$(CFG)" == "CursorIconExplorer - Win32 Release"

# Begin Custom Build - Compiling Message Table
ProjDir=.
InputPath=.\CursorIconMessages.mc
InputName=CursorIconMessages

BuildCmds= \
	mc -v -r $(ProjDir) -h $(ProjDir) $(InputName).mc \
	

"$(ProjDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(ProjDir)\$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ELSEIF  "$(CFG)" == "CursorIconExplorer - Win32 Debug"

# Begin Custom Build - Compiling Message Table
ProjDir=.
InputPath=.\CursorIconMessages.mc
InputName=CursorIconMessages

BuildCmds= \
	mc -v -r $(ProjDir) -h $(ProjDir) $(InputName).mc \
	

"$(ProjDir)\$(InputName).h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)

"$(ProjDir)\$(InputName).rc" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   $(BuildCmds)
# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
