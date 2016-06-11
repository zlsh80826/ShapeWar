#-------------------------------------------------
#
# Project created by QtCreator 2016-06-11T12:16:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Shape-War
TEMPLATE = app


SOURCES += main.cpp \
    scene.cpp \
    hero.cpp

HEADERS  += \
    scene.h \
    hero.h

CONFIG += qt

FORMS    += mainwindow.ui
