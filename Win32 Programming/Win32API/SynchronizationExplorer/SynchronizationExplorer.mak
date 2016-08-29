# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=SynchronizationExplorer - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to SynchronizationExplorer -\
 Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "SynchronizationExplorer - Win32 Release" && "$(CFG)" !=\
 "SynchronizationExplorer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "SynchronizationExplorer.mak"\
 CFG="SynchronizationExplorer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SynchronizationExplorer - Win32 Release" (based on\
 "Win32 (x86) Application")
!MESSAGE "SynchronizationExplorer - Win32 Debug" (based on\
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
# PROP Target_Last_Scanned "SynchronizationExplorer - Win32 Debug"
MTL=mktyplib.exe
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "SynchronizationExplorer - Win32 Release"

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

ALL : "$(OUTDIR)\SynchronizationExplorer.exe"\
 "$(OUTDIR)\SynchronizationExplorer.hlp"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\Delta.obj"
	-@erase "$(INTDIR)\evntname.obj"
	-@erase "$(INTDIR)\Intedit.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Panel.obj"
	-@erase "$(INTDIR)\PnlFrame.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SynchronizationExplorer.hlp"
	-@erase "$(INTDIR)\SynchronizationExplorer.obj"
	-@erase "$(INTDIR)\SynchronizationExplorer.pch"
	-@erase "$(INTDIR)\SynchronizationExplorer.res"
	-@erase "$(INTDIR)\SynchronizationExplorerDoc.obj"
	-@erase "$(INTDIR)\Thread.obj"
	-@erase "$(INTDIR)\Worker.obj"
	-@erase "$(OUTDIR)\SynchronizationExplorer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/SynchronizationExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/SynchronizationExplorer.res" /d "NDEBUG" /d\
 "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/SynchronizationExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/SynchronizationExplorer.pdb" /machine:I386\
 /out:"$(OUTDIR)/SynchronizationExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\Delta.obj" \
	"$(INTDIR)\evntname.obj" \
	"$(INTDIR)\Intedit.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Panel.obj" \
	"$(INTDIR)\PnlFrame.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SynchronizationExplorer.obj" \
	"$(INTDIR)\SynchronizationExplorer.res" \
	"$(INTDIR)\SynchronizationExplorerDoc.obj" \
	"$(INTDIR)\Thread.obj" \
	"$(INTDIR)\Worker.obj"

"$(OUTDIR)\SynchronizationExplorer.exe" : "$(OUTDIR)" $(DEF_FILE)\
 $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "SynchronizationExplorer - Win32 Debug"

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

ALL : "$(OUTDIR)\SynchronizationExplorer.exe"\
 "$(OUTDIR)\SynchronizationExplorer.hlp"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\Delta.obj"
	-@erase "$(INTDIR)\evntname.obj"
	-@erase "$(INTDIR)\Intedit.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Panel.obj"
	-@erase "$(INTDIR)\PnlFrame.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SynchronizationExplorer.hlp"
	-@erase "$(INTDIR)\SynchronizationExplorer.obj"
	-@erase "$(INTDIR)\SynchronizationExplorer.pch"
	-@erase "$(INTDIR)\SynchronizationExplorer.res"
	-@erase "$(INTDIR)\SynchronizationExplorerDoc.obj"
	-@erase "$(INTDIR)\Thread.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\Worker.obj"
	-@erase "$(OUTDIR)\SynchronizationExplorer.exe"
	-@erase "$(OUTDIR)\SynchronizationExplorer.ilk"
	-@erase "$(OUTDIR)\SynchronizationExplorer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/SynchronizationExplorer.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/SynchronizationExplorer.res" /d "_DEBUG" /d\
 "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/SynchronizationExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/SynchronizationExplorer.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/SynchronizationExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\Delta.obj" \
	"$(INTDIR)\evntname.obj" \
	"$(INTDIR)\Intedit.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Panel.obj" \
	"$(INTDIR)\PnlFrame.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SynchronizationExplorer.obj" \
	"$(INTDIR)\SynchronizationExplorer.res" \
	"$(INTDIR)\SynchronizationExplorerDoc.obj" \
	"$(INTDIR)\Thread.obj" \
	"$(INTDIR)\Worker.obj"

"$(OUTDIR)\SynchronizationExplorer.exe" : "$(OUTDIR)" $(DEF_FILE)\
 $(LINK32_OBJS)
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

# Name "SynchronizationExplorer - Win32 Release"
# Name "SynchronizationExplorer - Win32 Debug"

!IF  "$(CFG)" == "SynchronizationExplorer - Win32 Release"

!ELSEIF  "$(CFG)" == "SynchronizationExplorer - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "SynchronizationExplorer - Win32 Release"

!ELSEIF  "$(CFG)" == "SynchronizationExplorer - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SynchronizationExplorer.cpp
DEP_CPP_SYNCH=\
	".\About.h"\
	".\ChildFrm.h"\
	".\Delta.h"\
	".\IntEdit.h"\
	".\MainFrm.h"\
	".\panel.h"\
	".\pnlframe.h"\
	".\StdAfx.h"\
	".\SynchronizationExplorer.h"\
	".\SynchronizationExplorerDoc.h"\
	".\thread.h"\
	".\worker.h"\
	

"$(INTDIR)\SynchronizationExplorer.obj" : $(SOURCE) $(DEP_CPP_SYNCH)\
 "$(INTDIR)" "$(INTDIR)\SynchronizationExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "SynchronizationExplorer - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/SynchronizationExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SynchronizationExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "SynchronizationExplorer - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/SynchronizationExplorer.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\SynchronizationExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\MainFrm.h"\
	".\StdAfx.h"\
	".\SynchronizationExplorer.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\SynchronizationExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ChildFrm.cpp
DEP_CPP_CHILD=\
	".\ChildFrm.h"\
	".\StdAfx.h"\
	".\SynchronizationExplorer.h"\
	

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\SynchronizationExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SynchronizationExplorerDoc.cpp
DEP_CPP_SYNCHR=\
	".\StdAfx.h"\
	".\SynchronizationExplorer.h"\
	".\SynchronizationExplorerDoc.h"\
	

"$(INTDIR)\SynchronizationExplorerDoc.obj" : $(SOURCE) $(DEP_CPP_SYNCHR)\
 "$(INTDIR)" "$(INTDIR)\SynchronizationExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SynchronizationExplorer.rc
DEP_RSC_SYNCHRO=\
	".\res\jmnco.ico"\
	".\res\panel.ico"\
	".\res\synch.ico"\
	".\res\synchDoc.ico"\
	".\res\SynchronizationExplorer.rc2"\
	".\res\Toolbar.bmp"\
	

"$(INTDIR)\SynchronizationExplorer.res" : $(SOURCE) $(DEP_RSC_SYNCHRO)\
 "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\hlp\SynchronizationExplorer.hpj

!IF  "$(CFG)" == "SynchronizationExplorer - Win32 Release"

# Begin Custom Build - Making help file...
OutDir=.\Release
ProjDir=.
TargetName=SynchronizationExplorer
InputPath=.\hlp\SynchronizationExplorer.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "SynchronizationExplorer - Win32 Debug"

# Begin Custom Build - Making help file...
OutDir=.\Debug
ProjDir=.
TargetName=SynchronizationExplorer
InputPath=.\hlp\SynchronizationExplorer.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Worker.cpp
DEP_CPP_WORKE=\
	".\ChildFrm.h"\
	".\Delta.h"\
	".\EvntName.h"\
	".\IntEdit.h"\
	".\panel.h"\
	".\StdAfx.h"\
	".\SynchronizationExplorer.h"\
	".\thread.h"\
	".\UWM.h"\
	".\worker.h"\
	

"$(INTDIR)\Worker.obj" : $(SOURCE) $(DEP_CPP_WORKE) "$(INTDIR)"\
 "$(INTDIR)\SynchronizationExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Delta.cpp

!IF  "$(CFG)" == "SynchronizationExplorer - Win32 Release"

DEP_CPP_DELTA=\
	".\Delta.h"\
	".\StdAfx.h"\
	".\SynchronizationExplorer.h"\
	

"$(INTDIR)\Delta.obj" : $(SOURCE) $(DEP_CPP_DELTA) "$(INTDIR)"\
 "$(INTDIR)\SynchronizationExplorer.pch"


!ELSEIF  "$(CFG)" == "SynchronizationExplorer - Win32 Debug"

DEP_CPP_DELTA=\
	".\Delta.h"\
	".\StdAfx.h"\
	".\SynchronizationExplorer.h"\
	

"$(INTDIR)\Delta.obj" : $(SOURCE) $(DEP_CPP_DELTA) "$(INTDIR)"\
 "$(INTDIR)\SynchronizationExplorer.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\evntname.cpp
DEP_CPP_EVNTN=\
	".\StdAfx.h"\
	

"$(INTDIR)\evntname.obj" : $(SOURCE) $(DEP_CPP_EVNTN) "$(INTDIR)"\
 "$(INTDIR)\SynchronizationExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Intedit.cpp
DEP_CPP_INTED=\
	".\IntEdit.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Intedit.obj" : $(SOURCE) $(DEP_CPP_INTED) "$(INTDIR)"\
 "$(INTDIR)\SynchronizationExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Panel.cpp
DEP_CPP_PANEL=\
	".\ChildFrm.h"\
	".\Delta.h"\
	".\panel.h"\
	".\StdAfx.h"\
	".\SynchronizationExplorer.h"\
	".\thread.h"\
	".\UWM.h"\
	

"$(INTDIR)\Panel.obj" : $(SOURCE) $(DEP_CPP_PANEL) "$(INTDIR)"\
 "$(INTDIR)\SynchronizationExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PnlFrame.cpp
DEP_CPP_PNLFR=\
	".\ChildFrm.h"\
	".\pnlframe.h"\
	".\StdAfx.h"\
	".\SynchronizationExplorer.h"\
	

"$(INTDIR)\PnlFrame.obj" : $(SOURCE) $(DEP_CPP_PNLFR) "$(INTDIR)"\
 "$(INTDIR)\SynchronizationExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Thread.cpp
DEP_CPP_THREA=\
	".\IntEdit.h"\
	".\StdAfx.h"\
	".\SynchronizationExplorer.h"\
	".\thread.h"\
	".\UWM.h"\
	".\worker.h"\
	

"$(INTDIR)\Thread.obj" : $(SOURCE) $(DEP_CPP_THREA) "$(INTDIR)"\
 "$(INTDIR)\SynchronizationExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.cpp
DEP_CPP_ABOUT=\
	".\About.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"\
 "$(INTDIR)\SynchronizationExplorer.pch"


# End Source File
# End Target
# End Project
################################################################################
