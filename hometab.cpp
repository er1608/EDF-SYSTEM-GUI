#include "mainwindow.h"
#include "dashboardwidget.h"
#include <QtCharts>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <QtWidgets>

void MainWindow::setupHomeTab()
{
    // _homeTab->setStyleSheet("background: #1b2432;");

    auto *homeLayout = new QVBoxLayout(_homeTab);
    homeLayout->setContentsMargins(10, 10, 10, 10);
    homeLayout->setSpacing(10);

    auto *miniDashboard = new DashBoardWidget(_homeTab);
    miniDashboard->setMinimumSize(230, 230);
    miniDashboard->setBorderColor(QColor(27, 36, 50));
    miniDashboard->setBorderWidth(0);
    miniDashboard->setBorderRadius(10);
    // miniDashboard->setArcColor(QColor(65, 90, 119, 180));
    // miniDashboard->setScaleColor(QColor(255, 107, 107));
    // miniDashboard->setPointerColor(QColor(255, 107, 107));
    miniDashboard->setValueColor(QColor(255, 107, 107));
    miniDashboard->setTitleColor(QColor(255, 107, 107));

    auto *mainChartView = setupMainChart();

    auto *valueSlider = new QSlider(Qt::Horizontal, _homeTab);
    valueSlider->setRange(0, 100);
    valueSlider->setValue(0);

    auto *valueSpinBox = new QDoubleSpinBox(_homeTab);
    valueSpinBox->setRange(0, 100);
    valueSpinBox->setValue(0);
    valueSpinBox->setDecimals(2);

    auto *minValueLabel = new QLabel(tr("Min Value:"), _homeTab);
    auto *minValueSpinBox = new QDoubleSpinBox(_homeTab);
    minValueSpinBox->setRange(0, 100);
    minValueSpinBox->setValue(0);
    minValueSpinBox->setDecimals(2);

    auto *maxValueLabel = new QLabel(tr("Max Value:"), _homeTab);
    auto *maxValueSpinBox = new QDoubleSpinBox(_homeTab);
    maxValueSpinBox->setRange(0, 100);
    maxValueSpinBox->setValue(100);
    maxValueSpinBox->setDecimals(2);

    auto *clearPlotButton = new QPushButton(tr("Clear Plot"), _homeTab);
    auto *startPlotButton = new QPushButton(tr("Start Plot"), _homeTab);
    auto *stopPlotButton = new QPushButton(tr("Stop Plot"), _homeTab);
    auto *saveDataButton = new QPushButton(tr("Save Data to CSV"), _homeTab);

    auto *pwmValueLabel = new QLabel(tr("PWM: 0"), _homeTab);
    auto *throttleValueLabel = new QLabel(tr("Thrust: 0"), _homeTab);
    pwmValueLabel->setStyleSheet("font-weight: bold; color: #5E9C43; font-size: 13px; padding: 6px; border-radius: 4px;");
    throttleValueLabel->setStyleSheet("font-weight: bold; color: #ff6666; font-size: 13px; padding: 6px; border-radius: 4px;");

    auto *logTextEdit = new QTextEdit(_homeTab);
    logTextEdit->setMaximumHeight(100);
    logTextEdit->setReadOnly(true);

    auto *topContentLayout = new QHBoxLayout();

    auto *leftPanel = new QWidget(_homeTab);
    auto *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setAlignment(Qt::AlignTop);
    leftLayout->setSpacing(10);

    auto *displayLayout = new QVBoxLayout();
    displayLayout->setSpacing(10);

    displayLayout->addWidget(mainChartView, 1);

    auto *bottomControlsLayout = new QHBoxLayout();
    bottomControlsLayout->setSpacing(10);

    auto *valueGroup = new QGroupBox(tr("Value Settings"), _homeTab);
    auto *valueControlLayout = new QGridLayout(valueGroup);
    valueControlLayout->addWidget(new QLabel(tr("Current Value:")), 0, 0);
    valueControlLayout->addWidget(valueSlider, 0, 1);
    valueControlLayout->addWidget(valueSpinBox, 0, 2);
    valueControlLayout->addWidget(minValueLabel, 1, 0);
    valueControlLayout->addWidget(minValueSpinBox, 1, 1, 1, 2);
    valueControlLayout->addWidget(maxValueLabel, 2, 0);
    valueControlLayout->addWidget(maxValueSpinBox, 2, 1, 1, 2);

    auto *plotControlGroup = new QGroupBox(tr("Plot Control"), _homeTab);
    auto *plotControlLayout = new QGridLayout(plotControlGroup);
    plotControlLayout->addWidget(startPlotButton, 0, 0);
    plotControlLayout->addWidget(stopPlotButton, 0, 1);
    plotControlLayout->addWidget(clearPlotButton, 1, 0);
    plotControlLayout->addWidget(saveDataButton, 1, 1);
    plotControlLayout->addWidget(throttleValueLabel, 2, 0, 1, 2);
    plotControlLayout->addWidget(pwmValueLabel, 3, 0, 1, 2);

    bottomControlsLayout->addWidget(valueGroup, 2);
    bottomControlsLayout->addWidget(plotControlGroup, 1);

    displayLayout->addLayout(bottomControlsLayout);

    // UART Connection
    auto *connectionGroup = new QGroupBox(tr("Connection"), _homeTab);
    auto *connectionLayout = new QGridLayout(connectionGroup);
    auto *portComboBox = new QComboBox(_homeTab);
    auto *connectButton = new QPushButton(tr("Connect"), _homeTab);

    foreach (auto &port, QSerialPortInfo::availablePorts()) {
        portComboBox->addItem(port.portName());
    }

    portComboBox->setStyleSheet(R"(
        QComboBox {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                            stop:0 #5f758a, stop:1 #455a64);
            color: #e2e8f0;
            border: 1px solid #2d3250 !important;
            padding: 5px;
            border-radius: 4px;
        }
        QComboBox::drop-down {
            border: none;
            width: 20px;
        }
        QComboBox::down-arrow {
            width: 12px;
            height: 12px;
            image: none;
            border-left: 6px solid transparent;
            border-right: 6px solid transparent;
            border-top: 6px solid #00b4d8;
        }
        QComboBox QAbstractItemView {
            background: #121722;
            color: #121722;
            border: 1px solid #2d3250;
            selection-background-color: #2d3250;
        }
    )");

    connectionLayout->addWidget(new QLabel(tr("Port:")), 0, 0);
    connectionLayout->addWidget(portComboBox, 0, 1);
    connectionLayout->addWidget(connectButton, 0, 2);

    // UART Command Sending
    auto *commandGroup = new QGroupBox(tr("Command"), _homeTab);
    auto *commandLayout = new QGridLayout(commandGroup);
    auto *commandEdit = new QLineEdit(_homeTab);
    auto *sendButton = new QPushButton(tr("Send Command"), _homeTab);

    auto *startButton = new QPushButton(tr("Start"), _homeTab);
    auto *stopButton = new QPushButton(tr("Stop"), _homeTab);
    startButton->setObjectName("startButton");
    stopButton->setObjectName("stopButton");
    startButton->setStyleSheet("#startButton { color: green; font-weight: bold; }");
    stopButton->setStyleSheet("#stopButton { color: red; font-weight: bold; }");

    commandEdit->setPlaceholderText("Enter command to send via UART...");
    commandLayout->addWidget(new QLabel(tr("Command:")), 0, 0);
    commandLayout->addWidget(commandEdit, 0, 1);
    commandLayout->addWidget(startButton, 0, 2);
    commandLayout->addWidget(sendButton, 1, 1);
    commandLayout->addWidget(stopButton, 1, 2);

    auto *modeGroup = new QGroupBox(tr("Signal Generator Mode"), _homeTab);
    auto *modeLayout = new QGridLayout(modeGroup);

    auto *rampModeButton = new QPushButton(tr("Ramp Mode"), _homeTab);
    auto *sineWaveButton = new QPushButton(tr("Sine Wave Mode"), _homeTab);

    auto *amplitudeLabel = new QLabel(tr("Amplitude:"), _homeTab);
    auto *amplitudeSpinBox = new QDoubleSpinBox(_homeTab);
    amplitudeSpinBox->setRange(0, 100);
    amplitudeSpinBox->setValue(50);
    amplitudeSpinBox->setDecimals(1);

    auto *frequencyLabel = new QLabel(tr("Frequency (Hz):"), _homeTab);
    auto *frequencySpinBox = new QDoubleSpinBox(_homeTab);
    frequencySpinBox->setRange(0.1, 10.0);
    frequencySpinBox->setValue(1.0);
    frequencySpinBox->setDecimals(2);

    auto *durationLabel = new QLabel(tr("Duration (s):"), _homeTab);
    auto *durationSpinBox = new QDoubleSpinBox(_homeTab);
    durationSpinBox->setRange(1, 60);
    durationSpinBox->setValue(10);
    durationSpinBox->setDecimals(1);

    modeLayout->addWidget(rampModeButton, 0, 0);
    modeLayout->addWidget(sineWaveButton, 0, 1);
    modeLayout->addWidget(amplitudeLabel, 1, 0);
    modeLayout->addWidget(amplitudeSpinBox, 1, 1);
    modeLayout->addWidget(frequencyLabel, 2, 0);
    modeLayout->addWidget(frequencySpinBox, 2, 1);
    modeLayout->addWidget(durationLabel, 3, 0);
    modeLayout->addWidget(durationSpinBox, 3, 1);

    auto *dashboardGroup = new QGroupBox(tr("Motor Power"), _homeTab);
    auto *dashboardLayout = new QVBoxLayout(dashboardGroup);
    dashboardLayout->addWidget(miniDashboard);
    dashboardLayout->setAlignment(miniDashboard, Qt::AlignTop);

    leftLayout->addWidget(dashboardGroup);
    leftLayout->addWidget(connectionGroup);
    leftLayout->addWidget(commandGroup);
    leftLayout->addWidget(modeGroup);
    leftLayout->addStretch();

    topContentLayout->addWidget(leftPanel, 1);
    topContentLayout->addLayout(displayLayout, 3);

    // UART Log Display
    auto *logGroup = new QGroupBox(tr("Command Log"), _homeTab);
    auto *logLayout = new QVBoxLayout(logGroup);
    logLayout->addWidget(logTextEdit);

    homeLayout->addLayout(topContentLayout, 1);
    homeLayout->addWidget(logGroup);

    qApp->setStyleSheet(
        "QToolTip {"
        "   background-color: #1a202c;"
        "   color: #e2e8f0;"
        "   border: 1px solid #4a5568;"
        "   border-radius: 6px;"
        "   padding: 12px;"
        "   font-family: 'Segoe UI', system-ui;"
        "   font-size: 13px;"
        "   opacity: 230;"
        "}"
        );

    sendButton->setToolTip(
        "Send Commnad to device\n"
        "Available commands:\n"
        "   # s or start - Start Motor\n"
        "   # t or stop - Stop Motor\n"
        "   # u or up - Increase Motor Power\n"
        "   # d or down - Decrease Motor Power"
        );

    connect(valueSlider, &QSlider::valueChanged, _homeTab, [miniDashboard, valueSpinBox](int value) {
        valueSpinBox->blockSignals(true);
        miniDashboard->setValue(value);
        valueSpinBox->setValue(value);
        valueSpinBox->blockSignals(false);
    });

    connect(valueSpinBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            _homeTab,
            [miniDashboard, valueSlider](double value) {
                valueSlider->blockSignals(true);
                miniDashboard->setValue(value);
                valueSlider->setValue(static_cast<int>(value));
                valueSlider->blockSignals(false);
            });

    // valueSlider UART
    connect(valueSlider, &QSlider::valueChanged, _homeTab, [this, minValueSpinBox, maxValueSpinBox](int value) {
        if (_serialPort && _serialPort->isOpen()) {
            if (swcurrent)
            {
                if (value > maxValueSpinBox->value()) value = maxValueSpinBox->value();
                else if (value < minValueSpinBox->value()) value = minValueSpinBox->value();
            }
            else
            {
                if (value > 50) value = 50;
                else if (value < 0) value = 0;
            }
            QString command = QString("Power:%1\n").arg(value);
            _serialPort->write(command.toUtf8());

            QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
            _logTextEdit->append(QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
            saveLogToCSV(QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
        }
    });

    connect(valueSpinBox, &QDoubleSpinBox::editingFinished,
            this, [this, valueSpinBox, minValueSpinBox, maxValueSpinBox]() {
                if (_serialPort && _serialPort->isOpen()) {
                    double value = valueSpinBox->value();
                    if (swcurrent)
                    {
                        if (value > maxValueSpinBox->value()) value = maxValueSpinBox->value();
                        else if (value < minValueSpinBox->value()) value = minValueSpinBox->value();
                    }
                    else
                    {
                        if (value > 50) value = 50;
                        else if (value < 0) value = 0;
                    }
                    QString command = QString("Power:%1\n").arg(value, 0, 'f', 1);
                    _serialPort->write(command.toUtf8());

                    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
                    _logTextEdit->append(QString("[%1] AUTO SEND: %2")
                                             .arg(timestamp, command.trimmed()));
                    saveLogToCSV(QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
                }
            });

    connect(startButton, &QPushButton::clicked, _homeTab, [this]() {
        if (_serialPort && _serialPort->isOpen()) {
            QString command = QString("start\n");
            _serialPort->write(command.toUtf8());

            QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
            _logTextEdit->append(QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
            saveLogToCSV(QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
        }
    });

    connect(stopButton, &QPushButton::clicked, _homeTab, [this, valueSlider, valueSpinBox, miniDashboard]() {
        if (_serialPort && _serialPort->isOpen()) {
            valueSlider->blockSignals(true);
            miniDashboard->setValue(0);
            valueSlider->setValue(static_cast<int>(0));
            valueSpinBox->setValue(static_cast<int>(0));
            valueSlider->blockSignals(false);

            QString command = QString("stop\n");
            _serialPort->write(command.toUtf8());

            QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
            _logTextEdit->append(QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
            saveLogToCSV(QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
        }
    });

    connect(minValueSpinBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            miniDashboard,
            &DashBoardWidget::setMinValue);
    connect(maxValueSpinBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            miniDashboard,
            &DashBoardWidget::setMaxValue);

    // Kết nối UART
    connect(connectButton, &QPushButton::clicked, _homeTab, [this, portComboBox]() {
        _serialPort = new QSerialPort;
        _serialPort->setPortName(portComboBox->currentText());
        _serialPort->setBaudRate(QSerialPort::Baud115200);
        _serialPort->setDataBits(QSerialPort::Data8);
        _serialPort->setParity(QSerialPort::NoParity);
        _serialPort->setStopBits(QSerialPort::OneStop);

        if (_serialPort->open(QIODevice::ReadWrite))
        {
            QMessageBox::information(this, "Success", "UART Port Connected Successfully!");
            connect(_serialPort, &QSerialPort::readyRead, this, &MainWindow::readData);
        } else {
            QMessageBox::critical(this, "Error", "Unable to connect to UART port");
        }
    });

    connect(sendButton, &QPushButton::clicked, _homeTab, [this, commandEdit, logTextEdit]() {
        if (_serialPort && _serialPort->isOpen()) {
            QString command = commandEdit->text() + "\n";
            _serialPort->write(command.toUtf8());

            QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
            logTextEdit->append(QString("[%1] SENT: %2").arg(timestamp, command.trimmed()));

            qDebug() << "Command sent:" << command;
            commandEdit->clear();
        } else {
            QMessageBox::warning(this, "Warning", "UART port not connected");
        }
    });

    connect(rampModeButton, &QPushButton::clicked, _homeTab, [this, amplitudeSpinBox, durationSpinBox]() {
        if (_serialPort && _serialPort->isOpen()) {
            QString command = QString("ramp:%1:%2\n")
            .arg(amplitudeSpinBox->value(), 0, 'f', 1)
                .arg(durationSpinBox->value(), 0, 'f', 1);
            _serialPort->write(command.toUtf8());

            QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
            _logTextEdit->append(QString("[%1] MODE SET: %2").arg(timestamp, command.trimmed()));
            saveLogToCSV(QString("[%1] MODE SET: %2").arg(timestamp, command.trimmed()));
        }
    });

    connect(sineWaveButton, &QPushButton::clicked, _homeTab, [this, amplitudeSpinBox, frequencySpinBox, durationSpinBox]() {
        if (_serialPort && _serialPort->isOpen()) {
            QString command = QString("sine:%1:%2:%3\n")
            .arg(amplitudeSpinBox->value(), 0, 'f', 1)
                .arg(frequencySpinBox->value(), 0, 'f', 2)
                .arg(durationSpinBox->value(), 0, 'f', 1);
            _serialPort->write(command.toUtf8());

            QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
            _logTextEdit->append(QString("[%1] MODE SET: %2").arg(timestamp, command.trimmed()));
            saveLogToCSV(QString("[%1] MODE SET: %2").arg(timestamp, command.trimmed()));
        }
    });

    connect(commandEdit, &QLineEdit::returnPressed, _homeTab, [sendButton]() {
        sendButton->click();
    });

    connect(startPlotButton, &QPushButton::clicked, _homeTab, [this, pwmValueLabel, throttleValueLabel]() {
        _plotting = true;
        _throttleSeries->clear();
        _startTime = QTime::currentTime();
        pwmValueLabel->setText("PWM: 0");
        throttleValueLabel->setText("Thrust: 0");
        qDebug() << "Plotting started";
    });

    connect(stopPlotButton, &QPushButton::clicked, _homeTab, [this]() {
        _plotting = false;
        stopFlag = 1;
        qDebug() << "Plotting stopped";
    });

    connect(clearPlotButton, &QPushButton::clicked, _homeTab, [this, pwmValueLabel, throttleValueLabel]() {
        _throttleSeries->clear();
        _startTime = QTime::currentTime();
        pwmValueLabel->setText("PWM: 0");
        throttleValueLabel->setText("Thrust: 0");
        qDebug() << "Plot cleared";
    });

    connect(saveDataButton, &QPushButton::clicked, this, &MainWindow::saveDataToCSV);

    _pwmLabel = pwmValueLabel;
    _throttleLabel = throttleValueLabel;
    _logTextEdit = logTextEdit;

    miniDashboard->setValue(0);
}
