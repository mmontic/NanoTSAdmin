#-------------------------------------------------
#
# Project created by QtCreator 2017-03-14T23:07:07
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nanotsadmin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    meditproject.cpp \
    meditemployee.cpp \
    meditsettings.cpp \
    medittimesheet.cpp \
    meditcalendarhead.cpp \
    meditcalendarline.cpp

HEADERS  += mainwindow.h \
    meditproject.h \
    meditemployee.h \
    main.h \
    meditsettings.h \
    medittimesheet.h \
    meditcalendarhead.h \
    meditcalendarline.h

FORMS    += mainwindow.ui \
    meditproject.ui \
    meditemployee.ui \
    meditsettings.ui \
    medittimesheet.ui \
    meditcalendarhead.ui \
    meditcalendarline.ui
