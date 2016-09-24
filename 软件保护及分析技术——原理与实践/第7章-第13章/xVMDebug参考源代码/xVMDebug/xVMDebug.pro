TEMPLATE = lib
CONFIG += DLL
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += _UNICODE

contains(QMAKE_TARGET.arch, x86):TARGET = xVMDebug32
contains(QMAKE_TARGET.arch, x86_64):TARGET = xVMDebug64

DESTDIR = Q:/ToolKit/OllyDbg2/plugin/


include(../../xHooklib/xhooklib.pri)
include(../../../nCom/nCom.pri)
include(../../xHooklib/xhook_vmexec.pri)

win32-msvc*:QMAKE_LFLAGS_WINDOWS    = /SUBSYSTEM:WINDOWS,5.01
DEFINES += _WIN32_WINNT=0x501

win32-msvc*:QMAKE_LFLAGS += "/DEF:\"$$PWD/xVMDebug.def\""



LIBS += -lkernel32 -ladvapi32 -luser32 -lcomdlg32
#win32-msvc*:QMAKE_CFLAGS_DEBUG = -MTd
#win32-msvc*:QMAKE_CXXFLAGS_DEBUG = -MTd
win32-msvc*:QMAKE_CFLAGS_RELEASE = -MT -O1 -Os -Oy -GS-
win32-msvc*:QMAKE_CXXFLAGS_RELEASE = -MT -O1 -Os -Oy -GS-

SOURCES += \
    dllmain.cpp \
    xvmdebugframe.cpp \
    xdynamic_ollydbg.cpp \
    xvmblockframe.cpp \
    xvmdebugcpu.cpp \
    xvmiatcallframe.cpp \
    ../xVMRuntime/xvmiatcall.cpp \
    VirtualDebug/VirtualDebugApiLayer.cpp \
    ollydbg2_plugin.cpp \
    ../../../nCom/PipeBase.cpp \
    VirtualDebug/VirtualDebug.cpp \
    xvmdebug.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    xvmdebugframe.h \
    xdynamic_ollydbg.h \
    xvmblockframe.h \
    xvmdebugcpu.h \
    xvmiatcallframe.h \
    ../xVMRuntime/xvmiatcall.h \
    VirtualDebug/resource.h \
    VirtualDebug/VirtualDebugApiLayer.h \
    ../../../nCom/PipeBase.h \
    VirtualDebug/VirtualDebug.h \
    resource.h \
    xvmdebug.h

OTHER_FILES += \
    xVMDebug.def \
    xVMDebug.rc

RC_FILE = xVMDebug.rc

