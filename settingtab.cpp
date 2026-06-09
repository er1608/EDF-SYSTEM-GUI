#include "mainwindow.h"
#include <QDateTime>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QtWidgets>

namespace {
// CRC Table
const unsigned short crc16_tab[] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7, 0x8108,
    0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef, 0x1231, 0x0210,
    0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6, 0x9339, 0x8318, 0xb37b,
    0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de, 0x2462, 0x3443, 0x0420, 0x1401,
    0x64e6, 0x74c7, 0x44a4, 0x5485, 0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee,
    0xf5cf, 0xc5ac, 0xd58d, 0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6,
    0x5695, 0x46b4, 0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d,
    0xc7bc, 0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b, 0x5af5,
    0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12, 0xdbfd, 0xcbdc,
    0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a, 0x6ca6, 0x7c87, 0x4ce4,
    0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41, 0xedae, 0xfd8f, 0xcdec, 0xddcd,
    0xad2a, 0xbd0b, 0x8d68, 0x9d49, 0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13,
    0x2e32, 0x1e51, 0x0e70, 0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a,
    0x9f59, 0x8f78, 0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e,
    0xe16f, 0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e, 0x02b1,
    0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256, 0xb5ea, 0xa5cb,
    0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d, 0x34e2, 0x24c3, 0x14a0,
    0x0481, 0x7466, 0x6447, 0x5424, 0x4405, 0xa7db, 0xb7fa, 0x8799, 0x97b8,
    0xe75f, 0xf77e, 0xc71d, 0xd73c, 0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657,
    0x7676, 0x4615, 0x5634, 0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9,
    0xb98a, 0xa9ab, 0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882,
    0x28a3, 0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92, 0xfd2e,
    0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9, 0x7c26, 0x6c07,
    0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1, 0xef1f, 0xff3e, 0xcf5d,
    0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8, 0x6e17, 0x7e36, 0x4e55, 0x5e74,
    0x2e93, 0x3eb2, 0x0ed1, 0x1ef0};
} // namespace

QVector<LCConfig> _lcConfigs;

void MainWindow::setupSettingTab() {
  auto *settingLayout = new QVBoxLayout(_settingTab);
  settingLayout->setContentsMargins(10, 10, 10, 10);
  settingLayout->setSpacing(10);

  auto *settingTabs = new QTabWidget(_settingTab);
  settingTabs->setTabPosition(QTabWidget::South);
  settingTabs->setFont(QFont("Arial", 9, QFont::Bold));

  settingTabs->setStyleSheet(R"(
        QTabBar::tab {
        min-width: 79px;
        height: 18px;
        }
    )");

  settingTabs->setStyleSheet(R"(
        QTabBar::tab {
            min-width: 79px;
            height: 18px;
        }

        QTabWidget::pane {
            border: none;
            background-image: url(:/images/CTUAV.png);
            background-repeat: no-repeat;
            background-position: center;
        }
    )");

  auto *loadcellTab = new QWidget();
  setupLCTab(loadcellTab);
  settingTabs->addTab(loadcellTab, "SENSOR");

  auto *pwmTab = new QWidget();
  setupPWMTab(pwmTab);
  settingTabs->addTab(pwmTab, "PWM");

  auto *mavlinkTab = new QWidget();
  setupMavlinkTab(mavlinkTab);
  settingTabs->addTab(mavlinkTab, "MAVL");

  auto *fftTab = new QWidget();
  setupFFTTab(fftTab);
  settingTabs->addTab(fftTab, "FFT");

  auto *signalTab = new QWidget();
  setupSignalGeneratorTab(signalTab);
  settingTabs->addTab(signalTab, "MODE");

  auto *vescTab = new QWidget();
  setupVESCTab(vescTab);
  settingTabs->addTab(vescTab, "VESC");

  //   auto *configBtn = new QPushButton("Config");
  //   configBtn->setCheckable(true);

  //   configBtn->setText("Load");

  //   settingTabs->setCornerWidget(configBtn, Qt::TopRightCorner);
  //   configBtn->setFixedHeight(30);

  //   connect(configBtn, &QPushButton::clicked, this, [=]() {
  //     if (_serialPort && _serialPort->isOpen()) {
  //       QString cmd = "frconf:1\n";
  //       _serialPort->write(cmd.toUtf8());
  //       _logTextEdit->append(cmd);
  //     }
  //   });

  settingLayout->addWidget(settingTabs);
}

