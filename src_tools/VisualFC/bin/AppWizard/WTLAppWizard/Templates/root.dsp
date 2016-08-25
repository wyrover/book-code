# Microsoft Developer Studio Project File - Name="[!output PROJECT_NAME]" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=[!output PROJECT_NAME] - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "[!output PROJECT_NAME].mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "[!output PROJECT_NAME].mak" CFG="[!output PROJECT_NAME] - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "[!output PROJECT_NAME] - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "[!output PROJECT_NAME] - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "[!output PROJECT_NAME] - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
[!if WTL_USE_UNICODE]
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "STRICT" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
[!else]
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
[!endif]
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

[!if WTL_COM_SERVER]
# Begin Custom Build - Performing registration
OutDir=.\Release
TargetPath=.\Release\[!output PROJECT_NAME].exe
InputPath=.\Release\[!output PROJECT_NAME].exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build
[!endif]

!ELSEIF  "$(CFG)" == "[!output PROJECT_NAME] - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
[!if WTL_USE_UNICODE]
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /Yu"stdafx.h" /FD /GZ   /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "STRICT" /Yu"stdafx.h" /FD /GZ     /c
[!else]
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ   /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /Yu"stdafx.h" /FD /GZ     /c
[!endif]
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
[!if WTL_COM_SERVER]
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\Debug\[!output PROJECT_NAME].exe
InputPath=.\Debug\[!output PROJECT_NAME].exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build
[!endif]

!ENDIF 

# Begin Target

# Name "[!output PROJECT_NAME] - Win32 Release"
# Name "[!output PROJECT_NAME] - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
[!if WTL_USE_CPP_FILES]
[!if !WTL_APPTYPE_DLG_MODAL]
# Begin Source File

SOURCE=.\aboutdlg.cpp
# End Source File
[!endif]
[!if WTL_APPTYPE_MDI]
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
[!endif]
[!if !WTL_APPTYPE_DLG]
[!if !WTL_APPTYPE_DLG_MODAL]
# Begin Source File

SOURCE=.\mainfrm.cpp
# End Source File
[!if WTL_USE_VIEW]
# Begin Source File

SOURCE=.\[!output PROJECT_NAME]view.cpp
# End Source File
[!endif]
[!endif]
[!endif]


[!if WTL_APPTYPE_DLG || WTL_APPTYPE_DLG_MODAL]
# Begin Source File

SOURCE=.\maindlg.cpp
# End Source File
[!endif]
[!endif] //WTL_USE_CPP_FILES
# Begin Source File

SOURCE=.\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\[!output PROJECT_NAME].cpp
# End Source File
[!if WTL_COM_SERVER]
# Begin Source File

SOURCE=.\[!output PROJECT_NAME].idl
# ADD MTL /tlb ".\[!output PROJECT_NAME].tlb" /h "[!output PROJECT_NAME].h" /iid "[!output PROJECT_NAME]_i.c" /Oicf
# End Source File
[!endif]
# Begin Source File

SOURCE=.\[!output PROJECT_NAME].rc
# End Source File

# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
[!if !WTL_APPTYPE_DLG_MODAL]
# Begin Source File

SOURCE=.\aboutdlg.h
# End Source File
[!endif]

[!if WTL_APPTYPE_MDI]
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
[!endif]
[!if !WTL_APPTYPE_DLG]
[!if !WTL_APPTYPE_DLG_MODAL]
# Begin Source File

SOURCE=.\mainfrm.h
# End Source File
[!if WTL_USE_VIEW]
# Begin Source File

SOURCE=.\[!output PROJECT_NAME]view.h
# End Source File
[!endif]
[!endif]
[!endif]
[!if WTL_APPTYPE_DLG || WTL_APPTYPE_DLG_MODAL]
# Begin Source File

SOURCE=.\maindlg.h
# End Source File
[!endif]
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File

# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
[!if !WTL_APPTYPE_DLG]
[!if !WTL_APPTYPE_DLG_MODAL]
# Begin Source File

SOURCE=.\res\toolbar.bmp
# End Source File
[!endif]
[!endif]
# Begin Source File

SOURCE=.\res\[!output PROJECT_NAME].exe.manifest
# End Source File
# Begin Source File

SOURCE=.\res\[!output PROJECT_NAME].ico
# End Source File

[!if WTL_COM_SERVER]
# Begin Source File

SOURCE=.\[!output PROJECT_NAME].rgs
# End Source File
# Begin Source File

SOURCE=.\res\[!output PROJECT_NAME]doc.ico
# End Source File
[!endif]

# End Group
# End Target
# End Project
