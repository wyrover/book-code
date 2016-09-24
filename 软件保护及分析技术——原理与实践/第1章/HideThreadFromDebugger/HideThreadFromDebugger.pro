TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

LIBS += -ladvapi32
include(deployment.pri)
qtcAddDeployment()

