#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QtWidgets>

QVector<LCConfig> _lcConfigs;

void MainWindow::setupSettingTab()
{
    auto *settingLayout = new QVBoxLayout(_settingTab);
    settingLayout->setContentsMargins(10,10,10,10);
    settingLayout->setSpacing(10);

    auto *settingTabs = new QTabWidget(_settingTab);
    settingTabs->setTabPosition(QTabWidget::South);
    settingTabs->setFont(QFont("Arial", 9,QFont::Bold));

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
    settingTabs->addTab(loadcellTab,"Sensor");

    auto *pwmTab = new QWidget();
    setupPWMTab(pwmTab);
    settingTabs->addTab(pwmTab,"PWM");

    auto *mavlinkTab = new QWidget();
    setupMavlinkTab(mavlinkTab);
    settingTabs->addTab(mavlinkTab,"MAVL");

    auto *fftTab = new QWidget();
    setupFFTTab(fftTab);
    settingTabs->addTab(fftTab,"FFT");

    auto *signalTab = new QWidget();
    setupSignalGeneratorTab(signalTab);
    settingTabs->addTab(signalTab,"MODE");

    auto *configBtn = new QPushButton("Config");
    configBtn->setCheckable(true);

    configBtn->setText("Load");

    settingTabs->setCornerWidget(configBtn, Qt::TopRightCorner);
    configBtn->setFixedHeight(30);

    connect(configBtn, &QPushButton::clicked, this, [=](){

        if(_serialPort && _serialPort->isOpen())
        {
            QString cmd = "frconf:1\n";
            _serialPort->write(cmd.toUtf8());
            _logTextEdit->append(cmd);
        }

    });

    settingLayout->addWidget(settingTabs);
}

void MainWindow::setupSignalGeneratorTab(QWidget *tab)
{
    auto *layout = new QVBoxLayout(tab);

    auto *group = new QGroupBox("Signal Generator Mode");
    group->setFixedWidth(400);
    auto *form = new QFormLayout(group);

    auto *amplitude = new QDoubleSpinBox();
    amplitude->setRange(0,100);
    amplitude->setValue(50);
    amplitude->setDecimals(1);

    auto *frequency = new QDoubleSpinBox();
    frequency->setRange(0.1,10);
    frequency->setValue(1.0);
    frequency->setDecimals(2);

    auto *duration = new QDoubleSpinBox();
    duration->setRange(1,60);
    duration->setValue(10);

    auto *btnLayout = new QHBoxLayout();

    auto *applyBtn = new QPushButton("Apply");

    btnLayout->addWidget(applyBtn);

    form->addRow("Amplitude",amplitude);
    form->addRow("Frequency (Hz)",frequency);
    form->addRow("Duration (s)",duration);
    form->addRow(btnLayout);

    layout->addWidget(group, 0, Qt::AlignTop | Qt::AlignHCenter);

    connect(applyBtn, &QPushButton::clicked, this, [=](){

        if(_serialPort && _serialPort->isOpen())
        {
            QString cmd = QString("mode:%1:%2:%3\n")
                .arg(amplitude->value(),0,'f',1)
                .arg(frequency->value(),0,'f',2)
                .arg(duration->value(),0,'f',1);

            _serialPort->write(cmd.toUtf8());

            QString ts = QDateTime::currentDateTime().toString("hh:mm:ss");
            _logTextEdit->append(QString("[%1] MODE SET: %2").arg(ts,cmd.trimmed()));
            saveLogToCSV(QString("[%1] MODE SET: %2").arg(ts,cmd.trimmed()));
        }

    });
}

