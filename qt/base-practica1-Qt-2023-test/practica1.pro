#-------------------------------------------------
#
# Codigo de partida del proyecto practico de sistemas empotrados
#
#-------------------------------------------------

QT       += core gui serialport widgets svg

TARGET = practica1
TEMPLATE = app


SOURCES += main.cpp\
    crc.c \
    serialprotocol.c \
    mainusergui.cpp \
    tiva_remotelink.cpp

HEADERS  += \
    crc.h \
    serialprotocol.h \
    mainusergui.h \
    remotelink_messages.h \
    tiva_remotelink.h

FORMS    += \
    mainusergui.ui

CONFIG   += qwt analogwidgets colorwidgets #bibliotecas adicionales.
