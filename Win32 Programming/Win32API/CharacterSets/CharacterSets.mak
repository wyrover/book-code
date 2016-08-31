# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=CharacterSets - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to CharacterSets - Win32\
 Debug.
!ENDIF 

!IF "$(CFG)" != "CharacterSets - Win32 Release" && "$(CFG)" !=\
 "CharacterSets - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "CharacterSets.mak" CFG="CharacterSets - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CharacterSets - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CharacterSets - Win32 Debug" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "CharacterSets - Win32 Debug"
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "CharacterSets - Win32 Release"

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

ALL : "$(OUTDIR)\CharacterSets.exe" "$(OUTDIR)\CharacterSets.hlp"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\AnsiView.obj"
	-@erase "$(INTDIR)\CharacterSets.hlp"
	-@erase "$(INTDIR)\CharacterSets.obj"
	-@erase "$(INTDIR)\CharacterSets.pch"
	-@erase "$(INTDIR)\CharacterSets.res"
	-@erase "$(INTDIR)\CharacterSetsDoc.obj"
	-@erase "$(INTDIR)\CharacterSetsView.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\OEMView.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\CharacterSets.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/CharacterSets.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/CharacterSets.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/CharacterSets.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/CharacterSets.pdb" /machine:I386\
 /out:"$(OUTDIR)/CharacterSets.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\AnsiView.obj" \
	"$(INTDIR)\CharacterSets.obj" \
	"$(INTDIR)\CharacterSets.res" \
	"$(INTDIR)\CharacterSetsDoc.obj" \
	"$(INTDIR)\CharacterSetsView.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\OEMView.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\CharacterSets.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "CharacterSets - Win32 Debug"

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

ALL : "$(OUTDIR)\CharacterSets.exe" "$(OUTDIR)\CharacterSets.hlp"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\AnsiView.obj"
	-@erase "$(INTDIR)\CharacterSets.hlp"
	-@erase "$(INTDIR)\CharacterSets.obj"
	-@erase "$(INTDIR)\CharacterSets.pch"
	-@erase "$(INTDIR)\CharacterSets.res"
	-@erase "$(INTDIR)\CharacterSetsDoc.obj"
	-@erase "$(INTDIR)\CharacterSetsView.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\OEMView.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\CharacterSets.exe"
	-@erase "$(OUTDIR)\CharacterSets.ilk"
	-@erase "$(OUTDIR)\CharacterSets.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/CharacterSets.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/CharacterSets.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/CharacterSets.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/CharacterSets.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/CharacterSets.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\AnsiView.obj" \
	"$(INTDIR)\CharacterSets.obj" \
	"$(INTDIR)\CharacterSets.res" \
	"$(INTDIR)\CharacterSetsDoc.obj" \
	"$(INTDIR)\CharacterSetsView.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\OEMView.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\CharacterSets.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "CharacterSets - Win32 Release"
# Name "CharacterSets - Win32 Debug"

!IF  "$(CFG)" == "CharacterSets - Win32 Release"

!ELSEIF  "$(CFG)" == "CharacterSets - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "CharacterSets - Win32 Release"

!ELSEIF  "$(CFG)" == "CharacterSets - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CharacterSets.cpp
DEP_CPP_CHARA=\
	".\About.h"\
	".\AnsiView.h"\
	".\CharacterSets.h"\
	".\CharacterSetsDoc.h"\
	".\CharacterSetsView.h"\
	".\ChildFrm.h"\
	".\MainFrm.h"\
	".\OEMView.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CharacterSets.obj" : $(SOURCE) $(DEP_CPP_CHARA) "$(INTDIR)"\
 "$(INTDIR)\CharacterSets.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "CharacterSets - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/CharacterSets.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\CharacterSets.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "CharacterSets - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/CharacterSets.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\CharacterSets.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\CharacterSets.h"\
	".\MainFrm.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\CharacterSets.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ChildFrm.cpp
DEP_CPP_CHILD=\
	".\CharacterSets.h"\
	".\ChildFrm.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\CharacterSets.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CharacterSetsDoc.cpp
DEP_CPP_CHARAC=\
	".\CharacterSets.h"\
	".\CharacterSetsDoc.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CharacterSetsDoc.obj" : $(SOURCE) $(DEP_CPP_CHARAC) "$(INTDIR)"\
 "$(INTDIR)\CharacterSets.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CharacterSetsView.cpp
DEP_CPP_CHARACT=\
	".\CharacterSets.h"\
	".\CharacterSetsDoc.h"\
	".\CharacterSetsView.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CharacterSetsView.obj" : $(SOURCE) $(DEP_CPP_CHARACT) "$(INTDIR)"\
 "$(INTDIR)\CharacterSets.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\CharacterSets.rc
DEP_RSC_CHARACTE=\
	".\res\CharacterSets.ico"\
	".\res\CharacterSets.rc2"\
	".\res\CharacterSetsDoc.ico"\
	".\res\idr_ansi.ico"\
	".\res\idr_oemt.ico"\
	".\res\jmnco.ico"\
	".\res\Toolbar.bmp"\
	

"$(INTDIR)\CharacterSets.res" : $(SOURCE) $(DEP_RSC_CHARACTE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\hlp\CharacterSets.hpj

!IF  "$(CFG)" == "CharacterSets - Win32 Release"

# Begin Custom Build - Making help file...
OutDir=.\Release
ProjDir=.
TargetName=CharacterSets
InputPath=.\hlp\CharacterSets.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "CharacterSets - Win32 Debug"

# Begin Custom Build - Making help file...
OutDir=.\Debug
ProjDir=.
TargetName=CharacterSets
InputPath=.\hlp\CharacterSets.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\OEMView.cpp
DEP_CPP_OEMVI=\
	".\CharacterSets.h"\
	".\CharacterSetsDoc.h"\
	".\CharacterSetsView.h"\
	".\OEMView.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\OEMView.obj" : $(SOURCE) $(DEP_CPP_OEMVI) "$(INTDIR)"\
 "$(INTDIR)\CharacterSets.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\AnsiView.cpp
DEP_CPP_ANSIV=\
	".\AnsiView.h"\
	".\CharacterSets.h"\
	".\CharacterSetsDoc.h"\
	".\CharacterSetsView.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\AnsiView.obj" : $(SOURCE) $(DEP_CPP_ANSIV) "$(INTDIR)"\
 "$(INTDIR)\CharacterSets.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.cpp
DEP_CPP_ABOUT=\
	".\About.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"\
 "$(INTDIR)\CharacterSets.pch"


# End Source File
# End Target
# End Project
################################################################################
