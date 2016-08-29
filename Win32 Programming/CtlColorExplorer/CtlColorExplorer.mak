# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=CtlColorExplorer - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to CtlColorExplorer - Win32\
 Debug.
!ENDIF 

!IF "$(CFG)" != "CtlColorExplorer - Win32 Release" && "$(CFG)" !=\
 "CtlColorExplorer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "CtlColorExplorer.mak" CFG="CtlColorExplorer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CtlColorExplorer - Win32 Release" (based on\
 "Win32 (x86) Application")
!MESSAGE "CtlColorExplorer - Win32 Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "CtlColorExplorer - Win32 Release"

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

ALL : "$(OUTDIR)\CtlColorExplorer.exe" "$(OUTDIR)\CtlColorExplorer.hlp"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\CBTMsg.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\Color.obj"
	-@erase "$(INTDIR)\CtlColor.obj"
	-@erase "$(INTDIR)\CtlColorExplorer.hlp"
	-@erase "$(INTDIR)\CtlColorExplorer.obj"
	-@erase "$(INTDIR)\CtlColorExplorer.pch"
	-@erase "$(INTDIR)\CtlColorExplorer.res"
	-@erase "$(INTDIR)\CtlColorExplorerDoc.obj"
	-@erase "$(INTDIR)\CtlColorExplorerView.obj"
	-@erase "$(INTDIR)\error.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\RGB.obj"
	-@erase "$(INTDIR)\Sampler.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\CtlColorExplorer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/CtlColorExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/CtlColorExplorer.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/CtlColorExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/CtlColorExplorer.pdb" /machine:I386\
 /out:"$(OUTDIR)/CtlColorExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\CBTMsg.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\Color.obj" \
	"$(INTDIR)\CtlColor.obj" \
	"$(INTDIR)\CtlColorExplorer.obj" \
	"$(INTDIR)\CtlColorExplorer.res" \
	"$(INTDIR)\CtlColorExplorerDoc.obj" \
	"$(INTDIR)\CtlColorExplorerView.obj" \
	"$(INTDIR)\error.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\RGB.obj" \
	"$(INTDIR)\Sampler.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\CtlColorExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "CtlColorExplorer - Win32 Debug"

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

ALL : "$(OUTDIR)\CtlColorExplorer.exe" "$(OUTDIR)\CtlColorExplorer.hlp"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\CBTMsg.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\Color.obj"
	-@erase "$(INTDIR)\CtlColor.obj"
	-@erase "$(INTDIR)\CtlColorExplorer.hlp"
	-@erase "$(INTDIR)\CtlColorExplorer.obj"
	-@erase "$(INTDIR)\CtlColorExplorer.pch"
	-@erase "$(INTDIR)\CtlColorExplorer.res"
	-@erase "$(INTDIR)\CtlColorExplorerDoc.obj"
	-@erase "$(INTDIR)\CtlColorExplorerView.obj"
	-@erase "$(INTDIR)\error.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\RGB.obj"
	-@erase "$(INTDIR)\Sampler.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\CtlColorExplorer.exe"
	-@erase "$(OUTDIR)\CtlColorExplorer.ilk"
	-@erase "$(OUTDIR)\CtlColorExplorer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/CtlColorExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/CtlColorExplorer.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/CtlColorExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/CtlColorExplorer.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/CtlColorExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\CBTMsg.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\Color.obj" \
	"$(INTDIR)\CtlColor.obj" \
	"$(INTDIR)\CtlColorExplorer.obj" \
	"$(INTDIR)\CtlColorExplorer.res" \
	"$(INTDIR)\CtlColorExplorerDoc.obj" \
	"$(INTDIR)\CtlColorExplorerView.obj" \
	"$(INTDIR)\error.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\RGB.obj" \
	"$(INTDIR)\Sampler.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\CtlColorExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "CtlColorExplorer - Win32 Release"
# Name "CtlColorExplorer - Win32 Debug"

!IF  "$(CFG)" == "CtlColorExplorer - Win32 Release"

!ELSEIF  "$(CFG)" == "CtlColorExplorer - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "CtlColorExplorer - Win32 Release"

!ELSEIF  "$(CFG)" == "CtlColorExplorer - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CtlColorExplorer.cpp
DEP_CPP_CTLCO=\
	".\About.h"\
	".\ChildFrm.h"\
	".\CtlColorExplorer.h"\
	".\CtlColorExplorerDoc.h"\
	".\CtlColorExplorerView.h"\
	".\error.h"\
	".\MainFrm.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CtlColorExplorer.obj" : $(SOURCE) $(DEP_CPP_CTLCO) "$(INTDIR)"\
 "$(INTDIR)\CtlColorExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "CtlColorExplorer - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/CtlColorExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\CtlColorExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "CtlColorExplorer - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/CtlColorExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\CtlColorExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\Color.h"\
	".\CtlColor.h"\
	".\CtlColorExplorer.h"\
	".\MainFrm.h"\
	".\RGB.h"\
	".\Sampler.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\CtlColorExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ChildFrm.cpp
