# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=FontExplorer - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to FontExplorer - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "FontExplorer - Win32 Release" && "$(CFG)" !=\
 "FontExplorer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "FontExplorer.mak" CFG="FontExplorer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FontExplorer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FontExplorer - Win32 Debug" (based on "Win32 (x86) Application")
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
# PROP Target_Last_Scanned "FontExplorer - Win32 Debug"
MTL=mktyplib.exe
RSC=rc.exe
CPP=cl.exe

!IF  "$(CFG)" == "FontExplorer - Win32 Release"

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

ALL : "$(OUTDIR)\FontExplorer.exe" "$(OUTDIR)\FontExplorer.hlp"

CLEAN : 
	-@erase "$(INTDIR)\Abc.obj"
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Chformat.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\CPlace.obj"
	-@erase "$(INTDIR)\Degrees.obj"
	-@erase "$(INTDIR)\fontdesc.obj"
	-@erase "$(INTDIR)\FontDisp.obj"
	-@erase "$(INTDIR)\FontExp.obj"
	-@erase "$(INTDIR)\FontExplorer.hlp"
	-@erase "$(INTDIR)\FontExplorer.obj"
	-@erase "$(INTDIR)\FontExplorer.pch"
	-@erase "$(INTDIR)\FontExplorer.res"
	-@erase "$(INTDIR)\FontExplorerDoc.obj"
	-@erase "$(INTDIR)\FontExplorerView.obj"
	-@erase "$(INTDIR)\FontMet.obj"
	-@erase "$(INTDIR)\FontName.obj"
	-@erase "$(INTDIR)\Fontobj.obj"
	-@erase "$(INTDIR)\fontsel.obj"
	-@erase "$(INTDIR)\Fontstr.obj"
	-@erase "$(INTDIR)\IDCombo.obj"
	-@erase "$(INTDIR)\Intedit.obj"
	-@erase "$(INTDIR)\Kern.obj"
	-@erase "$(INTDIR)\Kerning.obj"
	-@erase "$(INTDIR)\KernOpt.obj"
	-@erase "$(INTDIR)\Kp.obj"
	-@erase "$(INTDIR)\Legend.obj"
	-@erase "$(INTDIR)\lfexp.obj"
	-@erase "$(INTDIR)\Logfont.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Makefont.obj"
	-@erase "$(INTDIR)\Message.obj"
	-@erase "$(INTDIR)\Options.obj"
	-@erase "$(INTDIR)\Otm.obj"
	-@erase "$(INTDIR)\Panose.obj"
	-@erase "$(INTDIR)\poplist.obj"
	-@erase "$(INTDIR)\Raster.obj"
	-@erase "$(INTDIR)\Rotate.obj"
	-@erase "$(INTDIR)\Sample.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StockFnt.obj"
	-@erase "$(INTDIR)\Subclass.obj"
	-@erase "$(INTDIR)\Tm.obj"
	-@erase "$(INTDIR)\Toclip.obj"
	-@erase "$(OUTDIR)\FontExplorer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/FontExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/FontExplorer.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/FontExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/FontExplorer.pdb" /machine:I386\
 /out:"$(OUTDIR)/FontExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Abc.obj" \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Chformat.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\CPlace.obj" \
	"$(INTDIR)\Degrees.obj" \
	"$(INTDIR)\fontdesc.obj" \
	"$(INTDIR)\FontDisp.obj" \
	"$(INTDIR)\FontExp.obj" \
	"$(INTDIR)\FontExplorer.obj" \
	"$(INTDIR)\FontExplorer.res" \
	"$(INTDIR)\FontExplorerDoc.obj" \
	"$(INTDIR)\FontExplorerView.obj" \
	"$(INTDIR)\FontMet.obj" \
	"$(INTDIR)\FontName.obj" \
	"$(INTDIR)\Fontobj.obj" \
	"$(INTDIR)\fontsel.obj" \
	"$(INTDIR)\Fontstr.obj" \
	"$(INTDIR)\IDCombo.obj" \
	"$(INTDIR)\Intedit.obj" \
	"$(INTDIR)\Kern.obj" \
	"$(INTDIR)\Kerning.obj" \
	"$(INTDIR)\KernOpt.obj" \
	"$(INTDIR)\Kp.obj" \
	"$(INTDIR)\Legend.obj" \
	"$(INTDIR)\lfexp.obj" \
	"$(INTDIR)\Logfont.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Makefont.obj" \
	"$(INTDIR)\Message.obj" \
	"$(INTDIR)\Options.obj" \
	"$(INTDIR)\Otm.obj" \
	"$(INTDIR)\Panose.obj" \
	"$(INTDIR)\poplist.obj" \
	"$(INTDIR)\Raster.obj" \
	"$(INTDIR)\Rotate.obj" \
	"$(INTDIR)\Sample.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StockFnt.obj" \
	"$(INTDIR)\Subclass.obj" \
	"$(INTDIR)\Tm.obj" \
	"$(INTDIR)\Toclip.obj"

