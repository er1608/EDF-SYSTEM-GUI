#include "mainwindow.h"
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QtCharts>

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _serialPort(nullptr), _plotting(false),
      _chart(nullptr), _startTime(QTime::currentTime()), _isDarkMode(true) {
#if defined(Q_OS_WIN)
  setStyleSheet(R"(
        QMainWindow {
            background: #0e202d;
            color: #d2c1b6;
            font-family: "Segoe UI", "Roboto", sans-serif;
        }

        QScrollArea {
            border: none;
            background: #0e202d;
        }

        QWidget#ChartContainerWidget {
            background: #0e202d;
        }

        QScrollBar:vertical {
            border: none;
            background: #0e202d;
            width: 10px;
            margin: 0px;
        }

        QScrollBar::handle:vertical {
            background: #1b3c53;
            min-height: 20px;
            border-radius: 5px;
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }

        QScrollBar:horizontal {
            border: none;
            background: #0e202d;
            height: 10px;
            margin: 0px;
        }

        QScrollBar::handle:horizontal {
            background: #1b3c53;
            min-width: 20px;
            border-radius: 5px;
        }

        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0px;
        }

        QGroupBox {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #0e202d, stop:1 #0e202d);
            color: #d2c1b6;
            border: 1px solid #456882;
            border-radius: 10px;
            margin-top: 1.4ex;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 15px;
            top: -2px;
            color: #d2c1b6;
        }

        QPushButton {
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #456882,
                stop:1 #1b3c53
            );
            color: #d2c1b6;
            border: 1px solid #456882;
            padding: 4px 8px;
            border-radius: 6px;
            font-weight: 600;
            font-size: 11px;
        }

        QPushButton:hover {
            background: #234c6a;
            color: #f5f5f5;
        }

        QPushButton:pressed {
            background: #d2c1b6;
            border: 1px solid #d2c1b6;
            color: #0e202d;
        }

        QLineEdit {
            background: #1b3c53;
            color: #d2c1b6;
            border: 1px solid #456882;
            border-radius: 5px;
            padding: 3px;
            font-size: 11px;
            selection-background-color: #456882;
        }

        QSpinBox, QDoubleSpinBox {
            background: #1b3c53;
            color: #d2c1b6;
            border: 1px solid #1b3c53;
            border-radius: 5px;
            font-size: 13px;
            selection-background-color: #456882;
        }

        QSpinBox::up-button, QDoubleSpinBox::up-button {
            subcontrol-origin: border;
            subcontrol-position: top right;
            border-radius: 5px;
            background: #1b3c53;
        }

        QSpinBox::down-button, QDoubleSpinBox::down-button {
            subcontrol-origin: border;
            subcontrol-position: bottom right;
            border-radius: 5px;
            background: #1b3c53;
        }

        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover {
            background: #456882;
        }

        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background: #456882;
        }

        QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {
            image: url(:/images/up_white.png);
            width: 10px;
            height: 10px;
        }

        QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
            image: url(:/images/down_white.png);
            width: 10px;
            height: 10px;
        }

        QLineEdit:focus,
        QComboBox:focus,
        QSpinBox:focus,
        QDoubleSpinBox:focus {
            border: 1px solid #d2c1b6;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #234c6a, stop:1 #234c6a);
        }

        QSlider::groove:horizontal {
            border: 1px solid #456882;
            height: 6px;
            background: #0e202d;
            border-radius: 4px;
        }

        QSlider::handle:horizontal {
            background: #d2c1b6;
            border: 1px solid #d2c1b6;
            width: 16px;
            margin: -3px 0;
            border-radius: 8px;
        }

        QLabel {
            color: #d2c1b6;
            font-size: 11px;
            font-weight: 500;
        }

        QTextEdit {
            background: #0e202d;
            color: #d2c1b6;
            border: 1px solid #456882;
            border-radius: 6px;
            font-size: 11px;
            font-family: "Consolas", "Roboto Mono", monospace;
        }

        QChartView {
            background: #0e202d;
            border: 1px solid #456882;
            border-radius: 10px;
        }

        QTabWidget::pane {
            border: 1px solid #0e202d;
            background: #0e202d;
            padding: 4px;
        }

        QTabWidget::tab-bar {
            alignment: left;
        }

        QTabBar {
            background: #0e202d;
        }

        QTabBar::tab {
            background: #0e202d;
            color: #d2c1b6;
            padding: 2px 14px;
            margin: 4px 1px;
            border: 1px solid #0e202d;
            border-radius: 6px;
            font-weight: 600;
            width: 30px;
            text-align: center;
        }

        QTabBar QWidget {
            background: transparent;
        }

        QTabBar QLabel {
            background: transparent;
            margin: 0px;
            padding: 0px;
        }

        QTabBar::tab:selected {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                      stop:0 #456882, stop:1 #1b3c53);
            color: #d2c1b6;
            border: 1px solid #456882;
        }

        QTabBar::tab:hover:!selected {
            background: #1b3c53;
            color: #f5f5f5;
        }

        QPushButton#toggleTabsBtn {
            background: #1b3c53;
            color: #d2c1b6;
            border: none;
            padding: 8px;
            border-radius: 4px;
            font-weight: bold;
        }

        QPushButton#toggleTabsBtn:hover {
            background: #456882;
            color: #f5f5f5;
        }

        QWidget#contentArea {
            background: #1b3c53;
        }

        QWidget#leftPanel {
            background: #1b3c53;
            border-right: 1px solid #1b3c53;
        }

        QComboBox {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                        stop:0 #456882, stop:1 #1b3c53);
            color: #d2c1b6;
            border: 1px solid #0e202d !important;
            border-radius: 4px;
            padding: 3px;
        }

        QComboBox::drop-down {
            border: none;
            width: 20px;
        }

        QComboBox::down-arrow {
            image: none;
            border-left: 6px solid transparent;
            border-right: 6px solid transparent;
            border-top: 6px solid #d2c1b6;
        }

        QComboBox QAbstractItemView {
            background: #0e202d;
            color: #d2c1b6;
            border: 1px solid #1b3c53;
            selection-background-color: #1b3c53;
            selection-color: #d2c1b6;
        }

        QToolTip {
            background-color: #0e202d;
            color: #d2c1b6;
            border: 1px solid #456882;
            border-radius: 6px;
            font-family: 'Segoe UI', system-ui;
            font-size: 13px;
            opacity: 230;
        }

        QMenu {
            background-color: #0e202d;
            border: 1px solid #1b3c53;
            border-radius: 5px;
            padding: 5px 0px;
        }

        QMenu::item {
            padding: 8px 30px 8px 20px;
            color: #d2c1b6;
            background-color: transparent;
        }

        QMenu::item:selected {
            background-color: #1b3c53;
            color: #d2c1b6;
        }

        QMenu::item:checked {
            background-color: #456882;
            color: #d2c1b6;
        }

        QMenu::separator {
            height: 1px;
            background-color: #456882;
            margin: 5px 0px;
        }
    )");
