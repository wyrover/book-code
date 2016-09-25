set fasmbin=\asm\fasm\bin
set fasminc=\asm\fasm\include
@echo off
if exist patch.exe del patch.exe
%fasmbin%\fasm.exe patch.asm
pause