"$(OUTDIR)\FontExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "FontExplorer - Win32 Debug"

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

ALL : "$(OUTDIR)\FontExplorer.exe" "$(OUTDIR)\FontExplorer.hlp"

CLEAN : 
	-@erase "$(INTDIR)\Abc.obj"
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Chformat.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\CPlace.obj"
	-@erase "$(INTDIR)\Degrees.obj"
	-@erase "$(INTDIR)\fontdesc.obj"
	-@erase "$(INTDIR)\FontDisp.obj"
	-@erase "$(INTDIR)\FontExp.obj"
	-@erase "$(INTDIR)\FontExplorer.hlp"
	-@erase "$(INTDIR)\FontExplorer.obj"
	-@erase "$(INTDIR)\FontExplorer.pch"
	-@erase "$(INTDIR)\FontExplorer.res"
	-@erase "$(INTDIR)\FontExplorerDoc.obj"
	-@erase "$(INTDIR)\FontExplorerView.obj"
	-@erase "$(INTDIR)\FontMet.obj"
	-@erase "$(INTDIR)\FontName.obj"
	-@erase "$(INTDIR)\Fontobj.obj"
	-@erase "$(INTDIR)\fontsel.obj"
	-@erase "$(INTDIR)\Fontstr.obj"
	-@erase "$(INTDIR)\IDCombo.obj"
	-@erase "$(INTDIR)\Intedit.obj"
	-@erase "$(INTDIR)\Kern.obj"
	-@erase "$(INTDIR)\Kerning.obj"
	-@erase "$(INTDIR)\KernOpt.obj"
	-@erase "$(INTDIR)\Kp.obj"
	-@erase "$(INTDIR)\Legend.obj"
	-@erase "$(INTDIR)\lfexp.obj"
	-@erase "$(INTDIR)\Logfont.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Makefont.obj"
	-@erase "$(INTDIR)\Message.obj"
	-@erase "$(INTDIR)\Options.obj"
	-@erase "$(INTDIR)\Otm.obj"
	-@erase "$(INTDIR)\Panose.obj"
	-@erase "$(INTDIR)\poplist.obj"
	-@erase "$(INTDIR)\Raster.obj"
	-@erase "$(INTDIR)\Rotate.obj"
	-@erase "$(INTDIR)\Sample.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StockFnt.obj"
	-@erase "$(INTDIR)\Subclass.obj"
	-@erase "$(INTDIR)\Tm.obj"
	-@erase "$(INTDIR)\Toclip.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\FontExplorer.exe"
	-@erase "$(OUTDIR)\FontExplorer.ilk"
	-@erase "$(OUTDIR)\FontExplorer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/FontExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/FontExplorer.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/FontExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 version.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=version.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/FontExplorer.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/FontExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Abc.obj" \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Chformat.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\CPlace.obj" \
	"$(INTDIR)\Degrees.obj" \
	"$(INTDIR)\fontdesc.obj" \
	"$(INTDIR)\FontDisp.obj" \
	"$(INTDIR)\FontExp.obj" \
	"$(INTDIR)\FontExplorer.obj" \
	"$(INTDIR)\FontExplorer.res" \
	"$(INTDIR)\FontExplorerDoc.obj" \
	"$(INTDIR)\FontExplorerView.obj" \
	"$(INTDIR)\FontMet.obj" \
	"$(INTDIR)\FontName.obj" \
	"$(INTDIR)\Fontobj.obj" \
	"$(INTDIR)\fontsel.obj" \
	"$(INTDIR)\Fontstr.obj" \
	"$(INTDIR)\IDCombo.obj" \
	"$(INTDIR)\Intedit.obj" \
	"$(INTDIR)\Kern.obj" \
	"$(INTDIR)\Kerning.obj" \
	"$(INTDIR)\KernOpt.obj" \
	"$(INTDIR)\Kp.obj" \
	"$(INTDIR)\Legend.obj" \
	"$(INTDIR)\lfexp.obj" \
	"$(INTDIR)\Logfont.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Makefont.obj" \
	"$(INTDIR)\Message.obj" \
	"$(INTDIR)\Options.obj" \
	"$(INTDIR)\Otm.obj" \
	"$(INTDIR)\Panose.obj" \
	"$(INTDIR)\poplist.obj" \
	"$(INTDIR)\Raster.obj" \
	"$(INTDIR)\Rotate.obj" \
	"$(INTDIR)\Sample.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StockFnt.obj" \
	"$(INTDIR)\Subclass.obj" \
	"$(INTDIR)\Tm.obj" \
	"$(INTDIR)\Toclip.obj"