void MainWindow::setupPWMTab(QWidget *tab)
{
    auto *layout = new QVBoxLayout(tab);

    auto *group = new QGroupBox("PWM Output Configuration");
    group->setFixedWidth(400);
    auto *form = new QFormLayout(group);

    auto *freq = new QComboBox();
    freq->addItem("50 Hz", 50);
    freq->addItem("400 Hz", 400);

    auto *minPWM = new QSpinBox();
    minPWM->setRange(800,1100);
    minPWM->setValue(1050);

    auto *maxPWM = new QSpinBox();
    maxPWM->setRange(1600,2200);
    maxPWM->setValue(1950);

    auto *applyBtn = new QPushButton("Apply", group);

    form->addRow("Frequency (Hz)", freq);
    form->addRow("Min PWM (us)", minPWM);
    form->addRow("Max PWM (us)", maxPWM);
    form->addRow(applyBtn);

    connect(applyBtn,&QPushButton::clicked,this,[=](){
        if(_serialPort && _serialPort->isOpen()) {
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

void MainWindow::setupLCTab(QWidget *tab)
{
    auto *layout = new QVBoxLayout(tab);

    auto *group = new QGroupBox("Loadcell Configuration");
    group->setFixedWidth(420);

    auto *quantity = new QSpinBox();
    quantity->setRange(1,3);
    quantity->setValue(1);

    auto *vpu = new QSpinBox();
    vpu->setRange(1,1000);
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
    sampleAverage->setRange(1,100);
    sampleAverage->setValue(10);

    auto *precision = new QSpinBox();
    precision->setRange(1,200);
    precision->setValue(100);

    auto *applyBtn = new QPushButton("Apply");

    auto *grid = new QGridLayout(group);
    grid->setHorizontalSpacing(20);
    grid->setVerticalSpacing(12);

    grid->addWidget(new QLabel("Loadcell Quantity"),0,0);
    grid->addWidget(quantity,0,1);

    grid->addWidget(new QLabel("Loadcell ID"),0,2);
    grid->addWidget(idSelect,0,3);

    grid->addWidget(new QLabel("Channel"),1,0);
    grid->addWidget(channel,1,1);

    grid->addWidget(new QLabel("Gain"),1,2);
    grid->addWidget(gain,1,3);

    grid->addWidget(new QLabel("Sign"),2,0);
    grid->addWidget(sign,2,1);

    grid->addWidget(new QLabel("Sample Average"),2,2);
    grid->addWidget(sampleAverage,2,3);

    grid->addWidget(new QLabel("Tare Precision"),3,0);
    grid->addWidget(precision,3,1);

    grid->addWidget(new QLabel("Value Per Unit"),3,2);
    grid->addWidget(vpu,3,3);

    grid->addWidget(applyBtn, 4, 0, 1, 4);

    layout->addWidget(group,0,Qt::AlignTop | Qt::AlignHCenter);
    layout->addStretch();

    auto updateQuantity = [=]() {

        int q = quantity->value();

        _lcConfigs.clear();
        idSelect->clear();

        for(int i=0;i<q;i++)
        {
            LCConfig cfg;
            cfg.id = i+1;
            cfg.channel = 'A';
            cfg.gain = 128;
            cfg.sign = 1;
            cfg.sampleAverage = 10;
            cfg.tarePrecision = 100;
            cfg.val_per_unit = 206;
            cfg.quantity = 1;

            _lcConfigs.append(cfg);

            idSelect->addItem(QString("LC%1").arg(i+1));
        }
    };

    updateQuantity();

    connect(quantity, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int) {
        updateQuantity();
    });

    auto saveCurrentLC = [=]() {

        if(_lcConfigs.isEmpty()) return;

        if(_currentLC < 0 || _currentLC >= _lcConfigs.size()) return;

        auto &cfg = _lcConfigs[_currentLC];

        cfg.channel = channel->currentText().toLatin1()[0];
        cfg.gain = gain->currentText().toInt();
        cfg.sign = sign->currentText().toInt();
        cfg.sampleAverage = sampleAverage->value();
        cfg.tarePrecision = precision->value();
        cfg.val_per_unit = vpu->value();
        cfg.quantity = quantity->value();
    };

    connect(idSelect,&QComboBox::currentIndexChanged,this,[=](int index){

        saveCurrentLC();   // lưu LC cũ

        _currentLC = index;

        if(index < 0 || index >= _lcConfigs.size()) return;

        auto &cfg = _lcConfigs[index];

        channel->setCurrentText(QString(cfg.channel));
        gain->setCurrentText(QString::number(cfg.gain));
        sign->setCurrentText(QString::number(cfg.sign));
        sampleAverage->setValue(cfg.sampleAverage);
        precision->setValue(cfg.tarePrecision);
        vpu->setValue(cfg.val_per_unit);
    });

    connect(channel,&QComboBox::currentTextChanged,this,[=](QString ch) {

        gain->clear();

        if(ch == "A")
        {
            gain->addItem("128");
            gain->addItem("64");
        }
        else
        {
            gain->addItem("32");
        }
    });

    connect(applyBtn,&QPushButton::clicked,this,[=](){

        saveCurrentLC();

        for(const auto &cfg : _lcConfigs)
        {
            QString cmd = QString("lcconf:%1:%2:%3:%4:%5:%6:%7:%8\n")
                .arg(cfg.id)
                .arg(cfg.channel)
                .arg(cfg.gain)
                .arg(cfg.sign)
                .arg(cfg.val_per_unit)
                .arg(cfg.sampleAverage)
                .arg(cfg.quantity)
                .arg(cfg.tarePrecision);

            if(_serialPort && _serialPort->isOpen()) {
                _serialPort->write(cmd.toUtf8());
            }
        }
    });
}

void MainWindow::setupMavlinkTab(QWidget *tab)
{
    auto *layout = new QVBoxLayout(tab);

    auto *group = new QGroupBox("MAVLink Connection");
    group->setFixedWidth(400);
    auto *form = new QFormLayout(group);

    auto *sysID = new QSpinBox();
    sysID->setRange(1,255);
    sysID->setValue(1);

    auto *compID = new QSpinBox();
    compID->setRange(1,255);
    compID->setValue(1);

    auto *baud = new QComboBox();
    baud->addItems({"57600","115200","921600"});

    auto *udpPort = new QSpinBox();
    udpPort->setRange(1000,65535);
    udpPort->setValue(14550);

    auto *connectBtn = new QPushButton("Connect");

    form->addRow("System ID",sysID);
    form->addRow("Component ID",compID);
    form->addRow("Baudrate",baud);
    form->addRow("UDP Port",udpPort);
    form->addRow(connectBtn);

    layout->addWidget(group, 0, Qt::AlignTop | Qt::AlignHCenter);
    layout->addStretch();
}

void MainWindow::setupFFTTab(QWidget *tab)
{
    auto *layout = new QVBoxLayout(tab);

    auto *group = new QGroupBox("FFT Analysis Settings");
    group->setFixedWidth(400);
    auto *form = new QFormLayout(group);

    auto *windowSize = new QSpinBox();
    windowSize->setRange(128,8192);
    windowSize->setValue(1024);

    auto *sampleRate = new QSpinBox();
    sampleRate->setRange(100,10000);
    sampleRate->setValue(1000);

    auto *overlap = new QSpinBox();
    overlap->setRange(0,90);
    overlap->setValue(50);

    auto *windowType = new QComboBox();
    windowType->addItems({"Hann","Hamming","Blackman"});

    auto *applyBtn = new QPushButton("Apply");

    form->addRow("Window Size",windowSize);
    form->addRow("Sample Rate (Hz)",sampleRate);
    form->addRow("Overlap (%)",overlap);
    form->addRow("Window Type",windowType);
    form->addRow(applyBtn);

    layout->addWidget(group, 0, Qt::AlignTop | Qt::AlignHCenter);
    layout->addStretch();
}
