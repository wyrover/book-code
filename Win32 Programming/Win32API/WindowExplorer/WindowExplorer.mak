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
!MESSAGE NMAKE /f "Window Explorer.mak" CFG="Window Explorer - Win32 Debug"
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
# PROP Target_Last_Scanned "Window Explorer - Win32 Debug"
MTL=mktyplib.exe
RSC=rc.exe
CPP=cl.exe

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
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\Createex.obj"
	-@erase "$(INTDIR)\Ctlmap.obj"
	-@erase "$(INTDIR)\Idcombo.obj"
	-@erase "$(INTDIR)\Is95.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Message.obj"
	-@erase "$(INTDIR)\Msglog.obj"
	-@erase "$(INTDIR)\NumericEdit.obj"
	-@erase "$(INTDIR)\Regclass.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Styles.obj"
	-@erase "$(INTDIR)\Window Explorer.pch"
	-@erase "$(INTDIR)\WindowExplorer.hlp"
	-@erase "$(INTDIR)\Winlist.obj"
	-@erase "$(INTDIR)\Winmgr.obj"
	-@erase "$(INTDIR)\WndExp.obj"
	-@erase "$(INTDIR)\WndExp.res"
	-@erase "$(INTDIR)\WndExpDoc.obj"
	-@erase "$(INTDIR)\WndExpView.obj"
	-@erase "$(OUTDIR)\WindowExplorer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/Window Explorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/WndExp.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Window Explorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /machine:I386 /out:"Release/WindowExplorer.exe"
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/WindowExplorer.pdb" /machine:I386\
 /out:"$(OUTDIR)/WindowExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\Createex.obj" \
	"$(INTDIR)\Ctlmap.obj" \
	"$(INTDIR)\Idcombo.obj" \
	"$(INTDIR)\Is95.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Message.obj" \
	"$(INTDIR)\Msglog.obj" \
	"$(INTDIR)\NumericEdit.obj" \
	"$(INTDIR)\Regclass.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Styles.obj" \
	"$(INTDIR)\Winlist.obj" \
	"$(INTDIR)\Winmgr.obj" \
	"$(INTDIR)\WndExp.obj" \
	"$(INTDIR)\WndExp.res" \
	"$(INTDIR)\WndExpDoc.obj" \
	"$(INTDIR)\WndExpView.obj"

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
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\Createex.obj"
	-@erase "$(INTDIR)\Ctlmap.obj"
	-@erase "$(INTDIR)\Idcombo.obj"
	-@erase "$(INTDIR)\Is95.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Message.obj"
	-@erase "$(INTDIR)\Msglog.obj"
	-@erase "$(INTDIR)\NumericEdit.obj"
	-@erase "$(INTDIR)\Regclass.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Styles.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\Window Explorer.pch"
	-@erase "$(INTDIR)\WindowExplorer.hlp"
	-@erase "$(INTDIR)\Winlist.obj"
	-@erase "$(INTDIR)\Winmgr.obj"
	-@erase "$(INTDIR)\WndExp.obj"
	-@erase "$(INTDIR)\WndExp.res"
	-@erase "$(INTDIR)\WndExpDoc.obj"
	-@erase "$(INTDIR)\WndExpView.obj"
	-@erase "$(OUTDIR)\WindowExplorer.exe"
	-@erase "$(OUTDIR)\WindowExplorer.ilk"
	-@erase "$(OUTDIR)\WindowExplorer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/Window Explorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/WndExp.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Window Explorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/WindowExplorer.exe"
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/WindowExplorer.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/WindowExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\Createex.obj" \
	"$(INTDIR)\Ctlmap.obj" \
	"$(INTDIR)\Idcombo.obj" \
	"$(INTDIR)\Is95.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Message.obj" \
	"$(INTDIR)\Msglog.obj" \
	"$(INTDIR)\NumericEdit.obj" \
	"$(INTDIR)\Regclass.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Styles.obj" \
	"$(INTDIR)\Winlist.obj" \
	"$(INTDIR)\Winmgr.obj" \
	"$(INTDIR)\WndExp.obj" \
	"$(INTDIR)\WndExp.res" \
	"$(INTDIR)\WndExpDoc.obj" \
	"$(INTDIR)\WndExpView.obj"

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

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "Window Explorer - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/Window Explorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Window Explorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "Window Explorer - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/Window Explorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Window Explorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\createex.h"\
	".\idcombo.h"\
	".\MainFrm.h"\
	".\message.h"\
	".\numericEdit.h"\
	".\StdAfx.h"\
	".\styles.h"\
	".\winList.h"\
	".\WinMgr.h"\
	".\WndExp.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\Window Explorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ChildFrm.cpp
