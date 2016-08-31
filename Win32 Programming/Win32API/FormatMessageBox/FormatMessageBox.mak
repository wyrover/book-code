# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=formatMessageBox DLL - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to formatMessageBox DLL -\
 Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "FormatMessageBox - Win32 Release" && "$(CFG)" !=\
 "FormatMessageBox - Win32 Debug" && "$(CFG)" !=\
 "formatMessageBox DLL - Win32 Release" && "$(CFG)" !=\
 "formatMessageBox DLL - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "FormatMessageBox.mak"\
 CFG="formatMessageBox DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FormatMessageBox - Win32 Release" (based on\
 "Win32 (x86) Application")
!MESSAGE "FormatMessageBox - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "formatMessageBox DLL - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "formatMessageBox DLL - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
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

!IF  "$(CFG)" == "FormatMessageBox - Win32 Release"

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

ALL : "$(OUTDIR)\FormatMessageBox.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\FormatMessageBox.obj"
	-@erase "$(INTDIR)\FormatMessageBox.pch"
	-@erase "$(INTDIR)\FormatMessageBox.res"
	-@erase "$(INTDIR)\FormatMessageBoxDoc.obj"
	-@erase "$(INTDIR)\FormatMessageBoxView.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\FormatMessageBox.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/FormatMessageBox.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.

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

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/FormatMessageBox.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/FormatMessageBox.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib Release\formatMessageBox.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=version.lib Release\formatMessageBox.lib /nologo\
 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)/FormatMessageBox.pdb"\
 /machine:I386 /out:"$(OUTDIR)/FormatMessageBox.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\FormatMessageBox.obj" \
	"$(INTDIR)\FormatMessageBox.res" \
	"$(INTDIR)\FormatMessageBoxDoc.obj" \
	"$(INTDIR)\FormatMessageBoxView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\FormatMessageBox.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "FormatMessageBox - Win32 Debug"

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

ALL : "$(OUTDIR)\FormatMessageBox.exe"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\FormatMessageBox.obj"
	-@erase "$(INTDIR)\FormatMessageBox.pch"
	-@erase "$(INTDIR)\FormatMessageBox.res"
	-@erase "$(INTDIR)\FormatMessageBoxDoc.obj"
	-@erase "$(INTDIR)\FormatMessageBoxView.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\FormatMessageBox.exe"
	-@erase "$(OUTDIR)\FormatMessageBox.ilk"
	-@erase "$(OUTDIR)\FormatMessageBox.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/FormatMessageBox.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.

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

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/FormatMessageBox.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/FormatMessageBox.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 version.lib Debug\formatMessageBox.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=version.lib Debug\formatMessageBox.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/FormatMessageBox.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/FormatMessageBox.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\FormatMessageBox.obj" \
	"$(INTDIR)\FormatMessageBox.res" \
	"$(INTDIR)\FormatMessageBoxDoc.obj" \
	"$(INTDIR)\FormatMessageBoxView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\FormatMessageBox.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "formatMessageBox DLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "formatMessageBox DLL\Release"
# PROP BASE Intermediate_Dir "formatMessageBox DLL\Release"
# PROP BASE Target_Dir "formatMessageBox DLL"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "formatMessageBox DLL\Release"
# PROP Intermediate_Dir "formatMessageBox DLL\Release"
# PROP Target_Dir "formatMessageBox DLL"
OUTDIR=.\formatMessageBox DLL\Release
INTDIR=.\formatMessageBox DLL\Release

ALL : ".\Release\formatMessageBox.dll"

CLEAN : 
	-@erase ".\Release\formatMessageBox.dll"
	-@erase ".\Release\formatMessageBox.exp"
	-@erase ".\Release\formatMessageBox.lib"
	-@erase ".\Release\Msgbox.obj"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"Release/formatMessageBox.pch" /YX /Fo"Release/" /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"Release/formatMessageBox.pch" /YX /Fo"Release/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.

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

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/formatMessageBox DLL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /pdb:"Releasee/formatMessageBox.pdb" /machine:I386 /out:"Release/formatMessageBox.dll" /implib:"Release/formatMessageBox.lib"
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"Releasee/formatMessageBox.pdb" /machine:I386\
 /out:"Release/formatMessageBox.dll" /implib:"Release/formatMessageBox.lib" 
LINK32_OBJS= \
	".\Release\Msgbox.obj"

