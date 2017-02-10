@set DBG_PATH=c:\debug.x86
@set SC_PATH=C:\Program Files\Microsoft Visual SourceSafe
@set DBGSDK_LIB_PATH=%DBG_PATH%\sdk\lib\%_BUILDARCH%
if "%_BUILDARCH%"=="x86" (
set DBGSDK_LIB_PATH=%DBG_PATH%\sdk\lib\i386
)

@set DBGSDK_INC_PATH=%DBG_PATH%\sdk\inc
@set path=%DBG_PATH%;%DBG_PATH%\sdk\srcsrv;%SC_PATH%;%path%;

@set AWD=%~d0\AWD
@set AWDBIN=%~d0\AWDBIN
SET BUFFER_OVERFLOW_CHECKS=0


@set _NT386TREE=%AWDBIN%\%DDK_TARGET_OS%.x86.%DDKBUILDENV%
@set _NTAMD64TREE=%AWDBIN%\%DDK_TARGET_OS%.AMD64.%DDKBUILDENV%
@set _NTIA64TREE=%AWDBIN%\%DDK_TARGET_OS%.IA64.%DDKBUILDENV%



@set _NT_SYMBOL_PATH=SRV*C:\SYMBOLS*%AWDBIN%\symstore.pri;SRV*c:\symbols*http://msdl.microsoft.com/download/symbols
@set _NT_SOURCE_PATH=SRV*


@set BINFOLDER=%AWDBIN%\%DDK_TARGET_OS%.%_BUILDARCH%.%DDKBUILDENV%
@set BINPLACE_PLACEFILE=%AWD%\placefil.loc

del %AWD%\*.exe /s
del %AWD%\*.dll /s
del %AWD%\*.log /s
del %AWD%\*.wrn /s
del %AWD%\*.err /s
for /F "usebackq" %%1 in (`dir /s /b  obj*`) do rd %%1 /q /s

REM build -Zc
PUSHD CHAPTER6
build -Zc
POPD


for /f %%1 in ('dir /s/b/a-d *.exe *.dll *.reg *.txt *.xml') do binplace -a -x -s %BINFOLDER%\sym.pub -n %BINFOLDER%\sym.pri %%1

symstore.exe add /F %BINFOLDER% /S %AwdBIN%\symstore.pub /t book /r /z pub
symstore.exe add /F %BINFOLDER% /S %AwdBIN%\symstore.pri /t book /r /z pri

@echo Add Source Server Information
set SSDIR=C:\VSS
set SSLABEL=VERSION2
set SRCSRV_SYMBOLS=%AwdBIN%\symstore.pri

ss workfold .
ss cp $/AWD
ss Label -L%SSLABEL% -c"Last Version"
call ssindex /SYSTEM=VSS /DEBUG

set SSDIR=
set SSLABEL=
set SRCSRV_SYMBOLS=

call walk %AwdBIN%\symstore.pri\*.pdb srctool -x  -d:%AWDBIN%\sources
for /f %%a in ('dir /s/b/a-d %AwdBIN%\symstore.pri\*.pdb') do pdbstr -r -s:srcsrv -p:%%a

call walk %AwdBIN%\symstore.pri\*.pdb cv2http.cmd HTTP_AWD http://www.advancedwindowsdebugging.com/sources
for /f %%a in ('dir /s/b/a-d %AwdBIN%\symstore.pri\*.pdb') do pdbstr -r -s:srcsrv -p:%%a
