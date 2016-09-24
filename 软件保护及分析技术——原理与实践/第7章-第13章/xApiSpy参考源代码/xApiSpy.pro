#-------------------------------------------------
#
# Project created by QtCreator 2015-03-12T08:04:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xApiSpy
TEMPLATE = app

DEFINES += _UNICODE
DEFINES += _WIN32_WINNT=0x501

DESTDIR = Q:/_bin/xApiSpy/

include(../../xHooklib/xHooklib.pri)
include(../../../nCom/nCom.pri)
include(../../../nNetLib/nNetLib.pri)
include(../../../3rdparty/nedmalloc/nedmalloc.pri)
include(../../../ncvm/ncasm/ncasm.pri)
win32-msvc*:QMAKE_LFLAGS_WINDOWS    = /SUBSYSTEM:WINDOWS,5.01

LIBS += -lkernel32 -ladvapi32 -luser32



SOURCES += main.cpp\
        xapispy.cpp \
    xprocessstartup.cpp \
    moduletreeitem.cpp \
    moduletreemodel.cpp \
    apitablemodel.cpp \
    resourcetreemodel.cpp \
    hexview/NHexView.cpp \
    xapihexbuffer.cpp \
    patchtablemodel.cpp \
    xapitreeview.cpp \
    importtreemodel.cpp \
    bookcover.cpp \
    aboutdialog.cpp \
    ../xVMDebug/NativeSysCallID.cpp

HEADERS  += xapispy.h \
    xprocessstartup.h \
    moduletreeitem.h \
    moduletreemodel.h \
    apitablemodel.h \
    resourcetreemodel.h \
    hexview/NHexView.h \
    hexview/NByteStream.h \
    hexview/NCommentServerInterface.h \
    xapihexbuffer.h \
    patchtablemodel.h \
    xapitreeview.h \
    importtreemodel.h \
    bookcover.h \
    aboutdialog.h

FORMS    += xapispy.ui \
    bookcover.ui \
    aboutdialog.ui

RESOURCES += \
    xApiSpy.qrc

RC_FILE = xApiSpy.rc
