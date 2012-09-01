#-------------------------------------------------
#
# Project created by QtCreator 2012-01-25T20:22:32
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = fibviewer
TEMPLATE = app
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    connections.cpp \
    edge.cpp \
    primitive.cpp \
    segment.cpp \
    artificialconnections.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    connections.h \
    edge.h \
    primitive.h \
    segment.h \
    artificialconnections.h \
    csvconnections.h

FORMS    += mainwindow.ui
