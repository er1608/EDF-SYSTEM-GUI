#include "mainwindow.h"
#include "dashboardwidget.h"
#include <QtCharts>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _serialPort(nullptr)
    , _plotting(false)
    , _chart(nullptr)
    , _throttleSeries(nullptr)
    , _startTime(QTime::currentTime())
    , _isDarkMode(true)
{
    setStyleSheet(R"(
        QMainWindow {
            background: #1b2432;
            color: #1b2432;
            font-family: "Segoe UI", "Roboto", sans-serif;
        }

        QChartView {
            background: #1b2432;
            border: none;
            border-radius: 10px;
            padding: 6px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3a3f66, stop:1 #2d3250);
        }

        QGroupBox {
            background: #1b2432;
            color: #e2e8f0;
            border: 1px solid #2d3250;
            border-radius: 10px;
            margin-top: 1.4ex;
            padding-top: 14px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1b2432, stop:1 #1b2432);
            font-size: 12px;
            font-weight: 500;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top center;
            padding: 6px 18px;
            background: qlineargradient(
                x1:0, y1:0, x2:1, y2:0,
                stop:0 #2d3250,
                stop:1 #3a3f66
            );
            color: #a6b1e1;
            border-radius: 6px;
            font-weight: 600;
        }

        QPushButton {
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #3a3f66,
                stop:1 #2d3250
            );
            color: #e2e8f0;
            border: 1px solid #2d3250;
            padding: 8px 16px;
            border-radius: 6px;
            font-weight: 600;
            font-size: 11px;
        }

        QPushButton:hover {
            background: #2d3b5a;
            color: white;
        }

        QPushButton:pressed {
            background: #00b4d8;
            border: 1px solid #00b4d8;
        }

        QLineEdit, QComboBox {
            background: #2d3250;
            color: #e2e8f0;
            border: 1px solid #2d3250;
            border-radius: 5px;
            padding: 6px 8px;
            font-size: 11px;
            selection-background-color: #00b4d8;
        }

        QSpinBox, QDoubleSpinBox {
            background: #2d3250;
            color: #e2e8f0;
            border: 1px solid #2d3250;
            border-radius: 5px;
            padding: 6px 8px;
            font-size: 11px;
            selection-background-color: #00b4d8;
        }

        QSpinBox::up-button, QDoubleSpinBox::up-button {
            background: #3a3f66;
            border: 1px solid #2d3250;
            border-top-right-radius: 4px;
        }

        QSpinBox::down-button, QDoubleSpinBox::down-button {
            background: #3a3f66;
            border: 1px solid #2d3250;
            border-bottom-right-radius: 4px;
        }

        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover {
            background: #00b4d8;
        }

        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background: #00b4d8;
        }

        QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {
            width: 0px;
            height: 0px;
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-bottom: 5px solid #a6b1e1;
        }

        QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
            width: 0px;
            height: 0px;
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 5px solid #a6b1e1;
        }

        QLineEdit:focus,
        QComboBox:focus,
        QSpinBox:focus,
        QDoubleSpinBox:focus {
            border: 1px solid #00b4d8;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1b2432, stop:1 #1b2432);
        }

        QSlider::groove:horizontal {
            border: 1px solid #2d3250;
            height: 6px;
            background: #1d2533;
            border-radius: 4px;
        }

        QSlider::handle:horizontal {
            background: #00b4d8;
            border: 1px solid #00b4d8;
            width: 16px;
            margin: -3px 0;
            border-radius: 8px;
        }

        QLabel {
            color: #a6b1e1;
            font-size: 11px;
            font-weight: 500;
        }

        QTextEdit {
            background: #121722;
            color: #e2e8f0;
            border: 1px solid #2d3250;
            border-radius: 6px;
            font-size: 11px;
            font-family: "Consolas", "Roboto Mono", monospace;
            padding: 6px;
        }

        QChartView {
            background: #1b2432;
            border: 1px solid #2d3250;
            border-radius: 10px;
            padding: 6px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1b2432, stop:1 #1b2432);
        }

        QTabWidget::pane {
            border: 1px solid #1b2432;
            border-radius: 8px;
            background: #1b2432;
            padding: 4px;
        }

        QTabWidget::tab-bar {
            alignment: left;
            width: 69px;
        }

        QTabBar {
            background: #1b2432;
        }

        QTabBar::tab {
            background: #1b2432;
            color: #a6b1e1;
            padding: 2px 14px;
            margin: 4px 1px;
            border: 1px solid #1b2432;
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
                                      stop:0 #3a3f66, stop:1 #2d3250);
            color: #ffffff;
            border: 1px solid #00b4d8;
        }

        QTabBar::tab:hover:!selected {
            background: #2d3250;
            color: #ffffff;
        }

        QPushButton#toggleTabsBtn {
            background: #2d3250;
            color: #a6b1e1;
            border: none;
            padding: 8px;
            border-radius: 4px;
            font-weight: bold;
        }

        QPushButton#toggleTabsBtn:hover {
            background: #3a3f66;
            color: #ffffff;
        }

        QWidget#contentArea {
            background: #2d3250;
        }

        QWidget#leftPanel {
            background: #2d3250;
            border-right: 1px solid #2d3250;
        }
    )");

    auto *mainWidget = new QWidget(this);
    auto *mainLayout = new QHBoxLayout(mainWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto *leftPanel = new QWidget(mainWidget);
    leftPanel->setMinimumWidth(100);
    leftPanel->setMaximumWidth(200);
    leftPanel->setObjectName("leftPanel");
    leftPanel->setStyleSheet("QWidget#leftPanel { background: #ffffff}");

    auto *leftPanelLayout = new QVBoxLayout(leftPanel);
    leftPanelLayout->setContentsMargins(0,0,0,0);
    leftPanelLayout->setSpacing(0);

    _tabWidget = new QTabWidget(leftPanel);
    _tabWidget->setTabPosition(QTabWidget::West);

    _homeTab = new QWidget();
    _analyzeTab = new QWidget();

    QLabel *homeLabel = new QLabel("🏠");
    QPixmap homePixmap(":/images/CTUAV.png");
    homeLabel->setFixedSize(40, 35);
    homeLabel->setPixmap(homePixmap.scaled(homeLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    homeLabel->setAlignment(Qt::AlignTop);

    QLabel *analyzeLabel = new QLabel("📊");
    analyzeLabel->setAlignment(Qt::AlignCenter);
    analyzeLabel->setFixedSize(50, 35);
    analyzeLabel->setStyleSheet("font-size: 25px;");

    _tabWidget->addTab(_homeTab, "");
    _tabWidget->addTab(_analyzeTab, "");

    _tabWidget->tabBar()->setTabButton(0, QTabBar::LeftSide, homeLabel);
    _tabWidget->tabBar()->setTabButton(1, QTabBar::LeftSide, analyzeLabel);

    mainLayout->addWidget(_tabWidget, 1);

    setCentralWidget(mainWidget);
    resize(1400, 900);
    setWindowTitle(tr("EDF SYSTEM"));

    _sidebarCollapsed = false;

    addThemeToggleButton();

    setupHomeTab();
    setupAnalyzeTab();
}

void MainWindow::addThemeToggleButton()
{
    QPushButton *themeToggleBtn = new QPushButton(this);
    themeToggleBtn->setText("🌙");
    themeToggleBtn->setFixedSize(50, 50);
    themeToggleBtn->setObjectName("themeToggleBtn");

    QFont font = themeToggleBtn->font();
    font.setPointSize(30);
    font.setBold(true);
    themeToggleBtn->setFont(font);

    themeToggleBtn->setStyleSheet(R"(
        QPushButton#themeToggleBtn {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                      stop:0 #2d3250, stop:1 #CFAB3E);
            color: #e2e8f0;
            border: 2px solid #2d3748;
            border-radius: 25px;
            font-weight: bold;
        }
        QPushButton#themeToggleBtn:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                      stop:0 #9AD16B, stop:1 #CFAB3E);
            border: 2px solid #00b4d8;
        }
        QPushButton#themeToggleBtn:pressed {
            background: #00b4d8;
            border: 2px solid #00b4d8;
        }
    )");

    themeToggleBtn->move(15, this->height() - 70);

    connect(themeToggleBtn, &QPushButton::clicked, this, &MainWindow::toggleTheme);

    connect(this, &MainWindow::resized, this, [this, themeToggleBtn]() {
        themeToggleBtn->move(15, this->height() - 70);
    });
}

