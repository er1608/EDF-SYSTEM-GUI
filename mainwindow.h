#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define Q_OS_LINUX

#include <QLabel>
#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTextEdit>
#include <QTime>
#include <QTimer>
#include <QVector>
#include <QtCharts>

QT_USE_NAMESPACE

struct DataPoint {
  double timestamp;
  double thrust;
  double rpm;
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

class MainWindow : public QMainWindow {
  Q_OBJECT

signals:
  void resized();

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  void resizeEvent(QResizeEvent *event) override;

private slots:
  void readData();
  void saveDataToCSV();

private:
  double _thrustMin = 0;
  double _thrustMax = 50;
  double _voltMin = 0;
  double _voltMax = 30;
  double _currentMin = 0;
  double _currentMax = 10;
  double _rpmMin = 0;
  double _rpmMax = 1000;

  double _lastKey = -1;
  double _lastThrust = 0;

  int stopFlag = 0;
  double thrustValue = 0.0, pwmValue = 0.0;
  double currentValue = 0.0, voltageValue = 0.0;
  int RPMValue = 0.0;
  bool swcurrent = false;
  int _currentLC = 0;

  QLabel *_thrustLabel;
  QLabel *_pwmLabel;
  QLabel *_voltageLabel;
  QLabel *_currentLabel;
  QLabel *_RPMLabel;

  QChartView *_thrustChart;
  QChartView *_voltChart;
  QChartView *_currentChart;
  QChartView *_rpmChart;

  QSerialPort *_serialPort;

  QSerialPort *_vescSerialPort = nullptr;
  QByteArray _vescRxBuffer;
  QTimer *_vescPollTimer = nullptr;
  QComboBox *_vescPortComboBox = nullptr;
  QComboBox *_vescBaudComboBox = nullptr;
  QPushButton *_vescConnectBtn = nullptr;
  QLabel *_vescStatusLabel = nullptr;
  bool _vescConnected = false;
  int _vescPolePairs = 21;
  QSpinBox *_polePairsSpinBox = nullptr;

  void vescSendGetValues();
  void vescReadData();
  void vescParsePacket(const QByteArray &payload);
  void vescRefreshPorts();
  static quint16 vescCrc16(const QByteArray &data);
  bool _plotting = false;
  bool _timeReset = false;
  QChart *_chart;
  QTime _startTime;

  QComboBox *portComboBox;
  QTimer *refreshTimer;
  QStringList lastPortList;

  QTextEdit *_logTextEdit;
  QVector<DataPoint> _dataBuffer;
  QVector<DataPoint> _dataPDFBuffer;
  QVector<DataPoint> bufferCopy;
  bool _isDarkMode;

  float L = 0.25;

  void updateAnalyzeCharts(double thrust, double voltage, double current,
                           double pwm, double rpm);
  void appendInterpolated(QLineSeries *series, double key, double value);

  QTabWidget *_tabWidget;
  QWidget *_homeTab;
  QWidget *_analyzeTab;
  QWidget *_settingTab;

  QTimer *_autoTimer = nullptr;
  int autoValue = 0;
  bool pdf = false;
  QByteArray _rxBuffer;

  bool _sidebarCollapsed;

  QLineSeries *_series1, *_series2, *_series3, *_series4;

  void setupHomeTab();
  void setupAnalyzeTab();
  void setupSettingTab();
  void updateAxis(QChart *chart, double key, double minY, double maxY);
  QChartView *createAnalyzeChart(const QString &yTitle, QLineSeries *series);

  void saveLogToCSV(const QString &message);
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
  void setupVESCTab(QWidget *tab);
  void refreshSerialPorts();

  QStringList _csvHeaders;
  QLineSeries *_csvSeries;
  QTableWidget *_csvTableWidget;
  QVector<QStringList> _csvData;

  void setupPlotTab(QWidget *tab);
  void setupTableTab(QWidget *tab);
};

#endif // MAINWANT
