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
    miniDashboard->setMinimumSize(200, 200);
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

    auto *clearPlotButton = new QPushButton(tr("Clear Plot"), _homeTab);
    auto *startPlotButton = new QPushButton(tr("Start Plot"), _homeTab);
    auto *stopPlotButton = new QPushButton(tr("Stop Plot"), _homeTab);
    auto *saveDataButton = new QPushButton(tr("Save Data"), _homeTab);

    auto *logTextEdit = new QTextEdit(_homeTab);
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
    displayLayout->setContentsMargins(10,10,10,10);

    _series1 = new QLineSeries();
    _series2 = new QLineSeries();
    _series3 = new QLineSeries();
    _series4 = new QLineSeries();

    _thrustChart  = createAnalyzeChart("Thrust",  "Thrust (N)",  _series1);
    _torqueChart  = createAnalyzeChart("Torque",  "Torque (Nm)", _series2);
    _voltChart    = createAnalyzeChart("Voltage", "Voltage (V)", _series3);
    _currentChart = createAnalyzeChart("Current", "Current (A)", _series4);

    _thrustChart->setMinimumHeight(250);
    _torqueChart->setMinimumHeight(250);
    _voltChart->setMinimumHeight(250);
    _currentChart->setMinimumHeight(250);

    displayLayout->addWidget(_thrustChart);
    displayLayout->addWidget(_torqueChart);
    displayLayout->addWidget(_voltChart);
    displayLayout->addWidget(_currentChart);

    _thrustChart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _torqueChart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _voltChart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _currentChart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    displayLayout->addWidget(_thrustChart, 1);
    displayLayout->addWidget(_torqueChart, 1);
    displayLayout->addWidget(_voltChart, 1);
    displayLayout->addWidget(_currentChart, 1);

    QToolButton *chartSelector = new QToolButton();
    chartSelector->setText("Charts");
    chartSelector->setPopupMode(QToolButton::InstantPopup);

    QMenu *menu = new QMenu(chartSelector);

    QAction *thrustAct  = menu->addAction("Thrust");
    QAction *torqueAct  = menu->addAction("Torque");
    QAction *voltAct    = menu->addAction("Voltage");
    QAction *currentAct = menu->addAction("Current");

    thrustAct->setCheckable(true);
    torqueAct->setCheckable(true);
    voltAct->setCheckable(true);
    currentAct->setCheckable(true);

    thrustAct->setChecked(true);
    torqueAct->setChecked(true);
    voltAct->setChecked(true);
    currentAct->setChecked(true);

    chartSelector->setMenu(menu);

    connect(thrustAct, &QAction::toggled, this, [=](bool checked){
        _thrustChart->setVisible(checked);
    });

    connect(torqueAct, &QAction::toggled, this, [=](bool checked){
        _torqueChart->setVisible(checked);
    });

    connect(voltAct, &QAction::toggled, this, [=](bool checked){
        _voltChart->setVisible(checked);
    });

    connect(currentAct, &QAction::toggled, this, [=](bool checked){
        _currentChart->setVisible(checked);
    });

    auto *topLayout = new QHBoxLayout();
    topLayout->addStretch();
    topLayout->addWidget(chartSelector);

    displayLayout->insertLayout(0, topLayout);

    auto *scrollArea = new QScrollArea();
    scrollArea->setWidget(scrollContentWidget);

    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    // UART Connection
    auto *connectionGroup = new QGroupBox(tr("Connection"), _homeTab);
    auto *connectionLayout = new QGridLayout(connectionGroup);
    auto *connectButton = new QPushButton(tr("Connect"), _homeTab);

    portComboBox = new QComboBox(this);

    refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &MainWindow::refreshSerialPorts);
    refreshTimer->start(1000);

    refreshSerialPorts();

    connectionLayout->addWidget(new QLabel(tr("Port:")), 0, 0);
    connectionLayout->addWidget(portComboBox, 0, 1);
    connectionLayout->addWidget(connectButton, 0, 2);

    // UART Command Sending
    auto *commandGroup = new QGroupBox(tr("Command"), _homeTab);
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
    startButton->setStyleSheet("#startButton { color: green; font-weight: bold; }");
    stopButton->setStyleSheet("#stopButton { color: red; font-weight: bold; }");

    commandEdit->setPlaceholderText("Enter command to send");
    commandLayout->addWidget(new QLabel(tr("Command:")), 0, 0);
    commandLayout->addWidget(commandEdit, 0, 1, 1, 2);
    commandLayout->addWidget(sendButton, 0, 3);
    commandLayout->addWidget(rampModeButton, 1, 0);
    commandLayout->addWidget(sineModeButton, 1, 1);
    commandLayout->addWidget(startButton, 1, 2);
    commandLayout->addWidget(stopButton, 1, 3);

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
            [miniDashboard, valueSlider](int value) {
                valueSlider->blockSignals(true);
                miniDashboard->setValue(value);
                valueSlider->setValue(value);
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
                if (value > 50) value = maxValueSpinBox->value() < 50 ? maxValueSpinBox->value() : 50;
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
                if (value > 50) value = maxValueSpinBox->value() < 50 ? maxValueSpinBox->value() : 50;
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
            _autoTimer->stop();
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

    connect(rampModeButton, &QPushButton::clicked, this,[this, valueSlider]() {
        if (!_serialPort || !_serialPort->isOpen())
            return;

        if (!_autoTimer) {
            _autoTimer = new QTimer(this);

            connect(_autoTimer, &QTimer::timeout, this,[this, valueSlider]() {
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

    connect(minValueSpinBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            miniDashboard,
            &DashBoardWidget::setMinValue);
    connect(maxValueSpinBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            miniDashboard,
            &DashBoardWidget::setMaxValue);

    // Kết nối UART
    connect(connectButton, &QPushButton::clicked, _homeTab, [this]() {
        _serialPort = new QSerialPort;
        _serialPort->setPortName(this->portComboBox->currentText());
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
