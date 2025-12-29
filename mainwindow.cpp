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
            background: #0e202d;
            color: #d2c1b6;
            font-family: "Segoe UI", "Roboto", sans-serif;
        }

        QChartView {
            background: #0e202d;
            border: none;
            border-radius: 10px;
            padding: 6px;
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1b3c53, stop:1 #0e202d);
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
            padding: 8px 16px;
            border-radius: 6px;
            font-weight: 600;
            font-size: 11px;
        }

        QPushButton:hover {
            background: #234c6a;
            color: #ffffff;
        }

        QPushButton:pressed {
            background: #d2c1b6;
            border: 1px solid #d2c1b6;
            color: #0e202d;
        }

        QLineEdit, QComboBox {
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
            border: 1px solid #456882;
            border-radius: 5px;
            padding: 6px 8px;
            font-size: 11px;
            selection-background-color: #456882;
        }

        QSpinBox::up-button, QDoubleSpinBox::up-button {
            background: #0e202d;
            border: 1px solid #456882;
            border-top-right-radius: 4px;
        }

        QSpinBox::down-button, QDoubleSpinBox::down-button {
            background: #0e202d;
            border: 1px solid #456882;
            border-bottom-right-radius: 4px;
        }

        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover {
            background: #456882;
        }

        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background: #456882;
        }

        QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {
            width: 0px;
            height: 0px;
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-bottom: 5px solid #d2c1b6;
        }

        QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
            width: 0px;
            height: 0px;
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 5px solid #d2c1b6;
        }

        QLineEdit:focus,
        QComboBox:focus,
        QSpinBox:focus,
        QDoubleSpinBox:focus {
            border: 1px solid #d2c1b6;
            /* Focus sáng hơn một chút */
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
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #0e202d, stop:1 #0e202d);
        }

        QTabWidget::pane {
            border: 1px solid #0e202d;
            border-radius: 8px;
            background: #0e202d;
            padding: 4px;
        }

        QTabWidget::tab-bar {
            alignment: left;
            width: 69px;
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
            color: #ffffff;
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
            color: #ffffff;
        }

        QWidget#contentArea {
            background: #1b3c53;
        }

        QWidget#leftPanel {
            background: #1b3c53;
            border-right: 1px solid #1b3c53;
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
