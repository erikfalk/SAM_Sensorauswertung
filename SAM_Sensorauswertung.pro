#-------------------------------------------------
#
# Project created by QtCreator 2015-09-28T10:37:12
#
#-------------------------------------------------

QT       += core gui webkitwidgets webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "SAM Sensorauswertung"
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    converter.cpp \
    addfile.cpp \
    filebrowser.cpp


HEADERS  += mainwindow.h \
            converter.h \
            addfile.h \
            filebrowser.h


FORMS    += mainwindow.ui \
            addfile.ui \
            filebrowser.ui
