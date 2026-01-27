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
    double thrust;
    double torque;
    double voltage;
    double current;
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
    double _thrustMin = 0;
    double _thrustMax = 100;
    double _voltMin = 0;
    double _voltMax = 100;
    double _currentMin = 0;
    double _currentMax = 100;
    double _torqueMin = 0;
    double _torqueMax = 100;

    int stopFlag = 0;
    double throttleValue = 0.0, pwmValue = 0.0, torqueValue = 0.0;
    double currentValue = 0.0, voltageValue = 0.0, temperatureValue = 0.0, RPMValue = 0.0;
    bool swcurrent = false;

    QLabel *_thrustLabel;
    QLabel *_pwmLabel;
    QLabel *_torqueLabel;
    QLabel *_voltageLabel;
    QLabel *_currentLabel;
    QLabel *_temperatureLabel;
    QLabel *_RPMLabel;

    QSerialPort *_serialPort;
    bool _plotting;
    bool _timeReset = false;
    QChart *_chart;
    QTime _startTime;

    QTextEdit *_logTextEdit;
    QVector<DataPoint> _dataBuffer;
    bool _isDarkMode;

    void updateAnalyzeCharts(double thrust, double torque, double voltage,double current, double pwm);

    QTabWidget *_tabWidget;
    QWidget *_homeTab;
    QWidget *_analyzeTab;
    QWidget *_settingTab;

    bool _sidebarCollapsed;

    QChart *_analyzeChart1, *_analyzeChart2, *_analyzeChart3;
    QLineSeries *_series1, *_series2, *_series3, *_series4;

    void setupHomeTab();
    void setupAnalyzeTab();
    void setupSettingTab();
    QChartView* createAnalyzeChart(const QString &title, const QString &yTitle, QLineSeries *series);

    void saveLogToCSV(const QString& message);
    void addThemeToggleButton();
    void toggleTheme();
    void addToggleSwitch();
    void applyDarkTheme();
    void applyLightTheme();
    void reset();

    QStringList _csvHeaders;
    QLineSeries *_csvSeries;
    QTableWidget *_csvTableWidget;
    QVector<QStringList> _csvData;

    void setupPlotTab(QWidget *tab);
    void setupTableTab(QWidget *tab);
};

#endif // MAINWANT
