# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=Window Explorer - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Window Explorer - Win32\
 Debug.
!ENDIF 

!IF "$(CFG)" != "Window Explorer - Win32 Release" && "$(CFG)" !=\
 "Window Explorer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "WindowExplorer.mak" CFG="Window Explorer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Window Explorer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Window Explorer - Win32 Debug" (based on "Win32 (x86) Application")
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
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "Window Explorer - Win32 Release"

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

ALL : "$(OUTDIR)\WindowExplorer.exe" "$(OUTDIR)\WindowExplorer.hlp"

CLEAN : 
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\WindowExplorer.hlp"
	-@erase "$(INTDIR)\WindowExplorer.obj"
	-@erase "$(INTDIR)\WindowExplorer.pch"
	-@erase "$(INTDIR)\WindowExplorer.res"
	-@erase "$(INTDIR)\WindowExplorerDoc.obj"
	-@erase "$(INTDIR)\WindowExplorerView.obj"
	-@erase "$(OUTDIR)\WindowExplorer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/WindowExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/WindowExplorer.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/WindowExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/WindowExplorer.pdb" /machine:I386\
 /out:"$(OUTDIR)/WindowExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\WindowExplorer.obj" \
	"$(INTDIR)\WindowExplorer.res" \
	"$(INTDIR)\WindowExplorerDoc.obj" \
	"$(INTDIR)\WindowExplorerView.obj"

"$(OUTDIR)\WindowExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Window Explorer - Win32 Debug"

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

ALL : "$(OUTDIR)\WindowExplorer.exe" "$(OUTDIR)\WindowExplorer.hlp"

CLEAN : 
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\WindowExplorer.hlp"
	-@erase "$(INTDIR)\WindowExplorer.obj"
	-@erase "$(INTDIR)\WindowExplorer.pch"
	-@erase "$(INTDIR)\WindowExplorer.res"
	-@erase "$(INTDIR)\WindowExplorerDoc.obj"
	-@erase "$(INTDIR)\WindowExplorerView.obj"
	-@erase "$(OUTDIR)\WindowExplorer.exe"
	-@erase "$(OUTDIR)\WindowExplorer.ilk"
	-@erase "$(OUTDIR)\WindowExplorer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/WindowExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/WindowExplorer.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/WindowExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/WindowExplorer.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/WindowExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\WindowExplorer.obj" \
	"$(INTDIR)\WindowExplorer.res" \
	"$(INTDIR)\WindowExplorerDoc.obj" \
	"$(INTDIR)\WindowExplorerView.obj"

"$(OUTDIR)\WindowExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "Window Explorer - Win32 Release"
# Name "Window Explorer - Win32 Debug"

!IF  "$(CFG)" == "Window Explorer - Win32 Release"

!ELSEIF  "$(CFG)" == "Window Explorer - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "Window Explorer - Win32 Release"

!ELSEIF  "$(CFG)" == "Window Explorer - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=".\WindowExplorer.cpp"

"$(INTDIR)\WindowExplorer.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Window Explorer - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/WindowExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\WindowExplorer.pch" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Window Explorer - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/WindowExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\WindowExplorer.pch" : $(SOURCE) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp

"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ChildFrm.cpp

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=".\WindowExplorerDoc.cpp"

"$(INTDIR)\WindowExplorerDoc.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=".\WindowExplorerView.cpp"

"$(INTDIR)\WindowExplorerView.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=".\WindowExplorer.rc"

"$(INTDIR)\WindowExplorer.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=".\hlp\WindowExplorer.hpj"

!IF  "$(CFG)" == "Window Explorer - Win32 Release"

# Begin Custom Build - Making help file...
OutDir=.\Release
ProjDir=.
TargetName=WindowExplorer
InputPath=.\hlp\WindowExplorer.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "Window Explorer - Win32 Debug"

# Begin Custom Build - Making help file...
OutDir=.\Debug
ProjDir=.
TargetName=WindowExplorer
InputPath=.\hlp\WindowExplorer.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