".\Release\formatMessageBox.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "formatMessageBox DLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "formatMessageBox DLL\Debug"
# PROP BASE Intermediate_Dir "formatMessageBox DLL\Debug"
# PROP BASE Target_Dir "formatMessageBox DLL"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "formatMessageBox DLL\Debug"
# PROP Intermediate_Dir "formatMessageBox DLL\Debug"
# PROP Target_Dir "formatMessageBox DLL"
OUTDIR=.\formatMessageBox DLL\Debug
INTDIR=.\formatMessageBox DLL\Debug

ALL : ".\Debug\formatMessageBox.dll"

CLEAN : 
	-@erase "$(OUTDIR)\formatMessageBox.pdb"
	-@erase ".\Debug\formatMessageBox.dll"
	-@erase ".\Debug\formatMessageBox.exp"
	-@erase ".\Debug\FormatMessageBox.ilk"
	-@erase ".\Debug\formatMessageBox.lib"
	-@erase ".\Debug\Msgbox.obj"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\vc40.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fp"Debug/formatMessageBox.pch" /YX /Fo"Debug/" /Fd"Debug/" /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"Debug/formatMessageBox.pch" /YX /Fo"Debug/" /Fd"Debug/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.

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

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/formatMessageBox DLL.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug/formatMessageBox.dll" /implib:"Debug/formatMessageBox.lib"
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/formatMessageBox.pdb" /debug /machine:I386\
 /out:"Debug/formatMessageBox.dll" /implib:"Debug/formatMessageBox.lib" 
LINK32_OBJS= \
	".\Debug\Msgbox.obj"

".\Debug\formatMessageBox.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

################################################################################
# Begin Target

# Name "FormatMessageBox - Win32 Release"
# Name "FormatMessageBox - Win32 Debug"

!IF  "$(CFG)" == "FormatMessageBox - Win32 Release"

!ELSEIF  "$(CFG)" == "FormatMessageBox - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "FormatMessageBox - Win32 Release"

!ELSEIF  "$(CFG)" == "FormatMessageBox - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FormatMessageBox.cpp
DEP_CPP_FORMA=\
	".\about.h"\
	".\ChildFrm.h"\
	".\FormatMessageBox.h"\
	".\FormatMessageBoxDoc.h"\
	".\FormatMessageBoxView.h"\
	".\MainFrm.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FormatMessageBox.obj" : $(SOURCE) $(DEP_CPP_FORMA) "$(INTDIR)"\
 "$(INTDIR)\FormatMessageBox.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "FormatMessageBox - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/FormatMessageBox.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\FormatMessageBox.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "FormatMessageBox - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/FormatMessageBox.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\FormatMessageBox.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\FormatMessageBox.h"\
	".\MainFrm.h"\
	".\StdAfx.h"\
	
NODEP_CPP_MAINF=\
	".\msgbox.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\FormatMessageBox.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ChildFrm.cpp
DEP_CPP_CHILD=\
	".\ChildFrm.h"\
	".\FormatMessageBox.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\FormatMessageBox.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FormatMessageBoxDoc.cpp
DEP_CPP_FORMAT=\
	".\FormatMessageBox.h"\
	".\FormatMessageBoxDoc.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FormatMessageBoxDoc.obj" : $(SOURCE) $(DEP_CPP_FORMAT) "$(INTDIR)"\
 "$(INTDIR)\FormatMessageBox.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FormatMessageBoxView.cpp
DEP_CPP_FORMATM=\
	".\FormatMessageBox.h"\
	".\FormatMessageBoxDoc.h"\
	".\FormatMessageBoxView.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FormatMessageBoxView.obj" : $(SOURCE) $(DEP_CPP_FORMATM) "$(INTDIR)"\
 "$(INTDIR)\FormatMessageBox.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FormatMessageBox.rc
DEP_RSC_FORMATME=\
	".\res\FormatMessageBox.ico"\
	".\res\FormatMessageBox.rc2"\
	".\res\FormatMessageBoxDoc.ico"\
	".\res\Toolbar.bmp"\
	

"$(INTDIR)\FormatMessageBox.res" : $(SOURCE) $(DEP_RSC_FORMATME) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.cpp
DEP_CPP_ABOUT=\
	".\about.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"\
 "$(INTDIR)\FormatMessageBox.pch"


# End Source File
# End Target
################################################################################
# Begin Target

# Name "formatMessageBox DLL - Win32 Release"
# Name "formatMessageBox DLL - Win32 Debug"

!IF  "$(CFG)" == "formatMessageBox DLL - Win32 Release"

!ELSEIF  "$(CFG)" == "formatMessageBox DLL - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Msgbox.c

!IF  "$(CFG)" == "formatMessageBox DLL - Win32 Release"


".\Release\Msgbox.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "formatMessageBox DLL - Win32 Debug"


".\Debug\Msgbox.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
