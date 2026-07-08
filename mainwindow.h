#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define Q_OS_LINUX

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSpinBox>
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
  double watt_hours;
  double watt_hours_charged;
};

struct LCConfig {
  uint8_t id;
  uint8_t quantity;
  uint8_t sign;
  char channel;
  uint16_t val_per_unit;
  uint16_t gain;
  uint16_t sampleAverage;
  uint16_t tarePrecision;
};

enum {
  COMM_SET_SYSTEM_CONF = 0x10,
  COMM_RESET_SYSTEM_CONF = 0x11,
  COMM_START_SYSTEM = 0x12,
  COMM_STOP_SYSTEM = 0x13,
  COMM_SET_POWER = 0x14,
  COMM_CALIB_LOADCELL = 0x15,
};

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
  static quint16 vescCrc16(const QByteArray &data);

  QSerialPort *getSerialPort() const { return _serialPort; }
  double getThrustValue() const { return thrustValue; }
  void setLCValuePerUnit(int value) {
    if (lcVpu)
      lcVpu->setValue(value);
    if (!_lcConfigs.isEmpty() && _currentLC >= 0 &&
        _currentLC < _lcConfigs.size()) {
      _lcConfigs[_currentLC].val_per_unit = value;
    }
  }

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
  double wattHoursValue = 0.0;
  double wattHoursChargedValue = 0.0;
  bool swcurrent = false;
  int _currentLC = 0;

  QLabel *_thrustLabel;
  QLabel *_pwmLabel;
  QLabel *_voltageLabel;
  QLabel *_currentLabel;
  QLabel *_RPMLabel;
  QLabel *_wattHoursLabel = nullptr;
  QLabel *_wattHoursChargedLabel = nullptr;

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

  void Send_PWM(uint16_t value);
  void Send_Start();
  void Send_Stop();

  void vescSendGetValues();
  void vescReadData();
  void vescParsePacket(const QByteArray &payload);
  void vescRefreshPorts();
  void buffer_append_ui8(QByteArray &buffer, uint8_t value);
  void buffer_append_ui16(QByteArray &buffer, uint16_t value);
  void buffer_append_ui32(QByteArray &buffer, uint32_t value);
  void buffer_append_ui64(QByteArray &buffer, uint64_t value);
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
  void Send_Configurations();
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

  QComboBox *freq = nullptr;
  QSpinBox *minPWM = nullptr, *maxPWM = nullptr;

  QSpinBox *windowSize = nullptr, *sampleRate = nullptr, *overlap = nullptr;
  QComboBox *fftWindowType = nullptr;

  QDoubleSpinBox *sigAmplitude = nullptr, *sigFrequency = nullptr,
                 *sigDuration = nullptr;

  QSpinBox *lcQuantity = nullptr, *lcVpu = nullptr, *lcSampleAverage = nullptr,
           *lcPrecision = nullptr;
  QComboBox *lcIdSelect = nullptr, *lcChannel = nullptr, *lcGain = nullptr,
            *lcSign = nullptr;
  QVector<LCConfig> _lcConfigs;
};

#endif // MAINWANT
