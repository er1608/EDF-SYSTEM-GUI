#include "autocalib.h"
#include "mainwindow.h"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QVBoxLayout>
#include <cmath>

AutoCalibDialog::AutoCalibDialog(MainWindow *mainWindow, const QString &lcId,
                                 QWidget *parent)
    : QDialog(parent), m_mainWindow(mainWindow), m_lcId(lcId), m_sampleCount(0),
      m_sumThrust(0.0), m_sumThrustInitial(0.0), m_isCollectingInitial(false) {
  setWindowTitle("Auto Calibration - " + m_lcId);
  setFixedSize(300, 420);

  auto *layout = new QVBoxLayout(this);
  layout->setAlignment(Qt::AlignCenter);
  layout->setSpacing(10);

  m_lblThrust = new QLabel("-- N", this);
  m_lblThrust->setFixedSize(140, 140);
  m_lblThrust->setAlignment(Qt::AlignCenter);
  m_lblThrust->setStyleSheet("QLabel {"
                             "  background-color: #f5f5f5;"
                             "  border: 5px solid #36b37e;"
                             "  border-radius: 70px;"
                             "  font-size: 24px;"
                             "  font-weight: bold;"
                             "  color: #238a63;"
                             "}");

  auto *thrustLayout = new QHBoxLayout();
  thrustLayout->addStretch();
  thrustLayout->addWidget(m_lblThrust);
  thrustLayout->addStretch();

  auto *settingsLayout = new QFormLayout();
  m_numSamples = new QSpinBox(this);
  m_numSamples->setRange(1, 1000);
  m_numSamples->setValue(10);

  m_sampleMass = new QDoubleSpinBox(this);
  m_sampleMass->setRange(0.1, 100000.0);
  m_sampleMass->setValue(100.0);
  m_sampleMass->setSuffix(" g");
  m_sampleMass->setDecimals(1);

  settingsLayout->addRow("Samples:", m_numSamples);
  settingsLayout->addRow("Reference Mass:", m_sampleMass);

  m_btnSendCmd = new QPushButton("Auto Calibration", this);
  m_btnStream = new QPushButton("Stream Thrust", this);
  m_btnInitialValue = new QPushButton("Initial Value", this);
  m_btnStartCalib = new QPushButton("Start Calibration", this);

  m_btnSendCmd->setMinimumHeight(35);
  m_btnStream->setMinimumHeight(35);
  m_btnStartCalib->setMinimumHeight(35);
  m_btnInitialValue->setMinimumHeight(35);

  layout->addLayout(thrustLayout);
  layout->addLayout(settingsLayout);
  layout->addSpacing(15);
  layout->addWidget(m_btnSendCmd);
  layout->addWidget(m_btnStream);
  layout->addWidget(m_btnInitialValue);
  layout->addWidget(m_btnStartCalib);

  m_streamTimer = new QTimer(this);
  m_streamTimer->setInterval(100);

  m_sampleTimer = new QTimer(this);
  m_sampleTimer->setInterval(100);

  connect(m_btnSendCmd, &QPushButton::clicked, this,
          &AutoCalibDialog::sendAutoCalibCmd);
  connect(m_btnStream, &QPushButton::clicked, this,
          &AutoCalibDialog::toggleStream);
  connect(m_streamTimer, &QTimer::timeout, this,
          &AutoCalibDialog::updateStream);

  connect(m_btnInitialValue, &QPushButton::clicked, this,
          &AutoCalibDialog::setInitialValue);
  connect(m_btnStartCalib, &QPushButton::clicked, this,
          &AutoCalibDialog::startCalibration);
  connect(m_sampleTimer, &QTimer::timeout, this,
          &AutoCalibDialog::collectSample);
}

AutoCalibDialog::~AutoCalibDialog() {}

