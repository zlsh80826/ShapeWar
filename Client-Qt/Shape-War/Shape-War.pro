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
    hpbar.cpp \
    logindialog.cpp \
    self.cpp \
    trianglegroup.cpp \
    rectanglegroup.cpp

HEADERS  += \
    scene.h \
    hero.h \
    view.h \
    triangle.h \
    rectangle.h \
    pentagon.h \
    bullet.h \
    hpbar.h \
    logindialog.h \
    self.h \
    trianglegroup.h \
    rectanglegroup.h \
    polygongroup.h

CONFIG += qt

FORMS    += mainwindow.ui
