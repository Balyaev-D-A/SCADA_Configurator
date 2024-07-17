QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    connectionform.cpp \
    database.cpp \
    deviceeditform.cpp \
    eventeditform.cpp \
    main.cpp \
    mainwindow.cpp \
    networkeditform.cpp \
    settings.cpp

HEADERS += \
    connectionform.h \
    database.h \
    deviceeditform.h \
    eventeditform.h \
    mainwindow.h \
    networkeditform.h \
    settings.h

FORMS += \
    connectionform.ui \
    deviceeditform.ui \
    eventeditform.ui \
    mainwindow.ui \
    networkeditform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