void MainWindow::setupSignalGeneratorTab(QWidget *tab) {
  auto *layout = new QVBoxLayout(tab);

  auto *group = new QGroupBox("Signal Generator Mode");
  group->setFixedWidth(400);
  auto *form = new QFormLayout(group);

  auto *amplitude = new QDoubleSpinBox();
  amplitude->setRange(0, 100);
  amplitude->setValue(50);
  amplitude->setDecimals(1);

  auto *frequency = new QDoubleSpinBox();
  frequency->setRange(0.1, 10);
  frequency->setValue(1.0);
  frequency->setDecimals(2);

  auto *duration = new QDoubleSpinBox();
  duration->setRange(1, 60);
  duration->setValue(10);

  auto *btnLayout = new QHBoxLayout();

  auto *applyBtn = new QPushButton("Apply");

  btnLayout->addWidget(applyBtn);

  form->addRow("Amplitude", amplitude);
  form->addRow("Frequency (Hz)", frequency);
  form->addRow("Duration (s)", duration);
  form->addRow(btnLayout);

  layout->addWidget(group, 0, Qt::AlignTop | Qt::AlignHCenter);

  connect(applyBtn, &QPushButton::clicked, this, [=]() {
    if (_serialPort && _serialPort->isOpen()) {
      QString cmd = QString("mode:%1:%2:%3\n")
                        .arg(amplitude->value(), 0, 'f', 1)
                        .arg(frequency->value(), 0, 'f', 2)
                        .arg(duration->value(), 0, 'f', 1);

      _serialPort->write(cmd.toUtf8());

      QString ts = QDateTime::currentDateTime().toString("hh:mm:ss");
      _logTextEdit->append(QString("[%1] MODE SET: %2").arg(ts, cmd.trimmed()));
      saveLogToCSV(QString("[%1] MODE SET: %2").arg(ts, cmd.trimmed()));
    }
  });
}

void MainWindow::setupPWMTab(QWidget *tab) {
  auto *layout = new QVBoxLayout(tab);

  auto *group = new QGroupBox("PWM Output Configuration");
  group->setFixedWidth(400);
  auto *form = new QFormLayout(group);

  auto *freq = new QComboBox();
  freq->addItem("50 Hz", 50);
  freq->addItem("400 Hz", 400);

  auto *minPWM = new QSpinBox();
  minPWM->setRange(800, 1100);
  minPWM->setValue(1050);

  auto *maxPWM = new QSpinBox();
  maxPWM->setRange(1600, 2200);
  maxPWM->setValue(1950);

  auto *applyBtn = new QPushButton("Apply", group);

  form->addRow("Frequency (Hz)", freq);
  form->addRow("Min PWM (us)", minPWM);
  form->addRow("Max PWM (us)", maxPWM);
  form->addRow(applyBtn);

  connect(applyBtn, &QPushButton::clicked, this, [=]() {
    if (_serialPort && _serialPort->isOpen()) {
      int frequency = freq->currentData().toInt();

      QString cmd = QString("pwmconf:%1:%2:%3\n")
                        .arg(frequency)
                        .arg(minPWM->value())
                        .arg(maxPWM->value());

      _serialPort->write(cmd.toUtf8());
      _logTextEdit->append(cmd);
    }
  });

  layout->addWidget(group, 0, Qt::AlignTop | Qt::AlignHCenter);
  layout->addStretch();
}

