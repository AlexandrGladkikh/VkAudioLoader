#-------------------------------------------------
#
# Project created by QtCreator 2015-11-22T10:05:51
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VkMusicLoader
TEMPLATE = app


SOURCES += main.cpp \
    gui/mainwindow.cpp \
    core/core.cpp \
    network/vkapi.cpp \
    network/vkauth.cpp \
    common/common.cpp \
    network/vkaudio.cpp

HEADERS  += \
    gui/mainwindow.h \
    common/common.h \
    core/core.h \
    network/vkapi.h \
    network/vkauth.h \
    network/vkaudio.h
