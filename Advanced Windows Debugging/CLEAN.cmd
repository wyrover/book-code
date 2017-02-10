@set DBG_PATH=c:\debug.x86
@set SC_PATH=E:\Program Files\Microsoft Visual SourceSafe
@set DBGSDK_LIB_PATH=%DBG_PATH%\sdk\lib\%_BUILDARCH%
if "%_BUILDARCH%"=="x86" (
set DBGSDK_LIB_PATH=%DBG_PATH%\sdk\lib\i386
)

rem @set DBGSDK_LIB_PATH=%DBGSDK_LIB_PATH:x86=i386%
@set DBGSDK_INC_PATH=%DBG_PATH%\sdk\inc
@set path=%path%;%DBG_PATH%;%DBG_PATH%\sdk\srcsrv;%SC_PATH%

@set AWD=%~d0\AWD
@set AWDBIN=%~d0\AWDBIN



@set _NT386TREE=%AWDBIN%\%DDK_TARGET_OS%.x86.%DDKBUILDENV%
@set _NTAMD64TREE=%AWDBIN%\%DDK_TARGET_OS%.AMD64.%DDKBUILDENV%
@set _NTIA64TREE=%AWDBIN%\%DDK_TARGET_OS%.IA64.%DDKBUILDENV%



@set _NT_SYMBOL_PATH=SRV*%AWDBIN%\symstore.pri;SRV*c:\symbols*http://msdl.microsoft.com/download/symbols
@set _NT_SOURCE_PATH=SRV*


@set BINFOLDER=%AWDBIN%\%DDK_TARGET_OS%.%_BUILDARCH%.%DDKBUILDENV%
@set BINPLACE_PLACEFILE=%AWD%\placefil.txt

del %AWD%\*.exe /s
del %AWD%\*.dll /s
del %AWD%\*.log /s
del %AWD%\*.wrn /s
del %AWD%\*.err /s
for /F "usebackq" %%1 in (`dir /s /b  obj*`) do rd %%1 /q /s

