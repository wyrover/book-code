@echo off
rem setup paths so build utilities can be reached
rem change as needed for your system
set PATH=E:\MSVCNT\BIN;%PATH%
set INCLUDE=E:\MSVCNT\INCLUDE;E:\MSVCNT\MFC\INCLUDE;%INCLUDE%
set LIB=E:\MSVCNT\LIB;E:\MSVCNT\MFC\LIB;%LIB%
set INIT=E:\MSVCNT;%INIT%
echo To build system enter command NMAKE