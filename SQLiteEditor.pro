#-------------------------------------------------
#
# Project created by QtCreator 2016-03-09T18:40:00
#
#-------------------------------------------------

QT       += core gui sql
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SQLiteEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tabledialog.cpp \
    columninfo.cpp \
    sqlitetableinforeader.cpp \
    tableinfo.cpp \
    tableinforeaderfactory.cpp \
    tabledialogdelegate.cpp

HEADERS  += mainwindow.h \
    tabledialog.h \
    columninfo.h \
    sqlitetableinforeader.h \
    tableinfo.h \
    tableinforeader.h \
    tableinforeaderfactory.h \
    tabledialogdelegate.h

FORMS    += mainwindow.ui \
    tabledialog.ui

RESOURCES += \
    resources.qrc
