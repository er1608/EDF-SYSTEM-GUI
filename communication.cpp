#include "mainwindow.h"
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QtCharts>

#include <QtWidgets>

void MainWindow::refreshSerialPorts() {
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

void MainWindow::readData() {
  if (!_serialPort)
    return;

  _rxBuffer.append(_serialPort->readAll());

  while (_rxBuffer.contains('\n')) {
    int idx = _rxBuffer.indexOf('\n');

    QByteArray line = _rxBuffer.left(idx);
    _rxBuffer.remove(0, idx + 1);

    QString data = QString::fromUtf8(line).trimmed();

    bool throttleOk = false;
    bool pwmOk = false;
    bool currentOk = false;
    bool voltageOk = false;
    bool RPMOk = false;

    QStringList parts = data.split(',');

    for (const QString &part : parts) {
      QString trimmedPart = part.trimmed();

      if (trimmedPart.startsWith("Voltage:")) {
        voltageValue = trimmedPart.section(':', 1).toDouble(&voltageOk);
      } else if (trimmedPart.startsWith("Current:")) {
        currentValue = trimmedPart.section(':', 1).toDouble(&currentOk);
      } else if (trimmedPart.startsWith("RPM:")) {
        RPMValue = trimmedPart.section(':', 1).toDouble(&RPMOk);
      } else if (trimmedPart.startsWith("Thrust:")) {
        // thrustValue =
        //     trimmedPart.section(':', 1).toDouble(&throttleOk) * 5 / 100;

        thrustValue = trimmedPart.section(':', 1).toDouble(&throttleOk);

        if (thrustValue > 300)
          thrustValue = 300;
        if (thrustValue < 0)
          thrustValue = 0;
      } else if (trimmedPart.startsWith("PWM:")) {
        pwmValue = trimmedPart.section(':', 1).toDouble(&pwmOk);
      }
    }

    _thrustLabel->setText(QString("Thrust: %1").arg(thrustValue, 0, 'f', 1));
    // _pwmLabel->setText(QString("PWM: %1").arg(pwmValue, 0, 'f', 1));

    if (_plotting && (voltageOk || currentOk || throttleOk)) {
      updateAnalyzeCharts(thrustValue, voltageValue, currentValue, pwmValue,
                          RPMValue);
    }
  }
}
