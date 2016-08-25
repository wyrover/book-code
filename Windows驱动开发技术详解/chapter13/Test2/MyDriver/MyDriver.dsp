# Microsoft Developer Studio Project File - Name="MyDriver" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=MyDriver - Win32 MyDriver_Check 
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MyDriver.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MyDriver.mak" CFG="MyDriver - Win32 MyDriver_Check "
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MyDriver - Win32 MyDriver_Check " (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "MyDriver___Win32_MyDriver_Check_"
# PROP BASE Intermediate_Dir "MyDriver___Win32_MyDriver_Check_"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "MyDriver_Check"
# PROP Intermediate_Dir "MyDriver_Check"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /W3 /WX /Z7 /Od /D WIN32=100 /D _X86_=1 /D WINVER=0x500 /D DBG=1 /FD /c
# ADD CPP /nologo /Gz /W3 /WX /Z7 /Od /D WIN32=100 /D _X86_=1 /D WINVER=0x500 /D DBG=1 /FR /FD /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wdm.lib /nologo /base:"0x10000" /stack:0x1000 /entry:"DriverEntry" /subsystem:console /incremental:no /debug /machine:I386 /nodefaultlib /out:"MyDriver_Check/HelloWDM.sys" /pdbtype:sept /subsystem:native /driver /SECTION:INIT,D /RELEASE /IGNORE:4078
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 wdm.lib /nologo /base:"0x10000" /stack:0x400000,0x1000 /entry:"DriverEntry" /subsystem:console /incremental:no /debug /machine:I386 /nodefaultlib /out:"MyDriver_Check/HelloWDM.sys" /pdbtype:sept /subsystem:native /driver /SECTION:INIT,D /RELEASE /IGNORE:4078
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "MyDriver - Win32 MyDriver_Check "
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\HelloWDM.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\HelloWDM.h
# End Source File
# Begin Source File

SOURCE=.\Ioctls.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\HelloWDM.inf
# End Source File
# End Target
# End Project
