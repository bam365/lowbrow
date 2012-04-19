#-------------------------------------------------
#
# Project created by QtCreator 2012-02-11T13:31:45
#
#-------------------------------------------------

QT       += core gui webkit

TARGET = lowbrow
TEMPLATE = app


SOURCES += main.cpp\
        lbmain.cpp \
    lbwebview.cpp \
    bindings.cpp \
    lbcommandbar.cpp

HEADERS  += lbmain.h \
    lbwebview.h \
    bindings.h \
    lbcommandbar.h

FORMS    += lbmain.ui

QMAKE_CXXFLAGS += -Wno-parentheses -Wno-write-strings
