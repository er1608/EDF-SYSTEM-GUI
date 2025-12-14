#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define Q_OS_LINUX

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

signals:
    void resized();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void readData();
    void saveDataToCSV();

private:
    double _tempMin = std::numeric_limits<double>::max();
    double _tempMax = std::numeric_limits<double>::lowest();
    double _voltMin = std::numeric_limits<double>::max();
    double _voltMax = std::numeric_limits<double>::lowest();
    double _currentMin = std::numeric_limits<double>::max();
    double _currentMax = std::numeric_limits<double>::lowest();
    double _throttleMin = std::numeric_limits<double>::max();
    double _throttleMax = std::numeric_limits<double>::lowest();
    double _RPMMin = std::numeric_limits<double>::max();
    double _RPMMax = std::numeric_limits<double>::lowest();
    int stopFlag = 0;
    double throttleValue = 0.0, pwmValue = 0.0;
    double currentValue = 0.0, voltageValue = 0.0, temperatureValue = 0.0, RPMValue = 0.0;
    bool swcurrent = false;

    QSerialPort *_serialPort;
    bool _plotting;
    QChart *_chart;
    QLineSeries *_throttleSeries;
    QTime _startTime;
    QLabel *_pwmLabel;
    QLabel *_throttleLabel;
    QTextEdit *_logTextEdit;
    QVector<DataPoint> _dataBuffer;
    bool _isDarkMode;

    QChartView* setupMainChart();
    void updatePlot(double throttle, double pwm);
    void updateAnalyzeCharts(double temp, double current, double voltage, double RPM);

    QTabWidget *_tabWidget;
    QWidget *_homeTab;
    QWidget *_analyzeTab;

    bool _sidebarCollapsed;

    QChart *_analyzeChart1, *_analyzeChart2, *_analyzeChart3;
    QLineSeries *_series1, *_series2, *_series3, *_series4;

    void setupHomeTab();
    void setupAnalyzeTab();
    QChartView* createAnalyzeChart(const QString &title, const QString &yTitle, QLineSeries *series);

    void saveLogToCSV(const QString& message);
    void addThemeToggleButton();
    void toggleTheme();
    void addToggleSwitch();
    void applyDarkTheme();
    void applyLightTheme();
};

#endif // MAINWANT
