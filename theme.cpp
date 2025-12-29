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
        }

        QSpinBox::down-button, QDoubleSpinBox::down-button {
            background: #0e202d;
        }

        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover {
            background: #456882;
        }

        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background: #456882;
        }

        QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {
            image: url(:/images/up.png);
            width: 12px;
            height: 12px;
        }

        QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
            image: url(:/images/down.png);
            width: 12px;
            height: 12px;
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
}

void MainWindow::applyLightTheme()
{
    setStyleSheet(R"(
        QMainWindow {
            background: #f5f5f5;
            color: #4a5b6c;
            font-family: "Segoe UI", "Roboto", sans-serif;
        }

        QChartView {
            background: #f5f5f5;
            border: 1px solid #80a1ba;
            border-radius: 10px;
            padding: 6px;
        }

        QGroupBox {
            background: #f5f5f5;
            color: #4a5b6c;
            border: 1px solid #80a1ba;
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
            color: #80a1ba;
        }

        QPushButton {
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #80a1ba,
                stop:1 #6c8a9e
            );
            color: #f5f5f5;
            border: 1px solid #80a1ba;
            padding: 8px 16px;
            border-radius: 6px;
            font-weight: 600;
            font-size: 11px;
        }

        QPushButton:hover {
            background: #91c4c3;
            color: #f5f5f5;
        }

        QPushButton:pressed {
            background: #b4debd;
            border: 1px solid #80a1ba;
            color: #4a5b6c;
        }

        QLineEdit, QComboBox {
            background: #f5f5f5;
            color: #4a5b6c;
            border: 1px solid #80a1ba;
            border-radius: 5px;
            padding: 6px 8px;
            font-size: 11px;
            selection-background-color: #b4debd;
            selection-color: #4a5b6c;
        }

        QSpinBox, QDoubleSpinBox {
            background: #f5f5f5;
            color: #4a5b6c;
            border: 1px solid #80a1ba;
            border-radius: 5px;
            padding: 6px 8px;
            font-size: 11px;
            selection-background-color: #b4debd;
        }

        QSpinBox::up-button, QDoubleSpinBox::up-button {
            background: transparent;
        }

        QSpinBox::down-button, QDoubleSpinBox::down-button {
            background: transparent;
        }

        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover {
            background: #b4debd;
        }

        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background: #b4debd;
        }

        QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {
            image: url(:/images/up.png);
            width: 12px;
            height: 12px;
        }

        QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
            image: url(:/images/down.png);
            width: 12px;
            height: 12px;
        }

        QLineEdit:focus,
        QComboBox:focus,
        QSpinBox:focus,
        QDoubleSpinBox:focus {
            border: 1px solid #91c4c3;
            background: #f5f5f5;
        }

        QSlider::groove:horizontal {
            border: 1px solid #80a1ba;
            height: 6px;
            background: #b4debd;
            border-radius: 4px;
        }

        QSlider::handle:horizontal {
            background: #80a1ba;
            border: 1px solid #6c8a9e;
            width: 16px;
            margin: -3px 0;
            border-radius: 8px;
        }

        QLabel {
            color: #4a5b6c;
            font-size: 11px;
            font-weight: 500;
        }

        QTextEdit {
            background: #f5f5f5;
            color: #4a5b6c;
            border: 1px solid #80a1ba;
            border-radius: 6px;
            font-size: 11px;
            font-family: "Consolas", "Roboto Mono", monospace;
            padding: 6px;
        }

        QTabWidget::pane {
            border: 1px solid #f5f5f5;
            border-radius: 8px;
            background: #f5f5f5;
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
            background: #f5f5f5;
            color: #80a1ba;
            padding: 2px 14px;
            margin: 4px 1px;
            border: 1px solid #f5f5f5;
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
                                        stop:0 #80a1ba, stop:1 #6c8a9e);
            color: #f5f5f5;
            border: 1px solid #80a1ba;
        }

        QTabBar::tab:hover:!selected {
            background: #91c4c3;
            color: #f5f5f5;
        }

        QPushButton#toggleTabsBtn {
            background: #f5f5f5;
            color: #80a1ba;
            border: 1px solid #80a1ba;
            padding: 8px;
            border-radius: 4px;
            font-weight: bold;
        }

        QPushButton#toggleTabsBtn:hover {
            background: #80a1ba;
            color: #ffffff;
        }

        QWidget#contentArea {
            background: #f5f5f5;
        }

        QWidget#leftPanel {
            background: #f5f5f5;
            border-right: 1px solid #b4debd;
        }
    )");
}
