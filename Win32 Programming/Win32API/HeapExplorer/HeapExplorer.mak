# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=HeapExplorer - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to HeapExplorer - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "HeapExplorer - Win32 Release" && "$(CFG)" !=\
 "HeapExplorer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "HeapExplorer.mak" CFG="HeapExplorer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HeapExplorer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "HeapExplorer - Win32 Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "HeapExplorer - Win32 Release"

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

ALL : "$(OUTDIR)\HeapExplorer.exe" "$(OUTDIR)\HeapExplorer.hlp"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\data.obj"
	-@erase "$(INTDIR)\DefHeap.obj"
	-@erase "$(INTDIR)\Heap.obj"
	-@erase "$(INTDIR)\HeapExplorer.hlp"
	-@erase "$(INTDIR)\HeapExplorer.obj"
	-@erase "$(INTDIR)\HeapExplorer.pch"
	-@erase "$(INTDIR)\HeapExplorer.res"
	-@erase "$(INTDIR)\HeapExplorerDoc.obj"
	-@erase "$(INTDIR)\HeapExplorerView.obj"
	-@erase "$(INTDIR)\HeapPic.obj"
	-@erase "$(INTDIR)\InfoBox.obj"
	-@erase "$(INTDIR)\Is95.obj"
	-@erase "$(INTDIR)\Legend.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MemMap.obj"
	-@erase "$(INTDIR)\Message.obj"
	-@erase "$(INTDIR)\Mystring.obj"
	-@erase "$(INTDIR)\PropBut.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StrDisp.obj"
	-@erase "$(INTDIR)\StrTest.obj"
	-@erase "$(OUTDIR)\HeapExplorer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/HeapExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/HeapExplorer.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/HeapExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/HeapExplorer.pdb" /machine:I386\
 /out:"$(OUTDIR)/HeapExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\data.obj" \
	"$(INTDIR)\DefHeap.obj" \
	"$(INTDIR)\Heap.obj" \
	"$(INTDIR)\HeapExplorer.obj" \
	"$(INTDIR)\HeapExplorer.res" \
	"$(INTDIR)\HeapExplorerDoc.obj" \
	"$(INTDIR)\HeapExplorerView.obj" \
	"$(INTDIR)\HeapPic.obj" \
	"$(INTDIR)\InfoBox.obj" \
	"$(INTDIR)\Is95.obj" \
	"$(INTDIR)\Legend.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MemMap.obj" \
	"$(INTDIR)\Message.obj" \
	"$(INTDIR)\Mystring.obj" \
	"$(INTDIR)\PropBut.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StrDisp.obj" \
	"$(INTDIR)\StrTest.obj"

"$(OUTDIR)\HeapExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "HeapExplorer - Win32 Debug"

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

ALL : "$(OUTDIR)\HeapExplorer.exe" "$(OUTDIR)\HeapExplorer.hlp"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\data.obj"
	-@erase "$(INTDIR)\DefHeap.obj"
	-@erase "$(INTDIR)\Heap.obj"
	-@erase "$(INTDIR)\HeapExplorer.hlp"
	-@erase "$(INTDIR)\HeapExplorer.obj"
	-@erase "$(INTDIR)\HeapExplorer.pch"
	-@erase "$(INTDIR)\HeapExplorer.res"
	-@erase "$(INTDIR)\HeapExplorerDoc.obj"
	-@erase "$(INTDIR)\HeapExplorerView.obj"
	-@erase "$(INTDIR)\HeapPic.obj"
	-@erase "$(INTDIR)\InfoBox.obj"
	-@erase "$(INTDIR)\Is95.obj"
	-@erase "$(INTDIR)\Legend.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MemMap.obj"
	-@erase "$(INTDIR)\Message.obj"
	-@erase "$(INTDIR)\Mystring.obj"
	-@erase "$(INTDIR)\PropBut.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StrDisp.obj"
	-@erase "$(INTDIR)\StrTest.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\HeapExplorer.exe"
	-@erase "$(OUTDIR)\HeapExplorer.ilk"
	-@erase "$(OUTDIR)\HeapExplorer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/HeapExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/HeapExplorer.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/HeapExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/HeapExplorer.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/HeapExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\data.obj" \
	"$(INTDIR)\DefHeap.obj" \
	"$(INTDIR)\Heap.obj" \
	"$(INTDIR)\HeapExplorer.obj" \
	"$(INTDIR)\HeapExplorer.res" \
	"$(INTDIR)\HeapExplorerDoc.obj" \
	"$(INTDIR)\HeapExplorerView.obj" \
	"$(INTDIR)\HeapPic.obj" \
	"$(INTDIR)\InfoBox.obj" \
	"$(INTDIR)\Is95.obj" \
	"$(INTDIR)\Legend.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MemMap.obj" \
	"$(INTDIR)\Message.obj" \
	"$(INTDIR)\Mystring.obj" \
	"$(INTDIR)\PropBut.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StrDisp.obj" \
	"$(INTDIR)\StrTest.obj"

"$(OUTDIR)\HeapExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "HeapExplorer - Win32 Release"
# Name "HeapExplorer - Win32 Debug"

!IF  "$(CFG)" == "HeapExplorer - Win32 Release"

!ELSEIF  "$(CFG)" == "HeapExplorer - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "HeapExplorer - Win32 Release"