void AutoCalibDialog::sendAutoCalibCmd() {
  QSerialPort *port = m_mainWindow->getSerialPort();
  if (port && port->isOpen()) {
    QByteArray payload;
    payload.append(static_cast<char>(COMM_CALIB_LOADCELL));

    uint8_t lc_idx = 0;
    if (m_lcId.startsWith("LC")) {
      lc_idx = static_cast<uint8_t>(m_lcId.mid(2).toUInt() - 1);
    }
    payload.append(static_cast<char>(lc_idx));

    quint16 crc = MainWindow::vescCrc16(payload);

    QByteArray packet;
    packet.append(static_cast<char>(0x02)); // start byte (short packet)
    packet.append(static_cast<char>(payload.size()));
    packet.append(payload);
    packet.append(static_cast<char>((crc >> 8) & 0xFF));
    packet.append(static_cast<char>(crc & 0xFF));
    packet.append(static_cast<char>(0x03)); // stop byte

    port->write(packet);
    QMessageBox::information(this, "Success", "Command sent.");
  } else {
    QMessageBox::warning(this, "Error", "Serial port is not open.");
  }
}

void AutoCalibDialog::toggleStream() {
  if (m_streamTimer->isActive()) {
    m_streamTimer->stop();
    m_btnStream->setText("Stream Thrust");
  } else {
    m_streamTimer->start();
    m_btnStream->setText("Stop Streaming");
  }
}

void AutoCalibDialog::updateStream() {
  m_lblThrust->setText(
      QString("%1 N").arg(m_mainWindow->getThrustValue(), 0, 'f', 2));
}

void AutoCalibDialog::startCalibration() {
  m_sampleCount = 0;
  m_sumThrust = 0.0;
  m_isCollectingInitial = false;
  m_btnStartCalib->setEnabled(false);
  m_btnStartCalib->setText("Calibrating...");
  m_sampleTimer->start();
}

void AutoCalibDialog::setInitialValue() {
  m_sampleCount = 0;
  m_sumThrustInitial = 0.0;
  m_isCollectingInitial = true;
  m_btnInitialValue->setEnabled(false);
  m_btnInitialValue->setText("Sampling...");
  m_sampleTimer->start();
}

void AutoCalibDialog::collectSample() {
  if (m_isCollectingInitial) {
    m_sumThrustInitial += m_mainWindow->getThrustValue();
  } else {
    m_sumThrust += m_mainWindow->getThrustValue();
  }

  m_sampleCount++;
  int targetSamples = m_numSamples->value();
  m_lblThrust->setText(
      QString("%1 / %2").arg(m_sampleCount).arg(targetSamples));

  if (m_sampleCount >= targetSamples) {
    m_sampleTimer->stop();

    if (m_isCollectingInitial) {
      m_btnInitialValue->setEnabled(true);
      m_btnInitialValue->setText("Initial Value");

      double initial_value = m_sumThrustInitial / targetSamples;
      QMessageBox::information(this, "Initial Value Complete",
                               QString("Initial offset recorded: %1 Raw")
                                   .arg(initial_value, 0, 'f', 2));
    } else {
      m_btnStartCalib->setEnabled(true);
      m_btnStartCalib->setText("Start Calibration");

      double avgThrust = m_sumThrust / targetSamples;
      double initial_value = m_sumThrustInitial / targetSamples;
      double mass = m_sampleMass->value();

      int calibValue = std::round((avgThrust - initial_value) / mass);
      if (calibValue == 0)
        calibValue = 1; // Prevent div by zero later

      m_mainWindow->setLCValuePerUnit(calibValue);

      QMessageBox::information(this, "Calibration Complete",
                               QString("Avg Raw: %1\nInitial Raw: %2\nMass: %3 "
                                       "g\nSet Value Per Unit to: %4")
                                   .arg(avgThrust, 0, 'f', 2)
                                   .arg(initial_value, 0, 'f', 2)
                                   .arg(mass, 0, 'f', 1)
                                   .arg(calibValue));
    }
  }
}