void MainWindow::setupLCTab(QWidget *tab) {
  auto *layout = new QVBoxLayout(tab);

  auto *group = new QGroupBox("Loadcell Configuration");
  group->setFixedWidth(420);

  auto *quantity = new QSpinBox();
  quantity->setRange(1, 3);
  quantity->setValue(1);

  auto *vpu = new QSpinBox();
  vpu->setRange(1, 1000);
  vpu->setValue(206);

  auto *idSelect = new QComboBox();

  auto *channel = new QComboBox();
  channel->addItem("A");
  channel->addItem("B");

  auto *gain = new QComboBox();
  gain->addItem("128");
  gain->addItem("64");

  auto *sign = new QComboBox();
  sign->addItem("1");
  sign->addItem("-1");

  auto *sampleAverage = new QSpinBox();
  sampleAverage->setRange(1, 100);
  sampleAverage->setValue(10);

  auto *precision = new QSpinBox();
  precision->setRange(1, 200);
  precision->setValue(100);

  auto *applyBtn = new QPushButton("Apply");

  auto *grid = new QGridLayout(group);
  grid->setHorizontalSpacing(20);
  grid->setVerticalSpacing(12);

  grid->addWidget(new QLabel("Loadcell Quantity"), 0, 0);
  grid->addWidget(quantity, 0, 1);

  grid->addWidget(new QLabel("Loadcell ID"), 0, 2);
  grid->addWidget(idSelect, 0, 3);

  grid->addWidget(new QLabel("Channel"), 1, 0);
  grid->addWidget(channel, 1, 1);

  grid->addWidget(new QLabel("Gain"), 1, 2);
  grid->addWidget(gain, 1, 3);

  grid->addWidget(new QLabel("Sign"), 2, 0);
  grid->addWidget(sign, 2, 1);

  grid->addWidget(new QLabel("Sample Average"), 2, 2);
  grid->addWidget(sampleAverage, 2, 3);

  grid->addWidget(new QLabel("Tare Precision"), 3, 0);
  grid->addWidget(precision, 3, 1);

  grid->addWidget(new QLabel("Value Per Unit"), 3, 2);
  grid->addWidget(vpu, 3, 3);

  grid->addWidget(applyBtn, 4, 0, 1, 4);

  layout->addWidget(group, 0, Qt::AlignTop | Qt::AlignHCenter);
  layout->addStretch();

  auto updateQuantity = [=]() {
    int q = quantity->value();

    _lcConfigs.clear();
    idSelect->clear();

    for (int i = 0; i < q; i++) {
      LCConfig cfg;
      cfg.id = i + 1;
      cfg.channel = 'A';
      cfg.gain = 128;
      cfg.sign = 1;
      cfg.sampleAverage = 10;
      cfg.tarePrecision = 100;
      cfg.val_per_unit = 206;
      cfg.quantity = 1;

      _lcConfigs.append(cfg);

      idSelect->addItem(QString("LC%1").arg(i + 1));
    }
  };

  updateQuantity();

  connect(quantity, QOverload<int>::of(&QSpinBox::valueChanged), this,
          [=](int) { updateQuantity(); });

  auto saveCurrentLC = [=]() {
    if (_lcConfigs.isEmpty())
      return;

    if (_currentLC < 0 || _currentLC >= _lcConfigs.size())
      return;

    auto &cfg = _lcConfigs[_currentLC];

    cfg.channel = channel->currentText().toLatin1()[0];
    cfg.gain = gain->currentText().toInt();
    cfg.sign = sign->currentText().toInt();
    cfg.sampleAverage = sampleAverage->value();
    cfg.tarePrecision = precision->value();
    cfg.val_per_unit = vpu->value();
    cfg.quantity = quantity->value();
  };

  connect(idSelect, &QComboBox::currentIndexChanged, this, [=](int index) {
    saveCurrentLC(); // lưu LC cũ

    _currentLC = index;

    if (index < 0 || index >= _lcConfigs.size())
      return;

    auto &cfg = _lcConfigs[index];

    channel->setCurrentText(QString(cfg.channel));
    gain->setCurrentText(QString::number(cfg.gain));
    sign->setCurrentText(QString::number(cfg.sign));
    sampleAverage->setValue(cfg.sampleAverage);
    precision->setValue(cfg.tarePrecision);
    vpu->setValue(cfg.val_per_unit);
  });

  connect(channel, &QComboBox::currentTextChanged, this, [=](QString ch) {
    gain->clear();

    if (ch == "A") {
      gain->addItem("128");
      gain->addItem("64");
    } else {
      gain->addItem("32");
    }
  });

  connect(applyBtn, &QPushButton::clicked, this, [=]() {
    saveCurrentLC();

    for (const auto &cfg : _lcConfigs) {
      QString cmd = QString("lcconf:%1:%2:%3:%4:%5:%6:%7:%8\n")
                        .arg(cfg.id)
                        .arg(cfg.channel)
                        .arg(cfg.gain)
                        .arg(cfg.sign)
                        .arg(cfg.val_per_unit)
                        .arg(cfg.sampleAverage)
                        .arg(cfg.quantity)
                        .arg(cfg.tarePrecision);

      if (_serialPort && _serialPort->isOpen()) {
        _serialPort->write(cmd.toUtf8());
      }
    }
  });
}

