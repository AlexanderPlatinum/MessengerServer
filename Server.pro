QT -= gui
QT += core sql network

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    Main.cpp \
    Application.cpp \
    Database.cpp \
    Actions.cpp \
    Utilities.cpp \
    SocketThread.cpp

HEADERS += \
    SQLCommands.h \
    Application.h \
    ErrorMessages.h \
    Database.h \
    Actions.h \
    Utilities.h \
    SocketThread.h

DISTFILES += \
    db.sql
