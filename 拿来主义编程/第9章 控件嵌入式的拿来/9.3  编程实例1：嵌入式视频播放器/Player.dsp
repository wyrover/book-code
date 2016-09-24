# Microsoft Developer Studio Project File - Name="Player" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Player - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Player.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Player.mak" CFG="Player - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Player - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Player - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Player - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Player - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Player - Win32 Release"
# Name "Player - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\Player.cpp
# End Source File
# Begin Source File

SOURCE=.\Player.rc
# End Source File
# Begin Source File

SOURCE=.\PlayerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\vlcaudio.cpp
# End Source File
# Begin Source File

SOURCE=.\vlccontrol2.cpp
# End Source File
# Begin Source File

SOURCE=.\vlcinput.cpp
# End Source File
# Begin Source File

SOURCE=.\vlclog.cpp
# End Source File
# Begin Source File

SOURCE=.\vlcmessage.cpp
# End Source File
# Begin Source File

SOURCE=.\vlcmessageiterator.cpp
# End Source File
# Begin Source File

SOURCE=.\vlcmessages.cpp
# End Source File
# Begin Source File

SOURCE=.\vlcplaylist.cpp
# End Source File
# Begin Source File

SOURCE=.\vlcplaylistitems.cpp
# End Source File
# Begin Source File

SOURCE=.\vlcvideo.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\Player.h
# End Source File
# Begin Source File

SOURCE=.\PlayerDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\vlcaudio.h
# End Source File
# Begin Source File

SOURCE=.\vlccontrol2.h
# End Source File
# Begin Source File

SOURCE=.\vlcinput.h
# End Source File
# Begin Source File

SOURCE=.\vlclog.h
# End Source File
# Begin Source File

SOURCE=.\vlcmessage.h
# End Source File
# Begin Source File

SOURCE=.\vlcmessageiterator.h
# End Source File
# Begin Source File

SOURCE=.\vlcmessages.h
# End Source File
# Begin Source File

SOURCE=.\vlcplaylist.h
# End Source File
# Begin Source File

SOURCE=.\vlcplaylistitems.h
# End Source File
# Begin Source File

SOURCE=.\vlcvideo.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\LeftPic.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Player.ico
# End Source File
# Begin Source File

SOURCE=.\res\Player.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section Player : {9ED00AFA-7BCD-4FFF-8D48-7DD4DB2C800D}
# 	2:5:Class:CVLCMessage
# 	2:10:HeaderFile:vlcmessage.h
# 	2:8:ImplFile:vlcmessage.cpp
# End Section
# Section Player : {49E0DBD1-9440-466C-9C97-95C67190C603}
# 	2:5:Class:CVLCInput
# 	2:10:HeaderFile:vlcinput.h
# 	2:8:ImplFile:vlcinput.cpp
# End Section
# Section Player : {0AAEDF0B-D333-4B27-A0C6-BBF31413A42E}
# 	2:5:Class:CVLCVideo
# 	2:10:HeaderFile:vlcvideo.h
# 	2:8:ImplFile:vlcvideo.cpp
# End Section
# Section Player : {6C5CE55D-2D6C-4AAD-8299-C62D2371F106}
# 	2:5:Class:CVLCMessages
# 	2:10:HeaderFile:vlcmessages.h
# 	2:8:ImplFile:vlcmessages.cpp
# End Section
# Section Player : {9E0BD17B-2D3C-4656-B94D-03084F3FD9D4}
# 	2:5:Class:CVLCAudio
# 	2:10:HeaderFile:vlcaudio.h
# 	2:8:ImplFile:vlcaudio.cpp
# End Section
# Section Player : {8E3BC3D9-62E9-48FB-8A6D-993F9ABC4A0A}
# 	2:5:Class:CVLCLog
# 	2:10:HeaderFile:vlclog.h
# 	2:8:ImplFile:vlclog.cpp
# End Section
# Section Player : {54613049-40BF-4035-9E70-0A9312C0188D}
# 	2:5:Class:CVLCPlaylist
# 	2:10:HeaderFile:vlcplaylist.h
# 	2:8:ImplFile:vlcplaylist.cpp
# End Section
# Section Player : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section Player : {2D719729-5333-406C-BF12-8DE787FD65E3}
# 	2:5:Class:CVLCControl2
# 	2:10:HeaderFile:vlccontrol2.h
# 	2:8:ImplFile:vlccontrol2.cpp
# End Section
# Section Player : {FD37FE32-82BC-4A25-B056-315F4DBB194D}
# 	2:5:Class:CVLCPlaylistItems
# 	2:10:HeaderFile:vlcplaylistitems.h
# 	2:8:ImplFile:vlcplaylistitems.cpp
# End Section
# Section Player : {9BE31822-FDAD-461B-AD51-BE1D1C159921}
# 	2:21:DefaultSinkHeaderFile:vlccontrol2.h
# 	2:16:DefaultSinkClass:CVLCControl2
# End Section
# Section Player : {15179CD8-CC12-4242-A58E-E412217FF343}
# 	2:5:Class:CVLCMessageIterator
# 	2:10:HeaderFile:vlcmessageiterator.h
# 	2:8:ImplFile:vlcmessageiterator.cpp
# End Section
