#-------------------------------------------------
#
# Project created by QtCreator 2012-06-28T20:22:45
#
#-------------------------------------------------

QT       += core

QT       += gui

QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

TARGET = bundler
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    connections.cpp \
    edge.cpp

HEADERS += \
    connections.h \
    edge.h




