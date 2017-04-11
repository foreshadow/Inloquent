QT       += core sql
QT       -= gui

CONFIG += c++11

TARGET = Inloquent
#TEMPLATE = lib

DEFINES += INLOQUENT_LIBRARY

SOURCES += inloquent.cpp \
    model.cpp \
    main.cpp \
    user.cpp \
    selectquerybuilder.cpp \
    querybuilder.cpp

HEADERS += inloquent.h\
        inloquent_global.h \
    model.h \
    user.h \
    selectquerybuilder.h \
    querybuilder.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
