# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=msgbox - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to msgbox - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "msgbox - Win32 Release" && "$(CFG)" != "msgbox - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "msgbox.mak" CFG="msgbox - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "msgbox - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "msgbox - Win32 Debug" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "msgbox - Win32 Debug"
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "msgbox - Win32 Release"

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

ALL : "$(OUTDIR)\MessageBoxExplorer.exe" "$(OUTDIR)\MessageBoxExplorer.hlp"

CLEAN : 
	-@erase "$(INTDIR)\ABOUT.OBJ"
	-@erase "$(INTDIR)\IS95.OBJ"
	-@erase "$(INTDIR)\MessageBoxExplorer.hlp"
	-@erase "$(INTDIR)\msgbox.obj"
	-@erase "$(INTDIR)\msgbox.pch"
	-@erase "$(INTDIR)\msgbox.res"
	-@erase "$(INTDIR)\msgboxDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\XFORM.OBJ"
	-@erase "$(OUTDIR)\MessageBoxExplorer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/msgbox.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /c\
 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/msgbox.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/msgbox.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /machine:I386 /out:"Release/MessageBoxExplorer.exe"
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/MessageBoxExplorer.pdb" /machine:I386\
 /out:"$(OUTDIR)/MessageBoxExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ABOUT.OBJ" \
	"$(INTDIR)\IS95.OBJ" \
	"$(INTDIR)\msgbox.obj" \
	"$(INTDIR)\msgbox.res" \
	"$(INTDIR)\msgboxDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\XFORM.OBJ"

"$(OUTDIR)\MessageBoxExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "msgbox - Win32 Debug"

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

ALL : "$(OUTDIR)\MessageBoxExplorer.exe" "$(OUTDIR)\MessageBoxExplorer.hlp"

CLEAN : 
	-@erase "$(INTDIR)\ABOUT.OBJ"
	-@erase "$(INTDIR)\IS95.OBJ"
	-@erase "$(INTDIR)\MessageBoxExplorer.hlp"
	-@erase "$(INTDIR)\msgbox.obj"
	-@erase "$(INTDIR)\msgbox.pch"
	-@erase "$(INTDIR)\msgbox.res"
	-@erase "$(INTDIR)\msgboxDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\XFORM.OBJ"
	-@erase "$(OUTDIR)\MessageBoxExplorer.exe"
	-@erase "$(OUTDIR)\MessageBoxExplorer.ilk"
	-@erase "$(OUTDIR)\MessageBoxExplorer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/msgbox.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/msgbox.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/msgbox.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/MessageBoxExplorer.exe"
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/MessageBoxExplorer.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/MessageBoxExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ABOUT.OBJ" \
	"$(INTDIR)\IS95.OBJ" \
	"$(INTDIR)\msgbox.obj" \
	"$(INTDIR)\msgbox.res" \
	"$(INTDIR)\msgboxDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\XFORM.OBJ"

"$(OUTDIR)\MessageBoxExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "msgbox - Win32 Release"
# Name "msgbox - Win32 Debug"

!IF  "$(CFG)" == "msgbox - Win32 Release"

!ELSEIF  "$(CFG)" == "msgbox - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "msgbox - Win32 Release"

!ELSEIF  "$(CFG)" == "msgbox - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\msgbox.cpp
DEP_CPP_MSGBO=\
	".\msgbox.h"\
	".\msgboxDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\msgbox.obj" : $(SOURCE) $(DEP_CPP_MSGBO) "$(INTDIR)"\
 "$(INTDIR)\msgbox.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\msgboxDlg.cpp
DEP_CPP_MSGBOX=\
	".\About.h"\
	".\is95.h"\
	".\msgbox.h"\
	".\msgboxDlg.h"\
	".\StdAfx.h"\
	".\xform.h"\
	

"$(INTDIR)\msgboxDlg.obj" : $(SOURCE) $(DEP_CPP_MSGBOX) "$(INTDIR)"\
 "$(INTDIR)\msgbox.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "msgbox - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/msgbox.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\msgbox.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "msgbox - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/msgbox.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\msgbox.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\msgbox.rc
DEP_RSC_MSGBOX_=\
	".\res\jmnco.ico"\
	".\res\msgbox.ico"\
	".\res\msgbox.rc2"\
	

"$(INTDIR)\msgbox.res" : $(SOURCE) $(DEP_RSC_MSGBOX_) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\hlp\msgbox.hpj

!IF  "$(CFG)" == "msgbox - Win32 Release"

# Begin Custom Build - Making help file...
OutDir=.\Release
ProjDir=.
TargetName=MessageBoxExplorer
InputPath=.\hlp\msgbox.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "msgbox - Win32 Debug"

# Begin Custom Build - Making help file...
OutDir=.\Debug
ProjDir=.
TargetName=MessageBoxExplorer
InputPath=.\hlp\msgbox.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\IS95.CPP
DEP_CPP_IS95_=\
	".\StdAfx.h"\
	

"$(INTDIR)\IS95.OBJ" : $(SOURCE) $(DEP_CPP_IS95_) "$(INTDIR)"\
 "$(INTDIR)\msgbox.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ABOUT.CPP
DEP_CPP_ABOUT=\
	".\About.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ABOUT.OBJ" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"\
 "$(INTDIR)\msgbox.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\XFORM.CPP
DEP_CPP_XFORM=\
	".\StdAfx.h"\
	".\xform.h"\
	

"$(INTDIR)\XFORM.OBJ" : $(SOURCE) $(DEP_CPP_XFORM) "$(INTDIR)"\
 "$(INTDIR)\msgbox.pch"


# End Source File
# End Target
# End Project
################################################################################