void MainWindow::setupMavlinkTab(QWidget *tab) {
  auto *layout = new QVBoxLayout(tab);

  auto *group = new QGroupBox("MAVLink Connection");
  group->setFixedWidth(400);
  auto *form = new QFormLayout(group);

  auto *sysID = new QSpinBox();
  sysID->setRange(1, 255);
  sysID->setValue(1);

  auto *compID = new QSpinBox();
  compID->setRange(1, 255);
  compID->setValue(1);

  auto *baud = new QComboBox();
  baud->addItems({"57600", "115200", "921600"});

  auto *udpPort = new QSpinBox();
  udpPort->setRange(1000, 65535);
  udpPort->setValue(14550);

  auto *connectBtn = new QPushButton("Connect");

  form->addRow("System ID", sysID);
  form->addRow("Component ID", compID);
  form->addRow("Baudrate", baud);
  form->addRow("UDP Port", udpPort);
  form->addRow(connectBtn);

  layout->addWidget(group, 0, Qt::AlignTop | Qt::AlignHCenter);
  layout->addStretch();
}

void MainWindow::setupFFTTab(QWidget *tab) {
  auto *layout = new QVBoxLayout(tab);

  auto *group = new QGroupBox("FFT Analysis Settings");
  group->setFixedWidth(400);
  auto *form = new QFormLayout(group);

  auto *windowSize = new QSpinBox();
  windowSize->setRange(128, 8192);
  windowSize->setValue(1024);

  auto *sampleRate = new QSpinBox();
  sampleRate->setRange(100, 10000);
  sampleRate->setValue(1000);

  auto *overlap = new QSpinBox();
  overlap->setRange(0, 90);
  overlap->setValue(50);

  auto *windowType = new QComboBox();
  windowType->addItems({"Hann", "Hamming", "Blackman"});

  auto *applyBtn = new QPushButton("Apply");

  form->addRow("Window Size", windowSize);
  form->addRow("Sample Rate (Hz)", sampleRate);
  form->addRow("Overlap (%)", overlap);
  form->addRow("Window Type", windowType);
  form->addRow(applyBtn);

  layout->addWidget(group, 0, Qt::AlignTop | Qt::AlignHCenter);
  layout->addStretch();
}

quint16 MainWindow::vescCrc16(const QByteArray &data) {
  quint16 crc = 0;
  for (int i = 0; i < data.size(); i++) {
    crc = crc16_tab[(((crc >> 8) ^ data[i]) & 0xFF)] ^ (crc << 8);
  }
  return crc;
}

