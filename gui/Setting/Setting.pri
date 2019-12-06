FORMS += \

HEADERS += \
    $$PWD/Setting.h \
    $$PWD/SettingSideBarWidget.h \

SOURCES += \
    $$PWD/Setting.cpp \
    $$PWD/SettingSideBarWidget.cpp \

include(UserManagement/UserManagement.pri)
include(RoleManagement/RoleManagement.pri)
include(SystemSettings/SystemSettings.pri)



