@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by SYNCHRONIZATIONEXPLORER.HPJ. >"hlp\SynchronizationExplorer.hm"
echo. >>"hlp\SynchronizationExplorer.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\SynchronizationExplorer.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\SynchronizationExplorer.hm"
echo. >>"hlp\SynchronizationExplorer.hm"
echo // Prompts (IDP_*) >>"hlp\SynchronizationExplorer.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\SynchronizationExplorer.hm"
echo. >>"hlp\SynchronizationExplorer.hm"
echo // Resources (IDR_*) >>"hlp\SynchronizationExplorer.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\SynchronizationExplorer.hm"
echo. >>"hlp\SynchronizationExplorer.hm"
echo // Dialogs (IDD_*) >>"hlp\SynchronizationExplorer.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\SynchronizationExplorer.hm"
echo. >>"hlp\SynchronizationExplorer.hm"
echo // Frame Controls (IDW_*) >>"hlp\SynchronizationExplorer.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\SynchronizationExplorer.hm"
REM -- Make help for Project SYNCHRONIZATIONEXPLORER


echo Building Win32 Help files
start /wait hcrtf -x "hlp\SynchronizationExplorer.hpj"
echo.
if exist Debug\nul copy "hlp\SynchronizationExplorer.hlp" Debug
if exist Debug\nul copy "hlp\SynchronizationExplorer.cnt" Debug
if exist Release\nul copy "hlp\SynchronizationExplorer.hlp" Release
if exist Release\nul copy "hlp\SynchronizationExplorer.cnt" Release
echo.


