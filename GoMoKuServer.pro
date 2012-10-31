#-------------------------------------------------
#
# Project created by QtCreator 2012-10-02T15:48:59
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = GoMoKuServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    net.cpp \
    field.cpp \
    Board.cpp \
    controller.cpp \
    client.cpp

HEADERS += \
    net.h \
    field.h \
    Board.h \
    controller.h \
    client.h
