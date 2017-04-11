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
    binarygenerator.h \
    binarycontainer.h \
    twolettersgenerator.h \
    multilettersgenerator.h

FORMS    += mainwindow.ui
