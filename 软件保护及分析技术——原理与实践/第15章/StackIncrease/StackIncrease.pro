TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

include(deployment.pri)
qtcAddDeployment()

win32-msvc*:QMAKE_CFLAGS_RELEASE = -MT -O1 -Os -Oy -GS-
win32-msvc*:QMAKE_CXXFLAGS_RELEASE = -MT -O1 -Os -Oy -GS-
