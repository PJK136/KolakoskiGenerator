#-------------------------------------------------
#
# Project created by QtCreator 2014-01-14T19:34:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += qwt

TARGET = KolakoskiGenerator
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    binarygenerator.cpp \
    twolettersgenerator.cpp \
    multilettersgenerator.cpp

HEADERS  += mainwindow.h \
    generator.h \
    container.h \
    binarygenerator.h \
    binarycontainer.h \
    twolettersgenerator.h \
    multilettersgenerator.h

FORMS    += mainwindow.ui

INCLUDEPATH += /usr/local/qwt-6.1.0/include
LIBS += -L/usr/local/qwt-6.1.0/lib -lqwt

QMAKE_CXXFLAGS += -Wall -Wextra -std=c++11