DEP_CPP_CHILD=\
	".\ChildFrm.h"\
	".\CtlColorExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\CtlColorExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CtlColorExplorerDoc.cpp
DEP_CPP_CTLCOL=\
	".\CtlColorExplorer.h"\
	".\CtlColorExplorerDoc.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CtlColorExplorerDoc.obj" : $(SOURCE) $(DEP_CPP_CTLCOL) "$(INTDIR)"\
 "$(INTDIR)\CtlColorExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CtlColorExplorerView.cpp
DEP_CPP_CTLCOLO=\
	".\CtlColorExplorer.h"\
	".\CtlColorExplorerDoc.h"\
	".\CtlColorExplorerView.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CtlColorExplorerView.obj" : $(SOURCE) $(DEP_CPP_CTLCOLO) "$(INTDIR)"\
 "$(INTDIR)\CtlColorExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CtlColorExplorer.rc
DEP_RSC_CTLCOLOR=\
	".\res\chap16.ico"\
	".\res\CtlColorExplorer.rc2"\
	".\res\CtlColorExplorerDoc.ico"\
	".\res\jmnco.ico"\
	".\res\main.ico"\
	".\res\Toolbar.bmp"\
	

"$(INTDIR)\CtlColorExplorer.res" : $(SOURCE) $(DEP_RSC_CTLCOLOR) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\hlp\CtlColorExplorer.hpj

!IF  "$(CFG)" == "CtlColorExplorer - Win32 Release"

# Begin Custom Build - Making help file...
OutDir=.\Release
ProjDir=.
TargetName=CtlColorExplorer
InputPath=.\hlp\CtlColorExplorer.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "CtlColorExplorer - Win32 Debug"

# Begin Custom Build - Making help file...
OutDir=.\Debug
ProjDir=.
TargetName=CtlColorExplorer
InputPath=.\hlp\CtlColorExplorer.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.h

!IF  "$(CFG)" == "CtlColorExplorer - Win32 Release"

!ELSEIF  "$(CFG)" == "CtlColorExplorer - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CBTMsg.cpp
DEP_CPP_CBTMS=\
	".\CBTMsg.h"\
	".\CtlColorExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CBTMsg.obj" : $(SOURCE) $(DEP_CPP_CBTMS) "$(INTDIR)"\
 "$(INTDIR)\CtlColorExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CBTMsg.h

!IF  "$(CFG)" == "CtlColorExplorer - Win32 Release"

!ELSEIF  "$(CFG)" == "CtlColorExplorer - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Color.cpp
DEP_CPP_COLOR=\
	".\Color.h"\
	".\colors.h"\
	".\CtlColorExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Color.obj" : $(SOURCE) $(DEP_CPP_COLOR) "$(INTDIR)"\
 "$(INTDIR)\CtlColorExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.cpp
DEP_CPP_ABOUT=\
	".\About.h"\
	".\CtlColorExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"\
 "$(INTDIR)\CtlColorExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Sampler.cpp
DEP_CPP_SAMPL=\
	".\CBTMsg.h"\
	".\CtlColorExplorer.h"\
	".\error.h"\
	".\Sampler.h"\
	".\sibling.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Sampler.obj" : $(SOURCE) $(DEP_CPP_SAMPL) "$(INTDIR)"\
 "$(INTDIR)\CtlColorExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\error.cpp
DEP_CPP_ERROR=\
	".\StdAfx.h"\
	

"$(INTDIR)\error.obj" : $(SOURCE) $(DEP_CPP_ERROR) "$(INTDIR)"\
 "$(INTDIR)\CtlColorExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\RGB.cpp
DEP_CPP_RGB_C=\
	".\colors.h"\
	".\CtlColorExplorer.h"\
	".\RGB.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\RGB.obj" : $(SOURCE) $(DEP_CPP_RGB_C) "$(INTDIR)"\
 "$(INTDIR)\CtlColorExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CtlColor.cpp
DEP_CPP_CTLCOLOR_=\
	".\Color.h"\
	".\colors.h"\
	".\CtlColor.h"\
	".\CtlColorExplorer.h"\
	".\RGB.h"\
	".\sibling.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CtlColor.obj" : $(SOURCE) $(DEP_CPP_CTLCOLOR_) "$(INTDIR)"\
 "$(INTDIR)\CtlColorExplorer.pch"


# End Source File
# End Target
# End Project
################################################################################
