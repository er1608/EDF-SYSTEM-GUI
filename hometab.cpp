#include "mainwindow.h"
#include "dashboardwidget.h"
#include <QtCharts>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <QtWidgets>

void MainWindow::setupHomeTab()
{
    auto *homeLayout = new QVBoxLayout(_homeTab);
    homeLayout->setContentsMargins(10, 10, 10, 10);
    homeLayout->setSpacing(10);

    auto *miniDashboard = new DashBoardWidget(_homeTab);
    miniDashboard->setMinimumSize(230, 230);
    miniDashboard->setBorderColor(QColor(27, 36, 50));
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
    auto *saveDataButton = new QPushButton(tr("Save Data"), _homeTab);

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

    auto *mainChartView = createAnalyzeChart("Thrust", "Thrust (N)", _series1);
    auto *torqueChart = createAnalyzeChart("Torque", "Torque (Nm)", _series2);
    auto *voltChart = createAnalyzeChart("Voltage", "Voltage (V)", _series3);
    auto *currentChart = createAnalyzeChart("Current", "Current (A)", _series4);

    mainChartView->setMinimumHeight(250);
    voltChart->setMinimumHeight(250);
    currentChart->setMinimumHeight(250);
    torqueChart->setMinimumHeight(250);

    displayLayout->addWidget(mainChartView);
    displayLayout->addWidget(torqueChart);
    displayLayout->addWidget(voltChart);
    displayLayout->addWidget(currentChart);

    displayLayout->addStretch();

    auto *scrollArea = new QScrollArea();
    scrollArea->setWidget(scrollContentWidget);

    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

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
    commandLayout->addWidget(commandEdit, 0, 1, 1, 2);
    commandLayout->addWidget(startButton, 1, 0);
    commandLayout->addWidget(stopButton, 1, 1);
    commandLayout->addWidget(sendButton, 1, 2);

    auto *dataGroup = new QGroupBox(tr("Data Signal"), _homeTab);
    auto *dataLayout = new QGridLayout(dataGroup);

    auto *thrustValueLabel = new QLabel(tr("Thrust: 0 N"), _homeTab);
    auto *pwmValueLabel = new QLabel(tr("PWM: 0"), _homeTab);
    auto *torqueValueLabel = new QLabel(tr("Torque: 0 Nm"), _homeTab);
    auto *voltageValueLabel = new QLabel(tr("Voltage: 0 V"), _homeTab);
    auto *currentValueLabel = new QLabel(tr("Current: 0 A"), _homeTab);
    auto *temperatureValueLabel = new QLabel(tr("Temperature: 0 °C"), _homeTab);
    auto *RPMValueLabel = new QLabel(tr("RPM: 0"), _homeTab);

    thrustValueLabel->setStyleSheet("font-weight: bold; font-size: 13px; padding: 6px; border-radius: 3px;");
    pwmValueLabel->setStyleSheet("font-weight: bold; font-size: 13px; padding: 6px; border-radius: 3px;");
    torqueValueLabel->setStyleSheet("font-weight: bold; font-size: 13px; padding: 6px; border-radius: 3px;");
    voltageValueLabel->setStyleSheet("font-weight: bold; font-size: 13px; padding: 6px; border-radius: 3px;");
    currentValueLabel->setStyleSheet("font-weight: bold; font-size: 13px; padding: 6px; border-radius: 3px;");
    temperatureValueLabel->setStyleSheet("font-weight: bold; font-size: 13px; padding: 6px; border-radius: 3px;");
    RPMValueLabel->setStyleSheet("font-weight: bold; font-size: 13px; padding: 6px; border-radius: 3px;");

    dataLayout->addWidget(thrustValueLabel, 0, 0);
    dataLayout->addWidget(currentValueLabel, 0, 1);
    dataLayout->addWidget(torqueValueLabel, 0, 2);
    dataLayout->addWidget(voltageValueLabel, 1, 0);
    dataLayout->addWidget(pwmValueLabel, 1, 1);
    dataLayout->addWidget(RPMValueLabel, 1, 2);
    dataLayout->addWidget(temperatureValueLabel, 2, 0);

    auto *dashboardGroup = new QGroupBox(tr("Motor Power"), _homeTab);
    auto *dashboardLayout = new QVBoxLayout(dashboardGroup);
    dashboardLayout->addWidget(miniDashboard);
    dashboardLayout->setAlignment(miniDashboard, Qt::AlignTop);

    auto *valueGroup = new QGroupBox(tr("Control"), _homeTab);
    auto *valueControlLayout = new QGridLayout(valueGroup);
    valueControlLayout->addWidget(new QLabel(tr("Throttle:")), 0, 0);
    valueControlLayout->addWidget(valueSlider, 0, 1, 1, 2);
    valueControlLayout->addWidget(valueSpinBox, 0, 3);
    valueControlLayout->addWidget(minValueLabel, 1, 0);
    valueControlLayout->addWidget(minValueSpinBox, 1, 1, 1, 3);
    valueControlLayout->addWidget(maxValueLabel, 2, 0);
    valueControlLayout->addWidget(maxValueSpinBox, 2, 1, 1, 3);
    valueControlLayout->addWidget(startPlotButton, 3, 0);
    valueControlLayout->addWidget(stopPlotButton, 3, 1);
    valueControlLayout->addWidget(clearPlotButton, 3, 2);
    valueControlLayout->addWidget(saveDataButton, 3, 3);

    valueControlLayout->setColumnStretch(1, 1);
    valueControlLayout->setColumnStretch(2, 1);

    // UART Log Display
    auto *logGroup = new QGroupBox(tr("Command Log"), _homeTab);
    auto *logLayout = new QVBoxLayout(logGroup);
    logLayout->addWidget(logTextEdit);

    leftLayout->addWidget(dashboardGroup);
    leftLayout->addWidget(valueGroup);
    leftLayout->addWidget(connectionGroup);
    leftLayout->addWidget(commandGroup);
    leftLayout->addWidget(dataGroup);
    leftLayout->addWidget(logGroup);
    leftLayout->addStretch();

    topContentLayout->addWidget(leftPanel, 1);
    topContentLayout->addWidget(scrollArea, 3);

    homeLayout->addLayout(topContentLayout, 1);

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

    connect(commandEdit, &QLineEdit::returnPressed, _homeTab, [sendButton]() {
        sendButton->click();
    });

    connect(startPlotButton, &QPushButton::clicked, _homeTab, [this]() {
        _plotting = true;

        _series1->clear();
        _series2->clear();
        _series3->clear();
        _series4->clear();

        _startTime = QTime::currentTime();

        reset();

        qDebug() << "Plotting started";
    });

    connect(stopPlotButton, &QPushButton::clicked, _homeTab, [this]() {
        _plotting = false;
        stopFlag = 1;
        qDebug() << "Plotting stopped";
    });

    connect(clearPlotButton, &QPushButton::clicked, _homeTab, [this, saveDataButton]() {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Warning");
        msgBox.setText("Would you like to save your Data before Clearing the plot?");
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

        QFont font;
        font.setPointSize(12);
        msgBox.setFont(font);

        msgBox.button(QMessageBox::Yes)->setFont(font);
        msgBox.button(QMessageBox::No)->setFont(font);

        if (msgBox.exec() == QMessageBox::Yes) {
            Q_EMIT saveDataButton->clicked();

            _series1->clear();
            _series2->clear();
            _series3->clear();
            _series4->clear();

            _startTime = QTime::currentTime();

            reset();

            qDebug() << "Plot cleared";
        }
        else {
            _series1->clear();
            _series2->clear();
            _series3->clear();
            _series4->clear();

            _startTime = QTime::currentTime();

            reset();

            qDebug() << "Plot cleared";
        }

        _timeReset = true;
    });

    connect(saveDataButton, &QPushButton::clicked, this, &MainWindow::saveDataToCSV);

    _thrustLabel = thrustValueLabel;
    _pwmLabel = pwmValueLabel;
    _torqueLabel = torqueValueLabel;
    _voltageLabel = voltageValueLabel;
    _currentLabel = currentValueLabel;
    _temperatureLabel = temperatureValueLabel;
    _RPMLabel = RPMValueLabel;

    _logTextEdit = logTextEdit;

    miniDashboard->setValue(0);
}
