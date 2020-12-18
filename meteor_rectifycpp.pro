TEMPLATE = app
#QT += core gui
CONFIG += console c++14
CONFIG -= app_bundle
QMAKE_LFLAGS += -static

SOURCES += \
        correctionfactor.cpp \
        main.cpp \
        rectifythread.cpp

HEADERS += \
    correctionfactor.h \
    rectifythread.h
