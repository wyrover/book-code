TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += _UNICODE

SOURCES += main.cpp

include(deployment.pri)
qtcAddDeployment()

