#-------------------------------------------------
#
# Project created by QtCreator 2019-01-09T10:02:42
#
#-------------------------------------------------

QT       += core gui network sql multimedia multimediawidgets network concurrent av avwidgets
RC_ICONS = Logo_Client.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AdDisplayerSystem_Client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_MESSAGELOGCONTEXT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Output directory
CONFIG(debug, debug|release) {
    output = debug
}
CONFIG(release, debug|release) {
    output = release
}

DESTDIR     = bin
OBJECTS_DIR = $$output
MOC_DIR     = $$output
RCC_DIR     = $$output
UI_DIR      = $$output

SOURCES += \
        main_Client.cpp

include(gui/gui_Client.pri)
include(util/util.pri)
include(function/function.pri)
#include(bean/bean.pri)
#include(dao/dao.pri)
#include(db/db.pri)

RESOURCES +=

DISTFILES +=

HEADERS += \
    global.h

INCLUDEPATH += $$PWD/3rdparty/QuaZIP/include
LIBS += -L$$PWD/"3rdparty/QuaZIP/lib" -lquazipd -L$$PWD/"3rdparty/QuaZIP/lib" -lzlib1
