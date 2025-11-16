#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtCharts>
#include <QTime>
#include <QLabel>
#include <QTextEdit>
#include <QVector>

QT_USE_NAMESPACE

struct DataPoint {
    double timestamp;
    double throttle;
    double pwm;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void readData();
    void saveDataToCSV();

private:
    QSerialPort *_serialPort;
    bool _plotting;
    QChart *_chart;
    QLineSeries *_throttleSeries;
    QTime _startTime;
    QLabel *_pwmLabel;
    QLabel *_throttleLabel;
    QTextEdit *_logTextEdit;
    QVector<DataPoint> _dataBuffer;

    QChartView* setupMainChart();
    void updatePlot(double throttle, double pwm);
};

#endif // MAINWANT
