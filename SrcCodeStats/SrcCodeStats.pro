#-------------------------------------------------
#
# Project created by QtCreator 2017-03-13T11:12:01
#
#-------------------------------------------------

QT       += core gui webengine webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 #Позволяет использовать константы

TARGET = SrcCodeStatsApp
TEMPLATE = app


SOURCES += main.cpp\
        main_window.cpp \
    search_pattern.cpp \
    stat_machine.cpp \
    app_settings.cpp \
    comment_machine.cpp \
    comment_pattern.cpp \
    description_machine.cpp \
    description_pattern.cpp

HEADERS  += main_window.h \
    search_pattern.h \
    stat_machine.h \
    appid.h \
    app_settings.h \
    comment_machine.h \
    comment_pattern.h \
    constants.h \
    description_machine.h \
    description_pattern.h

FORMS    += main_window.ui

RESOURCES += \
    images.qrc
