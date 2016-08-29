# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=MenuExplorer - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to MenuExplorer - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MenuExplorer - Win32 Release" && "$(CFG)" !=\
 "MenuExplorer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "MenuExplorer.mak" CFG="MenuExplorer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MenuExplorer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MenuExplorer - Win32 Debug" (based on "Win32 (x86) Application")
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

!IF  "$(CFG)" == "MenuExplorer - Win32 Release"

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

ALL : "$(OUTDIR)\MenuExplorer.exe" "$(OUTDIR)\MenuExplorer.hlp"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Bitmaps.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\Color.obj"
	-@erase "$(INTDIR)\Gcombo.obj"
	-@erase "$(INTDIR)\IDCombo.obj"
	-@erase "$(INTDIR)\Intedit.obj"
	-@erase "$(INTDIR)\Is95.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MenuChk.obj"
	-@erase "$(INTDIR)\menucomb.obj"
	-@erase "$(INTDIR)\Menudlg.obj"
	-@erase "$(INTDIR)\MenuExplorer.hlp"
	-@erase "$(INTDIR)\MenuExplorer.obj"
	-@erase "$(INTDIR)\MenuExplorer.pch"
	-@erase "$(INTDIR)\MenuExplorer.res"
	-@erase "$(INTDIR)\MenuExplorerDoc.obj"
	-@erase "$(INTDIR)\MenuExplorerView.obj"
	-@erase "$(INTDIR)\MenuInfo.obj"
	-@erase "$(INTDIR)\Menuitem.obj"
	-@erase "$(INTDIR)\MenuList.obj"
	-@erase "$(INTDIR)\menusht.obj"
	-@erase "$(INTDIR)\MsgLog.obj"
	-@erase "$(INTDIR)\OD.obj"
	-@erase "$(INTDIR)\Odinfo.obj"
	-@erase "$(INTDIR)\Odmenu.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SysColor.obj"
	-@erase "$(INTDIR)\TinyButt.obj"
	-@erase "$(INTDIR)\TPM.obj"
	-@erase "$(INTDIR)\Xform.obj"
	-@erase "$(OUTDIR)\MenuExplorer.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/MenuExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/MenuExplorer.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/MenuExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib comctl32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=version.lib comctl32.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/MenuExplorer.pdb" /machine:I386\
 /out:"$(OUTDIR)/MenuExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Bitmaps.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\Color.obj" \
	"$(INTDIR)\Gcombo.obj" \
	"$(INTDIR)\IDCombo.obj" \
	"$(INTDIR)\Intedit.obj" \
	"$(INTDIR)\Is95.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MenuChk.obj" \
	"$(INTDIR)\menucomb.obj" \
	"$(INTDIR)\Menudlg.obj" \
	"$(INTDIR)\MenuExplorer.obj" \
	"$(INTDIR)\MenuExplorer.res" \
	"$(INTDIR)\MenuExplorerDoc.obj" \
	"$(INTDIR)\MenuExplorerView.obj" \
	"$(INTDIR)\MenuInfo.obj" \
	"$(INTDIR)\Menuitem.obj" \
	"$(INTDIR)\MenuList.obj" \
	"$(INTDIR)\menusht.obj" \
	"$(INTDIR)\MsgLog.obj" \
	"$(INTDIR)\OD.obj" \
	"$(INTDIR)\Odinfo.obj" \
	"$(INTDIR)\Odmenu.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SysColor.obj" \
	"$(INTDIR)\TinyButt.obj" \
	"$(INTDIR)\TPM.obj" \
	"$(INTDIR)\Xform.obj"

"$(OUTDIR)\MenuExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MenuExplorer - Win32 Debug"

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

ALL : "$(OUTDIR)\MenuExplorer.exe" "$(OUTDIR)\MenuExplorer.hlp"