"$(OUTDIR)\FontExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "FontExplorer - Win32 Release"
# Name "FontExplorer - Win32 Debug"

!IF  "$(CFG)" == "FontExplorer - Win32 Release"

!ELSEIF  "$(CFG)" == "FontExplorer - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "FontExplorer - Win32 Release"

!ELSEIF  "$(CFG)" == "FontExplorer - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FontExplorer.cpp
DEP_CPP_FONTE=\
	".\about.h"\
	".\ChildFrm.h"\
	".\cplace.h"\
	".\degrees.h"\
	".\dispopts.h"\
	".\fontdesc.h"\
	".\fontdisp.h"\
	".\fontexp.h"\
	".\FontExplorer.h"\
	".\FontExplorerDoc.h"\
	".\FontExplorerView.h"\
	".\fontmet.h"\
	".\fontname.h"\
	".\fontobj.h"\
	".\fontopt.h"\
	".\fontsel.h"\
	".\idcombo.h"\
	".\IntEdit.h"\
	".\kerning.h"\
	".\legend.h"\
	".\legendi.h"\
	".\lfexp.h"\
	".\MainFrm.h"\
	".\message.h"\
	".\poplist.h"\
	".\raster.h"\
	".\rotate.h"\
	".\sample.h"\
	".\StdAfx.h"\
	".\stockfnt.h"\
	

"$(INTDIR)\FontExplorer.obj" : $(SOURCE) $(DEP_CPP_FONTE) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "FontExplorer - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/FontExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\FontExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "FontExplorer - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/FontExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\FontExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\FontExplorer.h"\
	".\MainFrm.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ChildFrm.cpp
DEP_CPP_CHILD=\
	".\ChildFrm.h"\
	".\FontExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FontExplorerDoc.cpp
DEP_CPP_FONTEX=\
	".\FontExplorer.h"\
	".\FontExplorerDoc.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FontExplorerDoc.obj" : $(SOURCE) $(DEP_CPP_FONTEX) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FontExplorerView.cpp
DEP_CPP_FONTEXP=\
	".\FontExplorer.h"\
	".\FontExplorerDoc.h"\
	".\FontExplorerView.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FontExplorerView.obj" : $(SOURCE) $(DEP_CPP_FONTEXP) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FontExplorer.rc
DEP_RSC_FONTEXPL=\
	".\res\bitmap1.bmp"\
	".\res\devfont.bmp"\
	".\res\FontExplorer.ico"\
	".\res\FontExplorer.rc2"\
	".\res\FontExplorerDoc.ico"\
	".\res\icon1.ico"\
	".\res\icon2.ico"\
	".\res\icon3.ico"\
	".\res\icon4.ico"\
	".\res\jmnco.ico"\
	".\res\mainfram.bmp"\
	".\res\rasfont.bmp"\
	".\res\Toolbar.bmp"\
	".\res\ttfont.bmp"\
	".\res\vecfont.bmp"\
	

"$(INTDIR)\FontExplorer.res" : $(SOURCE) $(DEP_RSC_FONTEXPL) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\hlp\FontExplorer.hpj

!IF  "$(CFG)" == "FontExplorer - Win32 Release"

# Begin Custom Build - Making help file...
OutDir=.\Release
ProjDir=.
TargetName=FontExplorer
InputPath=.\hlp\FontExplorer.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "FontExplorer - Win32 Debug"

