#-------------------------------------------------
#
# Project created by QtCreator 2016-06-11T12:16:44
#
#-------------------------------------------------

QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Shape-War
TEMPLATE = app


SOURCES += main.cpp \
    scene.cpp \
    hero.cpp \
    view.cpp \
    triangle.cpp \
    rectangle.cpp \
    pentagon.cpp \
    bullet.cpp \
    logindialog.cpp

HEADERS  += \
    scene.h \
    hero.h \
    view.h \
    triangle.h \
    rectangle.h \
    pentagon.h \
    bullet.h \
    logindialog.h

CONFIG += qt

FORMS    += mainwindow.ui
