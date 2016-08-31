@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by GDIEXPLORER.HPJ. >"hlp\GDIExplorer.hm"
echo. >>"hlp\GDIExplorer.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\GDIExplorer.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\GDIExplorer.hm"
echo. >>"hlp\GDIExplorer.hm"
echo // Prompts (IDP_*) >>"hlp\GDIExplorer.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\GDIExplorer.hm"
echo. >>"hlp\GDIExplorer.hm"
echo // Resources (IDR_*) >>"hlp\GDIExplorer.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\GDIExplorer.hm"
echo. >>"hlp\GDIExplorer.hm"
echo // Dialogs (IDD_*) >>"hlp\GDIExplorer.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\GDIExplorer.hm"
echo. >>"hlp\GDIExplorer.hm"
echo // Frame Controls (IDW_*) >>"hlp\GDIExplorer.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\GDIExplorer.hm"
REM -- Make help for Project GDI EXPLORER


echo Building Win32 Help files
start /wait hcrtf -x "hlp\GDIExplorer.hpj"
echo.
if exist Debug\nul copy "hlp\GDIExplorer.hlp" Debug
if exist Debug\nul copy "hlp\GDIExplorer.cnt" Debug
if exist Release\nul copy "hlp\GDIExplorer.hlp" Release
if exist Release\nul copy "hlp\GDIExplorer.cnt" Release
echo.