#else
  setStyleSheet(R"(
        QMainWindow {
            background: #0e202d;
            color: #d2c1b6;
            font-family: "Segoe UI", "Roboto", sans-serif;
        }

        QScrollArea {
            border: none;
            background: #0e202d;
        }

        QWidget#ChartContainerWidget {
            background: #0e202d;
        }

        QScrollBar:vertical {
            border: none;
            background: #0e202d;
            width: 10px;
            margin: 0px;
        }

        QScrollBar::handle:vertical {
            background: #1b3c53;
            min-height: 20px;
            border-radius: 5px;
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }

        QScrollBar:horizontal {
            border: none;
            background: #0e202d;
            height: 10px;
            margin: 0px;
        }

        QScrollBar::handle:horizontal {
            background: #1b3c53;
            min-width: 20px;
            border-radius: 5px;
        }

        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0px;
        }

        QGroupBox {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #0e202d, stop:1 #0e202d);
            color: #d2c1b6;
            border: 1px solid #456882;
            border-radius: 10px;
            margin-top: 1.4ex;
            padding-top: 5px;
            font-size: 12px;
            font-weight: 500;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            left: 15px;
            top: -2px;
            padding: 0 5px;
            color: #d2c1b6;
        }

        QPushButton {
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #456882,
                stop:1 #1b3c53
            );
            color: #d2c1b6;
            border: 1px solid #456882;
            padding: 6px 15px;
            border-radius: 6px;
            font-weight: 600;
            font-size: 11px;
        }

        QPushButton:hover {
            background: #234c6a;
            color: #f5f5f5;
        }

        QPushButton:pressed {
            background: #d2c1b6;
            border: 1px solid #d2c1b6;
            color: #0e202d;
        }

        QLineEdit {
            background: #1b3c53;
            color: #d2c1b6;
            border: 1px solid #456882;
            border-radius: 5px;
            padding: 6px 8px;
            font-size: 11px;
            selection-background-color: #456882;
        }

        QSpinBox, QDoubleSpinBox {
            background: #1b3c53;
            color: #d2c1b6;
            border: 1px solid #1b3c53;
            border-radius: 5px;
            font-size: 15px;
            selection-background-color: #456882;
        }

        QSpinBox::up-button, QDoubleSpinBox::up-button {
            subcontrol-origin: border;
            subcontrol-position: top right;
            border-radius: 5px;
            background: #1b3c53;
        }

        QSpinBox::down-button, QDoubleSpinBox::down-button {
            subcontrol-origin: border;
            subcontrol-position: bottom right;
            border-radius: 5px;
            background: #1b3c53;
        }

        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover {
            background: #456882;
        }

        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background: #456882;
        }

        QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {
            image: url(:/images/up_white.png);
            width: 8px;
            height: 8px;
        }

        QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
            image: url(:/images/down_white.png);
            width: 8px;
            height: 8px;
        }

        QLineEdit:focus,
        QComboBox:focus,
        QSpinBox:focus,
        QDoubleSpinBox:focus {
            border: 1px solid #d2c1b6;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #234c6a, stop:1 #234c6a);
        }

        QSlider::groove:horizontal {
            border: 1px solid #456882;
            height: 6px;
            background: #0e202d;
            border-radius: 4px;
        }

        QSlider::handle:horizontal {
            background: #d2c1b6;
            border: 1px solid #d2c1b6;
            width: 16px;
            margin: -3px 0;
            border-radius: 8px;
        }

        QLabel {
            color: #d2c1b6;
            font-size: 11px;
            font-weight: 500;
        }

        QTextEdit {
            background: #0e202d;
            color: #d2c1b6;
            border: 1px solid #456882;
            border-radius: 6px;
            font-size: 11px;
            font-family: "Consolas", "Roboto Mono", monospace;
            padding: 6px;
        }

        QChartView {
            background: #0e202d;
            border: 1px solid #456882;
            border-radius: 10px;
            padding: 6px;
        }

        QTabWidget::pane {
            border: 1px solid #0e202d;
            background: #0e202d;
            padding: 4px;
        }

        QTabWidget::tab-bar {
            alignment: left;
        }

        QTabBar {
            background: #0e202d;
        }

        QTabBar::tab {
            background: #0e202d;
            color: #d2c1b6;
            padding: 2px 14px;
            margin: 4px 1px;
            border: 1px solid #0e202d;
            border-radius: 6px;
            font-weight: 600;
            width: 30px;
            text-align: center;
        }

        QTabBar QWidget {
            background: transparent;
        }

        QTabBar QLabel {
            background: transparent;
            margin: 0px;
            padding: 0px;
        }

        QTabBar::tab:selected {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                      stop:0 #456882, stop:1 #1b3c53);
            color: #d2c1b6;
            border: 1px solid #456882;
        }

        QTabBar::tab:hover:!selected {
            background: #1b3c53;
            color: #f5f5f5;
        }

        QPushButton#toggleTabsBtn {
            background: #1b3c53;
            color: #d2c1b6;
            border: none;
            padding: 8px;
            border-radius: 4px;
            font-weight: bold;
        }

        QPushButton#toggleTabsBtn:hover {
            background: #456882;
            color: #f5f5f5;
        }

        QWidget#contentArea {
            background: #1b3c53;
        }

        QWidget#leftPanel {
            background: #1b3c53;
            border-right: 1px solid #1b3c53;
        }

        QComboBox {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                        stop:0 #456882, stop:1 #1b3c53);
            color: #d2c1b6;
            border: 1px solid #0e202d !important;
            padding: 3px;
            border-radius: 4px;
        }

        QComboBox::drop-down {
            border: none;
            width: 20px;
        }

        QComboBox::down-arrow {
            width: 12px;
            height: 12px;
            image: none;
            border-left: 6px solid transparent;
            border-right: 6px solid transparent;
            border-top: 6px solid #d2c1b6;
        }

        QComboBox QAbstractItemView {
            background: #0e202d;
            color: #d2c1b6;
            border: 1px solid #1b3c53;
            selection-background-color: #1b3c53;
            selection-color: #d2c1b6;
        }

        QToolTip {
            background-color: #0e202d;
            color: #d2c1b6;
            border: 1px solid #456882;
            border-radius: 6px;
            padding: 12px;
            font-family: 'Segoe UI', system-ui;
            font-size: 13px;
            opacity: 230;
        }

        QMenu {
            background-color: #0e202d;
            border: 1px solid #1b3c53;
            border-radius: 5px;
            padding: 5px 0px;
        }

        QMenu::item {
            padding: 8px 30px 8px 20px;
            color: #d2c1b6;
            background-color: transparent;
        }

        QMenu::item:selected {
            background-color: #1b3c53;
            color: #d2c1b6;
        }

        QMenu::item:checked {
            background-color: #456882;
            color: #d2c1b6;
        }

        QMenu::separator {
            height: 1px;
            background-color: #456882;
            margin: 5px 0px;
        }
    )");