void MainWindow::toggleTheme()
{
    _isDarkMode = !_isDarkMode;

    if (_isDarkMode) {
        applyDarkTheme();
    } else {
        applyLightTheme();
    }

    // Update button text
    QPushButton *themeBtn = findChild<QPushButton*>("themeToggleBtn");
    if (themeBtn) {
        themeBtn->setText(_isDarkMode ? "🌙" : "☀️");
    }
}

void MainWindow::applyDarkTheme()
{
    setStyleSheet(R"(
        QMainWindow {
            background: #1b2432;
            color: #e2e8f0;
            font-family: "Segoe UI", "Roboto", sans-serif;
        }

        QChartView {
            background: #1b2432;
            border: none;
            border-radius: 10px;
            padding: 6px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3a3f66, stop:1 #2d3250);
        }

        QGroupBox {
            background: #1b2432;
            color: #e2e8f0;
            border: 1px solid #2d3250;
            border-radius: 10px;
            margin-top: 1.4ex;
            padding-top: 14px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1b2432, stop:1 #1b2432);
            font-size: 12px;
            font-weight: 500;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top center;
            padding: 6px 18px;
            background: qlineargradient(
                x1:0, y1:0, x2:1, y2:0,
                stop:0 #2d3250,
                stop:1 #3a3f66
            );
            color: #a6b1e1;
            border-radius: 6px;
            font-weight: 600;
        }

        QPushButton {
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #3a3f66,
                stop:1 #2d3250
            );
            color: #e2e8f0;
            border: 1px solid #2d3250;
            padding: 8px 16px;
            border-radius: 6px;
            font-weight: 600;
            font-size: 11px;
        }

        QPushButton:hover {
            background: #2d3b5a;
            color: white;
        }

        QPushButton:pressed {
            background: #00b4d8;
            border: 1px solid #00b4d8;
        }

        QLineEdit, QComboBox {
            background: #2d3250;
            color: #e2e8f0;
            border: 1px solid #2d3250;
            border-radius: 5px;
            padding: 6px 8px;
            font-size: 11px;
            selection-background-color: #00b4d8;
        }

        QSpinBox, QDoubleSpinBox {
            background: #2d3250;
            color: #e2e8f0;
            border: 1px solid #2d3250;
            border-radius: 5px;
            padding: 6px 8px;
            font-size: 11px;
            selection-background-color: #00b4d8;
        }

        QSpinBox::up-button, QDoubleSpinBox::up-button {
            background: #3a3f66;
            border: 1px solid #2d3250;
            border-top-right-radius: 4px;
        }

        QSpinBox::down-button, QDoubleSpinBox::down-button {
            background: #3a3f66;
            border: 1px solid #2d3250;
            border-bottom-right-radius: 4px;
        }

        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover {
            background: #00b4d8;
        }

        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background: #00b4d8;
        }

        QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {
            width: 0px;
            height: 0px;
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-bottom: 5px solid #a6b1e1;
        }

        QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
            width: 0px;
            height: 0px;
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 5px solid #a6b1e1;
        }

        QLineEdit:focus,
        QComboBox:focus,
        QSpinBox:focus,
        QDoubleSpinBox:focus {
            border: 1px solid #00b4d8;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1b2432, stop:1 #1b2432);
        }

        QSlider::groove:horizontal {
            border: 1px solid #2d3250;
            height: 6px;
            background: #1d2533;
            border-radius: 4px;
        }

        QSlider::handle:horizontal {
            background: #00b4d8;
            border: 1px solid #00b4d8;
            width: 16px;
            margin: -3px 0;
            border-radius: 8px;
        }

        QLabel {
            color: #a6b1e1;
            font-size: 11px;
            font-weight: 500;
        }

        QTextEdit {
            background: #121722;
            color: #e2e8f0;
            border: 1px solid #2d3250;
            border-radius: 6px;
            font-size: 11px;
            font-family: "Consolas", "Roboto Mono", monospace;
            padding: 6px;
        }

        QChartView {
            background: #1b2432;
            border: 1px solid #2d3250;
            border-radius: 10px;
            padding: 6px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1b2432, stop:1 #1b2432);
        }

        QTabWidget::pane {
            border: 1px solid #1b2432;
            border-radius: 8px;
            background: #1b2432;
            padding: 4px;
        }

        QTabWidget::tab-bar {
            alignment: left;
            width: 69px;
        }

        QTabBar {
            background: #1b2432;
        }

        QTabBar::tab {
            background: #1b2432;
            color: #a6b1e1;
            padding: 2px 14px;
            margin: 4px 1px;
            border: 1px solid #1b2432;
            border-radius: 6px;
            font-weight: 600;
            width: 30px;
            text-align: left;
        }

        QTabBar::tab:selected {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                                      stop:0 #3a3f66, stop:1 #2d3250);
            color: #ffffff;
            border: 1px solid #00b4d8;
        }

        QTabBar::tab:hover:!selected {
            background: #2d3250;
            color: #ffffff;
        }

        QPushButton#toggleTabsBtn {
            background: #2d3250;
            color: #a6b1e1;
            border: none;
            padding: 8px;
            border-radius: 4px;
            font-weight: bold;
        }

        QPushButton#toggleTabsBtn:hover {
            background: #3a3f66;
            color: #ffffff;
        }

        QWidget#contentArea {
            background: #2d3250;
        }

        QWidget#leftPanel {
            background: #2d3250;
            border-right: 1px solid #2d3250;
        }
    )");
}

