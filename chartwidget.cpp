#include "mainwindow.h"
#include <QtCharts>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QtWidgets>

void MainWindow::updateAnalyzeCharts(double thrust, double torque, double voltage, double current, double pwm)
{
    if (_timeReset) {
        _startTime = QTime::currentTime();
        _timeReset = false;
    }

    double key = _startTime.msecsTo(QTime::currentTime()) / 1000.0;

    _thrustMin = std::min(_thrustMin, thrust);
    _thrustMax = std::max(_thrustMax, thrust);
    _voltMin = std::min(_voltMin, voltage);
    _voltMax = std::max(_voltMax, voltage);
    _currentMin = std::min(_currentMin, current);
    _currentMax = std::max(_currentMax, current);
    _torqueMin = std::min(_torqueMin, torque);
    _torqueMax = std::max(_torqueMax, torque);

    _series1->append(key, thrust);
    _series2->append(key, torque);
    // _series3->append(key, current);
    // _series4->append(key, voltage);

    if (_series1->count() > 4700) {
        _series1->remove(0);
        _series2->remove(0);
        // _series3->remove(0);
        // _series4->remove(0);
    }

    _dataBuffer.append(DataPoint{key, thrust, torque, voltage, current, pwm});
    if(pdf) _dataPDFBuffer.append(DataPoint{key, thrust, torque, voltage, current, pwm});

    auto updateChartAxis = [key](QChart* chart, double minY, double maxY, int stopFlag) {
        auto axes = chart->axes();
        if (axes.size() >= 2) {
            auto *axisX = static_cast<QValueAxis*>(axes[0]);
            auto *axisY = static_cast<QValueAxis*>(axes[1]);

            if (stopFlag) {
                axisX->setRange(-10, 0);
                stopFlag = 0;
            }

            if (key > axisX->max()) {
                axisX->setRange(key - 10, key);
            }

            double margin = (maxY - minY) * 0.1;
            if (margin == 0) margin = std::abs(minY) * 0.1 + 0.1;
            axisY->setRange(minY - margin, maxY + margin);
        }
    };

    if (_series1->chart()) updateChartAxis(_series1->chart(), _thrustMin, _thrustMax, stopFlag);
    if (_series2->chart()) updateChartAxis(_series4->chart(), _torqueMin, _torqueMax, stopFlag);
    // if (_series3->chart()) updateChartAxis(_series3->chart(), _currentMin, _currentMax, stopFlag);
    // if (_series4->chart()) updateChartAxis(_series2->chart(), _voltMin, _voltMax, stopFlag);
}

QChartView* MainWindow::createAnalyzeChart(const QString &title, const QString &yTitle, QLineSeries *series)
{
    auto *chart = new QChart();

    chart->addSeries(series);
    chart->legend()->setVisible(false);
    chart->setBackgroundBrush(QBrush(QColor(0, 0, 0, 0)));

    auto *axisX = new QValueAxis();
    auto *axisY = new QValueAxis();

    axisY->setTitleText(yTitle);
    axisX->setRange(-10, 0);

    QColor textColor("#6B7280");

    chart->setTitleBrush(QBrush(textColor));

    axisX->setLabelsColor(textColor);
    axisY->setLabelsColor(textColor);

    QPen pen(textColor);
    axisX->setLinePen(pen);
    axisY->setLinePen(pen);

    QColor gridColor = textColor;
    gridColor.setAlpha(80);

    axisX->setGridLinePen(QPen(gridColor));
    axisY->setGridLinePen(QPen(gridColor));

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    axisX->setTitleBrush(QBrush(textColor));
    axisY->setTitleBrush(QBrush(textColor));

    if (title.contains("Thrust")) {
        axisY->setRange(0, 100);
    } else if (title.contains("Torque")) {
        axisY->setRange(0, 100);
    } else if (title.contains("Voltage")) {
        axisY->setRange(0, 100);
    } else {
        axisX->setTitleText("Time(s)");
        axisY->setRange(0, 100);
    }

    series->attachAxis(axisX);
    series->attachAxis(axisY);

    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setBackgroundRoundness(0);

    auto *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setVisible(true);
    chartView->setMinimumSize(400, 300);

    return chartView;
}
