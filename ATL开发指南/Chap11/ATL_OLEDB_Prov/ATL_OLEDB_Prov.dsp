# Microsoft Developer Studio Project File - Name="ATL_OLEDB_Prov" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ATL_OLEDB_Prov - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ATL_OLEDB_Prov.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ATL_OLEDB_Prov.mak" CFG="ATL_OLEDB_Prov - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ATL_OLEDB_Prov - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ATL_OLEDB_Prov - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ATL_OLEDB_Prov - Win32 Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ATL_OLEDB_Prov - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ATL_OLEDB_Prov - Win32 Unicode Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ATL_OLEDB_Prov - Win32 Unicode Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ATL_OLEDB_Prov - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ  /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ    /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\Debug\ATL_OLEDB_Prov.dll
InputPath=.\Debug\ATL_OLEDB_Prov.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "ATL_OLEDB_Prov - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ    /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ  /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\DebugU
TargetPath=.\DebugU\ATL_OLEDB_Prov.dll
InputPath=.\DebugU\ATL_OLEDB_Prov.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "ATL_OLEDB_Prov - Win32 Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinSize"
# PROP Intermediate_Dir "ReleaseMinSize"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_ATL_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinSize
TargetPath=.\ReleaseMinSize\ATL_OLEDB_Prov.dll
InputPath=.\ReleaseMinSize\ATL_OLEDB_Prov.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "ATL_OLEDB_Prov - Win32 Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinDependency
TargetPath=.\ReleaseMinDependency\ATL_OLEDB_Prov.dll
InputPath=.\ReleaseMinDependency\ATL_OLEDB_Prov.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "ATL_OLEDB_Prov - Win32 Unicode Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinSize"
# PROP BASE Intermediate_Dir "ReleaseUMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinSize"
# PROP Intermediate_Dir "ReleaseUMinSize"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /D "_ATL_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinSize
TargetPath=.\ReleaseUMinSize\ATL_OLEDB_Prov.dll
InputPath=.\ReleaseUMinSize\ATL_OLEDB_Prov.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "ATL_OLEDB_Prov - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinDependency"
# PROP Intermediate_Dir "ReleaseUMinDependency"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinDependency
TargetPath=.\ReleaseUMinDependency\ATL_OLEDB_Prov.dll
InputPath=.\ReleaseUMinDependency\ATL_OLEDB_Prov.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "ATL_OLEDB_Prov - Win32 Debug"
# Name "ATL_OLEDB_Prov - Win32 Unicode Debug"
# Name "ATL_OLEDB_Prov - Win32 Release MinSize"
# Name "ATL_OLEDB_Prov - Win32 Release MinDependency"
# Name "ATL_OLEDB_Prov - Win32 Unicode Release MinSize"
# Name "ATL_OLEDB_Prov - Win32 Unicode Release MinDependency"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ATL_OLEDB_Prov.cpp
# End Source File
# Begin Source File

SOURCE=.\ATL_OLEDB_Prov.def
# End Source File
# Begin Source File

SOURCE=.\ATL_OLEDB_Prov.idl
# ADD MTL /tlb ".\ATL_OLEDB_Prov.tlb" /h "ATL_OLEDB_Prov.h" /iid "ATL_OLEDB_Prov_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\ATL_OLEDB_Prov.rc
# End Source File
# Begin Source File

SOURCE=.\CarusoeProvRS.cpp
# End Source File
# Begin Source File

SOURCE=.\CarusoeServerFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleCSVParser.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CarusoeProvDS.h
# End Source File
# Begin Source File

SOURCE=.\CarusoeProvRS.h
# End Source File
# Begin Source File

SOURCE=.\CarusoeProvSess.h
# End Source File
# Begin Source File

