#-------------------------------------------------
#
# Project created by QtCreator 2015-09-28T10:37:12
#
#-------------------------------------------------

QT       += core gui webkitwidgets positioning printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "SAM Sensorauswertung"
TEMPLATE = app
CONFIG += c++11


SOURCES +=  main.cpp\
            mainwindow.cpp \
            converter.cpp \
            addfile.cpp \
            filebrowser.cpp \
            qcustomplot.cpp \
            cesiumwebview.cpp \
            sensordata.cpp \
            specialfunctions.cpp \
            alglibinternal.cpp \
            ap.cpp \
            reader.cpp \
            csvreader.cpp \
            cleaner.cpp \
            grubbsdata.cpp \
            czmlreader.cpp \
            czmlconverter.cpp

HEADERS  += mainwindow.h \
            converter.h \
            addfile.h \
            filebrowser.h \
            qcustomplot.h \
            sensordata.h \
            specialfunctions.h \
            stdafx.h \
            alglibinternal.h \
            ap.h \
            reader.h \
            csvreader.h \
            cleaner.h \
            grubbsdata.h \
            czmlreader.h \
            czmlconverter.h \
            cesiumwebview.h

FORMS    += mainwindow.ui \
            addfile.ui \
            filebrowser.ui

RESOURCES += \
    resources.qrc

ICON = SAM_Logo.icns

QMAKE_CXXFLAGS_RELEASE += -Ofast