#endif

  auto *mainWidget = new QWidget(this);
  auto *mainLayout = new QHBoxLayout(mainWidget);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setSpacing(0);

  auto *leftPanel = new QWidget(mainWidget);
  leftPanel->setMinimumWidth(100);
  leftPanel->setMaximumWidth(200);
  leftPanel->setObjectName("leftPanel");
  leftPanel->setStyleSheet("QWidget#leftPanel { background: #f5f5f5}");

  auto *leftPanelLayout = new QVBoxLayout(leftPanel);
  leftPanelLayout->setContentsMargins(0, 0, 0, 0);
  leftPanelLayout->setSpacing(0);

  _tabWidget = new QTabWidget(leftPanel);
  _tabWidget->setTabPosition(QTabWidget::West);

  _homeTab = new QWidget();
  _analyzeTab = new QWidget();
  _settingTab = new QWidget();

  QLabel *homeLabel = new QLabel();
  QPixmap homePixmap(":/images/CTUAV.png");
  homeLabel->setFixedSize(40, 35);
  homeLabel->setPixmap(homePixmap.scaled(homeLabel->size(), Qt::KeepAspectRatio,
                                         Qt::SmoothTransformation));
  homeLabel->setAlignment(Qt::AlignTop);

  QLabel *analyzeLabel = new QLabel();
  QPixmap analPixmap(":/images/Anal.png");
  QPixmap smallPixmap =
      analPixmap.scaled(35, 35, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  QIcon analIcon(smallPixmap);
  analyzeLabel->setPixmap(analIcon.pixmap(40, 40));
  analyzeLabel->setFixedSize(40, 40);
  analyzeLabel->setAlignment(Qt::AlignTop);

  QLabel *settingLabel = new QLabel();
  QPixmap settingPixmap(":/images/setting.png");
  QPixmap smallsettingPixmap = settingPixmap.scaled(35, 35, Qt::KeepAspectRatio,
                                                    Qt::SmoothTransformation);
  QIcon settingIcon(smallsettingPixmap);
  settingLabel->setPixmap(settingIcon.pixmap(40, 40));
  settingLabel->setFixedSize(40, 45);
  settingLabel->setAlignment(Qt::AlignTop);

  _tabWidget->addTab(_homeTab, "");
  _tabWidget->addTab(_analyzeTab, "");
  _tabWidget->addTab(_settingTab, "");

  _tabWidget->tabBar()->setTabButton(0, QTabBar::LeftSide, homeLabel);
  _tabWidget->tabBar()->setTabButton(1, QTabBar::LeftSide, analyzeLabel);
  _tabWidget->tabBar()->setTabButton(2, QTabBar::LeftSide, settingLabel);

  mainLayout->addWidget(_tabWidget, 1);

  setCentralWidget(mainWidget);
  resize(1400, 900);
  setWindowTitle(tr("THRUST STAND SYSTEM"));

  _sidebarCollapsed = false;

  addThemeToggleButton();
  addToggleSwitch();

  setupHomeTab();
  setupAnalyzeTab();
  setupSettingTab();
}

void MainWindow::reset() {
  _thrustLabel->setText(QString("Thrust: %1 N").arg(thrustValue, 0, 'f', 1));
  _pwmLabel->setText(QString("PWM: %1").arg(pwmValue, 0, 'f', 1));
  _voltageLabel->setText(QString("Voltage: %1 V").arg(voltageValue, 0, 'f', 1));
  _currentLabel->setText(QString("Current: %1 A").arg(currentValue, 0, 'f', 1));
  _RPMLabel->setText(QString("RPM: %1").arg(RPMValue));
}

MainWindow::~MainWindow() {
  if (_serialPort != nullptr) {
    _serialPort->close();
    delete _serialPort;
  }

  if (_vescSerialPort != nullptr) {
    if (_vescPollTimer)
      _vescPollTimer->stop();
    _vescSerialPort->close();
    delete _vescSerialPort;
  }
}