void MainWindow::vescRefreshPorts() {
  if (!_vescPortComboBox)
    return;

  QStringList currentPorts;
  for (const auto &port : QSerialPortInfo::availablePorts()) {
    currentPorts << port.portName();
  }

  QString selected = _vescPortComboBox->currentText();
  QStringList oldPorts;
  for (int i = 0; i < _vescPortComboBox->count(); i++)
    oldPorts << _vescPortComboBox->itemText(i);

  if (currentPorts != oldPorts) {
    _vescPortComboBox->blockSignals(true);
    _vescPortComboBox->clear();
    _vescPortComboBox->addItems(currentPorts);
    int idx = currentPorts.indexOf(selected);
    if (idx >= 0)
      _vescPortComboBox->setCurrentIndex(idx);
    _vescPortComboBox->blockSignals(false);
  }
}

void MainWindow::vescSendGetValues() {
  if (!_vescSerialPort || !_vescSerialPort->isOpen())
    return;

  // Payload: 1 byte = COMM_GET_VALUES (0x04)
  QByteArray payload;
  payload.append(static_cast<char>(0x04));

  quint16 crc = vescCrc16(payload);

  QByteArray packet;
  packet.append(static_cast<char>(0x02)); // start byte (short packet)
  packet.append(static_cast<char>(payload.size()));
  packet.append(payload);
  packet.append(static_cast<char>((crc >> 8) & 0xFF));
  packet.append(static_cast<char>(crc & 0xFF));
  packet.append(static_cast<char>(0x03)); // stop byte

  _vescSerialPort->write(packet);
}

void MainWindow::vescReadData() {
  if (!_vescSerialPort)
    return;

  _vescRxBuffer.append(_vescSerialPort->readAll());

  while (_vescRxBuffer.size() >= 5) {
    quint8 startByte = static_cast<quint8>(_vescRxBuffer[0]);

    int payloadLenSize = 0;
    int payloadLen = 0;

    if (startByte == 0x02) {
      if (_vescRxBuffer.size() < 2)
        return;
      payloadLen = static_cast<quint8>(_vescRxBuffer[1]);
      payloadLenSize = 1;
    } else if (startByte == 0x03) {
      if (_vescRxBuffer.size() < 3)
        return;
      payloadLen = (static_cast<quint8>(_vescRxBuffer[1]) << 8) |
                   static_cast<quint8>(_vescRxBuffer[2]);
      payloadLenSize = 2;
    } else {
      _vescRxBuffer.remove(0, 1);
      continue;
    }

    // Total packet size: 1(start) + payloadLenSize + payloadLen + 2(CRC) +
    // 1(stop)
    int totalLen = 1 + payloadLenSize + payloadLen + 3;

    if (_vescRxBuffer.size() < totalLen)
      return;

    quint8 stopByte = static_cast<quint8>(_vescRxBuffer[totalLen - 1]);
    if (stopByte != 0x03) {
      _vescRxBuffer.remove(0, 1);
      continue;
    }

    int payloadStart = 1 + payloadLenSize;
    QByteArray payload = _vescRxBuffer.mid(payloadStart, payloadLen);

    quint16 receivedCrc =
        (static_cast<quint8>(_vescRxBuffer[payloadStart + payloadLen]) << 8) |
        static_cast<quint8>(_vescRxBuffer[payloadStart + payloadLen + 1]);
    quint16 calcCrc = vescCrc16(payload);

    if (receivedCrc == calcCrc) {
      vescParsePacket(payload);
    } else {
      if (_logTextEdit) {
        QString ts = QDateTime::currentDateTime().toString("hh:mm:ss");
        _logTextEdit->append(QString("[%1] CRC Error: expected 0x%2, got 0x%3")
                                 .arg(ts)
                                 .arg(calcCrc, 4, 16, QChar('0'))
                                 .arg(receivedCrc, 4, 16, QChar('0')));
      }
    }

    _vescRxBuffer.remove(0, totalLen);
  }
}

