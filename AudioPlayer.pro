#-------------------------------------------------
#
# Project created by QtCreator 2013-11-03T22:50:42
#
#-------------------------------------------------

QT       += core

QT       -= gui

include(./qextserialport-1.2rc/src/qextserialport.pri)

TARGET = AudioPlayer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
           serialmain.cpp \
    audioplayer.cpp

HEADERS += \
    serialmain.h \
    audioplayer.h
