QT += core gui widgets serialport charts

CONFIG += c++17

TARGET = DashboardApp
TEMPLATE = app

SOURCES += \
    autocalib.cpp \
    analyzetab.cpp \
    chartwidget.cpp \
    communication.cpp \
    hometab.cpp \
    logger.cpp \
    main.cpp \
    mainwindow.cpp \
    dashboardwidget.cpp \
    settingtab.cpp \
    theme.cpp \
    toggleswitch.cpp

HEADERS += \
    autocalib.h \
    mainwindow.h \
    dashboardwidget.h \
    toggleswitch.h

RESOURCE += resources.qrc

RESOURCES += \
    resources.qrc