# Begin Custom Build - Making help file...
OutDir=.\Debug
ProjDir=.
TargetName=FontExplorer
InputPath=.\hlp\FontExplorer.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CPlace.cpp
DEP_CPP_CPLAC=\
	".\cplace.h"\
	".\FontExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CPlace.obj" : $(SOURCE) $(DEP_CPP_CPLAC) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Intedit.cpp
DEP_CPP_INTED=\
	".\IntEdit.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Intedit.obj" : $(SOURCE) $(DEP_CPP_INTED) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Kerning.cpp
DEP_CPP_KERNI=\
	".\color.h"\
	".\dispopts.h"\
	".\fontdesc.h"\
	".\fontdisp.h"\
	".\FontExplorer.h"\
	".\kerning.h"\
	".\kernopt.h"\
	".\legend.h"\
	".\legendi.h"\
	".\poplist.h"\
	".\StdAfx.h"\
	".\toclip.h"\
	

"$(INTDIR)\Kerning.obj" : $(SOURCE) $(DEP_CPP_KERNI) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.cpp
DEP_CPP_ABOUT=\
	".\about.h"\
	".\FontExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Fontobj.cpp
DEP_CPP_FONTO=\
	".\fontobj.h"\
	".\fontstr.h"\
	".\makefont.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Fontobj.obj" : $(SOURCE) $(DEP_CPP_FONTO) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Toclip.cpp
DEP_CPP_TOCLI=\
	".\StdAfx.h"\
	".\toclip.h"\
	

"$(INTDIR)\Toclip.obj" : $(SOURCE) $(DEP_CPP_TOCLI) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Degrees.cpp
DEP_CPP_DEGRE=\
	".\degrees.h"\
	".\FontExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Degrees.obj" : $(SOURCE) $(DEP_CPP_DEGRE) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\fontdesc.CPP
DEP_CPP_FONTD=\
	".\color.h"\
	".\fontdesc.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\fontdesc.obj" : $(SOURCE) $(DEP_CPP_FONTD) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FontDisp.cpp
DEP_CPP_FONTDI=\
	".\dispopts.h"\
	".\fontdesc.h"\
	".\fontdisp.h"\
	".\FontExplorer.h"\
	".\kern.h"\
	".\legend.h"\
	".\legendi.h"\
	".\poplist.h"\
	".\savedc.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FontDisp.obj" : $(SOURCE) $(DEP_CPP_FONTDI) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FontExp.cpp
DEP_CPP_FONTEXP_=\
	".\abc.h"\
	".\chformat.h"\
	".\dispopts.h"\
	".\fontdesc.h"\
	".\fontdisp.h"\
	".\fontexp.h"\
	".\FontExplorer.h"\
	".\fontmet.h"\
	".\kp.h"\
	".\legend.h"\
	".\legendi.h"\
	".\logfont.h"\
	".\options.h"\
	".\otm.h"\
	".\poplist.h"\
	".\StdAfx.h"\
	".\subclass.h"\
	".\tm.h"\
	".\toclip.h"\
	

"$(INTDIR)\FontExp.obj" : $(SOURCE) $(DEP_CPP_FONTEXP_) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FontMet.cpp
DEP_CPP_FONTM=\
	".\FontExplorer.h"\
	".\fontmet.h"\
	".\fontobj.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FontMet.obj" : $(SOURCE) $(DEP_CPP_FONTM) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\FontName.cpp
DEP_CPP_FONTN=\
	".\FontExplorer.h"\
	".\fontname.h"\
	".\savedc.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\FontName.obj" : $(SOURCE) $(DEP_CPP_FONTN) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\fontsel.cpp
DEP_CPP_FONTS=\
	".\fontobj.h"\
	".\fontopt.h"\
	".\fontsel.h"\
	".\fontstr.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\fontsel.obj" : $(SOURCE) $(DEP_CPP_FONTS) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Fontstr.cpp
DEP_CPP_FONTST=\
	".\fontstr.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Fontstr.obj" : $(SOURCE) $(DEP_CPP_FONTST) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IDCombo.cpp
DEP_CPP_IDCOM=\
	".\FontExplorer.h"\
	".\idcombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\IDCombo.obj" : $(SOURCE) $(DEP_CPP_IDCOM) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Kern.cpp
DEP_CPP_KERN_=\
	".\kern.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Kern.obj" : $(SOURCE) $(DEP_CPP_KERN_) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\KernOpt.cpp