void MainWindow::applyLightTheme()
{
    setStyleSheet(R"(
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f8f9ff, stop:1 #e8ebff);
            color: #2d3748;
            font-family: "Segoe UI", "Roboto", sans-serif;
        }

        QChartView {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ffffff, stop:1 #f0f4ff);
            border: 1px solid #d1d9ff;
            border-radius: 12px;
            padding: 8px;
        }

        QGroupBox {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ffffff, stop:1 #f8f9ff);
            color: #2d3748;
            border: 1px solid #e2e8ff;
            border-radius: 12px;
            margin-top: 1.4ex;
            padding-top: 16px;
            font-size: 12px;
            font-weight: 500;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top center;
            padding: 8px 20px;
            background: qlineargradient(
                x1:0, y1:0, x2:1, y2:0,
                stop:0 #65BADB,
                stop:1 #057A9E
            );
            color: #ffffff;
            border-radius: 8px;
            font-weight: 600;
            border: 1px solid #c7d2fe;
            font-size: 11px;
        }

        QPushButton {
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #65BADB,
                stop:1 #057A9E
            );
            color: #ffffff;
            border: 1px solid #65BADB;
            padding: 10px 18px;
            border-radius: 8px;
            font-weight: 600;
            font-size: 11px;
        }

        QPushButton:hover {
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #6366f1,
                stop:1 #057A9E
            );
            border: 1px solid #057A9E;
        }

        QPushButton:pressed {
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #057A9E,
                stop:1 #4338ca
            );
            border: 1px solid #4338ca;
            padding: 9px 17px;
        }

        QLineEdit, QComboBox {
            background: #ffffff;
            color: #374151;
            border: 1px solid #d1d5ff;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 11px;
            selection-background-color: #c7d2fe;
        }

        QSpinBox, QDoubleSpinBox {
            background: #ffffff;
            color: #374151;
            border: 1px solid #d1d5ff;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 11px;
            selection-background-color: #c7d2fe;
        }

        QSpinBox::up-button, QDoubleSpinBox::up-button {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #c7d2fe, stop:1 #a5b4fc);
            border: 1px solid #a5b4fc;
            border-top-right-radius: 5px;
            width: 20px;
        }

        QSpinBox::down-button, QDoubleSpinBox::down-button {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #c7d2fe, stop:1 #a5b4fc);
            border: 1px solid #a5b4fc;
            border-bottom-right-radius: 5px;
            width: 20px;
        }

        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover,
        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #a5b4fc, stop:1 #65BADB);
        }

        QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {
            width: 0px;
            height: 0px;
            image: none;
            border-left: 4px solid transparent;
            border-right: 4px solid transparent;
            border-bottom: 4px solid #057A9E;
        }

        QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
            width: 0px;
            height: 0px;
            image: none;
            border-left: 4px solid transparent;
            border-right: 4px solid transparent;
            border-top: 4px solid #057A9E;
        }

        QLineEdit:focus,
        QComboBox:focus,
        QSpinBox:focus,
        QDoubleSpinBox:focus {
            border: 1px solid #65BADB;
            background: #fafbff;
        }

        QSlider::groove:horizontal {
            border: 1px solid #d1d5ff;
            height: 6px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #c7d2fe, stop:1 #a5b4fc);
            border-radius: 4px;
        }

        QSlider::handle:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #65BADB, stop:1 #6366f1);
            border: 1px solid #057A9E;
            width: 18px;
            margin: -3px 0;
            border-radius: 9px;
        }

        QLabel {
            color: #4b5563;
            font-size: 11px;
            font-weight: 500;
        }

        QTextEdit {
            background: #ffffff;
            color: #374151;
            border: 1px solid #d1d5ff;
            border-radius: 8px;
            font-size: 11px;
            font-family: "Consolas", "Roboto Mono", monospace;
            padding: 8px;
        }

        QTabWidget::pane {
            border-radius: 8px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f8f9ff, stop:1 #e8ebff);
            padding: 6px;
        }

        QTabWidget::tab-bar {
            alignment: left;
        }

        QTabBar {
            background: transparent;
        }

        QTabBar::tab {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #f1f5ff, stop:1 #e8ebff);
            color: #6b7280;
            padding: 2px 14px;
            margin: 4px 2px;

            border-radius: 6px;
            font-weight: 600;
            width: 30px;
            text-align: left;
        }

        QTabBar::tab:selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2B3D2B, stop:1 #324F02);
            color: #ffffff;
            border: 1px solid #6366f1;
        }

        QTabBar::tab:hover:!selected {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #e0e7ff, stop:1 #c7d2fe);
            color: #057A9E;
            border: 1px solid #c7d2fe;
        }

        QPushButton#toggleTabsBtn {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #65BADB, stop:1 #8b5cf6);
            color: #ffffff;
            border: 1px solid #8b5cf6;
            padding: 10px;
            border-radius: 6px;
            font-weight: bold;
        }

        QPushButton#toggleTabsBtn:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #8b5cf6, stop:1 #7c3aed);
        }

        QWidget#contentArea {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f8f9ff, stop:1 #e8ebff);
        }

        QWidget#leftPanel {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #f0f4ff, stop:1 #e8ebff);
            border-right: 1px solid #d1d9ff;
        }

        QProgressBar {
            border: 1px solid #d1d5ff;
            border-radius: 6px;
            background: #ffffff;
            text-align: center;
            color: #4b5563;
        }

        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #65BADB, stop:1 #6366f1);
            border-radius: 5px;
        }

        QCheckBox {
            color: #4b5563;
            spacing: 8px;
        }

        QCheckBox::indicator {
            width: 16px;
            height: 16px;
            border: 1px solid #d1d5ff;
            border-radius: 4px;
            background: #ffffff;
        }

        QCheckBox::indicator:checked {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #65BADB, stop:1 #6366f1);
            border: 1px solid #6366f1;
        }

        QCheckBox::indicator:checked:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #6366f1, stop:1 #057A9E);
        }

        QScrollBar:vertical {
            background: #f0f4ff;
            width: 12px;
            margin: 0px;
        }

        QScrollBar::handle:vertical {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #c7d2fe, stop:1 #a5b4fc);
            border-radius: 6px;
            min-height: 20px;
        }

        QScrollBar::handle:vertical:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #a5b4fc, stop:1 #65BADB);
        }

        QTableView {
            background: #ffffff;
            border: 1px solid #d1d5ff;
            border-radius: 6px;
            gridline-color: #e2e8ff;
            selection-background-color: #c7d2fe;
        }

        QHeaderView::section {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #f0f4ff, stop:1 #e8ebff);
            color: #4b5563;
            padding: 6px;
            border: 1px solid #e2e8ff;
            font-weight: 600;
        }
    })");
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    emit resized();
}

