#include "mainwindow.h"
#include <QtCharts>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QtWidgets>

// void MainWindow::appendInterpolated(QLineSeries* series, double key, double value)
// {
//     const int INTERP_POINTS = 4;   // số điểm nội suy

//     if (_lastKey < 0) {
//         series->append(key, value);
//         _lastKey = key;
//         _lastThrust = value;
//         return;
//     }

//     double dt = (key - _lastKey) / (INTERP_POINTS + 1);
//     double dv = (value - _lastThrust) / (INTERP_POINTS + 1);

//     for (int i = 1; i <= INTERP_POINTS; i++) {
//         double t = _lastKey + dt * i;
//         double v = _lastThrust + dv * i;
//         series->append(t, v);
//     }

//     series->append(key, value);

//     _lastKey = key;
//     _lastThrust = value;
// }

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
    // _series2->append(key, voltage);
    // _series3->append(key, current);
    // _series4->append(key, torque);

    if (_series1->count() > 4700) {
        _series1->remove(0);
        // _series2->remove(0);
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
    // if (_series2->chart()) updateChartAxis(_series2->chart(), _voltMin, _voltMax, stopFlag);
    // if (_series3->chart()) updateChartAxis(_series3->chart(), _currentMin, _currentMax, stopFlag);
    // if (_series4->chart()) updateChartAxis(_series4->chart(), _torqueMin, _torqueMax, stopFlag);
}

// void MainWindow::updateAnalyzeCharts(double thrust, double torque, double voltage, double current, double pwm)
// {
//     static int frame = 0;

//     if (_timeReset) {
//         _startTime = QTime::currentTime();
//         _timeReset = false;
//     }

//     double key = _startTime.msecsTo(QTime::currentTime()) / 1000.0;

//     _series1->append(key, thrust);
//     _series2->append(key, voltage);
//     _series3->append(key, current);
//     _series4->append(key, torque);

//     if (_series1->count() > 1200) {
//         _series1->removePoints(0, 1);
//         _series2->removePoints(0, 1);
//         _series3->removePoints(0, 1);
//         _series4->removePoints(0, 1);
//     }

//     frame++;

//     // chỉ update axis mỗi 5 lần
//     if (frame % 5 == 0) {

//         auto axisX = static_cast<QValueAxis*>(_series1->chart()->axisX());
//         if (key > axisX->max())
//             axisX->setRange(key - 10, key);

//         auto axisY = static_cast<QValueAxis*>(_series1->chart()->axisY());
//         axisY->setRange(_thrustMin, _thrustMax);
//     }
// }

QChartView* MainWindow::createAnalyzeChart(const QString &title, const QString &yTitle, QLineSeries *series)
{
    auto *chart = new QChart();

    QPen pen(QColor(65, 90, 119), 3);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    series->setPen(pen);

    chart->addSeries(series);
    chart->legend()->setVisible(false);
    chart->setBackgroundBrush(QBrush(QColor(27, 36, 50)));
    chart->setTheme(QChart::ChartThemeBlueCerulean);

    auto *axisX = new QValueAxis();
    auto *axisY = new QValueAxis();

    axisY->setTitleText(yTitle);
    axisX->setRange(-10, 0);

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

    axisX->setLabelsColor(QColor(166, 177, 225));
    axisY->setLabelsColor(QColor(166, 177, 225));
    axisX->setTitleBrush(QBrush(QColor(226, 232, 240)));
    axisY->setTitleBrush(QBrush(QColor(226, 232, 240)));
    axisX->setGridLineColor(QColor(45, 50, 80, 100));
    axisY->setGridLineColor(QColor(45, 50, 80, 100));

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

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
