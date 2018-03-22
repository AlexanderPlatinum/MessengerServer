QT -= gui
QT += core sql network

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    Main.cpp \
    Application.cpp

HEADERS += \
    SQLCommands.h \
    Application.h \
    ErrorMessages.h