void MainWindow::saveDataToCSV()
{
    QString fileName = QDateTime::currentDateTime().toString("'system_data_'yyyy-MM-dd_hh-mm-ss'.csv'");
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        // Write header
        stream << "Timestamp,Throttle,PWM\n";

        // Write data
        for (auto it = _dataBuffer.begin(); it != _dataBuffer.end(); ++it) {
            const auto &data = *it;
            stream << data.timestamp << "," << data.throttle << "," << data.pwm << "\n";
        }

        file.close();

        // Thêm vào log
        QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
        _logTextEdit->append(QString("[%1] DATA SAVED: %2 (%3 records)").arg(timestamp, fileName, QString::number(_dataBuffer.size())));

        QMessageBox::information(this, "Success", QString("Data saved to %1\n%2 records written").arg(fileName, QString::number(_dataBuffer.size())));
    } else {
        QMessageBox::critical(this, "Error", "Could not save data to file");
    }
}

void MainWindow::updateAnalyzeCharts(double temp, double current, double voltage, double RPM)
{
    // double tempValue = throttle * 0.8 + 20 + (rand() % 10 - 5) * 0.1;
    // double voltValue = throttle * 0.24 + 12 + (rand() % 10 - 5) * 0.05;
    // double currentValue = throttle * 0.1 + 2 + (rand() % 10 - 5) * 0.02;

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
    double key = _startTime.msecsTo(QTime::currentTime()) / 1000.0;

    _throttleSeries->append(key, throttle);

    if (_throttleSeries->count() > 1200) {
        _throttleSeries->remove(0);
    }

    // qDebug() << "Start Time: " << _startTime;
    // qDebug() << "Key: " << key;

    _throttleLabel->setText(QString("Throttle: %1").arg(throttle, 0, 'f', 1));
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

void MainWindow::readData()
{
    if (!_serialPort)
        return;

    if (_serialPort->canReadLine())
    {
        const QByteArray payload = _serialPort->readLine();
        QString data = QString::fromUtf8(payload).trimmed();

        QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
        _logTextEdit->append(QString("[%1] RECV: %2").arg(timestamp, data));

        // qDebug() << "UART data:" << data;

        bool throttleOk = false, pwmOk = false, currentOk = false, voltageOk = false, temperatureOk = false, RPMOk = false;

        QStringList parts = data.split(',');

        for (const QString &part : std::as_const(parts)) {
            QString trimmedPart = part.trimmed();

            if (trimmedPart.startsWith("Temp:")) {
                QString valueStr = trimmedPart.split(':')[1].trimmed();
                temperatureValue = valueStr.toDouble(&temperatureOk);
            }
            else if (trimmedPart.startsWith("Voltage:")) {
                QString valueStr = trimmedPart.split(':')[1].trimmed();
                voltageValue = valueStr.toDouble(&voltageOk);
            }
            else if (trimmedPart.startsWith("Current:")) {
                QString valueStr = trimmedPart.split(':')[1].trimmed();
                currentValue = valueStr.toDouble(&currentOk);
            }
            else if (trimmedPart.startsWith("RPM:")) {
                QString valueStr = trimmedPart.split(':')[1].trimmed();
                RPMValue = valueStr.toDouble(&RPMOk);
            }
            else if (trimmedPart.startsWith("Throttle:")) {
                QString valueStr = trimmedPart.split(':')[1].trimmed();
                throttleValue = valueStr.toDouble(&throttleOk);
            }
            else if (trimmedPart.startsWith("PWM:")) {
                QString valueStr = trimmedPart.split(':')[1].trimmed();
                pwmValue = valueStr.toDouble(&pwmOk);
            }
        }

        if (_plotting && (temperatureOk || voltageOk || currentOk || RPMOk || throttleOk || pwmOk)) {
            updateAnalyzeCharts(temperatureValue, currentValue, voltageValue, RPMValue);
            updatePlot(throttleValue, pwmValue);

        }
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

    _throttleSeries->setName("Throttle");
    QPen pen(QColor(65, 90, 119), 3);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    _throttleSeries->setPen(pen);

    _chart->addSeries(_throttleSeries);
    _chart->setTitle("Throttle");
    _chart->setTitleBrush(QBrush(QColor(226, 232, 240))); // Light text
    _chart->legend()->setVisible(false);
    _chart->setBackgroundBrush(QBrush(QColor(27, 36, 50))); // Graphite blue
    _chart->setTheme(QChart::ChartThemeBlueCerulean);

    // Tạo axes
    auto *axisX = new QValueAxis();
    auto *axisY = new QValueAxis();

    axisX->setTitleText("Time(s)");
    axisY->setTitleText("%");
    axisX->setRange(-30, 0);
    axisY->setRange(0, 100);

    // Thiết lập màu cho axes
    axisX->setLabelsColor(QColor(166, 177, 225)); // Fog blue
    axisY->setLabelsColor(QColor(166, 177, 225));
    axisX->setTitleBrush(QBrush(QColor(226, 232, 240))); // Light text
    axisY->setTitleBrush(QBrush(QColor(226, 232, 240)));
    axisX->setGridLineColor(QColor(45, 50, 80, 100)); // Deep steel
    axisY->setGridLineColor(QColor(45, 50, 80, 100));

    _chart->addAxis(axisX, Qt::AlignBottom);
    _chart->addAxis(axisY, Qt::AlignLeft);

    _throttleSeries->attachAxis(axisX);
    _throttleSeries->attachAxis(axisY);

    auto *chartView = new QChartView(_chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}

void MainWindow::setupAnalyzeTab()
{
    // _analyzeTab->setStyleSheet("background: #1b2432;");
    auto *analyzeLayout = new QVBoxLayout(_analyzeTab);
    analyzeLayout->setContentsMargins(10, 10, 10, 10);
    analyzeLayout->setSpacing(10);

    // Tạo các series cho analyze tab
    _series1 = new QLineSeries();
    _series2 = new QLineSeries();
    _series3 = new QLineSeries();
    _series4 = new QLineSeries();

    // Tạo các biểu đồ
    auto *tempChart = createAnalyzeChart("Temperature", "°C", _series1);
    auto *voltChart = createAnalyzeChart("Voltage", "V", _series2);
    auto *currentChart = createAnalyzeChart("Current", "A", _series3);
    auto *RPMChart = createAnalyzeChart("RPM", "R/M", _series4);

    // Tạo layout lưới cho 3 biểu đồ
    auto *chartsLayout = new QGridLayout();
    chartsLayout->addWidget(tempChart, 0, 0);
    chartsLayout->addWidget(voltChart, 0, 1);
    chartsLayout->addWidget(currentChart, 1, 0);
    chartsLayout->addWidget(RPMChart, 1, 1);

    // Control panel cho analyze tab
    auto *controlGroup = new QGroupBox("Analyze Controls", _analyzeTab);
    auto *controlLayout = new QHBoxLayout(controlGroup);

    auto *clearChartsBtn = new QPushButton("Clear Charts", controlGroup);
    auto *exportDataBtn = new QPushButton("Export Data", controlGroup);

    controlLayout->addWidget(clearChartsBtn);
    controlLayout->addWidget(exportDataBtn);
    controlLayout->addStretch();

    analyzeLayout->addLayout(chartsLayout, 1);
    analyzeLayout->addWidget(controlGroup);

    // Kết nối signals
    connect(clearChartsBtn, &QPushButton::clicked, this, [this]() {
        _series1->clear();
        _series2->clear();
        _series3->clear();
        _series4->clear();
        _logTextEdit->append("[ANALYZE] Charts cleared");
    });

    connect(exportDataBtn, &QPushButton::clicked, this, &MainWindow::saveDataToCSV);
}

void MainWindow::setupHomeTab()
{
    // _homeTab->setStyleSheet("background: #1b2432;");
    // Tạo layout chính cho home tab
    auto *homeLayout = new QVBoxLayout(_homeTab);
    homeLayout->setContentsMargins(10, 10, 10, 10);
    homeLayout->setSpacing(10);

    auto *miniDashboard = new DashBoardWidget(_homeTab);
    miniDashboard->setFixedSize(260, 260);
    miniDashboard->setBorderColor(QColor(45, 50, 80));
    miniDashboard->setBorderWidth(2);
    miniDashboard->setBorderRadius(10);
    miniDashboard->setArcColor(QColor(65, 90, 119, 180));
    miniDashboard->setScaleColor(QColor(255, 107, 107));
    miniDashboard->setPointerColor(QColor(255, 107, 107));
    miniDashboard->setValueColor(QColor(226, 232, 240));
    miniDashboard->setTitleColor(QColor(255, 107, 107));
    // miniDashboard->setBackgroundColor(QColor(13, 17, 23, 100));

    auto *mainChartView = setupMainChart();

    // Tạo các control widgets
    auto *valueSlider = new QSlider(Qt::Horizontal, _homeTab);
    valueSlider->setRange(0, 100);
    valueSlider->setValue(0);

    auto *valueSpinBox = new QDoubleSpinBox(_homeTab);
    valueSpinBox->setRange(0, 100);
    valueSpinBox->setValue(0);
    valueSpinBox->setDecimals(2);

    auto *minValueLabel = new QLabel(tr("Min Value:"), _homeTab);
    auto *minValueSpinBox = new QDoubleSpinBox(_homeTab);
    minValueSpinBox->setRange(-1000, 1000);
    minValueSpinBox->setValue(0);
    minValueSpinBox->setDecimals(2);

    auto *maxValueLabel = new QLabel(tr("Max Value:"), _homeTab);
    auto *maxValueSpinBox = new QDoubleSpinBox(_homeTab);
    maxValueSpinBox->setRange(-1000, 1000);
    maxValueSpinBox->setValue(100);
    maxValueSpinBox->setDecimals(2);

    auto *clearPlotButton = new QPushButton(tr("Clear Plot"), _homeTab);
    auto *startPlotButton = new QPushButton(tr("Start Plot"), _homeTab);
    auto *stopPlotButton = new QPushButton(tr("Stop Plot"), _homeTab);
    auto *saveDataButton = new QPushButton(tr("Save Data to CSV"), _homeTab);

    auto *pwmValueLabel = new QLabel(tr("PWM: 0"), _homeTab);
    auto *throttleValueLabel = new QLabel(tr("Throttle: 0"), _homeTab);
    pwmValueLabel->setStyleSheet("font-weight: bold; color: #5E9C43; font-size: 13px; padding: 6px; border-radius: 4px;");
    throttleValueLabel->setStyleSheet("font-weight: bold; color: #ff6666; font-size: 13px; padding: 6px; border-radius: 4px;");

    auto *logTextEdit = new QTextEdit(_homeTab);
    logTextEdit->setMaximumHeight(100);
    logTextEdit->setReadOnly(true);

    auto *topContentLayout = new QHBoxLayout();

    auto *leftPanel = new QWidget(_homeTab);
    auto *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setAlignment(Qt::AlignTop);
    leftLayout->setSpacing(10);

    // Layout chính cho hiển thị biểu đồ
    auto *displayLayout = new QVBoxLayout();
    displayLayout->setSpacing(10);

    // Biểu đồ chính - THÊM STRETCH FACTOR
    displayLayout->addWidget(mainChartView, 1);

    // Layout cho các control dưới biểu đồ
    auto *bottomControlsLayout = new QHBoxLayout();
    bottomControlsLayout->setSpacing(10);

    auto *valueGroup = new QGroupBox(tr("Value Settings"), _homeTab);
    auto *valueControlLayout = new QGridLayout(valueGroup);
    valueControlLayout->addWidget(new QLabel(tr("Current Value:")), 0, 0);
    valueControlLayout->addWidget(valueSlider, 0, 1);
    valueControlLayout->addWidget(valueSpinBox, 0, 2);
    valueControlLayout->addWidget(minValueLabel, 1, 0);
    valueControlLayout->addWidget(minValueSpinBox, 1, 1, 1, 2);
    valueControlLayout->addWidget(maxValueLabel, 2, 0);
    valueControlLayout->addWidget(maxValueSpinBox, 2, 1, 1, 2);

    auto *plotControlGroup = new QGroupBox(tr("Plot Control"), _homeTab);
    auto *plotControlLayout = new QGridLayout(plotControlGroup);
    plotControlLayout->addWidget(startPlotButton, 0, 0);
    plotControlLayout->addWidget(stopPlotButton, 0, 1);
    plotControlLayout->addWidget(clearPlotButton, 1, 0);
    plotControlLayout->addWidget(saveDataButton, 1, 1);
    plotControlLayout->addWidget(throttleValueLabel, 2, 0, 1, 2);
    plotControlLayout->addWidget(pwmValueLabel, 3, 0, 1, 2);

    bottomControlsLayout->addWidget(valueGroup, 2);
    bottomControlsLayout->addWidget(plotControlGroup, 1);

    displayLayout->addLayout(bottomControlsLayout);

    // UART Connection
    auto *connectionGroup = new QGroupBox(tr("Connection"), _homeTab);
    auto *connectionLayout = new QGridLayout(connectionGroup);
    auto *portComboBox = new QComboBox(_homeTab);
    auto *connectButton = new QPushButton(tr("Connect"), _homeTab);

    // qDebug() << "=== SCANNING SERIAL PORTS ===";

    // Physical ports
    // qDebug() << "Physical ports from QSerialPortInfo:";
    foreach (auto &port, QSerialPortInfo::availablePorts()) {
        portComboBox->addItem(port.portName());
        // qDebug() << "  - " << port.portName();
    }

#ifdef Q_OS_LINUX
    // qDebug() << "Scanning /dev for virtual ports...";

    QDir dir("/dev");

    QStringList nameFilters;
    nameFilters << "pts/*" << "pts[0-9]*" << "pts*";

    QStringList ptsPorts = dir.entryList(nameFilters, QDir::System);
    // qDebug() << "Found pts ports with filter:" << ptsPorts;

    // qDebug() << "Trying QDirIterator...";
    QDirIterator it("/dev", QStringList() << "pts*", QDir::System | QDir::Files);
    // int iteratorCount = 0;
    // while (it.hasNext()) {
    //     QString port = it.next();
    //     // qDebug() << "QDirIterator found:" << port;
    //     if (portComboBox->findText(port) == -1) {
    //         portComboBox->addItem(port);
    //     }
    //     iteratorCount++;
    // }
    // qDebug() << "QDirIterator found" << iteratorCount << "ports";

    // qDebug() << "Trying system command...";
    QProcess process;
    process.start("find", QStringList() << "/dev" << "-name" << "pts*");
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    // qDebug() << "find command output:" << output;

    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    foreach (QString line, lines) {
        if (line.startsWith("/dev/pts")) {
            if (portComboBox->findText(line) == -1) {
                portComboBox->addItem(line);
                // qDebug() << "Added from find:" << line;
            }
        }
    }
#endif

    // qDebug() << "Final port count:" << portComboBox->count();

    portComboBox->setStyleSheet(R"(
        QComboBox {
            background: ##0C2A97 !important;
            color: #ffffff !important;
            border: 1px solid #2d3250 !important;
            padding: 5px;
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
            border-top: 6px solid #ffffff;
        }
        QComboBox QAbstractItemView {
            background: #121722;
            color: #ffffff;
            border: 1px solid #2d3250;
            selection-background-color: #2d3250;
        }
    )");

    connectionLayout->addWidget(new QLabel(tr("Port:")), 0, 0);
    connectionLayout->addWidget(portComboBox, 0, 1);
    connectionLayout->addWidget(connectButton, 0, 2);

    // UART Command Sending
    auto *commandGroup = new QGroupBox(tr("Command"), _homeTab);
    auto *commandLayout = new QGridLayout(commandGroup);
    auto *commandEdit = new QLineEdit(_homeTab);
    auto *sendButton = new QPushButton(tr("Send Command"), _homeTab);

    commandEdit->setPlaceholderText("Enter command to send via UART...");
    commandLayout->addWidget(new QLabel(tr("Command:")), 0, 0);
    commandLayout->addWidget(commandEdit, 0, 1);
    commandLayout->addWidget(sendButton, 0, 2);

    auto *modeGroup = new QGroupBox(tr("Signal Generator Mode"), _homeTab);
    auto *modeLayout = new QGridLayout(modeGroup);

    auto *rampModeButton = new QPushButton(tr("Ramp Mode"), _homeTab);
    auto *sineWaveButton = new QPushButton(tr("Sine Wave Mode"), _homeTab);

    auto *amplitudeLabel = new QLabel(tr("Amplitude:"), _homeTab);
    auto *amplitudeSpinBox = new QDoubleSpinBox(_homeTab);
    amplitudeSpinBox->setRange(0, 100);
    amplitudeSpinBox->setValue(50);
    amplitudeSpinBox->setDecimals(1);

    auto *frequencyLabel = new QLabel(tr("Frequency (Hz):"), _homeTab);
    auto *frequencySpinBox = new QDoubleSpinBox(_homeTab);
    frequencySpinBox->setRange(0.1, 10.0);
    frequencySpinBox->setValue(1.0);
    frequencySpinBox->setDecimals(2);

    auto *durationLabel = new QLabel(tr("Duration (s):"), _homeTab);
    auto *durationSpinBox = new QDoubleSpinBox(_homeTab);
    durationSpinBox->setRange(1, 60);
    durationSpinBox->setValue(10);
    durationSpinBox->setDecimals(1);

    modeLayout->addWidget(rampModeButton, 0, 0);
    modeLayout->addWidget(sineWaveButton, 0, 1);
    modeLayout->addWidget(amplitudeLabel, 1, 0);
    modeLayout->addWidget(amplitudeSpinBox, 1, 1);
    modeLayout->addWidget(frequencyLabel, 2, 0);
    modeLayout->addWidget(frequencySpinBox, 2, 1);
    modeLayout->addWidget(durationLabel, 3, 0);
    modeLayout->addWidget(durationSpinBox, 3, 1);

    auto *dashboardGroup = new QGroupBox(tr("Motor Power"), _homeTab);
    auto *dashboardLayout = new QVBoxLayout(dashboardGroup);
    dashboardLayout->addWidget(miniDashboard);
    dashboardLayout->setAlignment(miniDashboard, Qt::AlignCenter);

    leftLayout->addWidget(dashboardGroup);
    leftLayout->addWidget(connectionGroup);
    leftLayout->addWidget(commandGroup);
    leftLayout->addWidget(modeGroup);
    leftLayout->addStretch();

    topContentLayout->addWidget(leftPanel, 1);
    topContentLayout->addLayout(displayLayout, 3);

    // UART Log Display
    auto *logGroup = new QGroupBox(tr("Command Log"), _homeTab);
    auto *logLayout = new QVBoxLayout(logGroup);
    logLayout->addWidget(logTextEdit);

    homeLayout->addLayout(topContentLayout, 1);
    homeLayout->addWidget(logGroup);

    qApp->setStyleSheet(
        "QToolTip {"
        "   background-color: #1a202c;"
        "   color: #e2e8f0;"
        "   border: 1px solid #4a5568;"
        "   border-radius: 6px;"
        "   padding: 12px;"
        "   font-family: 'Segoe UI', system-ui;"
        "   font-size: 13px;"
        "   opacity: 230;"
        "}"
    );

    sendButton->setToolTip(
        "Send Commnad to device\n"
        "Available commands:\n"
        "   # s or start - Start Motor\n"
        "   # t or stop - Stop Motor\n"
        "   # u or up - Increase Motor Power\n"
        "   # d or down - Decrease Motor Power"
    );

    connect(valueSlider, &QSlider::valueChanged, _homeTab, [miniDashboard, valueSpinBox](int value) {
        valueSpinBox->blockSignals(true);
        miniDashboard->setValue(value);
        valueSpinBox->setValue(value);
        valueSpinBox->blockSignals(false);
    });

    connect(valueSpinBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            _homeTab,
            [miniDashboard, valueSlider](double value) {
                valueSlider->blockSignals(true);
                miniDashboard->setValue(value);
                valueSlider->setValue(static_cast<int>(value));
                valueSlider->blockSignals(false);
            });

    // valueSlider UART
    connect(valueSlider, &QSlider::valueChanged, _homeTab, [this](int value) {
        if (_serialPort && _serialPort->isOpen()) {
            QString command = QString("Power:%1\n").arg(value);
            _serialPort->write(command.toUtf8());

            QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
            _logTextEdit->append(QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
        }
    });

    // valueSpinBox UART
    connect(valueSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            _homeTab, [this](double value) {
                if (_serialPort && _serialPort->isOpen()) {
                    QString command = QString("Power:%1\n").arg(value, 0, 'f', 2);
                    _serialPort->write(command.toUtf8());

                    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
                    _logTextEdit->append(QString("[%1] AUTO SEND: %2").arg(timestamp, command.trimmed()));
                }
            });

    // Kết nối các control với mini dashboard
    connect(minValueSpinBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            miniDashboard,
            &DashBoardWidget::setMinValue);
    connect(maxValueSpinBox,
            QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            miniDashboard,
            &DashBoardWidget::setMaxValue);

    // Kết nối UART
    connect(connectButton, &QPushButton::clicked, _homeTab, [this, portComboBox]() {
        _serialPort = new QSerialPort;
        _serialPort->setPortName(portComboBox->currentText());
        _serialPort->setBaudRate(QSerialPort::Baud115200);
        _serialPort->setDataBits(QSerialPort::Data8);
        _serialPort->setParity(QSerialPort::NoParity);
        _serialPort->setStopBits(QSerialPort::OneStop);

        if (_serialPort->open(QIODevice::ReadWrite))
        {
            QMessageBox::information(this, "Success", "UART Port Connected Successfully!");
            connect(_serialPort, &QSerialPort::readyRead, this, &MainWindow::readData);
        } else {
            QMessageBox::critical(this, "Error", "Unable to connect to UART port");
        }
    });

    connect(sendButton, &QPushButton::clicked, _homeTab, [this, commandEdit, logTextEdit]() {
        if (_serialPort && _serialPort->isOpen()) {
            QString command = commandEdit->text() + "\n";
            _serialPort->write(command.toUtf8());

            QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
            logTextEdit->append(QString("[%1] SENT: %2").arg(timestamp, command.trimmed()));

            qDebug() << "Command sent:" << command;
            commandEdit->clear();
        } else {
            QMessageBox::warning(this, "Warning", "UART port not connected");
        }
    });

    connect(rampModeButton, &QPushButton::clicked, _homeTab, [this, amplitudeSpinBox, durationSpinBox]() {
        if (_serialPort && _serialPort->isOpen()) {
            QString command = QString("ramp:%1:%2\n")
            .arg(amplitudeSpinBox->value(), 0, 'f', 1)
                .arg(durationSpinBox->value(), 0, 'f', 1);
            _serialPort->write(command.toUtf8());

            QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
            _logTextEdit->append(QString("[%1] MODE SET: %2").arg(timestamp, command.trimmed()));
        }
    });

    connect(sineWaveButton, &QPushButton::clicked, _homeTab, [this, amplitudeSpinBox, frequencySpinBox, durationSpinBox]() {
        if (_serialPort && _serialPort->isOpen()) {
            QString command = QString("sine:%1:%2:%3\n")
            .arg(amplitudeSpinBox->value(), 0, 'f', 1)
                .arg(frequencySpinBox->value(), 0, 'f', 2)
                .arg(durationSpinBox->value(), 0, 'f', 1);
            _serialPort->write(command.toUtf8());

            QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
            _logTextEdit->append(QString("[%1] MODE SET: %2").arg(timestamp, command.trimmed()));
        }
    });

    // Kết nối Enter key để gửi command
    connect(commandEdit, &QLineEdit::returnPressed, _homeTab, [sendButton]() {
        sendButton->click();
    });

    // Kết nối control biểu đồ
    connect(startPlotButton, &QPushButton::clicked, _homeTab, [this, pwmValueLabel, throttleValueLabel]() {
        _plotting = true;
        _throttleSeries->clear();
        _startTime = QTime::currentTime();
        pwmValueLabel->setText("PWM: 0");
        throttleValueLabel->setText("Throttle: 0");
        qDebug() << "Plotting started";
    });

    connect(stopPlotButton, &QPushButton::clicked, _homeTab, [this]() {
        _plotting = false;
        stopFlag = 1;
        qDebug() << "Plotting stopped";
    });

    connect(clearPlotButton, &QPushButton::clicked, _homeTab, [this, pwmValueLabel, throttleValueLabel]() {
        _throttleSeries->clear();
        _startTime = QTime::currentTime();
        pwmValueLabel->setText("PWM: 0");
        throttleValueLabel->setText("Throttle: 0");
        qDebug() << "Plot cleared";
    });

    // Kết nối save data
    connect(saveDataButton, &QPushButton::clicked, this, &MainWindow::saveDataToCSV);

    _pwmLabel = pwmValueLabel;
    _throttleLabel = throttleValueLabel;
    _logTextEdit = logTextEdit;

    miniDashboard->setValue(0);
}

MainWindow::~MainWindow()
{
    if (_serialPort != nullptr)
    {
        _serialPort->close();
        delete _serialPort;
    }
}
