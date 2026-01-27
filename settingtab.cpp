#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QtWidgets>

void MainWindow::setupSettingTab()
{
    auto *settingLayout = new QVBoxLayout(_settingTab);
    settingLayout->setContentsMargins(10, 10, 10, 10);
    settingLayout->setSpacing(10);

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

    settingLayout->addLayout(modeLayout, 1);

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

}
