TEMPLATE = lib
CONFIG += designer plugin release

DEPENDPATH += .

TARGET = circlebarplugin

HEADERS += circlebar.h circlebarplugin.h
SOURCES += circlebar.cpp circlebarplugin.cpp

DESTDIR = $$[QT_INSTALL_DATA]/plugins/designer
