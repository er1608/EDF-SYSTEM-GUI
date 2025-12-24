#include "mainwindow.h"
#include "toggleswitch.h"
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
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1b2432, stop:1 #1b2432);
            color: #e2e8f0;
            border: 1px solid #2d3250;
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
            color: #a6b1e1;
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
    QPixmap analPixmap(":/images/Anal.png");
    analyzeLabel->setFixedSize(40, 35);
    analyzeLabel->setPixmap(analPixmap.scaled(analyzeLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    analyzeLabel->setAlignment(Qt::AlignTop);

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

    addToggleSwitch();

    setupHomeTab();
    setupAnalyzeTab();
}

MainWindow::~MainWindow()
{
    if (_serialPort != nullptr)
    {
        _serialPort->close();
        delete _serialPort;
    }
}