CLEAN : 
	-@erase "$(INTDIR)\About.obj"
	-@erase "$(INTDIR)\Bitmaps.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\Color.obj"
	-@erase "$(INTDIR)\Gcombo.obj"
	-@erase "$(INTDIR)\IDCombo.obj"
	-@erase "$(INTDIR)\Intedit.obj"
	-@erase "$(INTDIR)\Is95.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MenuChk.obj"
	-@erase "$(INTDIR)\menucomb.obj"
	-@erase "$(INTDIR)\Menudlg.obj"
	-@erase "$(INTDIR)\MenuExplorer.hlp"
	-@erase "$(INTDIR)\MenuExplorer.obj"
	-@erase "$(INTDIR)\MenuExplorer.pch"
	-@erase "$(INTDIR)\MenuExplorer.res"
	-@erase "$(INTDIR)\MenuExplorerDoc.obj"
	-@erase "$(INTDIR)\MenuExplorerView.obj"
	-@erase "$(INTDIR)\MenuInfo.obj"
	-@erase "$(INTDIR)\Menuitem.obj"
	-@erase "$(INTDIR)\MenuList.obj"
	-@erase "$(INTDIR)\menusht.obj"
	-@erase "$(INTDIR)\MsgLog.obj"
	-@erase "$(INTDIR)\OD.obj"
	-@erase "$(INTDIR)\Odinfo.obj"
	-@erase "$(INTDIR)\Odmenu.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SysColor.obj"
	-@erase "$(INTDIR)\TinyButt.obj"
	-@erase "$(INTDIR)\TPM.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\Xform.obj"
	-@erase "$(OUTDIR)\MenuExplorer.exe"
	-@erase "$(OUTDIR)\MenuExplorer.ilk"
	-@erase "$(OUTDIR)\MenuExplorer.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/MenuExplorer.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/MenuExplorer.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/MenuExplorer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 version.lib comctl32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=version.lib comctl32.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/MenuExplorer.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/MenuExplorer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\About.obj" \
	"$(INTDIR)\Bitmaps.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\Color.obj" \
	"$(INTDIR)\Gcombo.obj" \
	"$(INTDIR)\IDCombo.obj" \
	"$(INTDIR)\Intedit.obj" \
	"$(INTDIR)\Is95.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MenuChk.obj" \
	"$(INTDIR)\menucomb.obj" \
	"$(INTDIR)\Menudlg.obj" \
	"$(INTDIR)\MenuExplorer.obj" \
	"$(INTDIR)\MenuExplorer.res" \
	"$(INTDIR)\MenuExplorerDoc.obj" \
	"$(INTDIR)\MenuExplorerView.obj" \
	"$(INTDIR)\MenuInfo.obj" \
	"$(INTDIR)\Menuitem.obj" \
	"$(INTDIR)\MenuList.obj" \
	"$(INTDIR)\menusht.obj" \
	"$(INTDIR)\MsgLog.obj" \
	"$(INTDIR)\OD.obj" \
	"$(INTDIR)\Odinfo.obj" \
	"$(INTDIR)\Odmenu.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SysColor.obj" \
	"$(INTDIR)\TinyButt.obj" \
	"$(INTDIR)\TPM.obj" \
	"$(INTDIR)\Xform.obj"

"$(OUTDIR)\MenuExplorer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "MenuExplorer - Win32 Release"
# Name "MenuExplorer - Win32 Debug"

!IF  "$(CFG)" == "MenuExplorer - Win32 Release"

!ELSEIF  "$(CFG)" == "MenuExplorer - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\ReadMe.txt

!IF  "$(CFG)" == "MenuExplorer - Win32 Release"

!ELSEIF  "$(CFG)" == "MenuExplorer - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MenuExplorer.cpp

"$(INTDIR)\MenuExplorer.obj" : $(SOURCE) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "MenuExplorer - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/MenuExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MenuExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "MenuExplorer - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/MenuExplorer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MenuExplorer.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\Bitmaps.h"\
	".\Color.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\IntEdit.h"\
	".\MainFrm.h"\
	".\MenuChk.h"\
	".\menudlg.h"\
	".\MenuExplorer.h"\
	".\MenuInfo.h"\
	".\menuitem.h"\
	".\menulist.h"\
	".\MenuSht.h"\
	".\msglog.h"\
	".\OD.h"\
	".\odmenu.h"\
	".\StdAfx.h"\
	".\syscolor.h"\
	".\tinybutt.h"\
	".\TPM.h"\
	".\uwm.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\ChildFrm.cpp
DEP_CPP_CHILD=\
	".\ChildFrm.h"\
	".\MenuExplorer.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ChildFrm.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MenuExplorerDoc.cpp