void MainWindow::vescParsePacket(const QByteArray &payload) {
  if (payload.isEmpty())
    return;

  quint8 cmd = static_cast<quint8>(payload[0]);
  if (cmd != 0x04)
    return;

  // Offset (after cmd byte):
  //  0-1:  temp_mos        (int16, scale 10)
  //  2-3:  temp_motor      (int16, scale 10)
  //  4-7:  avg_motor_current (int32, scale 100)
  //  8-11: avg_input_current (int32, scale 100)
  // 12-15: avg_id          (int32, scale 100)
  // 16-19: avg_iq          (int32, scale 100)
  // 20-21: duty_cycle      (int16, scale 1000)
  // 22-25: rpm             (int32, scale 1)
  // 26-27: v_in            (int16, scale 10)
  // 28-31: amp_hours       (int32, scale 10000)
  // 32-35: amp_hours_charged (int32, scale 10000)
  // 36-39: watt_hours       (int32, scale 10000)
  // 40-43: watt_hours_charged (int32, scale 10000)
  // 44-47: tachometer       (int32)
  // 48-51: tachometer_abs   (int32)

  const int minLen = 1 + 52; // cmd + data
  if (payload.size() < minLen) {
    if (_logTextEdit) {
      QString ts = QDateTime::currentDateTime().toString("hh:mm:ss");
      _logTextEdit->append(QString("[%1] Short packet: %2 bytes (need %3)")
                               .arg(ts)
                               .arg(payload.size())
                               .arg(minLen));
    }
    return;
  }

  const quint8 *d = reinterpret_cast<const quint8 *>(payload.constData()) + 1;

  auto getInt16 = [&d](int offset) -> qint16 {
    return static_cast<qint16>((d[offset] << 8) | d[offset + 1]);
  };
  auto getInt32 = [&d](int offset) -> qint32 {
    return static_cast<qint32>((static_cast<quint32>(d[offset]) << 24) |
                               (static_cast<quint32>(d[offset + 1]) << 16) |
                               (static_cast<quint32>(d[offset + 2]) << 8) |
                               static_cast<quint32>(d[offset + 3]));
  };

  //   double tempMos = getInt16(0) / 10.0;
  //   double tempMotor = getInt16(2) / 10.0;
  //   double avgMotorCurrent = getInt32(4) / 100.0;
  currentValue = getInt32(8) / 100.0;
  // skip avg_id (12-15), avg_iq (16-19)
  pwmValue = getInt16(20) / 1000.0 * 100.0;
  RPMValue = getInt32(22) / 21;
  voltageValue = getInt16(26) / 10.0;
  //   double ampHours = getInt32(28) / 10000.0;
  //   double wattHours = getInt32(36) / 10000.0;

  if (_voltageLabel)
    _voltageLabel->setText(
        QString("Voltage: %1 V").arg(voltageValue, 0, 'f', 1));
  if (_currentLabel)
    _currentLabel->setText(
        QString("Current: %1 A").arg(currentValue, 0, 'f', 2));
  if (_RPMLabel)
    _RPMLabel->setText(QString("RPM: %1").arg(RPMValue));
  if (_pwmLabel)
    _pwmLabel->setText(QString("PWM: %1").arg(pwmValue, 0, 'f', 1));

  if (_plotting) {
    updateAnalyzeCharts(thrustValue, voltageValue, currentValue, pwmValue,
                        RPMValue);
  }
}

