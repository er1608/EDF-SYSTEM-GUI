#include "mainwindow.h"
#include <QtCharts>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <QtWidgets>

void MainWindow::refreshSerialPorts()
{
    QStringList currentPorts;

    for (const auto &port : QSerialPortInfo::availablePorts()) {
        currentPorts << port.portName();
    }

    if (currentPorts != lastPortList) {
        portComboBox->clear();
        portComboBox->addItems(currentPorts);

        qDebug() << "Ports updated:" << currentPorts;

        lastPortList = currentPorts;
    }
}

void MainWindow::readData()
{
    if (!_serialPort)
        return;

    _rxBuffer.append(_serialPort->readAll());

    while (_rxBuffer.contains('\n'))
    {
        int idx = _rxBuffer.indexOf('\n');

        QByteArray line = _rxBuffer.left(idx);
        _rxBuffer.remove(0, idx + 1);

        QString data = QString::fromUtf8(line).trimmed();

        bool throttleOk = false;
        bool pwmOk = false;
        bool currentOk = false;
        bool voltageOk = false;
        bool temperatureOk = false;
        bool RPMOk = false;

        QStringList parts = data.split(',');

        for (const QString &part : parts)
        {
            QString trimmedPart = part.trimmed();

            if (trimmedPart.startsWith("Temp:"))
            {
                temperatureValue = trimmedPart.section(':',1).toDouble(&temperatureOk);
            }
            else if (trimmedPart.startsWith("Voltage:"))
            {
                voltageValue = trimmedPart.section(':',1).toDouble(&voltageOk);
            }
            else if (trimmedPart.startsWith("Current:"))
            {
                currentValue = trimmedPart.section(':',1).toDouble(&currentOk);
            }
            else if (trimmedPart.startsWith("RPM:"))
            {
                RPMValue = trimmedPart.section(':',1).toDouble(&RPMOk);
            }
            else if (trimmedPart.startsWith("Throttle:"))
            {
                throttleValue = trimmedPart.section(':',1).toDouble(&throttleOk) * 5 / 100;

                if (throttleValue > 300) throttleValue = 300;
                if (throttleValue < 0) throttleValue = 0;
            }
            else if (trimmedPart.startsWith("PWM:"))
            {
                pwmValue = trimmedPart.section(':',1).toDouble(&pwmOk);
            }
        }

        torqueValue = throttleValue * L;

        _thrustLabel->setText(QString("Thrust: %1").arg(throttleValue,0,'f',1));
        _pwmLabel->setText(QString("PWM: %1").arg(pwmValue,0,'f',1));

        if (_plotting && (voltageOk || currentOk || throttleOk))
        {
            updateAnalyzeCharts(throttleValue,
                                torqueValue,
                                voltageValue,
                                currentValue,
                                pwmValue);
        }
    }
}
