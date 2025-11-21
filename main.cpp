#include "mainwindow.h"

#include <QApplication>

auto main(int argc, char *argv[]) -> int
{
    QApplication a(argc, argv);

    Q_INIT_RESOURCE(resources);

    MainWindow w;
    w.show();

    return a.exec();
}