void MainWindow::setupVESCTab(QWidget *tab) {
  auto *layout = new QVBoxLayout(tab);
  layout->setSpacing(10);

  auto *connGroup = new QGroupBox("VESC Connection");
  connGroup->setFixedWidth(420);
  auto *connGrid = new QGridLayout(connGroup);

  _vescPortComboBox = new QComboBox();
  _vescBaudComboBox = new QComboBox();
  _vescBaudComboBox->addItems({"115200", "57600", "921600", "256000"});

  _vescConnectBtn = new QPushButton("Connect");

  _vescStatusLabel = new QLabel("Disconnected");
  _vescStatusLabel->setStyleSheet(
      "color: #ff6b6b; font-weight: bold; font-size: 12px;");

  connGrid->addWidget(_vescStatusLabel, 0, 0);
  connGrid->addWidget(_vescPortComboBox, 1, 0);
  connGrid->addWidget(_vescBaudComboBox, 1, 1);
  connGrid->addWidget(_vescConnectBtn, 1, 2);

  auto *pollGroup = new QGroupBox("Poll Settings");
  pollGroup->setFixedWidth(420);
  auto *pollForm = new QFormLayout(pollGroup);
  auto *pollInterval = new QSpinBox();
  pollInterval->setRange(50, 5000);
  pollInterval->setValue(100);
  pollInterval->setSuffix(" ms");
  pollForm->addRow("Poll Interval:", pollInterval);

  layout->addWidget(connGroup, 0, Qt::AlignTop | Qt::AlignHCenter);
  layout->addWidget(pollGroup, 0, Qt::AlignTop | Qt::AlignHCenter);
  layout->addStretch();

  auto *vescRefreshTimer = new QTimer(this);
  connect(vescRefreshTimer, &QTimer::timeout, this,
          &MainWindow::vescRefreshPorts);
  vescRefreshTimer->start(1000);
  vescRefreshPorts();

  _vescPollTimer = new QTimer(this);
  connect(_vescPollTimer, &QTimer::timeout, this,
          &MainWindow::vescSendGetValues);

  connect(pollInterval, QOverload<int>::of(&QSpinBox::valueChanged), this,
          [this](int ms) {
            if (_vescPollTimer->isActive()) {
              _vescPollTimer->setInterval(ms);
            }
          });

  connect(_vescConnectBtn, &QPushButton::clicked, this, [this, pollInterval]() {
    if (_vescConnected) {
      _vescPollTimer->stop();
      if (_vescSerialPort) {
        _vescSerialPort->close();
        delete _vescSerialPort;
        _vescSerialPort = nullptr;
      }
      _vescConnected = false;
      _vescConnectBtn->setText("Connect");
      _vescStatusLabel->setText("Disconnected");
      _vescStatusLabel->setStyleSheet(
          "color: #ff6b6b; font-weight: bold; font-size: 12px;");
      _vescRxBuffer.clear();

      QString ts = QDateTime::currentDateTime().toString("hh:mm:ss");
      _logTextEdit->append(QString("[%1] VESC disconnected").arg(ts));
    } else {
      _vescSerialPort = new QSerialPort(this);
      _vescSerialPort->setPortName(_vescPortComboBox->currentText());
      _vescSerialPort->setBaudRate(_vescBaudComboBox->currentText().toInt());
      _vescSerialPort->setDataBits(QSerialPort::Data8);
      _vescSerialPort->setParity(QSerialPort::NoParity);
      _vescSerialPort->setStopBits(QSerialPort::OneStop);
      _vescSerialPort->setFlowControl(QSerialPort::NoFlowControl);

      if (_vescSerialPort->open(QIODevice::ReadWrite)) {
        _vescConnected = true;
        _vescConnectBtn->setText("Disconnect");
        _vescStatusLabel->setText(
            QString("Connected: %1").arg(_vescPortComboBox->currentText()));
        _vescStatusLabel->setStyleSheet(
            "color: #36b37e; font-weight: bold; font-size: 12px;");

        connect(_vescSerialPort, &QSerialPort::readyRead, this,
                &MainWindow::vescReadData);

        _vescRxBuffer.clear();
        _vescPollTimer->start(pollInterval->value());

        QString ts = QDateTime::currentDateTime().toString("hh:mm:ss");
        _logTextEdit->append(QString("[%1] Connected to %2 @ %3 baud")
                                 .arg(ts, _vescPortComboBox->currentText(),
                                      _vescBaudComboBox->currentText()));
      } else {
        QMessageBox::critical(this, "VESC Error",
                              QString("Cannot open port %1:\n%2")
                                  .arg(_vescPortComboBox->currentText(),
                                       _vescSerialPort->errorString()));
        delete _vescSerialPort;
        _vescSerialPort = nullptr;
      }
    }
  });
}