!ELSEIF  "$(CFG)" == "HeapExplorer - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\HeapExplorer.cpp
DEP_CPP_HEAPE=\
	".\About.h"\
	".\ChildFrm.h"\
	".\HeapExplorer.h"\
	".\HeapExplorerDoc.h"\
	".\HeapExplorerView.h"\
	".\MainFrm.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\HeapExplorer.obj" : $(SOURCE) $(DEP_CPP_HEAPE) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "HeapExplorer - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/HeapExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\HeapExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "HeapExplorer - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/HeapExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\HeapExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\data.h"\
	".\Heap.h"\
	".\HeapExplorer.h"\
	".\heappic.h"\
	".\infobox.h"\
	".\legend.h"\
	".\legendi.h"\
	".\MainFrm.h"\
	".\memmap.h"\
	".\message.h"\
	".\mystring.h"\
	".\propbut.h"\
	".\StdAfx.h"\
	".\strdisp.h"\
	".\StrTest.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ChildFrm.cpp
DEP_CPP_CHILD=\
	".\ChildFrm.h"\
	".\HeapExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\HeapExplorerDoc.cpp
DEP_CPP_HEAPEX=\
	".\HeapExplorer.h"\
	".\HeapExplorerDoc.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\HeapExplorerDoc.obj" : $(SOURCE) $(DEP_CPP_HEAPEX) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\HeapExplorerView.cpp
DEP_CPP_HEAPEXP=\
	".\HeapExplorer.h"\
	".\HeapExplorerDoc.h"\
	".\HeapExplorerView.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\HeapExplorerView.obj" : $(SOURCE) $(DEP_CPP_HEAPEXP) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\HeapExplorer.rc
DEP_RSC_HEAPEXPL=\
	".\res\HeapExplorer.ico"\
	".\res\HeapExplorer.rc2"\
	".\res\HeapExplorerDoc.ico"\
	".\res\jmnco.ico"\
	".\res\Toolbar.bmp"\
	

"$(INTDIR)\HeapExplorer.res" : $(SOURCE) $(DEP_RSC_HEAPEXPL) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\hlp\HeapExplorer.hpj

!IF  "$(CFG)" == "HeapExplorer - Win32 Release"

# Begin Custom Build - Making help file...
OutDir=.\Release
ProjDir=.
TargetName=HeapExplorer
InputPath=.\hlp\HeapExplorer.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "HeapExplorer - Win32 Debug"

# Begin Custom Build - Making help file...
OutDir=.\Debug
ProjDir=.
TargetName=HeapExplorer
InputPath=.\hlp\HeapExplorer.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StrTest.cpp
DEP_CPP_STRTE=\
	".\HeapExplorer.h"\
	".\heappic.h"\
	".\legend.h"\
	".\legendi.h"\
	".\memmap.h"\
	".\mystring.h"\
	".\StdAfx.h"\
	".\strdisp.h"\
	".\StrTest.h"\
	

"$(INTDIR)\StrTest.obj" : $(SOURCE) $(DEP_CPP_STRTE) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\data.cpp
DEP_CPP_DATA_=\
	".\data.h"\
	".\HeapExplorer.h"\
	".\infomsg.h"\
	".\legendi.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\data.obj" : $(SOURCE) $(DEP_CPP_DATA_) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\DefHeap.cpp
DEP_CPP_DEFHE=\
	".\DefHeap.h"\
	".\HeapExplorer.h"\
	".\heappic.h"\
	".\legend.h"\
	".\legendi.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\DefHeap.obj" : $(SOURCE) $(DEP_CPP_DEFHE) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Heap.cpp
DEP_CPP_HEAP_=\
	".\data.h"\
	".\Heap.h"\
	".\HeapExplorer.h"\
	".\infobox.h"\
	".\infomsg.h"\
	".\Is95.h"\
	".\legend.h"\
	".\legendi.h"\
	".\message.h"\
	".\propbut.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Heap.obj" : $(SOURCE) $(DEP_CPP_HEAP_) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\HeapPic.cpp
DEP_CPP_HEAPP=\
	".\HeapExplorer.h"\
	".\heappic.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\HeapPic.obj" : $(SOURCE) $(DEP_CPP_HEAPP) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\InfoBox.cpp
DEP_CPP_INFOB=\
	".\HeapExplorer.h"\
	".\infobox.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\InfoBox.obj" : $(SOURCE) $(DEP_CPP_INFOB) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Is95.cpp
DEP_CPP_IS95_=\
	".\StdAfx.h"\
	

"$(INTDIR)\Is95.obj" : $(SOURCE) $(DEP_CPP_IS95_) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Legend.cpp
DEP_CPP_LEGEN=\
	".\HeapExplorer.h"\
	".\legend.h"\
	".\legendi.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Legend.obj" : $(SOURCE) $(DEP_CPP_LEGEN) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MemMap.cpp
DEP_CPP_MEMMA=\
	".\data.h"\
	".\HeapExplorer.h"\
	".\legendi.h"\
	".\memmap.h"\
	".\mystring.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MemMap.obj" : $(SOURCE) $(DEP_CPP_MEMMA) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Message.cpp
DEP_CPP_MESSA=\
	".\HeapExplorer.h"\
	".\message.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Message.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mystring.cpp
DEP_CPP_MYSTR=\
	".\mystring.h"\
	

"$(INTDIR)\Mystring.obj" : $(SOURCE) $(DEP_CPP_MYSTR) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropBut.cpp
DEP_CPP_PROPB=\
	".\HeapExplorer.h"\
	".\infomsg.h"\
	".\propbut.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\PropBut.obj" : $(SOURCE) $(DEP_CPP_PROPB) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StrDisp.cpp
DEP_CPP_STRDI=\
	".\HeapExplorer.h"\
	".\mystring.h"\
	".\StdAfx.h"\
	".\strdisp.h"\
	

"$(INTDIR)\StrDisp.obj" : $(SOURCE) $(DEP_CPP_STRDI) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.cpp
DEP_CPP_ABOUT=\
	".\About.h"\
	".\HeapExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"\
 "$(INTDIR)\HeapExplorer.pch"


# End Source File
# End Target
# End Project
################################################################################