DEP_CPP_MENUE=\
	".\MenuExplorer.h"\
	".\MenuExplorerDoc.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MenuExplorerDoc.obj" : $(SOURCE) $(DEP_CPP_MENUE) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MenuExplorerView.cpp
DEP_CPP_MENUEX=\
	".\MenuExplorer.h"\
	".\MenuExplorerDoc.h"\
	".\MenuExplorerView.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MenuExplorerView.obj" : $(SOURCE) $(DEP_CPP_MENUEX) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MenuExplorer.rc
DEP_RSC_MENUEXP=\
	".\res\bitmap1.bmp"\
	".\res\check1.bmp"\
	".\res\check3.bmp"\
	".\res\check4.bmp"\
	".\res\check5.bmp"\
	".\res\check6.bmp"\
	".\res\check7.bmp"\
	".\res\check8.bmp"\
	".\res\computer.bmp"\
	".\res\flounder.bmp"\
	".\res\graycat.bmp"\
	".\res\jmnco.ico"\
	".\res\MenuExplorer.ico"\
	".\res\MenuExplorer.rc2"\
	".\res\MenuExplorerDoc.ico"\
	".\res\plaid.bmp"\
	".\res\Toolbar.bmp"\
	

"$(INTDIR)\MenuExplorer.res" : $(SOURCE) $(DEP_RSC_MENUEXP) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=.\hlp\MenuExplorer.hpj

!IF  "$(CFG)" == "MenuExplorer - Win32 Release"

# Begin Custom Build - Making help file...
OutDir=.\Release
ProjDir=.
TargetName=MenuExplorer
InputPath=.\hlp\MenuExplorer.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "MenuExplorer - Win32 Debug"

# Begin Custom Build - Making help file...
OutDir=.\Debug
ProjDir=.
TargetName=MenuExplorer
InputPath=.\hlp\MenuExplorer.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Xform.cpp
DEP_CPP_XFORM=\
	".\StdAfx.h"\
	".\xform.h"\
	

"$(INTDIR)\Xform.obj" : $(SOURCE) $(DEP_CPP_XFORM) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Bitmaps.cpp
DEP_CPP_BITMA=\
	".\Bitmaps.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Bitmaps.obj" : $(SOURCE) $(DEP_CPP_BITMA) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Color.cpp
DEP_CPP_COLOR=\
	".\Color.h"\
	".\colors.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Color.obj" : $(SOURCE) $(DEP_CPP_COLOR) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Gcombo.cpp
DEP_CPP_GCOMB=\
	".\gcombo.h"\
	".\idcombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Gcombo.obj" : $(SOURCE) $(DEP_CPP_GCOMB) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\IDCombo.cpp
DEP_CPP_IDCOM=\
	".\idcombo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\IDCombo.obj" : $(SOURCE) $(DEP_CPP_IDCOM) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Intedit.cpp
DEP_CPP_INTED=\
	".\IntEdit.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Intedit.obj" : $(SOURCE) $(DEP_CPP_INTED) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Is95.cpp
DEP_CPP_IS95_=\
	".\StdAfx.h"\
	

"$(INTDIR)\Is95.obj" : $(SOURCE) $(DEP_CPP_IS95_) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MenuChk.cpp
DEP_CPP_MENUC=\
	".\is95.h"\
	".\MenuChk.h"\
	".\StdAfx.h"\
	
NODEP_CPP_MENUC=\
	".\menuapp.h"\
	

"$(INTDIR)\MenuChk.obj" : $(SOURCE) $(DEP_CPP_MENUC) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\menucomb.CPP
DEP_CPP_MENUCO=\
	".\MenuComb.h"\
	".\menuitem.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\menucomb.obj" : $(SOURCE) $(DEP_CPP_MENUCO) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MenuInfo.cpp
DEP_CPP_MENUI=\
	".\Bitmaps.h"\
	".\IntEdit.h"\
	".\is95.h"\
	".\MenuChk.h"\
	".\MenuExplorer.h"\
	".\MenuInfo.h"\
	".\menuitem.h"\
	".\menulist.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\uwm.h"\
	

