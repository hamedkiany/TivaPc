#-------------------------------------------------
#
# Project created by QtCreator 2022
#
# Compatible con Qt 6.2 hacia delante
#-------------------------------------------------

QT       += core gui widgets serialport
CONFIG   += qwt  analogwidgets

TARGET = FlightSim
TEMPLATE = app


SOURCES += main.cpp\
        guipanel.cpp

HEADERS  += guipanel.h

FORMS    += guipanel.ui

#CONFIG   +=console# Añadir para usar la consola de depuracion --> Window-Views-Debugger Log

QMAKE_LFLAGS_RELEASE ="/OPT:REF" # Evita el molesto problema de Debug/xxx.exe

RESOURCES += \
    images.qrc
