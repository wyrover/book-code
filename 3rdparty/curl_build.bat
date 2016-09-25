@echo off
setlocal 
set PATH=C:\nginxstack\Perl64\site\bin;C:\nginxstack\Perl64\bin;C:\nginxstack\nasm-2.12.01-win32\nasm-2.12.01;%PATH%

call "%VS80COMNTOOLS%\vsvars32.bat"

::½« zlib ºÍ openssl ¿â copy µ½ deps

cd /d "curl-curl-7_50_3/winbuild"


nmake -f Makefile.vc mode=static vc=8 MACHINE=x86 WITH_SSL=dll WITH_ZLIB=static ENABLE_IDN=no ENABLE_IPV6=no ENABLE_SSPI=no ENABLE_WINSSL=no DEBUG=no GEN_PDB=yes WITH_DEVEL=../deps
::nmake -f Makefile.vc mode=dll vc=8 MACHINE=x86 WITH_SSL=dll WITH_ZLIB=dll ENABLE_IDN=no GEN_PDB=yes WITH_DEVEL=../deps
::nmake -f Makefile.vc mode=dll vc=8 MACHINE=x86 WITH_ZLIB=dll ENABLE_IDN=no GEN_PDB=yes WITH_DEVEL=../deps
::nmake -f Makefile.vc mode=dll vc=8 MACHINE=x86 WITH_ZLIB=dll ENABLE_IDN=no ENABLE_IPV6=no ENABLE_SSPI=no ENABLE_WINSSL=no GEN_PDB=yes DEBUG=no WITH_DEVEL=../deps




pause




