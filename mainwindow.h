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
#include <QTimer>

QT_USE_NAMESPACE

struct DataPoint {
    double timestamp;
    double thrust;
    double torque;
    double voltage;
    double current;
    double pwm;
};

struct LCConfig {
    int id;
    int quantity;
    int val_per_unit;
    int gain;
    int sign;
    char channel;
    int sampleAverage;
    int tarePrecision;
};

extern QVector<LCConfig> _lcConfigs;

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

    double _lastKey = -1;
    double _lastThrust = 0;

    int stopFlag = 0;
    double throttleValue = 0.0, pwmValue = 0.0, torqueValue = 0.0;
    double currentValue = 0.0, voltageValue = 0.0, temperatureValue = 0.0, RPMValue = 0.0;
    bool swcurrent = false;
    int _currentLC = 0;

    QLabel *_thrustLabel;
    QLabel *_pwmLabel;
    QLabel *_torqueLabel;
    QLabel *_voltageLabel;
    QLabel *_currentLabel;
    QLabel *_temperatureLabel;
    QLabel *_RPMLabel;

    QChartView *_thrustChart;
    QChartView *_torqueChart;
    QChartView *_voltChart;
    QChartView *_currentChart;

    QSerialPort *_serialPort;
    bool _plotting;
    bool _timeReset = false;
    QChart *_chart;
    QTime _startTime;

    QTextEdit *_logTextEdit;
    QVector<DataPoint> _dataBuffer;
    QVector<DataPoint> _dataPDFBuffer;
    QVector<DataPoint> bufferCopy;
    bool _isDarkMode;

    void updateAnalyzeCharts(double thrust, double torque, double voltage,double current, double pwm);
    void appendInterpolated(QLineSeries* series, double key, double value);

    QTabWidget *_tabWidget;
    QWidget *_homeTab;
    QWidget *_analyzeTab;
    QWidget *_settingTab;

    QTimer *_autoTimer = nullptr;
    int autoValue = 0;
    bool pdf = false;
    QByteArray _rxBuffer;

    bool _sidebarCollapsed;

    QChart *_analyzeChart1, *_analyzeChart2, *_analyzeChart3;
    QLineSeries *_series1, *_series2, *_series3, *_series4;

    void setupHomeTab();
    void setupAnalyzeTab();
    void setupSettingTab();
    void updateAxis(QChart* chart, double key, double minY, double maxY);
    QChartView* createAnalyzeChart(const QString &title, const QString &yTitle, QLineSeries *series);

    void saveLogToCSV(const QString& message);
    void saveDataToPDF();
    void addThemeToggleButton();
    void toggleTheme();
    void addToggleSwitch();
    void applyDarkTheme();
    void applyLightTheme();
    void reset();

    void setupSignalGeneratorTab(QWidget *tab);
    void setupPWMTab(QWidget *tab);
    void setupLCTab(QWidget *tab);
    void setupMavlinkTab(QWidget *tab);
    void setupFFTTab(QWidget *tab);

    QStringList _csvHeaders;
    QLineSeries *_csvSeries;
    QTableWidget *_csvTableWidget;
    QVector<QStringList> _csvData;

    void setupPlotTab(QWidget *tab);
    void setupTableTab(QWidget *tab);
};

#endif // MAINWANT
