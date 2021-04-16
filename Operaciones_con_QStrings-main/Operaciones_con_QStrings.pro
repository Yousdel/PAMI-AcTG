#-------------------------------------------------
#
# Project created by QtCreator 2020-07-04T15:20:17
#
#-------------------------------------------------
# es una DLL

QT       -= gui

TARGET = Operaciones_con_QStrings
TEMPLATE = lib

CONFIG += c++14

DEFINES += OPERACIONES_CON_QSTRINGS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        operaciones_con_qstrings.cpp

HEADERS += \
        operaciones_con_qstrings.h \
        operaciones_con_qstrings_global.h 

unix {
    target.path = /usr/lib
    INSTALLS += target
}
