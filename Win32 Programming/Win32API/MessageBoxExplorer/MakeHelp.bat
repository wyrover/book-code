@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by MSGBOX.HPJ. >"hlp\msgbox.hm"
echo. >>"hlp\msgbox.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\msgbox.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\msgbox.hm"
echo. >>"hlp\msgbox.hm"
echo // Prompts (IDP_*) >>"hlp\msgbox.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\msgbox.hm"
echo. >>"hlp\msgbox.hm"
echo // Resources (IDR_*) >>"hlp\msgbox.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\msgbox.hm"
echo. >>"hlp\msgbox.hm"
echo // Dialogs (IDD_*) >>"hlp\msgbox.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\msgbox.hm"
echo. >>"hlp\msgbox.hm"
echo // Frame Controls (IDW_*) >>"hlp\msgbox.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\msgbox.hm"
REM -- Make help for Project MSGBOX


echo Building Win32 Help files
start /wait hcrtf -x "hlp\msgbox.hpj"
echo.
if exist Debug\nul copy "hlp\msgbox.hlp" Debug
if exist Debug\nul copy "hlp\msgbox.cnt" Debug
if exist Release\nul copy "hlp\msgbox.hlp" Release
if exist Release\nul copy "hlp\msgbox.cnt" Release
echo.


