TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

win32-msvc*:QMAKE_LFLAGS_WINDOWS    = /SUBSYSTEM:WINDOWS,5.01

DEFINES += _UNICODE NO_USE_NEDMALLOC
DEFINES += _WIN32_WINNT=0x501

win32-msvc*:QMAKE_CFLAGS_DEBUG = -MTd
win32-msvc*:QMAKE_CXXFLAGS_DEBUG = -MTd
win32-msvc*:QMAKE_CFLAGS_RELEASE = -MT -O1 -Os -Oy -GS-
win32-msvc*:QMAKE_CXXFLAGS_RELEASE = -MT -O1 -Os -Oy -GS-


SOURCES += main.cpp

include(deployment.pri)
qtcAddDeployment()

