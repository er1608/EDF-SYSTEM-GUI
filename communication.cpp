#include "mainwindow.h"
#include <QtCharts>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <QtWidgets>

void MainWindow::readData()
{
    if (!_serialPort)
        return;

    if (_serialPort->canReadLine())
    {
        const QByteArray payload = _serialPort->readLine();
        QString data = QString::fromUtf8(payload).trimmed();

        QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");

        bool throttleOk = false, pwmOk = false, currentOk = false, voltageOk = false, temperatureOk = false, RPMOk = false;

        QStringList parts = data.split(',');

        for (const QString &part : std::as_const(parts)) {
            QString trimmedPart = part.trimmed();

            if (trimmedPart.startsWith("Temp:")) {
                QString valueStr = trimmedPart.split(':')[1].trimmed();
                temperatureValue = valueStr.toDouble(&temperatureOk);
            }
            else if (trimmedPart.startsWith("Voltage:")) {
                QString valueStr = trimmedPart.split(':')[1].trimmed();
                voltageValue = valueStr.toDouble(&voltageOk);
            }
            else if (trimmedPart.startsWith("Current:")) {
                QString valueStr = trimmedPart.split(':')[1].trimmed();
                currentValue = valueStr.toDouble(&currentOk);
            }
            else if (trimmedPart.startsWith("RPM:")) {
                QString valueStr = trimmedPart.split(':')[1].trimmed();
                RPMValue = valueStr.toDouble(&RPMOk);
            }
            else if (trimmedPart.startsWith("Throttle:")) {
                QString valueStr = trimmedPart.split(':')[1].trimmed();
                throttleValue = (valueStr.toDouble(&throttleOk)) * 5 / 100;

                if (throttleValue > 300.0) throttleValue = 300.0;
                if (throttleValue < 0.0) throttleValue = 0.0;
            }
            else if (trimmedPart.startsWith("PWM:")) {
                QString valueStr = trimmedPart.split(':')[1].trimmed();
                pwmValue = valueStr.toDouble(&pwmOk);
            }
        }

        if (_plotting && (temperatureOk || voltageOk || currentOk || RPMOk || throttleOk || pwmOk)) {
            updateAnalyzeCharts(temperatureValue, currentValue, voltageValue, RPMValue);
            updatePlot(throttleValue, pwmValue);

        }
    }
}