DEP_CPP_CHILD=\
	".\ChildFrm.h"\
	".\StdAfx.h"\
	".\WndExp.h"\
	

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\Window Explorer.pch"


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
################################################################################
# Begin Source File

SOURCE=.\Winmgr.cpp
DEP_CPP_WINMG=\
	".\StdAfx.h"\
	".\WinMgr.h"\
	

"$(INTDIR)\Winmgr.obj" : $(SOURCE) $(DEP_CPP_WINMG) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Createex.cpp
DEP_CPP_CREAT=\
	".\createex.h"\
	".\ctlmap.h"\
	".\idcombo.h"\
	".\is95.h"\
	".\MainFrm.h"\
	".\message.h"\
	".\numericEdit.h"\
	".\StdAfx.h"\
	".\winList.h"\
	".\WinMgr.h"\
	".\WndExp.h"\
	

"$(INTDIR)\Createex.obj" : $(SOURCE) $(DEP_CPP_CREAT) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Ctlmap.cpp
DEP_CPP_CTLMA=\
	".\ctlmap.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Ctlmap.obj" : $(SOURCE) $(DEP_CPP_CTLMA) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Idcombo.cpp
DEP_CPP_IDCOM=\
	".\idcombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Idcombo.obj" : $(SOURCE) $(DEP_CPP_IDCOM) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Is95.cpp
DEP_CPP_IS95_=\
	".\StdAfx.h"\
	

"$(INTDIR)\Is95.obj" : $(SOURCE) $(DEP_CPP_IS95_) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Message.cpp
DEP_CPP_MESSA=\
	".\message.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Message.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Msglog.cpp
DEP_CPP_MSGLO=\
	".\MsgLog.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Msglog.obj" : $(SOURCE) $(DEP_CPP_MSGLO) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\NumericEdit.cpp
DEP_CPP_NUMER=\
	".\numericEdit.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\NumericEdit.obj" : $(SOURCE) $(DEP_CPP_NUMER) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Regclass.cpp
DEP_CPP_REGCL=\
	".\regclass.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Regclass.obj" : $(SOURCE) $(DEP_CPP_REGCL) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Styles.cpp
DEP_CPP_STYLE=\
	".\ctlmap.h"\
	".\numericEdit.h"\
	".\StdAfx.h"\
	".\styles.h"\
	".\WndExp.h"\
	

"$(INTDIR)\Styles.obj" : $(SOURCE) $(DEP_CPP_STYLE) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Winlist.cpp
DEP_CPP_WINLI=\
	".\StdAfx.h"\
	".\winList.h"\
	".\WndExp.h"\
	

"$(INTDIR)\Winlist.obj" : $(SOURCE) $(DEP_CPP_WINLI) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.cpp
DEP_CPP_ABOUT=\
	".\About.h"\
	".\StdAfx.h"\
	".\WndExp.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\WndExpView.cpp
DEP_CPP_WNDEX=\
	".\StdAfx.h"\
	".\WndExp.h"\
	".\WndExpDoc.h"\
	".\WndExpView.h"\
	

"$(INTDIR)\WndExpView.obj" : $(SOURCE) $(DEP_CPP_WNDEX) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\WndExpDoc.cpp
DEP_CPP_WNDEXP=\
	".\StdAfx.h"\
	".\WndExp.h"\
	".\WndExpDoc.h"\
	

"$(INTDIR)\WndExpDoc.obj" : $(SOURCE) $(DEP_CPP_WNDEXP) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\WndExp.cpp
DEP_CPP_WNDEXP_=\
	".\About.h"\
	".\ChildFrm.h"\
	".\MainFrm.h"\
	".\regclass.h"\
	".\StdAfx.h"\
	".\WndExp.h"\
	".\WndExpDoc.h"\
	".\WndExpView.h"\
	

"$(INTDIR)\WndExp.obj" : $(SOURCE) $(DEP_CPP_WNDEXP_) "$(INTDIR)"\
 "$(INTDIR)\WindowExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\WndExp.rc
DEP_RSC_WNDEXP_R=\
	".\res\child.ico"\
	".\res\jmnco.ico"\
	".\res\overlap.ico"\
	".\res\popup.ico"\
	".\res\Toolbar.bmp"\
	".\res\WndExp.ico"\
	".\res\WndExp.rc2"\
	".\res\WndExpDoc.ico"\
	

"$(INTDIR)\WndExp.res" : $(SOURCE) $(DEP_RSC_WNDEXP_R) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
