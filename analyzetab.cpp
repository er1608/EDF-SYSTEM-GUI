#include "mainwindow.h"
#include <QtCharts>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <QtWidgets>

void MainWindow::setupAnalyzeTab()
{
    // _analyzeTab->setStyleSheet("background: #1b2432;");
    auto *analyzeLayout = new QVBoxLayout(_analyzeTab);
    analyzeLayout->setContentsMargins(10, 10, 10, 10);
    analyzeLayout->setSpacing(10);

    _series1 = new QLineSeries();
    _series2 = new QLineSeries();
    _series3 = new QLineSeries();
    _series4 = new QLineSeries();

    auto *tempChart = createAnalyzeChart("Temperature", "°C", _series1);
    auto *voltChart = createAnalyzeChart("Voltage", "V", _series2);
    auto *currentChart = createAnalyzeChart("Current", "A", _series3);
    auto *RPMChart = createAnalyzeChart("RPM", "R/M", _series4);

    auto *chartsLayout = new QGridLayout();
    chartsLayout->addWidget(tempChart, 0, 0);
    chartsLayout->addWidget(voltChart, 0, 1);
    chartsLayout->addWidget(currentChart, 1, 0);
    chartsLayout->addWidget(RPMChart, 1, 1);

    auto *controlGroup = new QGroupBox("Analyze Controls", _analyzeTab);
    auto *controlLayout = new QHBoxLayout(controlGroup);

    auto *clearChartsBtn = new QPushButton("Clear Charts", controlGroup);
    auto *exportDataBtn = new QPushButton("Export Data", controlGroup);

    controlLayout->addWidget(clearChartsBtn);
    controlLayout->addWidget(exportDataBtn);
    controlLayout->addStretch();

    analyzeLayout->addLayout(chartsLayout, 1);
    analyzeLayout->addWidget(controlGroup);

    connect(clearChartsBtn, &QPushButton::clicked, this, [this]() {
        _series1->clear();
        _series2->clear();
        _series3->clear();
        _series4->clear();
        _logTextEdit->append("[ANALYZE] Charts cleared");
        saveLogToCSV("[ANALYZE] Charts cleared");
    });

    connect(exportDataBtn, &QPushButton::clicked, this, &MainWindow::saveDataToCSV);
}