"$(INTDIR)\MenuInfo.obj" : $(SOURCE) $(DEP_CPP_MENUI) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Menuitem.cpp
DEP_CPP_MENUIT=\
	".\menuitem.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Menuitem.obj" : $(SOURCE) $(DEP_CPP_MENUIT) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\menusht.cpp
DEP_CPP_MENUS=\
	".\Bitmaps.h"\
	".\IntEdit.h"\
	".\is95.h"\
	".\menudlg.h"\
	".\MenuExplorer.h"\
	".\menuitem.h"\
	".\menulist.h"\
	".\MenuSht.h"\
	".\msglog.h"\
	".\StdAfx.h"\
	".\tinybutt.h"\
	".\uwm.h"\
	

"$(INTDIR)\menusht.obj" : $(SOURCE) $(DEP_CPP_MENUS) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MsgLog.cpp
DEP_CPP_MSGLO=\
	".\msglog.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\MsgLog.obj" : $(SOURCE) $(DEP_CPP_MSGLO) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\OD.cpp
DEP_CPP_OD_CP=\
	".\Color.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\IntEdit.h"\
	".\MenuChk.h"\
	".\MenuExplorer.h"\
	".\OD.h"\
	".\odinfo.h"\
	".\odmenu.h"\
	".\StdAfx.h"\
	".\syscolor.h"\
	".\tinybutt.h"\
	".\uwm.h"\
	".\xform.h"\
	

"$(INTDIR)\OD.obj" : $(SOURCE) $(DEP_CPP_OD_CP) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Odinfo.cpp
DEP_CPP_ODINF=\
	".\odinfo.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Odinfo.obj" : $(SOURCE) $(DEP_CPP_ODINF) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Odmenu.cpp
DEP_CPP_ODMEN=\
	".\odinfo.h"\
	".\odmenu.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\Odmenu.obj" : $(SOURCE) $(DEP_CPP_ODMEN) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\SysColor.cpp
DEP_CPP_SYSCO=\
	".\Color.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\StdAfx.h"\
	".\syscolor.h"\
	

"$(INTDIR)\SysColor.obj" : $(SOURCE) $(DEP_CPP_SYSCO) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TinyButt.cpp
DEP_CPP_TINYB=\
	".\StdAfx.h"\
	".\tinybutt.h"\
	

"$(INTDIR)\TinyButt.obj" : $(SOURCE) $(DEP_CPP_TINYB) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TPM.cpp
DEP_CPP_TPM_C=\
	".\is95.h"\
	".\MenuExplorer.h"\
	".\MenuSht.h"\
	".\StdAfx.h"\
	".\TPM.h"\
	".\uwm.h"\
	

"$(INTDIR)\TPM.obj" : $(SOURCE) $(DEP_CPP_TPM_C) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\About.cpp
DEP_CPP_ABOUT=\
	".\About.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\About.obj" : $(SOURCE) $(DEP_CPP_ABOUT) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Menudlg.cpp
DEP_CPP_MENUD=\
	".\Bitmaps.h"\
	".\Color.h"\
	".\gcombo.h"\
	".\idcombo.h"\
	".\IntEdit.h"\
	".\is95.h"\
	".\MainFrm.h"\
	".\MenuChk.h"\
	".\menudlg.h"\
	".\MenuExplorer.h"\
	".\MenuInfo.h"\
	".\menuitem.h"\
	".\menulist.h"\
	".\MenuSht.h"\
	".\msglog.h"\
	".\OD.h"\
	".\odinfo.h"\
	".\odmenu.h"\
	".\StdAfx.h"\
	".\syscolor.h"\
	".\tinybutt.h"\
	".\TPM.h"\
	".\uwm.h"\
	".\xform.h"\
	

"$(INTDIR)\Menudlg.obj" : $(SOURCE) $(DEP_CPP_MENUD) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\MenuList.cpp
DEP_CPP_MENUL=\
	".\menuitem.h"\
	".\menulist.h"\
	".\StdAfx.h"\
	".\xform.h"\
	

"$(INTDIR)\MenuList.obj" : $(SOURCE) $(DEP_CPP_MENUL) "$(INTDIR)"\
 "$(INTDIR)\MenuExplorer.pch"


# End Source File
# End Target
# End Project
################################################################################
