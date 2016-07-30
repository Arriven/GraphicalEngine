QT += core
QT -= gui

CONFIG += c++11

TARGET = GraphicalEngine
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

unix|win32: LIBS += -lGL

unix|win32: LIBS += -lglfw

unix|win32: LIBS += -lSOIL

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../External_Libs/GLEW/release/ -lGLEW
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../External_Libs/GLEW/debug/ -lGLEW
else:unix: LIBS += -L$$PWD/../External_Libs/GLEW/ -lGLEW

INCLUDEPATH += $$PWD/../External_Libs/GLEW
DEPENDPATH += $$PWD/../External_Libs/GLEW

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../External_Libs/GLEW/release/libGLEW.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../External_Libs/GLEW/debug/libGLEW.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../External_Libs/GLEW/release/GLEW.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../External_Libs/GLEW/debug/GLEW.lib
else:unix: PRE_TARGETDEPS += $$PWD/../External_Libs/GLEW/libGLEW.a

HEADERS += \
    shader.h \
    model.h \
    viewpoint.h


unix|win32: LIBS += -lfreeimage
