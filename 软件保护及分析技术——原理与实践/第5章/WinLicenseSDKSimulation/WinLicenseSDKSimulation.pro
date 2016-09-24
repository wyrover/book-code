TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += _UNICODE
SOURCES += main.cpp

LIBS += -LQ:/ToolKit/WL/WinlicenseSDK/Lib/COFF -lWinlicenseSDK
include(deployment.pri)
qtcAddDeployment()

