#-------------------------------------------------
#
# Project created by QtCreator 2016-05-14T13:10:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YM-RM
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    codeeditor.cpp \
    highlighter.cpp \
    operation.cpp \
    myexception.cpp \
    programm.cpp

HEADERS  += mainwindow.h \
    codeeditor.h \
    highlighter.h \
    operation.h \
    myexception.h \
    data_type.h \
    programm.h

FORMS    += mainwindow.ui
