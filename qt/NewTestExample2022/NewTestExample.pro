#-------------------------------------------------
#
# Project created by QtCreator 2013-04-05T19:35:15
#
#-------------------------------------------------

QT       += core gui widgets charts

TARGET = NewTestExample
TEMPLATE = app


SOURCES += main.cpp\
        guipanel.cpp \    

HEADERS  += guipanel.h \
    

FORMS    += guipanel.ui

CONFIG   +=qwt analogwidgets colorwidgets embeddeduma # Añadir para usar Qwidgets

RESOURCES              += qledmatrix_demo.qrc
