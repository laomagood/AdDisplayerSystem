include(qtsingleapplication/qtsingleapplication.pri)

HEADERS += \
    $$PWD/Singleton.h \
    $$PWD/Json.h \
    $$PWD/Config.h \
    $$PWD/NinePatchPainter.h \
    $$PWD/TopWindow.h \
    $$PWD/Util.h

SOURCES += \
    $$PWD/Json.cpp \
    $$PWD/Config.cpp \
    $$PWD/NinePatchPainter.cpp \
    $$PWD/TopWindow.cpp \
    $$PWD/Util.cpp

FORMS += \
    $$PWD/TopWindow.ui
