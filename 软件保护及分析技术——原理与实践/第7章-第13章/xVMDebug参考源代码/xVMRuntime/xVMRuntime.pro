
QT       -= core gui
CONFIG -=qt

DEFINES += _UNICODE NO_USE_NEDMALLOC

contains(QMAKE_TARGET.arch, x86):TARGET = xVMRuntime32
contains(QMAKE_TARGET.arch, x86_64):TARGET = xVMRuntime64

DESTDIR = Q:/_bin/xApiSpy/

include(../../xHooklib/xhooklib.pri)
include(../../../nCom/nCom.pri)
include(../../xHooklib/xhook_vmexec.pri)
include(../../../nNetLib/nNetClient.pri)

#CONFIG(release, debug|release):{
    #message(Release build!) #no print
    TEMPLATE = lib
    CONFIG += dll
    CONFIG -= embed_manifest_dll

#}
#CONFIG(debug, debug|release):{
    #message(Debug build!) #no print
#    DEFINES += _DEBUG
#    TEMPLATE = app
#    CONFIG += console
#    SOURCES =  main.cpp
#}

win32-msvc*:QMAKE_LFLAGS_WINDOWS    = /SUBSYSTEM:WINDOWS,5.01
DEFINES += _WIN32_WINNT=0x501

win32-msvc*:QMAKE_LFLAGS += "/DEF:\"$$PWD/xVMRuntime.def\""

LIBS += -lkernel32 -ladvapi32 -luser32
win32-msvc*:QMAKE_CFLAGS_DEBUG = -MTd
win32-msvc*:QMAKE_CXXFLAGS_DEBUG = -MTd
win32-msvc*:QMAKE_CFLAGS_RELEASE = -MT -O1 -Os -Oy -GS-
win32-msvc*:QMAKE_CXXFLAGS_RELEASE = -MT -O1 -Os -Oy -GS-
SOURCES += \
    xvmhookruntime.cpp \
    xpedumper.cpp \
    xvmresource.cpp \
    xvmruntime.cpp \
    xvmdebug_crt.cpp \
    VirtualDebugCRT/VirtualDebugApiLayerCRT.cpp \
    ../../../nCom/PipeBase.cpp \
    VirtualDebugCRT/threadidcache.cpp \
    VirtualDebugCRT/breakpoints.cpp \
    xsandbox.cpp



OTHER_FILES +=  $$PWD/xvmhookStub32.asm \
                $$PWD/xvmhookStub64.asm \
                $$PWD/xvmRuntime.def


contains(QMAKE_TARGET.arch, x86):{
    ASM_xvmHOOK_SOURCES += $$PWD/xvmhookStub32.asm
    asm_xvmhook_obj.commands =ml -c ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
}
contains(QMAKE_TARGET.arch, x86_64):{
    ASM_xvmHOOK_SOURCES += $$PWD/xvmhookStub64.asm
    asm_xvmhook_obj.commands =ml64 -c ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
}

asm_xvmhook_obj.output  = $$OUT_PWD/${QMAKE_FILE_BASE}.obj
asm_xvmhook_obj.input = ASM_xvmHOOK_SOURCES
QMAKE_EXTRA_COMPILERS += asm_xvmhook_obj

HEADERS += \
    xvmhookruntime.h \
    xpedumper.h \
    xvmruntime.h \
    xvmresource.h \
    xvmdebug_crt.h \
    xvmdebug.h \
    VirtualDebugCRT/VirtualDebugApiLayerCRT.h \
    xsandbox.h



