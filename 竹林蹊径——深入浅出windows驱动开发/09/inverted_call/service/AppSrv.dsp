# Microsoft Developer Studio Project File - Name="AppSrv" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=AppSrv - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AppSrv.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AppSrv.mak" CFG="AppSrv - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AppSrv - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "AppSrv - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "AppSrv - Win32 Release"

# PROP BASE Use_MFC
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f AppSrv.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AppSrv.exe"
# PROP BASE Bsc_Name "AppSrv.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "e:\ddkbuild\ddkbuild -WNETXP fre ."
# PROP Rebuild_Opt "-cZ"
# PROP Target_File "AppSrv.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "AppSrv - Win32 Debug"

# PROP BASE Use_MFC
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f AppSrv.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "AppSrv.exe"
# PROP BASE Bsc_Name "AppSrv.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "e:\ddkbuild\ddkbuild -WNETXP chk ."
# PROP Rebuild_Opt "-cZ"
# PROP Target_File "AppSrv.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "AppSrv - Win32 Release"
# Name "AppSrv - Win32 Debug"

!IF  "$(CFG)" == "AppSrv - Win32 Release"

!ELSEIF  "$(CFG)" == "AppSrv - Win32 Debug"

!ENDIF 

# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AppSrv.cpp
# End Source File
# Begin Source File

SOURCE=.\NTService.cpp
# End Source File
# Begin Source File

SOURCE=.\reflectorservice.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AppSrv.h
# End Source File
# Begin Source File

SOURCE=.\NTService.h
# End Source File
# Begin Source File

SOURCE=.\reflectorservice.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Build Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MAKEFILE
# End Source File
# Begin Source File

SOURCE=.\sources
# End Source File
# End Group
# Begin Group "Misc Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AppSrv.rc
# End Source File
# Begin Source File

SOURCE=.\NTServMsg.mc
# End Source File
# Begin Source File

SOURCE=.\ntservmsg.rc
# End Source File
# End Group
# End Target
# End Project
