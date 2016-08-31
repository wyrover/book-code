@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by CHARACTERSETS.HPJ. >"hlp\CharacterSets.hm"
echo. >>"hlp\CharacterSets.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\CharacterSets.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\CharacterSets.hm"
echo. >>"hlp\CharacterSets.hm"
echo // Prompts (IDP_*) >>"hlp\CharacterSets.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\CharacterSets.hm"
echo. >>"hlp\CharacterSets.hm"
echo // Resources (IDR_*) >>"hlp\CharacterSets.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\CharacterSets.hm"
echo. >>"hlp\CharacterSets.hm"
echo // Dialogs (IDD_*) >>"hlp\CharacterSets.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\CharacterSets.hm"
echo. >>"hlp\CharacterSets.hm"
echo // Frame Controls (IDW_*) >>"hlp\CharacterSets.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\CharacterSets.hm"
REM -- Make help for Project CHARACTERSETS


echo Building Win32 Help files
start /wait hcrtf -x "hlp\CharacterSets.hpj"
echo.
if exist Debug\nul copy "hlp\CharacterSets.hlp" Debug
if exist Debug\nul copy "hlp\CharacterSets.cnt" Debug
if exist Release\nul copy "hlp\CharacterSets.hlp" Release
if exist Release\nul copy "hlp\CharacterSets.cnt" Release
echo.


