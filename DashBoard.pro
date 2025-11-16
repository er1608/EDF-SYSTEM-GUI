QT += core gui widgets serialport charts

CONFIG += c++17

TARGET = DashboardApp
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    dashboardwidget.cpp

HEADERS += \
    mainwindow.h \
    dashboardwidget.h
