#include "mainwindow.h"
#include <QtCharts>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <QtWidgets>

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    emit resized();
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
            border: none;
            border-radius: 10px;
            padding: 6px;
        }

        QGroupBox {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ffffff, stop:1 #f8f9ff);
            color: #2d3748;
            border: 1px solid #e2e8ff;
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
                stop:0 #65BADB,
                stop:1 #057A9E
            );
            color: #ffffff;
            border: 1px solid #65BADB;
            padding: 8px 16px;
            border-radius: 6px;
            font-weight: 600;
            font-size: 11px;
        }

        QPushButton:hover {
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #6366f1,
                stop:1 #057A9E
            );
        }

        QPushButton:pressed {
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #057A9E,
                stop:1 #4338ca
            );
            border: 1px solid #4338ca;
        }

        QLineEdit, QComboBox {
            background: #ffffff;
            color: #374151;
            border: 1px solid #d1d5ff;
            border-radius: 5px;
            padding: 6px 8px;
            font-size: 11px;
            selection-background-color: #c7d2fe;
        }

        QSpinBox, QDoubleSpinBox {
            background: #ffffff;
            color: #374151;
            border: 1px solid #d1d5ff;
            border-radius: 5px;
            padding: 6px 8px;
            font-size: 11px;
            selection-background-color: #c7d2fe;
        }

        QSpinBox::up-button, QDoubleSpinBox::up-button {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #c7d2fe, stop:1 #a5b4fc);
            border: 1px solid #a5b4fc;
            border-top-right-radius: 4px;
        }

        QSpinBox::down-button, QDoubleSpinBox::down-button {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #c7d2fe, stop:1 #a5b4fc);
            border: 1px solid #a5b4fc;
            border-bottom-right-radius: 4px;
        }

        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover,
        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #a5b4fc, stop:1 #65BADB);
        }

        QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {
            width: 0px;
            height: 0px;
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-bottom: 5px solid #057A9E;
        }

        QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
            width: 0px;
            height: 0px;
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 5px solid #057A9E;
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
            width: 16px;
            margin: -3px 0;
            border-radius: 8px;
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
            border-radius: 6px;
            font-size: 11px;
            font-family: "Consolas", "Roboto Mono", monospace;
            padding: 6px;
        }

        QTabWidget::pane {
            border-radius: 8px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #f8f9ff, stop:1 #e8ebff);
            padding: 4px;
        }

        QTabWidget::tab-bar {
            alignment: left;
            width: 69px;
        }

        QTabBar {
            background: transparent;
        }

        QTabBar::tab {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #f1f5ff, stop:1 #e8ebff);
            color: #6b7280;
            padding: 2px 14px;
            margin: 4px 1px;
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
            padding: 8px;
            border-radius: 4px;
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
    })");
}
