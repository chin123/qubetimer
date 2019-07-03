######################################################################
# Automatically generated by qmake (3.1) Thu Apr 18 02:15:41 2019
######################################################################

TEMPLATE = app
TARGET = rt
INCLUDEPATH += .
INCLUDEPATH += ./include/

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += cubeui.h \
           include/color.h \
           include/coordcube.h \
           include/corner.h \
           include/cubiecube.h \
           include/edge.h \
           include/facecube.h \
           include/facelet.h \
           include/getch.h \
           include/search.h
SOURCES += cubeui.cpp \
           main.cpp \
           kociemba/coordcube.c \
           kociemba/cubiecube.c \
           kociemba/facecube.c \
           kociemba/search.c

QT += widgets
QT += charts

test_conf {
    TARGET = rttest
    QT += testlib
    CONFIG += testcase
    SOURCES = tests.cpp \
              cubeui.cpp \
              kociemba/coordcube.c \
              kociemba/cubiecube.c \
              kociemba/facecube.c \
              kociemba/search.c

}