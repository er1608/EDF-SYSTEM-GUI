#include "mainwindow.h"
#include <QtCharts>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <QtWidgets>

void MainWindow::updateAnalyzeCharts(double temp, double current, double voltage, double RPM)
{
    double key = _startTime.msecsTo(QTime::currentTime()) / 1000.0;

    _tempMin = std::min(_tempMin, temp);
    _tempMax = std::max(_tempMax, temp);
    _voltMin = std::min(_voltMin, voltage);
    _voltMax = std::max(_voltMax, voltage);
    _currentMin = std::min(_currentMin, current);
    _currentMax = std::max(_currentMax, current);
    _RPMMin = std::min(_RPMMin, RPM);
    _RPMMax = std::max(_RPMMax, RPM);

    _series1->append(key, temp);
    _series2->append(key, voltage);
    _series3->append(key, current);
    _series4->append(key, RPM);

    const int MAX_POINTS = 1000;
    if (_series1->count() > MAX_POINTS) {
        _series1->remove(0);
        _series2->remove(0);
        _series3->remove(0);
        _series4->remove(0);
    }

    auto updateChartAxis = [key](QChart* chart, double minY, double maxY, int stopFlag) {
        auto axes = chart->axes();
        if (axes.size() >= 2) {
            auto *axisX = static_cast<QValueAxis*>(axes[0]);
            auto *axisY = static_cast<QValueAxis*>(axes[1]);

            if (stopFlag) axisX->setRange(-30, 0);

            if (key > axisX->max()) {
                axisX->setRange(key - 30, key);
            }

            double margin = (maxY - minY) * 0.1;
            if (margin == 0) margin = std::abs(minY) * 0.1 + 0.1;
            axisY->setRange(minY - margin, maxY + margin);
        }
    };

    if (_series1->chart()) updateChartAxis(_series1->chart(), _tempMin, _tempMax, stopFlag);
    if (_series2->chart()) updateChartAxis(_series2->chart(), _voltMin, _voltMax, stopFlag);
    if (_series3->chart()) updateChartAxis(_series3->chart(), _currentMin, _currentMax, stopFlag);
    if (_series4->chart()) updateChartAxis(_series4->chart(), _RPMMin, _RPMMax, stopFlag);
}

void MainWindow::updatePlot(double throttle, double pwm)
{
    if (_timeReset) {
        _startTime = QTime::currentTime();
        _timeReset = false;
    }

    double key = _startTime.msecsTo(QTime::currentTime()) / 1000.0;

    _throttleSeries->append(key, throttle);

    if (_throttleSeries->count() > 1200) {
        _throttleSeries->remove(0);
    }

    _throttleLabel->setText(QString("Thrust: %1").arg(throttle, 0, 'f', 1));
    _pwmLabel->setText(QString("PWM: %1").arg(pwm, 0, 'f', 1));

    _dataBuffer.append(DataPoint{key, throttle, pwm});

    _throttleMin = std::min(_throttleMin, throttle);
    _throttleMax = std::max(_throttleMax, throttle);

    auto axes = _chart->axes();
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

        double margin = (_throttleMax - _throttleMin) * 0.1;
        if (margin == 0) margin = std::abs(_throttleMin) * 0.1 + 0.1;
        axisY->setRange(_throttleMin - margin, _throttleMax + margin);
    }
}

QChartView* MainWindow::createAnalyzeChart(const QString &title, const QString &yTitle, QLineSeries *series)
{
    auto *chart = new QChart();

    QPen pen;
    if (title.contains("Temperature")) {
        pen = QPen(QColor(255, 107, 107), 2);
    } else if (title.contains("Voltage")) {
        pen = QPen(QColor(102, 204, 255), 2);
    } else if (title.contains("Current")){
        pen = QPen(QColor(102, 255, 102), 2);
    } else {
        pen = QPen(QColor(102, 255, 102), 2);
    }

    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    series->setPen(pen);

    chart->addSeries(series);
    chart->setTitle(title);
    chart->setTitleBrush(QBrush(QColor(226, 232, 240)));
    chart->legend()->setVisible(false);
    chart->setBackgroundBrush(QBrush(QColor(27, 36, 50)));
    chart->setTheme(QChart::ChartThemeBlueCerulean);

    // Tạo axes
    auto *axisX = new QValueAxis();
    auto *axisY = new QValueAxis();

    axisX->setTitleText("Time(s)");
    axisY->setTitleText(yTitle);
    axisX->setRange(-30, 0);

    if (title.contains("Temperature")) {
        axisY->setRange(0, 100);
    } else if (title.contains("Voltage")) {
        axisY->setRange(0, 24);
    } else {
        axisY->setRange(0, 10);
    }

    // Thiết lập màu cho axes
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

    auto *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 300);

    return chartView;
}

QChartView* MainWindow::setupMainChart()
{
    _chart = new QChart();
    _throttleSeries = new QLineSeries();

    _throttleSeries->setName("Thrust");
    QPen pen(QColor(65, 90, 119), 3);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    _throttleSeries->setPen(pen);

    _chart->addSeries(_throttleSeries);
    _chart->setTitle("Thrust");
    _chart->setTitleBrush(QBrush(QColor(226, 232, 240)));
    _chart->legend()->setVisible(false);
    _chart->setBackgroundBrush(QBrush(QColor(27, 36, 50)));
    _chart->setTheme(QChart::ChartThemeBlueCerulean);

    auto *axisX = new QValueAxis();
    auto *axisY = new QValueAxis();

    axisX->setTitleText("Time(s)");
    axisY->setTitleText("Newton");
    axisX->setRange(-30, 0);
    axisY->setRange(0, 100);

    axisX->setLabelsColor(QColor(166, 177, 225));
    axisY->setLabelsColor(QColor(166, 177, 225));
    axisX->setTitleBrush(QBrush(QColor(226, 232, 240)));
    axisY->setTitleBrush(QBrush(QColor(226, 232, 240)));
    axisX->setGridLineColor(QColor(45, 50, 80, 100));
    axisY->setGridLineColor(QColor(45, 50, 80, 100));

    _chart->addAxis(axisX, Qt::AlignBottom);
    _chart->addAxis(axisY, Qt::AlignLeft);

    _throttleSeries->attachAxis(axisX);
    _throttleSeries->attachAxis(axisY);

    auto *chartView = new QChartView(_chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}