DEP_CPP_KERNO=\
	".\dispopts.h"\
	".\FontExplorer.h"\
	".\kernopt.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\KernOpt.obj" : $(SOURCE) $(DEP_CPP_KERNO) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Kp.cpp
DEP_CPP_KP_CP=\
	".\kp.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Kp.obj" : $(SOURCE) $(DEP_CPP_KP_CP) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Legend.cpp
DEP_CPP_LEGEN=\
	".\FontExplorer.h"\
	".\legend.h"\
	".\legendi.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Legend.obj" : $(SOURCE) $(DEP_CPP_LEGEN) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\lfexp.cpp
DEP_CPP_LFEXP=\
	".\degrees.h"\
	".\FontExplorer.h"\
	".\fontmet.h"\
	".\fontname.h"\
	".\idcombo.h"\
	".\kp.h"\
	".\lfexp.h"\
	".\logfont.h"\
	".\message.h"\
	".\otm.h"\
	".\rotate.h"\
	".\sample.h"\
	".\StdAfx.h"\
	".\tm.h"\
	

"$(INTDIR)\lfexp.obj" : $(SOURCE) $(DEP_CPP_LFEXP) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Logfont.cpp
DEP_CPP_LOGFO=\
	".\logfont.h"\
	".\panose.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Logfont.obj" : $(SOURCE) $(DEP_CPP_LOGFO) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Makefont.cpp
DEP_CPP_MAKEF=\
	".\makefont.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Makefont.obj" : $(SOURCE) $(DEP_CPP_MAKEF) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Message.cpp
DEP_CPP_MESSA=\
	".\FontExplorer.h"\
	".\message.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Message.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Options.cpp
DEP_CPP_OPTIO=\
	".\dispopts.h"\
	".\FontExplorer.h"\
	".\options.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Options.obj" : $(SOURCE) $(DEP_CPP_OPTIO) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Otm.cpp
DEP_CPP_OTM_C=\
	".\otm.h"\
	".\panose.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Otm.obj" : $(SOURCE) $(DEP_CPP_OTM_C) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Panose.cpp
DEP_CPP_PANOS=\
	".\panose.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Panose.obj" : $(SOURCE) $(DEP_CPP_PANOS) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\poplist.cpp
DEP_CPP_POPLI=\
	".\FontExplorer.h"\
	".\poplist.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\poplist.obj" : $(SOURCE) $(DEP_CPP_POPLI) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Raster.cpp
DEP_CPP_RASTE=\
	".\FontExplorer.h"\
	".\IntEdit.h"\
	".\raster.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Raster.obj" : $(SOURCE) $(DEP_CPP_RASTE) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Rotate.cpp
DEP_CPP_ROTAT=\
	".\degrees.h"\
	".\FontExplorer.h"\
	".\rotate.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Rotate.obj" : $(SOURCE) $(DEP_CPP_ROTAT) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Sample.cpp
DEP_CPP_SAMPL=\
	".\FontExplorer.h"\
	".\sample.h"\
	".\savedc.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Sample.obj" : $(SOURCE) $(DEP_CPP_SAMPL) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StockFnt.cpp
DEP_CPP_STOCK=\
	".\FontExplorer.h"\
	".\StdAfx.h"\
	".\stockfnt.h"\
	".\toclip.h"\
	

"$(INTDIR)\StockFnt.obj" : $(SOURCE) $(DEP_CPP_STOCK) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Subclass.cpp
DEP_CPP_SUBCL=\
	".\StdAfx.h"\
	".\subclass.h"\
	

"$(INTDIR)\Subclass.obj" : $(SOURCE) $(DEP_CPP_SUBCL) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Tm.cpp
DEP_CPP_TM_CP=\
	".\logfont.h"\
	".\StdAfx.h"\
	".\tm.h"\
	

"$(INTDIR)\Tm.obj" : $(SOURCE) $(DEP_CPP_TM_CP) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Chformat.cpp
DEP_CPP_CHFOR=\
	".\StdAfx.h"\
	

"$(INTDIR)\Chformat.obj" : $(SOURCE) $(DEP_CPP_CHFOR) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Abc.cpp
DEP_CPP_ABC_C=\
	".\chformat.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Abc.obj" : $(SOURCE) $(DEP_CPP_ABC_C) "$(INTDIR)"\
 "$(INTDIR)\FontExplorer.pch"


# End Source File
# End Target
# End Project
################################################################################
