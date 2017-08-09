QT       += core sql
QT       -= gui

CONFIG += c++11

TARGET = Inloquent
TEMPLATE = app

DEFINES += INLOQUENT_LIBRARY

SOURCES += inloquent.cpp \
    model.cpp \
    main.cpp \
    builder.cpp \
    collection.cpp \
    db.cpp \
    blueprint.cpp

HEADERS += inloquent.h\
    inloquent_global.h \
    model.h \
    builder.h \
    collection.h \
    db.h \
    inloquentmodel.h \
    blueprint.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
