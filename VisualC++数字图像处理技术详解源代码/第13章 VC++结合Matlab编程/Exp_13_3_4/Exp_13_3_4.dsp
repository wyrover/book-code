# Microsoft Developer Studio Project File - Name="Exp_13_3_4" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Exp_13_3_4 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Exp_13_3_4.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Exp_13_3_4.mak" CFG="Exp_13_3_4 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Exp_13_3_4 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Exp_13_3_4 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Exp_13_3_4 - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "d:\MATLAB6p5\extern\include\cpp" /I "d:\MATLAB6p5\extern\include" /I "d:\MATLAB6p5\simulink\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "IBMPC" /D "MSVC" /D "MSWIND" /D "__STDC__" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  libmmfile.lib  libmatlb.lib  libmx.lib  libmat.lib  libmwservices.lib  libmex.lib  libut.lib  d:\MATLAB6p5\extern\lib\win32\libmatpm.lib  /nologo /subsystem:console /machine:I386 /libpath:"d:\MATLAB6p5\extern\lib\win32\microsoft\msvc60"

!ELSEIF  "$(CFG)" == "Exp_13_3_4 - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ""
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "d:\MATLAB6p5\extern\include\cpp" /I "d:\MATLAB6p5\extern\include" /I "d:\MATLAB6p5\simulink\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "IBMPC" /D "MSVC" /D "MSWIND" /D "__STDC__" /YX /FD /GZ       /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  libmmfile.lib  libmatlb.lib  libmx.lib  libmat.lib  libmwservices.lib  libmex.lib  libut.lib  d:\MATLAB6p5\extern\lib\win32\libmatpm.lib  /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"d:\MATLAB6p5\extern\lib\win32\microsoft\msvc60"

!ENDIF 

# Begin Target

# Name "Exp_13_3_4 - Win32 Release"
# Name "Exp_13_3_4 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "MATLAB M-files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="D:\VC2005\Exp_13_3_4\myeig.m"

!IF  "$(CFG)" == "Exp_13_3_4 - Win32 Release"

# PROP Ignore_Default_Tool 1

!ELSEIF  "$(CFG)" == "Exp_13_3_4 - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath="D:\VC2005\Exp_13_3_4\myeig.m"

"myeig.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	d:\matlab6p5\bin\win32\mcc -L Cpp libmmfile.mlib -A line:on -G -A debugline:on -O fold_scalar_mxarrays:off -O fold_non_scalar_mxarrays:off -O optimize_integer_for_loops:off -O array_indexing:off -O optimize_conditionals:off -O speculate:off libmmfile.mlib -v -I D:\VC2005\Exp_13_3_4 -I D:\VC2005\Exp_13_3_4 -t -T codegen D:\VC2005\Exp_13_3_4\myeig.m

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE="D:\VC2005\Exp_13_3_4\mymagic.m"

!IF  "$(CFG)" == "Exp_13_3_4 - Win32 Release"

# PROP Ignore_Default_Tool 1

!ELSEIF  "$(CFG)" == "Exp_13_3_4 - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath="D:\VC2005\Exp_13_3_4\mymagic.m"

"mymagic.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	d:\matlab6p5\bin\win32\mcc -L Cpp libmmfile.mlib -A line:on -G -A debugline:on -O fold_scalar_mxarrays:off -O fold_non_scalar_mxarrays:off -O optimize_integer_for_loops:off -O array_indexing:off -O optimize_conditionals:off -O speculate:off libmmfile.mlib -v -I D:\VC2005\Exp_13_3_4 -I D:\VC2005\Exp_13_3_4 -t -T codegen D:\VC2005\Exp_13_3_4\mymagic.m

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "MATLAB C/C++ files"

# PROP Default_Filter ""
# Begin Source File

SOURCE="myeig.cpp"
# End Source File
# Begin Source File

SOURCE="myeig_main.cpp"
# End Source File
# Begin Source File

SOURCE="mymagic.cpp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\readme.txt
# End Source File
# End Target
# End Project
