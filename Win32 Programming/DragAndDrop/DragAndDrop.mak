# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=dipper - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to dipper - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "dipper - Win32 Release" && "$(CFG)" != "dipper - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "dipper.mak" CFG="dipper - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dipper - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "dipper - Win32 Debug" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "dipper - Win32 Debug"
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "dipper - Win32 Release"

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

ALL : "$(OUTDIR)\dipper.exe"

CLEAN : 
	-@erase "$(INTDIR)\ABOUT.OBJ"
	-@erase "$(INTDIR)\Bucket.obj"
	-@erase "$(INTDIR)\dipper.obj"
	-@erase "$(INTDIR)\dipper.pch"
	-@erase "$(INTDIR)\dipper.res"
	-@erase "$(INTDIR)\dipperDlg.obj"
	-@erase "$(INTDIR)\Glass.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\dipper.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/dipper.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/" /c\
 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dipper.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dipper.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/dipper.pdb" /machine:I386 /out:"$(OUTDIR)/dipper.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ABOUT.OBJ" \
	"$(INTDIR)\Bucket.obj" \
	"$(INTDIR)\dipper.obj" \
	"$(INTDIR)\dipper.res" \
	"$(INTDIR)\dipperDlg.obj" \
	"$(INTDIR)\Glass.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\dipper.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dipper - Win32 Debug"

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

ALL : "$(OUTDIR)\dipper.exe"

CLEAN : 
	-@erase "$(INTDIR)\ABOUT.OBJ"
	-@erase "$(INTDIR)\Bucket.obj"
	-@erase "$(INTDIR)\dipper.obj"
	-@erase "$(INTDIR)\dipper.pch"
	-@erase "$(INTDIR)\dipper.res"
	-@erase "$(INTDIR)\dipperDlg.obj"
	-@erase "$(INTDIR)\Glass.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\dipper.exe"
	-@erase "$(OUTDIR)\dipper.ilk"
	-@erase "$(OUTDIR)\dipper.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/dipper.pch" /Yu"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/dipper.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/dipper.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/dipper.pdb" /debug /machine:I386 /out:"$(OUTDIR)/dipper.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ABOUT.OBJ" \
	"$(INTDIR)\Bucket.obj" \
	"$(INTDIR)\dipper.obj" \
	"$(INTDIR)\dipper.res" \
	"$(INTDIR)\dipperDlg.obj" \
	"$(INTDIR)\Glass.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\dipper.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "dipper - Win32 Release"
# Name "dipper - Win32 Debug"

!IF  "$(CFG)" == "dipper - Win32 Release"

!ELSEIF  "$(CFG)" == "dipper - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "dipper - Win32 Release"

!ELSEIF  "$(CFG)" == "dipper - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dipper.cpp
DEP_CPP_DIPPE=\
	".\Bucket.h"\
	".\dipper.h"\
	".\dipperDlg.h"\
	".\Glass.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\dipper.obj" : $(SOURCE) $(DEP_CPP_DIPPE) "$(INTDIR)"\
 "$(INTDIR)\dipper.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\dipperDlg.cpp
DEP_CPP_DIPPER=\
	".\about.h"\
	".\Bucket.h"\
	".\dipper.h"\
	".\dipperDlg.h"\
	".\Glass.h"\
	".\status.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\dipperDlg.obj" : $(SOURCE) $(DEP_CPP_DIPPER) "$(INTDIR)"\
 "$(INTDIR)\dipper.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "dipper - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/dipper.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/" /c\
 $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dipper.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "dipper - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/dipper.pch" /Yc"stdafx.h" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\dipper.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dipper.rc
DEP_RSC_DIPPER_=\
	".\res\bucket.ico"\
	".\res\dipper.ico"\
	".\res\dipper.rc2"\
	".\res\empty.cur"\
	".\res\full.cur"\
	".\res\g0.ico"\
	".\res\g1.ico"\
	".\res\g2.ico"\
	".\res\g3.ico"\
	".\res\jmnco.ico"\
	

"$(INTDIR)\dipper.res" : $(SOURCE) $(DEP_RSC_DIPPER_) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Bucket.cpp
DEP_CPP_BUCKE=\
	".\Bucket.h"\
	".\dipper.h"\
	".\query.h"\
	".\status.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Bucket.obj" : $(SOURCE) $(DEP_CPP_BUCKE) "$(INTDIR)"\
 "$(INTDIR)\dipper.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Glass.cpp
DEP_CPP_GLASS=\
	".\Bucket.h"\
	".\dipper.h"\
	".\Glass.h"\
	".\query.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Glass.obj" : $(SOURCE) $(DEP_CPP_GLASS) "$(INTDIR)"\
 "$(INTDIR)\dipper.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ABOUT.CPP
DEP_CPP_ABOUT=\
	".\about.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ABOUT.OBJ" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"\
 "$(INTDIR)\dipper.pch"


# End Source File
# End Target
# End Project
################################################################################