SOURCE=.\CarusoeServerFile.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SimpleCSVParser.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CarusoeProv.rgs
# End Source File
# End Group
# End Target
# End Project
# Section ATL_OLEDB_Prov : {745C2E2E-6D65-6C70-6174-655C61746C5C}
# 	1:26:IDS_DBPROP_LITERALIDENTITY:224
# 	1:30:IDS_DBPROP_INIT_PROVIDERSTRING:190
# 	1:28:IDS_DBPROP_ISupportErrorInfo:169
# 	1:24:IDS_DBPROP_IRowsetUpdate:168
# 	1:28:IDS_DBPROP_MAXTABLESINSELECT:151
# 	1:20:IDS_DBPROP_TABLETERM:127
# 	1:25:IDS_DBPROP_ROWTHREADMODEL:237
# 	1:18:IDS_DBPROP_MAXROWS:227
# 	1:23:IDS_DBPROP_IColumnsInfo:197
# 	1:30:IDS_DBPROP_AUTH_CACHE_AUTHINFO:173
# 	1:33:IDS_DBPROP_BLOCKINGSTORAGEOBJECTS:166
# 	1:30:IDS_DBPROP_NOTIFYROWUNDOINSERT:160
# 	1:29:IDS_DBPROP_CONCATNULLBEHAVIOR:110
# 	1:26:IDS_DBPROP_IRowsetIdentity:207
# 	1:25:IDS_DBPROP_IColumnsRowset:198
# 	1:27:IDS_DBPROP_TRANSACTEDOBJECT:193
# 	1:23:IDS_DBPROP_INIT_TIMEOUT:188
# 	1:32:IDS_DBPROP_AUTH_ENCRYPT_PASSWORD:174
# 	1:26:IDS_DBPROP_MULTIPLERESULTS:163
# 	1:33:IDS_DBPROP_ORDERBYCOLUMNSINSELECT:157
# 	1:26:IDS_DBPROP_NOTIFYCOLUMNSET:135
# 	1:27:IDS_DBPROP_ORDEREDBOOKMARKS:238
# 	1:22:IDS_DBPROP_ROWRESTRICT:236
# 	1:22:IDS_DBPROP_INIT_PROMPT:186
# 	1:24:IDS_DBPROP_INIT_LOCATION:184
# 	1:26:IDS_DBPROP_AUTH_INTEGRATED:175
# 	1:29:IDS_DBPROP_CHANGEINSERTEDROWS:170
# 	1:31:IDS_DBPROP_NOTIFYROWFIRSTCHANGE:137
# 	1:23:IDS_DBPROP_IMMOBILEROWS:222
# 	1:22:IDS_DBPROP_CANHOLDROWS:215
# 	1:23:IDS_DBPROP_SERVERCURSOR:192
# 	1:24:IDS_DBPROP_IRowsetScroll:167
# 	1:31:IDS_DBPROP_PREPAREABORTBEHAVIOR:159
# 	1:21:IDS_DBPROP_MAXROWSIZE:149
# 	1:30:IDS_DBPROP_NOTIFYROWUNDODELETE:143
# 	1:19:IDS_DBPROP_DBMSNAME:113
# 	1:25:IDS_DBPROP_COLUMNRESTRICT:218
# 	1:29:IDS_DBPROP_CANSCROLLBACKWARDS:217
# 	1:22:IDS_DBPROP_IRowsetInfo:208
# 	1:24:IDS_DBPROP_IRowsetChange:206
# 	1:18:IDS_DBPROP_IRowset:205
# 	1:36:IDS_DBPROP_IConnectionPointContainer:199
# 	1:23:IDS_DBPROP_UPDATABILITY:194
# 	1:23:IDS_DBPROP_IConvertType:172
# 	1:21:IDS_DBPROP_OLEOBJECTS:156
# 	1:28:IDS_DBPROP_STRUCTUREDSTORAGE:123
# 	1:24:IDS_DBPROP_PROCEDURETERM:115
# 	1:18:IDS_DBPROP_DBMSVER:114
# 	1:25:IDS_DBPROP_ACTIVESESSIONS:103
# 	1:23:IDS_DBPROP_QUICKRESTART:232
# 	1:35:IDS_DBPROP_INIT_IMPERSONATION_LEVEL:183
# 	1:27:IDS_DBPROP_LITERALBOOKMARKS:223
# 	1:30:IDS_DBPROP_DELAYSTORAGEOBJECTS:221
# 	1:20:IDS_DBPROP_BOOKMARKS:211
# 	1:25:IDS_DBPROP_STRONGIDENTITY:195
# 	1:22:IDS_DBPROP_AUTH_USERID:180
# 	1:37:IDS_DBPROP_ROWSETCONVERSIONSONCOMMAND:162
# 	1:33:IDS_DBPROP_MAXROWSIZEINCLUDESBLOB:150
# 	1:25:IDS_DBPROP_IDENTIFIERCASE:146
# 	1:30:IDS_DBPROP_NOTIFYROWUNDOCHANGE:142
# 	1:26:IDS_DBPROP_NOTIFYROWINSERT:138
# 	1:28:IDS_DBPROP_MULTIPLEPARAMSETS:132
# 	1:19:IDS_DBPROP_USERNAME:128
# 	1:32:IDS_DBPROP_SUPPORTEDTXNISOLEVELS:125
# 	1:23:IDS_DBPROP_PROVIDERNAME:117
# 	1:22:IDS_DBPROP_MAXOPENROWS:225
# 	1:20:IDS_DBPROP_IAccessor:196
# 	1:31:IDS_DBPROP_RETURNPENDINGINSERTS:171
# 	1:26:IDS_DBPROP_NOTIFYROWUPDATE:161
# 	1:30:IDS_DBPROP_HETEROGENEOUSTABLES:145
# 	1:42:IDS_DBPROP_NOTIFYROWSETFETCHPOSITIONCHANGE:141
# 	1:22:IDS_DBPROP_PROVIDERVER:118
# 	1:25:IDS_DBPROP_MAXPENDINGROWS:226
# 	1:25:IDS_DBPROP_COMMANDTIMEOUT:219
# 	1:25:IDS_DBPROP_IRowsetResynch:210
# 	1:24:IDS_DBPROP_IRowsetLocate:209
# 	1:26:IDS_DBPROP_INIT_DATASOURCE:181
# 	1:25:IDS_DBPROP_DSOTHREADMODEL:131
# 	1:22:IDS_DBPROP_SCHEMAUSAGE:121
# 	1:26:IDS_DBPROP_REENTRANTEVENTS:233
# 	1:20:IDS_DBPROP_OWNINSERT:230
# 	1:20:IDS_DBPROP_INIT_LCID:189
# 	1:20:IDS_DBPROP_INIT_MODE:185
# 	1:20:IDS_DBPROP_LOCKMODES:147
# 	1:26:IDS_DBPROP_NOTIFYROWDELETE:136
# 	1:26:IDS_DBPROP_SUPPORTEDTXNDDL:129
# 	1:21:IDS_DBPROP_SQLSUPPORT:122
# 	1:27:IDS_DBPROP_PROVIDEROLEDBVER:116
# 	1:27:IDS_DBPROP_COLUMNDEFINITION:109
# 	1:23:IDS_DBPROP_CATALOGUSAGE:108
# 	1:22:IDS_DBPROP_CATALOGTERM:107
# 	1:22:IDS_DBPROP_OTHERINSERT:228
# 	1:25:IDS_DBPROP_COMMITPRESERVE:220
# 	1:23:IDS_DBPROP_BOOKMARKTYPE:213
# 	1:32:IDS_DBPROP_INIT_PROTECTION_LEVEL:187
# 	1:42:IDS_DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO:179
# 	1:24:IDS_DBPROP_NULLCOLLATION:155
# 	1:26:IDS_DBPROP_CATALOGLOCATION:106
# 	1:15:IDR_CARUSOEPROV:102
# 	1:21:IDS_DBPROP_APPENDONLY:216
# 	1:20:IDS_DBPROP_INIT_HWND:182
# 	1:28:IDS_DBPROP_ISequentialStream:164
# 	1:29:IDS_DBPROP_NOTIFICATIONPHASES:154
# 	1:18:IDS_DBPROP_GROUPBY:144
# 	1:21:IDS_DBPROP_SUBQUERIES:124
# 	1:29:IDS_DBPROP_DATASOURCEREADONLY:112
# 	1:25:IDS_DBPROP_ASYNCTXNCOMMIT:104
# 	1:32:IDS_DBPROP_REPORTMULTIPLECHANGES:235
# 	1:26:IDS_DBPROP_BOOKMARKSKIPPED:212
# 	1:24:IDS_DBPROP_AUTH_PASSWORD:177
# 	1:24:IDS_DBPROP_ABORTPRESERVE:165
# 	1:21:IDS_DBPROP_SCHEMATERM:120
# 	1:25:IDS_DBPROP_BYREFACCESSORS:105
# 	1:24:IDS_DBPROP_REMOVEDELETED:234
# 	1:27:IDS_DBPROP_MULTITABLEUPDATE:153
# 	1:24:IDS_DBPROP_ASYNCTXNABORT:130
# 	1:32:IDS_DBPROP_SUPPORTEDTXNISORETAIN:126
# 	1:35:IDS_DBPROP_SESS_AUTOCOMMITISOLEVELS:191
# 	1:32:IDS_DBPROP_PREPARECOMMITBEHAVIOR:158
# 	1:33:IDS_DBPROP_MULTIPLESTORAGEOBJECTS:152
# 	1:23:IDS_DBPROP_MAXINDEXSIZE:148
# 	1:30:IDS_DBPROP_NOTIFYROWSETRELEASE:140
# 	1:31:IDS_DBPROP_QUOTEDIDENTIFIERCASE:119
# 	1:26:IDS_DBPROP_OWNUPDATEDELETE:231
# 	1:28:IDS_DBPROP_OTHERUPDATEDELETE:229
# 	1:28:IDS_DBPROP_CANFETCHBACKWARDS:214
# 	1:26:IDS_DBPROP_IProvideMoniker:200
# 	1:33:IDS_DBPROP_AUTH_PERSIST_ENCRYPTED:178
# 	1:29:IDS_DBPROP_AUTH_MASK_PASSWORD:176
# 	1:27:IDS_DBPROP_NOTIFYROWRESYNCH:139
# 	1:27:IDS_DBPROP_PERSISTENTIDTYPE:134
# 	1:38:IDS_DBPROP_OUTPUTPARAMETERAVAILABILITY:133
# 	1:25:IDS_DBPROP_DATASOURCENAME:111
# End Section
