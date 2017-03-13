#-------------------------------------------------
#
# Project created by QtCreator 2017-02-27T09:40:59
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = reqBDD
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

TRANSLATIONS += gapMea_fr_FR.ts\
            gapMea_es.ts

RESOURCES += \
    ressource.qrc
