#-------------------------------------------------
#
# Project created by QtCreator 2019-04-30T15:59:20
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lightning
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        adminmainwindow.cpp \
    dlglogin.cpp \
    cdtormainwindow.cpp \
    dlgsettings.cpp \
    dlgregister.cpp \
    dlgfindpswd.cpp \
    qpswdvalidator.cpp \
    dbconnector.cpp \
    dlgaddmovie.cpp

HEADERS += \
        adminmainwindow.h \
    dlglogin.h \
    cdtormainwindow.h \
    dlgsettings.h \
    stringsource.h \
    dlgregister.h \
    dlgfindpswd.h \
    qpswdvalidator.h \
    dbconnector.h \
    tablestate.h \
    dlgaddmovie.h

FORMS += \
        adminmainwindow.ui \
    dlglogin.ui \
    cdtormainwindow.ui \
    dlgsettings.ui \
    dlgregister.ui \
    dlgfindpswd.ui \
    dlgaddmovie.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    bkgresource.qrc \ 
    icon.qrc
