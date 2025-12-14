#include "mainwindow.h"
#include <QtCharts>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <QtWidgets>

void MainWindow::saveDataToCSV()
{
    QString fileName = QDateTime::currentDateTime().toString("'system_data_'yyyy-MM-dd_hh-mm-ss'.csv'");
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        stream << "Timestamp,Thrust,PWM\n";

        for (auto it = _dataBuffer.begin(); it != _dataBuffer.end(); ++it) {
            const auto &data = *it;
            stream << data.timestamp << "," << data.throttle << "," << data.pwm << "\n";
        }

        file.close();

        QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
        _logTextEdit->append(QString("[%1] DATA SAVED: %2 (%3 records)").arg(timestamp, fileName, QString::number(_dataBuffer.size())));
        saveLogToCSV(QString("[%1] DATA SAVED: %2 (%3 records)").arg(timestamp, fileName, QString::number(_dataBuffer.size())));

        QMessageBox::information(this, "Success", QString("Data saved to %1\n%2 records written").arg(fileName, QString::number(_dataBuffer.size())));
    } else {
        QMessageBox::critical(this, "Error", "Could not save data to file");
    }
}

void MainWindow::saveLogToCSV(const QString& message)
{
    QString fileName = QDateTime::currentDateTime().toString("'command_log_'yyyy-MM-dd'.csv'");
    QFile   file(fileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");

        if (file.pos() == 0) stream << "Timestamp,Message\n";

        stream << timestamp << "," << "\"" << message << "\"" << "\n";

        file.close();
    }
}
