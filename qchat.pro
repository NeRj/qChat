#-------------------------------------------------
#
# Project created by QtCreator 2011-05-12T22:08:11
#
#-------------------------------------------------

QT       += network\
        phonon
TARGET = qchat
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    server.cpp \
    connect.cpp \
    client.cpp \
    peer.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    server.h \
    connect.h \
    client.h \
    peer.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    aboutdialog.ui

RESOURCES += \
    src.qrc

RC_FILE = myico.rc

TRANSLATIONS += qchat_ru.ts
