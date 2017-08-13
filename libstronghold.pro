#-------------------------------------------------
#
# Project created by QtCreator 2017-04-27T18:53:58
#
#-------------------------------------------------

QT       += sql
QT       += network
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stronghold
TEMPLATE = lib

DEFINES += STRONGHOLD_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    user.cpp \
    pem.cpp \
    sum.cpp \
    dialogjoin.cpp \
    wincreate.cpp \
    wingenerate.cpp \
    xsabout.cpp \
    plugin.cpp \
    uitablelist.cpp \
    uiactions.cpp \
    mainwindow.cpp

HEADERS +=\
    stronghold.h \
    stronghold_global.h \
    user.h \
    pem.h \
    sum.h \
    dialogjoin.h \
    wincreate.h \
    wingenerate.h \
    xsabout.h \
    plugin.h \
    uitablelist.h \
    uiactions.h \
    mainwindow.h

FORMS    += \
    dialogjoin.ui \
    wincreate.ui \
    wingenerate.ui \
    xsabout.ui

RESOURCES += ../xsResource/res.qrc

linux {
    INCLUDEPATH += /usr/include/xslib
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../bin/release/ -lxs
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../bin/debug/ -lxs
else:unix: LIBS += -L$$PWD/../../bin/ -lxs

INCLUDEPATH += $$PWD/../xslib
DEPENDPATH += $$PWD/../xslib

