QT += core
QT -= gui

CONFIG += c++11

TARGET = GraphicalEngine
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    model.cpp \
    shader.cpp \
    viewpoint.cpp \
    interface.cpp

unix|win32: LIBS += -lGL

unix|win32: LIBS += -lglfw

HEADERS += \
    shader.h \
    model.h \
    viewpoint.h \
    interface.h \
    customgltemplates.h


unix|win32: LIBS += -lfreeimage

unix|win32: LIBS += -lGLEW

unix|win32: LIBS += -lassimp
