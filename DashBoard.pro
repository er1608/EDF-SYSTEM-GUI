QT += core gui widgets serialport charts

CONFIG += c++17

TARGET = DashboardApp
TEMPLATE = app

SOURCES += \
    analyzetab.cpp \
    chartwidget.cpp \
    communication.cpp \
    hometab.cpp \
    logger.cpp \
    main.cpp \
    mainwindow.cpp \
    dashboardwidget.cpp \
    theme.cpp \
    toggleswitch.cpp

HEADERS += \
    mainwindow.h \
    dashboardwidget.h \
    toggleswitch.h

RESOURCE += resources.qrc

RESOURCES += \
    resources.qrc
