#include "dashboardwidget.h"
#include "mainwindow.h"
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QtCharts>

#include <QtWidgets>

void MainWindow::setupHomeTab() {
  auto *homeLayout = new QVBoxLayout(_homeTab);
  homeLayout->setContentsMargins(10, 10, 10, 10);
  homeLayout->setSpacing(10);

  auto *miniDashboard = new DashBoardWidget(_homeTab);
#if defined(Q_OS_WIN)
  miniDashboard->setMinimumSize(180, 180);
#else
  miniDashboard->setMinimumSize(230, 230);
#endif
  miniDashboard->setBorderColor(QColor(27, 36, 50));
  miniDashboard->setArcColor(QColor(215, 170, 60));
  miniDashboard->setBorderWidth(0);
  miniDashboard->setBorderRadius(10);
  miniDashboard->setValueColor(QColor(255, 107, 107));
  miniDashboard->setTitleColor(QColor(255, 107, 107));

  auto *valueSlider = new QSlider(Qt::Horizontal, _homeTab);
  valueSlider->setRange(0, 100);
  valueSlider->setValue(0);

  auto *valueSpinBox = new QDoubleSpinBox(_homeTab);
  valueSpinBox->setRange(0, 100);
  valueSpinBox->setValue(0);
  valueSpinBox->setDecimals(0);

  auto *minValueLabel = new QLabel(tr("Min Value:"), _homeTab);
  auto *minValueSpinBox = new QDoubleSpinBox(_homeTab);
  minValueSpinBox->setRange(0, 100);
  minValueSpinBox->setValue(0);
  minValueSpinBox->setDecimals(0);

  auto *maxValueLabel = new QLabel(tr("Max Value:"), _homeTab);
  auto *maxValueSpinBox = new QDoubleSpinBox(_homeTab);
  maxValueSpinBox->setRange(0, 100);
  maxValueSpinBox->setValue(100);
  maxValueSpinBox->setDecimals(0);

  auto *propLenLabel = new QLabel(tr("Propeller Size:"), _homeTab);

  auto *propLenSpinBox = new QDoubleSpinBox(_homeTab);
  propLenSpinBox->setRange(0, 100);
  propLenSpinBox->setValue(0);
  propLenSpinBox->setDecimals(0);

  auto *propLenUnitComboBox = new QComboBox(_homeTab);
  propLenUnitComboBox->addItem("mm");
  propLenUnitComboBox->addItem("cm");
  propLenUnitComboBox->addItem("m");
  propLenUnitComboBox->addItem("inch");

  L = propLenSpinBox->value();

  QString unit = propLenUnitComboBox->currentText();

  if (unit == "mm") {
    L /= 1000.0f;
  } else if (unit == "cm") {
    L /= 100.0f;
  } else if (unit == "inch") {
    L *= 0.0254f;
  }

  auto *plotButton = new QPushButton(tr("Start Plot"), _homeTab);

  auto *logTextEdit = new QTextEdit(_homeTab);
  logTextEdit->setMaximumHeight(100);
  logTextEdit->setReadOnly(true);

  auto *topContentLayout = new QHBoxLayout();

  auto *leftPanel = new QWidget(_homeTab);
  auto *leftLayout = new QVBoxLayout(leftPanel);
  leftLayout->setAlignment(Qt::AlignTop);
  leftLayout->setSpacing(10);

  // Chart Display
  auto *scrollContentWidget = new QWidget();

  scrollContentWidget->setObjectName("ChartContainerWidget");

  auto *displayLayout = new QVBoxLayout(scrollContentWidget);
  displayLayout->setSpacing(10);
  displayLayout->setContentsMargins(10, 10, 10, 10);

  _series1 = new QLineSeries();
  _series2 = new QLineSeries();
  _series3 = new QLineSeries();
  _series4 = new QLineSeries();

  _thrustChart = createAnalyzeChart("Thrust (N)", _series1);
  _voltChart = createAnalyzeChart("Voltage (V)", _series2);
  _currentChart = createAnalyzeChart("Current (A)", _series3);
  _rpmChart = createAnalyzeChart("RPM", _series4);

  _thrustChart->setMinimumHeight(250);
  _voltChart->setMinimumHeight(250);
  _currentChart->setMinimumHeight(250);
  _rpmChart->setMinimumHeight(250);

  displayLayout->addWidget(_thrustChart);
  displayLayout->addWidget(_voltChart);
  displayLayout->addWidget(_currentChart);
  displayLayout->addWidget(_rpmChart);

  _thrustChart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  _voltChart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  _currentChart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  _rpmChart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  displayLayout->addWidget(_thrustChart, 1);
  displayLayout->addWidget(_voltChart, 1);
  displayLayout->addWidget(_currentChart, 1);
  displayLayout->addWidget(_rpmChart, 1);

  QToolButton *chartSelector = new QToolButton();
  chartSelector->setText("Charts");
  chartSelector->setPopupMode(QToolButton::InstantPopup);

  QMenu *menu = new QMenu(chartSelector);

  QAction *thrustAct = menu->addAction("Thrust");
  QAction *voltAct = menu->addAction("Voltage");
  QAction *currentAct = menu->addAction("Current");
  QAction *rpmAct = menu->addAction("RPM");

  thrustAct->setCheckable(true);
  voltAct->setCheckable(true);
  currentAct->setCheckable(true);
  rpmAct->setCheckable(true);

  thrustAct->setChecked(true);
  voltAct->setChecked(false);
  currentAct->setChecked(false);
  rpmAct->setChecked(true);

  chartSelector->setMenu(menu);

  connect(thrustAct, &QAction::toggled, this,
          [=](bool checked) { _thrustChart->setVisible(checked); });

  connect(voltAct, &QAction::toggled, this,
          [=](bool checked) { _voltChart->setVisible(checked); });

  connect(currentAct, &QAction::toggled, this,
          [=](bool checked) { _currentChart->setVisible(checked); });

  connect(rpmAct, &QAction::toggled, this,
          [=](bool checked) { _rpmChart->setVisible(checked); });

  _voltChart->setVisible(false);
  _currentChart->setVisible(false);

  auto *topLayout = new QHBoxLayout();
  topLayout->addStretch();
  topLayout->addWidget(chartSelector);

  displayLayout->insertLayout(0, topLayout);

  auto *scrollArea = new QScrollArea();
  scrollArea->setWidget(scrollContentWidget);

  scrollArea->setWidgetResizable(true);
  scrollArea->setFrameShape(QFrame::NoFrame);

  // UART Connection
  auto *connectionGroup = new QGroupBox(tr("Serial Connection"), _homeTab);
  auto *connectionLayout = new QGridLayout(connectionGroup);
  auto *connectButton = new QPushButton(tr("Connect"), _homeTab);

  portComboBox = new QComboBox(this);
  auto *baudComboBox = new QComboBox(this);
  baudComboBox->addItems({"9600", "19200", "38400", "57600", "115200", "230400",
                          "460800", "921600"});
  baudComboBox->setCurrentText("115200");

  refreshTimer = new QTimer(this);
  connect(refreshTimer, &QTimer::timeout, this,
          &MainWindow::refreshSerialPorts);
  refreshTimer->start(1000);

  refreshSerialPorts();

  connectionLayout->addWidget(portComboBox, 0, 0);
  connectionLayout->addWidget(baudComboBox, 0, 1);
  connectionLayout->addWidget(connectButton, 0, 2);

  // UART Command Sending
  auto *commandGroup = new QGroupBox(tr("Test Modes"), _homeTab);
  auto *commandLayout = new QGridLayout(commandGroup);
  auto *commandEdit = new QLineEdit(_homeTab);
  auto *sendButton = new QPushButton(tr("Send"), _homeTab);

  auto *startButton = new QPushButton(tr("Start"), _homeTab);
  auto *stopButton = new QPushButton(tr("Stop"), _homeTab);
  auto *rampModeButton = new QPushButton(tr("Ramp Mode"), _homeTab);
  auto *sineModeButton = new QPushButton(tr("Sine Mode"), _homeTab);
  startButton->setObjectName("startButton");
  stopButton->setObjectName("stopButton");
  rampModeButton->setObjectName("rampModeButton");
  sineModeButton->setObjectName("sineModeButton");

  startButton->setMinimumHeight(60);
  stopButton->setMinimumHeight(60);

  startButton->setCursor(Qt::PointingHandCursor);
  stopButton->setCursor(Qt::PointingHandCursor);

  startButton->setStyleSheet(R"(
        QPushButton#startButton {
            background-color: #238a63;
            color: white;
            font-size: 26px;
            font-weight: 600;
            border-radius: 14px;
            border: 2px solid #36b37e;
            padding: 14px 28px;
        }

        QPushButton#startButton:hover {
            background-color: #2fa574;
            border: 2px solid #4cc38f;
        }

        QPushButton#startButton:pressed {
            background-color: #1c6e50;
        }
    )");

  stopButton->setStyleSheet(R"(
        QPushButton#stopButton {
            background-color: #b23a3a;
            color: white;
            font-size: 26px;
            font-weight: 600;
            border-radius: 14px;
            border: 2px solid #d65a5a;
            padding: 14px 28px;
        }

        QPushButton#stopButton:hover {
            background-color: #c94b4b;
            border: 2px solid #e57373;
        }

        QPushButton#stopButton:pressed {
            background-color: #8f2f2f;
        }
    )");

  startButton->setText("▶ START");
  stopButton->setText("■ STOP");

  commandEdit->setPlaceholderText("Enter command to send");
  commandLayout->addWidget(new QLabel(tr("Command:")), 0, 0);
  commandLayout->addWidget(commandEdit, 0, 1, 1, 2);
  commandLayout->addWidget(sendButton, 0, 3);
  commandLayout->addWidget(rampModeButton, 1, 0, 1, 2);
  commandLayout->addWidget(sineModeButton, 1, 2, 1, 2);
  commandLayout->setColumnStretch(0, 1);
  commandLayout->setColumnStretch(1, 1);
  commandLayout->setColumnStretch(2, 1);
  commandLayout->setColumnStretch(3, 1);

  auto *dataGroup = new QGroupBox(tr("Live Telemetry"), _homeTab);
  auto *dataLayout = new QGridLayout(dataGroup);

  auto *thrustValueLabel = new QLabel(tr("Thrust: -- N"), _homeTab);
  auto *pwmValueLabel = new QLabel(tr("PWM: -- %"), _homeTab);
  auto *voltageValueLabel = new QLabel(tr("Voltage: -- V"), _homeTab);
  auto *currentValueLabel = new QLabel(tr("Current: -- A"), _homeTab);
  auto *RPMValueLabel = new QLabel(tr("RPM: --"), _homeTab);

  thrustValueLabel->setStyleSheet(
      "font-weight: bold; font-size: 13px; padding: 6px; border-radius: 3px;");
  pwmValueLabel->setStyleSheet(
      "font-weight: bold; font-size: 13px; padding: 6px; border-radius: 3px;");
  voltageValueLabel->setStyleSheet(
      "font-weight: bold; font-size: 13px; padding: 6px; border-radius: 3px;");
  currentValueLabel->setStyleSheet(
      "font-weight: bold; font-size: 13px; padding: 6px; border-radius: 3px;");
  RPMValueLabel->setStyleSheet(
      "font-weight: bold; font-size: 13px; padding: 6px; border-radius: 3px;");

  dataLayout->addWidget(thrustValueLabel, 0, 0);
  dataLayout->addWidget(currentValueLabel, 0, 1);
  dataLayout->addWidget(voltageValueLabel, 0, 2);
  dataLayout->addWidget(pwmValueLabel, 1, 0);
  dataLayout->addWidget(RPMValueLabel, 1, 1);

  auto *dashboardGroup = new QGroupBox(tr("Motor Output"), _homeTab);
  auto *dashboardLayout = new QVBoxLayout(dashboardGroup);
  dashboardLayout->addWidget(miniDashboard);
  dashboardLayout->setAlignment(miniDashboard, Qt::AlignTop);

  auto *valueGroup = new QGroupBox(tr("Control Panel"), _homeTab);
  auto *valueControlLayout = new QGridLayout(valueGroup);
  valueControlLayout->addWidget(new QLabel(tr("Throttle:")), 0, 0);
  valueControlLayout->addWidget(valueSlider, 0, 1, 1, 2);
  valueControlLayout->addWidget(valueSpinBox, 0, 3);
  valueControlLayout->addWidget(minValueLabel, 1, 0);
  valueControlLayout->addWidget(minValueSpinBox, 1, 1);
  valueControlLayout->addWidget(maxValueLabel, 1, 2);
  valueControlLayout->addWidget(maxValueSpinBox, 1, 3);
  valueControlLayout->addWidget(propLenLabel, 2, 0);
  valueControlLayout->addWidget(propLenSpinBox, 2, 1, 1, 2);
  valueControlLayout->addWidget(propLenUnitComboBox, 2, 3);
  valueControlLayout->addWidget(plotButton, 3, 0, 1, 4);

  valueControlLayout->setColumnStretch(1, 1);
  valueControlLayout->setColumnStretch(2, 1);

  // UART Log Display
  auto *logGroup = new QGroupBox(tr("System Log"), _homeTab);
  auto *logLayout = new QVBoxLayout(logGroup);
  logLayout->addWidget(logTextEdit);

  QHBoxLayout *enableLayout = new QHBoxLayout();

  enableLayout->addWidget(startButton);
  enableLayout->addWidget(stopButton);

  leftLayout->addWidget(dashboardGroup);
  leftLayout->addWidget(valueGroup);
  leftLayout->addWidget(connectionGroup);
  leftLayout->addWidget(commandGroup);
  leftLayout->addWidget(dataGroup);
  leftLayout->addWidget(logGroup);
  leftLayout->addLayout(enableLayout);
  leftLayout->addStretch();

  topContentLayout->addWidget(leftPanel, 1);
  topContentLayout->addWidget(scrollArea, 3);

  homeLayout->addLayout(topContentLayout, 1);

  sendButton->setToolTip("Send Commnad to device\n"
                         "Available commands:\n"
                         "   # s or start - Start Motor\n"
                         "   # t or stop - Stop Motor\n"
                         "   # u or up - Increase Motor Power\n"
                         "   # d or down - Decrease Motor Power");

  connect(valueSlider, &QSlider::valueChanged, _homeTab,
          [miniDashboard, valueSpinBox](int value) {
            valueSpinBox->blockSignals(true);
            miniDashboard->setValue(value);
            valueSpinBox->setValue(value);
            valueSpinBox->blockSignals(false);
          });

  connect(valueSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          _homeTab, [miniDashboard, valueSlider](int value) {
            valueSlider->blockSignals(true);
            miniDashboard->setValue(value);
            valueSlider->setValue(value);
            valueSlider->blockSignals(false);
          });

  // valueSlider UART
  connect(
      valueSlider, &QSlider::valueChanged, _homeTab,
      [this, minValueSpinBox, maxValueSpinBox](int value) {
        if (_serialPort && _serialPort->isOpen()) {
          if (swcurrent) {
            if (value > maxValueSpinBox->value())
              value = maxValueSpinBox->value();
            else if (value < minValueSpinBox->value())
              value = minValueSpinBox->value();
          } else {
            if (value > 50)
              value = 50;
            else if (value < 0)
              value = 0;
          }
          QString command = QString("Power:%1\n").arg(value);
          _serialPort->write(command.toUtf8());

          QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
          _logTextEdit->append(
              QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
          saveLogToCSV(
              QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
        }
      });

  connect(
      valueSpinBox, &QDoubleSpinBox::editingFinished, this,
      [this, valueSpinBox, minValueSpinBox, maxValueSpinBox]() {
        if (_serialPort && _serialPort->isOpen()) {
          double value = valueSpinBox->value();
          if (swcurrent) {
            if (value > maxValueSpinBox->value())
              value = maxValueSpinBox->value();
            else if (value < minValueSpinBox->value())
              value = minValueSpinBox->value();
          } else {
            if (value > 50)
              value = 50;
            else if (value < 0)
              value = 0;
          }
          QString command = QString("Power:%1\n").arg(value, 0, 'f', 1);
          _serialPort->write(command.toUtf8());

          QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
          _logTextEdit->append(
              QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
          saveLogToCSV(
              QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
        }
      });

  connect(startButton, &QPushButton::clicked, _homeTab, [this]() {
    if (_serialPort && _serialPort->isOpen()) {
      QString command = QString("start\n");
      _serialPort->write(command.toUtf8());

      QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
      _logTextEdit->append(
          QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
      saveLogToCSV(
          QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
    }
  });

  connect(
      stopButton, &QPushButton::clicked, _homeTab,
      [this, valueSlider, valueSpinBox, miniDashboard]() {
        if (_serialPort && _serialPort->isOpen()) {
          _autoTimer->stop();
          valueSlider->blockSignals(true);
          miniDashboard->setValue(0);
          valueSlider->setValue(static_cast<int>(0));
          valueSpinBox->setValue(static_cast<int>(0));
          valueSlider->blockSignals(false);

          QString command = QString("stop\n");
          _serialPort->write(command.toUtf8());

          QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
          _logTextEdit->append(
              QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
          saveLogToCSV(
              QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
        }
      });

  connect(rampModeButton, &QPushButton::clicked, this, [this, valueSlider]() {
    if (!_serialPort || !_serialPort->isOpen())
      return;

    if (!_autoTimer) {
      _autoTimer = new QTimer(this);

      connect(_autoTimer, &QTimer::timeout, this, [this, valueSlider]() {
        if (autoValue >= 23) {
          _autoTimer->stop();
          autoValue = 0;
          pdf = false;
          bufferCopy = _dataPDFBuffer;
          saveDataToPDF();
          _dataPDFBuffer.clear();
          return;
        }

        autoValue++;
        valueSlider->setValue(autoValue);
      });
    }

    autoValue = 0;
    valueSlider->setValue(0);
    pdf = true;

    _autoTimer->start(1000);
  });

  connect(minValueSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          miniDashboard, &DashBoardWidget::setMinValue);
  connect(maxValueSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
          miniDashboard, &DashBoardWidget::setMaxValue);

  connect(connectButton, &QPushButton::clicked, _homeTab,
          [this, baudComboBox]() {
            _serialPort = new QSerialPort;
            _serialPort->setPortName(this->portComboBox->currentText());

            bool ok;
            qint32 baudRate = baudComboBox->currentText().toInt(&ok);
            if (ok) {
              _serialPort->setBaudRate(baudRate);
            } else {
              _serialPort->setBaudRate(QSerialPort::Baud115200);
            }

            _serialPort->setDataBits(QSerialPort::Data8);
            _serialPort->setParity(QSerialPort::NoParity);
            _serialPort->setStopBits(QSerialPort::OneStop);

            if (_serialPort->open(QIODevice::ReadWrite)) {
              QMessageBox::information(this, "Success",
                                       "UART Port Connected Successfully!");
              connect(_serialPort, &QSerialPort::readyRead, this,
                      &MainWindow::readData);
            } else {
              QMessageBox::critical(this, "Error",
                                    "Unable to connect to UART port");
            }
          });

  connect(sendButton, &QPushButton::clicked, _homeTab,
          [this, commandEdit, logTextEdit]() {
            if (_serialPort && _serialPort->isOpen()) {
              QString command = commandEdit->text() + "\n";
              _serialPort->write(command.toUtf8());

              QString timestamp =
                  QDateTime::currentDateTime().toString("hh:mm:ss");
              logTextEdit->append(
                  QString("[%1] SENT: %2").arg(timestamp, command.trimmed()));

              qDebug() << "Command sent:" << command;
              commandEdit->clear();
            } else {
              QMessageBox::warning(this, "Warning", "UART port not connected");
            }
          });

  connect(commandEdit, &QLineEdit::returnPressed, _homeTab,
          [sendButton]() { sendButton->click(); });

  connect(plotButton, &QPushButton::clicked, _homeTab, [this, plotButton]() {
    if (!_plotting) {
      _plotting = true;
      plotButton->setText(tr("Stop Plot"));
      plotButton->setStyleSheet(
          "background-color: #b23a3a; color: white; font-weight: bold;");

      _series1->clear();
      _series2->clear();
      _series3->clear();
      _series4->clear();
      _dataBuffer.clear();

      _startTime = QTime::currentTime();
      qDebug() << "Plotting started";
    } else {
      _plotting = false;
      stopFlag = 1;
      plotButton->setText(tr("Start Plot"));
      plotButton->setStyleSheet("");

      qDebug() << "Plotting stopped";

      if (!_dataBuffer.isEmpty()) {
        QMessageBox::StandardButton reply =
            QMessageBox::question(this, tr("Save Data"),
                                  tr("Plotting stopped. Would you like to save "
                                     "the recorded data to CSV?"),
                                  QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
          saveDataToCSV();
        }
      }
    }
  });

  _thrustLabel = thrustValueLabel;
  _pwmLabel = pwmValueLabel;
  _voltageLabel = voltageValueLabel;
  _currentLabel = currentValueLabel;
  _RPMLabel = RPMValueLabel;

  _logTextEdit = logTextEdit;

  miniDashboard->setValue(0);
}
