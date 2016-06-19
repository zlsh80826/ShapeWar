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
    bullet.cpp \
    hpbar.cpp \
    logindialog.cpp \
    self.cpp \
    selfinfo.cpp \
    polygon.cpp \
    chatbar.cpp

HEADERS  += \
    scene.h \
    hero.h \
    view.h \
    bullet.h \
    hpbar.h \
    logindialog.h \
    self.h \
    polygongroup.h \
    selfinfo.h \
    polygon.h \
    chatbar.h

CONFIG += qt

FORMS    += mainwindow.ui
