@echo off
setlocal 
set PATH=C:\nginxstack\Perl64\site\bin;C:\nginxstack\Perl64\bin;C:\nginxstack\nasm-2.12.01-win32\nasm-2.12.01;%PATH%
call "C:\Program Files (x86)\Microsoft Visual Studio 8\VC\vcvarsall.bat" x86
cd /d "openssl-OpenSSL_1_0_2i"
::perl Configure enable-static-engine VC-WIN32
::call ms\do_nasm.bat


:: 编译 md 版本

::nmake -f ms\ntdll.mak -a


:: 运行测试
::nmake -f ms\ntdll.mak test

:: 运行安装
::nmake -f ms\ntdll.mak install
::pause

:: 清理
::nmake -f ms\ntdll.mak clean
::pause

:: 编译 mt 版本
:: 修改 ms\ntdll.mak 文件 /MD 为 /MT
::nmake -f ms\ntdll.mak -a


:: 运行安装
nmake -f ms\ntdll.mak install
pause


::call "C:\Program Files (x86)\Microsoft Visual Studio 8\VC\vcvarsall.bat" amd64



::::call "%VS80COMNTOOLS%\vsvars32.bat"
::call "%VS140COMNTOOLS%\vsvars32.bat" 
::
::
::cd /d "E:\openssl-1.0.2"
::
::
::
::::perl Configure VC-WIN32 no-asm --prefix=E:\openssl-1.0.2\bin
::
::::call ms\do_nasm.bat
::
::
::::release
::call ms\do_ms.bat
::pause
::nmake -f ms\ntdll.mak
::nmake -f ms\nt.mak
::pause
::nmake -f ms\ntdll.mak install
::nmake -f ms\nt.mak install
::pause
::nmake -f ms\ntdll.mak clean
::nmake -f ms\nt.mak clean
::::
::::
::::::debug
::::call ms\do_ms_debug.bat
::::nmake -f ms\ntdll_debug.mak
::::nmake -f ms\nt_debug.mak
::::pause
::::nmake -f ms\ntdll_debug.mak install
::::nmake -f ms\nt_debug.mak install
::::pause
::::nmake -f ms\ntdll_debug.mak clean
::::nmake -f ms\nt_debug.mak clean