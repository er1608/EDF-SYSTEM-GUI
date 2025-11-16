#include "mainwindow.h"
#include "dashboardwidget.h"
#include <QtCharts>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _serialPort(nullptr)
    , _plotting(false)
    , _chart(nullptr)
    , _throttleSeries(nullptr)
    , _startTime(QTime::currentTime())
{
    setStyleSheet(R"(
        QMainWindow {
            background: #0d1117;      /* Space black */
            color: #e2e8f0;
            font-family: "Segoe UI", "Roboto", sans-serif;
        }

        QWidget {
            background: transparent;
            color: #e2e8f0;
        }

        QChartView {
            background: #1b2432;
            border: none;
            border-radius: 10px;
            padding: 6px;
            box-shadow: 0px 2px 8px rgba(0,0,0,0.45);
        }

        QGroupBox {
            background: #1b2432;
            color: #e2e8f0;
            border: 1px solid #2d3250;
            border-radius: 10px;
            margin-top: 1.4ex;
            padding-top: 14px;
            box-shadow: 0px 2px 6px rgba(0,0,0,0.45);
            font-size: 12px;
            font-weight: 500;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top center;
            padding: 6px 18px;
            background: qlineargradient(
                x1:0, y1:0, x2:1, y2:0,
                stop:0 #2d3250,
                stop:1 #3a3f66
            );
            color: #a6b1e1;
            border-radius: 6px;
            font-weight: 600;
        }

        QPushButton {
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #3a3f66,
                stop:1 #2d3250
            );
            color: #e2e8f0;
            border: 1px solid #2d3250;
            padding: 8px 16px;
            border-radius: 6px;
            font-weight: 600;
            font-size: 11px;
            transition: 120ms;
        }

        QPushButton:hover {
            background: #2d3b5a;         /* Galaxy highlight */
            color: white;
        }

        QPushButton:pressed {
            background: #4f3bb8;
            border: 1px solid #7f5af0;
        }

        QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox {
            background: #121722;       /* deeper black-blue */
            color: #e2e8f0;
            border: 1px solid #2d3250;
            border-radius: 5px;
            padding: 6px 8px;
            font-size: 11px;
            selection-background-color: #7f5af0;
        }

        QLineEdit:focus,
        QComboBox:focus,
        QSpinBox:focus,
        QDoubleSpinBox:focus {
            border: 1px solid #7f5af0;
            box-shadow: 0 0 6px rgba(127,90,240,0.45);
        }

        QSlider::groove:horizontal {
            border: 1px solid #2d3250;
            height: 6px;
            background: #1d2533;
            border-radius: 4px;
        }

        QSlider::handle:horizontal {
            background: #00b4d8;
            border: 1px solid #00b4d8;
            width: 16px;
            margin: -5px 0;
            border-radius: 8px;
        }

        QLabel {
            color: #a6b1e1;      /* gentle fog-blue */
            font-size: 11px;
            font-weight: 500;
        }

        QTextEdit {
            background: #121722;
            color: #e2e8f0;
            border: 1px solid #2d3250;
            border-radius: 6px;
            font-size: 11px;
            font-family: "Consolas", "Roboto Mono", monospace;
            padding: 6px;
        }

        QChartView {
            background: #1b2432;
            border: 1px solid #2d3250;
            border-radius: 10px;
            padding: 6px;
            box-shadow: 0px 2px 8px rgba(0,0,0,0.45);
        }
    )");

    // Tạo dashboard widget nhỏ cho bên trái
    auto *miniDashboard = new DashBoardWidget(this);
    miniDashboard->setFixedSize(260, 260);
    miniDashboard->setBorderColor(QColor(45, 50, 80));
    miniDashboard->setBorderWidth(2);
    miniDashboard->setBorderRadius(10);
    miniDashboard->setArcColor(QColor(65, 90, 119, 180));
    miniDashboard->setScaleColor(QColor(166, 177, 225));
    miniDashboard->setPointerColor(QColor(255, 107, 107));
    miniDashboard->setValueColor(QColor(226, 232, 240));
    miniDashboard->setTitleColor(QColor(166, 177, 225));
    miniDashboard->setBackgroundColor(QColor(13, 17, 23, 100));

    // Tạo biểu đồ chính cho throttle
    auto *mainChartView = setupMainChart();

    // Tạo các control widgets
    auto *valueSlider = new QSlider(Qt::Horizontal, this);
    valueSlider->setRange(0, 100);
    valueSlider->setValue(0);

    auto *valueSpinBox = new QDoubleSpinBox(this);
    valueSpinBox->setRange(0, 100);
    valueSpinBox->setValue(0);
    valueSpinBox->setDecimals(2);

    auto *minValueLabel = new QLabel(tr("Min Value:"), this);
    auto *minValueSpinBox = new QDoubleSpinBox(this);
    minValueSpinBox->setRange(-1000, 1000);
    minValueSpinBox->setValue(0);
    minValueSpinBox->setDecimals(2);

    auto *maxValueLabel = new QLabel(tr("Max Value:"), this);
    auto *maxValueSpinBox = new QDoubleSpinBox(this);
    maxValueSpinBox->setRange(-1000, 1000);
    maxValueSpinBox->setValue(100);
    maxValueSpinBox->setDecimals(2);

    auto *clearPlotButton = new QPushButton(tr("Clear Plot"), this);
    auto *startPlotButton = new QPushButton(tr("Start Plot"), this);
    auto *stopPlotButton = new QPushButton(tr("Stop Plot"), this);
    auto *saveDataButton = new QPushButton(tr("Save Data to CSV"), this);

    // Thêm các widget hiển thị giá trị PWM và Throttle với dark theme
    auto *pwmValueLabel = new QLabel(tr("PWM: 0"), this);
    auto *throttleValueLabel = new QLabel(tr("Throttle: 0"), this);
    pwmValueLabel->setStyleSheet("font-weight: bold; color: #66ccff; font-size: 13px; padding: 6px; background: #1b2432; border: 1px solid #2d3250; border-radius: 4px;");
    throttleValueLabel->setStyleSheet("font-weight: bold; color: #ff6666; font-size: 13px; padding: 6px; background: #1b2432; border: 1px solid #2d3250; border-radius: 4px;");

    // Tạo log display cho các lệnh UART đã gửi - đặt ở dưới cùng
    auto *logTextEdit = new QTextEdit(this);
    logTextEdit->setMaximumHeight(100);
    logTextEdit->setReadOnly(true);

    auto *mainWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(mainWidget); // Chuyển sang QVBoxLayout chính

    // Layout trên cùng chứa nội dung chính
    auto *topContentLayout = new QHBoxLayout();

    // Layout bên trái cho mini dashboard
    auto *leftPanel = new QWidget(this);
    auto *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setAlignment(Qt::AlignTop);

    // Layout control panel bên phải
    auto *controlPanel = new QWidget(this);
    auto *controlLayout = new QVBoxLayout(controlPanel);
    controlLayout->setAlignment(Qt::AlignTop);

    // Layout chính cho hiển thị biểu đồ
    auto *displayLayout = new QVBoxLayout();

    // Biểu đồ chính
    displayLayout->addWidget(mainChartView);

    // Layout cho các control dưới biểu đồ
    auto *bottomControlsLayout = new QHBoxLayout();

    auto *valueGroup = new QGroupBox(tr("Value Settings"), this);
    auto *valueControlLayout = new QGridLayout(valueGroup);
    valueControlLayout->addWidget(new QLabel(tr("Current Value:")), 0, 0);
    valueControlLayout->addWidget(valueSlider, 0, 1);
    valueControlLayout->addWidget(valueSpinBox, 0, 2);
    valueControlLayout->addWidget(minValueLabel, 1, 0);
    valueControlLayout->addWidget(minValueSpinBox, 1, 1, 1, 2);
    valueControlLayout->addWidget(maxValueLabel, 2, 0);
    valueControlLayout->addWidget(maxValueSpinBox, 2, 1, 1, 2);

    auto *plotControlGroup = new QGroupBox(tr("Plot Control"), this);
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
    auto *connectionGroup = new QGroupBox(tr("UART Connection"), this);
    auto *connectionLayout = new QGridLayout(connectionGroup);
    auto *portComboBox = new QComboBox(this);
    auto *connectButton = new QPushButton(tr("Connect"), this);

    foreach (auto &port, QSerialPortInfo::availablePorts()) {
        portComboBox->addItem(port.portName());
    }

    portComboBox->setStyleSheet(R"(
        QComboBox {
            background: #121722 !important;
            color: #ffffff !important;
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
            border-top: 6px solid #ffffff;
        }
        QComboBox QAbstractItemView {
            background: #121722;
            color: #ffffff;
            border: 1px solid #2d3250;
            selection-background-color: #2d3250;
        }
    )");

    connectionLayout->addWidget(new QLabel(tr("Port:")), 0, 0);
    connectionLayout->addWidget(portComboBox, 0, 1);
    connectionLayout->addWidget(connectButton, 0, 2);

    // UART Command Sending
    auto *commandGroup = new QGroupBox(tr("UART Command"), this);
    auto *commandLayout = new QGridLayout(commandGroup);
    auto *commandEdit = new QLineEdit(this);
    auto *sendButton = new QPushButton(tr("Send Command"), this);

    commandEdit->setPlaceholderText("Enter command to send via UART...");
    commandLayout->addWidget(new QLabel(tr("Command:")), 0, 0);
    commandLayout->addWidget(commandEdit, 0, 1);
    commandLayout->addWidget(sendButton, 0, 2);

    auto *modeGroup = new QGroupBox(tr("Signal Generator Mode"), this);
    auto *modeLayout = new QGridLayout(modeGroup);

    auto *rampModeButton = new QPushButton(tr("Ramp Mode"), this);
    auto *sineWaveButton = new QPushButton(tr("Sine Wave Mode"), this);

    // Thêm các thông số cho từng mode
    auto *amplitudeLabel = new QLabel(tr("Amplitude:"), this);
    auto *amplitudeSpinBox = new QDoubleSpinBox(this);
    amplitudeSpinBox->setRange(0, 100);
    amplitudeSpinBox->setValue(50);
    amplitudeSpinBox->setDecimals(1);

    auto *frequencyLabel = new QLabel(tr("Frequency (Hz):"), this);
    auto *frequencySpinBox = new QDoubleSpinBox(this);
    frequencySpinBox->setRange(0.1, 10.0);
    frequencySpinBox->setValue(1.0);
    frequencySpinBox->setDecimals(2);

    auto *durationLabel = new QLabel(tr("Duration (s):"), this);
    auto *durationSpinBox = new QDoubleSpinBox(this);
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

    // Thêm mini dashboard vào panel bên trái
    auto *dashboardGroup = new QGroupBox(tr("System Dashboard"), this);
    auto *dashboardLayout = new QVBoxLayout(dashboardGroup);
    dashboardLayout->addWidget(miniDashboard);
    dashboardLayout->setAlignment(miniDashboard, Qt::AlignCenter);

    // Thêm các thành phần vào panel trái
    leftLayout->addWidget(dashboardGroup);
    leftLayout->addWidget(connectionGroup);
    leftLayout->addWidget(commandGroup);
    leftLayout->addWidget(modeGroup);

    // Thêm các thành phần vào panel phải
    controlLayout->addWidget(valueGroup);
    controlLayout->addWidget(plotControlGroup);

    // Sắp xếp layout trên cùng
    topContentLayout->addWidget(leftPanel, 1);
    topContentLayout->addLayout(displayLayout, 2);

    // UART Log Display - ĐẶT Ở DƯỚI CÙNG
    auto *logGroup = new QGroupBox(tr("Command Log"), this);
    auto *logLayout = new QVBoxLayout(logGroup);
    logLayout->addWidget(logTextEdit);

    // Sắp xếp layout chính: trên cùng là content, dưới cùng là log
    mainLayout->addLayout(topContentLayout, 4);
    mainLayout->addWidget(logGroup, 1);

    setCentralWidget(mainWidget);
    resize(1200, 900);
    setWindowTitle(tr("EDF SYSTEM"));

    // [Phần kết nối signals/slots giữ nguyên...]
    // Kết nối tín hiệu cho dashboard nhỏ
    connect(valueSlider, &QSlider::valueChanged, this, [miniDashboard, valueSpinBox](int value) {
        valueSpinBox->blockSignals(true);
        miniDashboard->setValue(value);
        valueSpinBox->setValue(value);
        valueSpinBox->blockSignals(false);
    });

    connect(valueSpinBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this,
            [miniDashboard, valueSlider](double value) {
                valueSlider->blockSignals(true);
                miniDashboard->setValue(value);
                valueSlider->setValue(static_cast<int>(value));
                valueSlider->blockSignals(false);
            });

    // Kết nối valueSlider để tự động gửi UART khi giá trị thay đổi
    connect(valueSlider, &QSlider::valueChanged, this, [this](int value) {
        if (_serialPort && _serialPort->isOpen()) {
            QString command = QString("s:%1\n").arg(value);
            _serialPort->write(command.toUtf8());

            // Thêm vào log
            QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
            _logTextEdit->append(QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
        }
    });

    // Kết nối valueSpinBox cũng tương tự
    connect(valueSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this](double value) {
        if (_serialPort && _serialPort->isOpen()) {
            QString command = QString("s:%1\n").arg(value, 0, 'f', 2);
            _serialPort->write(command.toUtf8());

            QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
            _logTextEdit->append(QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
        }
    });

    // Kết nối các control với dashboard nhỏ
    connect(minValueSpinBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            miniDashboard,
            &DashBoardWidget::setMinValue);
    connect(maxValueSpinBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            miniDashboard,
            &DashBoardWidget::setMaxValue);

    // Kết nối UART
    connect(connectButton, &QPushButton::clicked, this, [this, portComboBox]() {
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

    // Kết nối gửi command UART
    connect(sendButton, &QPushButton::clicked, this, [this, commandEdit, logTextEdit]() {
        if (_serialPort && _serialPort->isOpen()) {
            QString command = commandEdit->text() + "\n";
            _serialPort->write(command.toUtf8());

            // Thêm vào log
            QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
            logTextEdit->append(QString("[%1] SENT: %2").arg(timestamp, command.trimmed()));

            qDebug() << "Command sent:" << command;
            commandEdit->clear();
        } else {
            QMessageBox::warning(this, "Warning", "UART port not connected");
        }
    });

    connect(rampModeButton, &QPushButton::clicked, this, [this, amplitudeSpinBox, durationSpinBox]() {
        if (_serialPort && _serialPort->isOpen()) {
            QString command = QString("ramp:%1:%2\n")
            .arg(amplitudeSpinBox->value(), 0, 'f', 1)
                .arg(durationSpinBox->value(), 0, 'f', 1);
            _serialPort->write(command.toUtf8());

            QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
            _logTextEdit->append(QString("[%1] MODE SET: %2").arg(timestamp, command.trimmed()));
        }
    });

    connect(sineWaveButton, &QPushButton::clicked, this, [this, amplitudeSpinBox, frequencySpinBox, durationSpinBox]() {
        if (_serialPort && _serialPort->isOpen()) {
            QString command = QString("sine:%1:%2:%3\n")
            .arg(amplitudeSpinBox->value(), 0, 'f', 1)
                .arg(frequencySpinBox->value(), 0, 'f', 2)
                .arg(durationSpinBox->value(), 0, 'f', 1);
            _serialPort->write(command.toUtf8());

            QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
            _logTextEdit->append(QString("[%1] MODE SET: %2").arg(timestamp, command.trimmed()));
        }
    });

    // Kết nối Enter key để gửi command
    connect(commandEdit, &QLineEdit::returnPressed, this, [sendButton]() {
        sendButton->click();
    });

    // Kết nối control biểu đồ
    connect(startPlotButton, &QPushButton::clicked, this, [this]() {
        _plotting = true;
        _startTime = QTime::currentTime();
        qDebug() << "Plotting started";
    });

    connect(stopPlotButton, &QPushButton::clicked, this, [this]() {
        _plotting = false;
        qDebug() << "Plotting stopped";
    });

    connect(clearPlotButton, &QPushButton::clicked, this, [this, pwmValueLabel, throttleValueLabel]() {
        _throttleSeries->clear();
        _startTime = QTime::currentTime();
        pwmValueLabel->setText("PWM: 0");
        throttleValueLabel->setText("Throttle: 0");
        qDebug() << "Plot cleared";
    });

    // Kết nối save data
    connect(saveDataButton, &QPushButton::clicked, this, &MainWindow::saveDataToCSV);

    // Lưu con trỏ đến các label để cập nhật giá trị
    _pwmLabel = pwmValueLabel;
    _throttleLabel = throttleValueLabel;
    _logTextEdit = logTextEdit;

    miniDashboard->setValue(0);
}

QChartView* MainWindow::setupMainChart()
{
    _chart = new QChart();
    _throttleSeries = new QLineSeries();

    // Thiết lập màu sắc cho series throttle với dark theme
    _throttleSeries->setName("Throttle");
    QPen pen(QColor(65, 90, 119), 3);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    _throttleSeries->setPen(pen);

    _chart->addSeries(_throttleSeries);
    _chart->setTitle("Throttle Real-time Monitor");
    _chart->setTitleBrush(QBrush(QColor(226, 232, 240))); // Light text
    _chart->legend()->setVisible(true);
    _chart->legend()->setAlignment(Qt::AlignBottom);
    _chart->setBackgroundBrush(QBrush(QColor(27, 36, 50))); // Graphite blue
    _chart->setTheme(QChart::ChartThemeDark);

    // Tạo axes
    auto *axisX = new QValueAxis();
    auto *axisY = new QValueAxis();

    axisX->setTitleText("Time (s)");
    axisY->setTitleText("Throttle Value");
    axisX->setRange(0, 60);
    axisY->setRange(0, 100);

    // Thiết lập màu cho axes
    axisX->setLabelsColor(QColor(166, 177, 225)); // Fog blue
    axisY->setLabelsColor(QColor(166, 177, 225));
    axisX->setTitleBrush(QBrush(QColor(226, 232, 240))); // Light text
    axisY->setTitleBrush(QBrush(QColor(226, 232, 240)));
    axisX->setGridLineColor(QColor(45, 50, 80, 100)); // Deep steel
    axisY->setGridLineColor(QColor(45, 50, 80, 100));

    _chart->addAxis(axisX, Qt::AlignBottom);
    _chart->addAxis(axisY, Qt::AlignLeft);

    _throttleSeries->attachAxis(axisX);
    _throttleSeries->attachAxis(axisY);

    auto *chartView = new QChartView(_chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}

// [Các hàm updatePlot, saveDataToCSV, readData giữ nguyên...]
void MainWindow::updatePlot(double throttle, double pwm)
{
    // Tính thời gian từ khi bắt đầu plot
    double key = _startTime.msecsTo(QTime::currentTime()) / 1000.0;

    _throttleSeries->append(key, throttle);

    // Giới hạn số điểm hiển thị
    if (_throttleSeries->count() > 1000) {
        _throttleSeries->remove(0);
    }

    // Cập nhật hiển thị giá trị
    _throttleLabel->setText(QString("Throttle: %1").arg(throttle, 0, 'f', 1));
    _pwmLabel->setText(QString("PWM: %1").arg(pwm, 0, 'f', 1));

    // Lưu dữ liệu vào buffer để ghi CSV
    _dataBuffer.append(DataPoint{key, throttle, pwm});

    // Tự động scroll trục X
    auto axes = _chart->axes();
    if (axes.size() >= 2) {
        auto *axisX = static_cast<QValueAxis*>(axes[0]);
        if (key > axisX->max()) {
            axisX->setRange(key - 60, key);
        }
    }
}

void MainWindow::saveDataToCSV()
{
    QString fileName = QDateTime::currentDateTime().toString("'system_data_'yyyy-MM-dd_hh-mm-ss'.csv'");
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        // Write header
        stream << "Timestamp,Throttle,PWM\n";

        // Write data
        for (const auto &data : _dataBuffer) {
            stream << data.timestamp << "," << data.throttle << "," << data.pwm << "\n";
        }

        file.close();

        // Thêm vào log
        QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
        _logTextEdit->append(QString("[%1] DATA SAVED: %2 (%3 records)").arg(timestamp, fileName, QString::number(_dataBuffer.size())));

        QMessageBox::information(this, "Success", QString("Data saved to %1\n%2 records written").arg(fileName, QString::number(_dataBuffer.size())));
    } else {
        QMessageBox::critical(this, "Error", "Could not save data to file");
    }
}

void MainWindow::readData()
{
    if (!_serialPort)
        return;

    const QByteArray payload = _serialPort->readAll();
    QString data = QString::fromUtf8(payload).trimmed();

    // Thêm vào log
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    _logTextEdit->append(QString("[%1] RECV: %2").arg(timestamp, data));

    qDebug() << "UART data:" << data;

    // Phân tích dữ liệu UART
    bool throttleOk = false, pwmOk = false;
    double throttleValue = 0.0, pwmValue = 0.0;

    if (data.contains("THROTTLE") && data.contains("PWM")) {
        QStringList parts = data.split(',');
        for (const QString &part : parts) {
            if (part.contains("THROTTLE:")) {
                QString valueStr = part.split(':')[1];
                throttleValue = valueStr.toDouble(&throttleOk);
            } else if (part.contains("PWM:")) {
                QString valueStr = part.split(':')[1];
                pwmValue = valueStr.toDouble(&pwmOk);
            }
        }
    } else {
        // Nếu không có định dạng rõ ràng, thử parse trực tiếp
        throttleValue = data.toDouble(&throttleOk);
        pwmValue = throttleValue; // Giả sử cùng giá trị nếu chỉ có 1 số
        pwmOk = throttleOk;
    }

    if (throttleOk && pwmOk) {
        qDebug() << "Parsed - Throttle:" << throttleValue << "PWM:" << pwmValue;

        // Cập nhật biểu đồ nếu đang plot
        if (_plotting) {
            updatePlot(throttleValue, pwmValue);
        }
    }
}

MainWindow::~MainWindow()
{
    if (_serialPort != nullptr)
    {
        _serialPort->close();
        delete _serialPort;
    }
}
