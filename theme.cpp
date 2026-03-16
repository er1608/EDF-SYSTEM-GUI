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
            padding: 8px 16px;
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
            image: url(:/images/up_white.png);
            width: 12px;
            height: 12px;
        }

        QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
            image: url(:/images/down_white.png);
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
    )");
}

void MainWindow::applyLightTheme()
{
    setStyleSheet(R"(
        QMainWindow {
            background: #f3f4f6;
            color: #1f2933;
            font-family: "Segoe UI", "Roboto", sans-serif;
        }

        QScrollArea {
            border: none;
            background: #f3f4f6;
        }

        QWidget#ChartContainerWidget {
            background: #f3f4f6;
        }

        QScrollBar:vertical {
            border: none;
            background: #f3f4f6;
            width: 10px;
            margin: 0px;
        }

        QScrollBar::handle:vertical {
            background: #9aa6b2;
            min-height: 20px;
            border-radius: 5px;
        }

        QScrollBar:horizontal {
            border: none;
            background: #f3f4f6;
            height: 10px;
            margin: 0px;
        }

        QScrollBar::handle:horizontal {
            background: #9aa6b2;
            min-width: 20px;
            border-radius: 5px;
        }

        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0px;
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }

        QChartView {
            background: #ffffff;
            border: 1px solid #c7ccd4;
            border-radius: 10px;
            padding: 6px;
        }

        QGroupBox {
            background: #ffffff;
            color: #1f2933;
            border: 1px solid #c7ccd4;
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
            color: #5b6f8a;
        }

        QPushButton {
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 #6b7f99,
                stop:1 #4f647f
            );
            color: #ffffff;
            border: 1px solid #4f647f;
            padding: 8px 16px;
            border-radius: 6px;
            font-weight: 600;
            font-size: 11px;
        }

        QPushButton:hover {
            background: #5b6f8a;
            color: #ffffff;
        }

        QPushButton:pressed {
            background: #3e516b;
            border: 1px solid #3e516b;
            color: #ffffff;
        }

        QLineEdit, QComboBox {
            background: #ffffff;
            color: #1f2933;
            border: 1px solid #c7ccd4;
            border-radius: 5px;
            padding: 6px 8px;
            font-size: 11px;
            selection-background-color: #d8dee6;
            selection-color: #1f2933;
        }

        QSpinBox, QDoubleSpinBox {
            background: #ffffff;
            color: #1f2933;
            border: 1px solid #c7ccd4;
            border-radius: 5px;
            padding: 6px 8px;
            font-size: 11px;
            selection-background-color: #d8dee6;
        }

        QSpinBox::up-button, QDoubleSpinBox::up-button {
            background: transparent;
        }

        QSpinBox::down-button, QDoubleSpinBox::down-button {
            background: transparent;
        }

        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover {
            background: #e6eaef;
        }

        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background: #e6eaef;
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
            border: 1px solid #5b6f8a;
            background: #ffffff;
        }

        QSlider::groove:horizontal {
            border: 1px solid #c7ccd4;
            height: 6px;
            background: #e6eaef;
            border-radius: 4px;
        }

        QSlider::handle:horizontal {
            background: #5b6f8a;
            border: 1px solid #3e516b;
            width: 16px;
            margin: -3px 0;
            border-radius: 8px;
        }

        QLabel {
            color: #1f2933;
            font-size: 11px;
            font-weight: 500;
        }

        QTextEdit {
            background: #ffffff;
            color: #1f2933;
            border: 1px solid #c7ccd4;
            border-radius: 6px;
            font-size: 11px;
            font-family: "Consolas", "Roboto Mono", monospace;
            padding: 6px;
        }

        QTabWidget::pane {
            border: 1px solid #f3f4f6;
            background: #f3f4f6;
            padding: 4px;
        }

        QTabWidget::tab-bar {
            alignment: left;
        }

        QTabBar {
            background: transparent;
        }

        QTabBar::tab {
            background: #f3f4f6;
            color: #5b6f8a;
            padding: 2px 14px;
            margin: 4px 1px;
            border: 1px solid #f3f4f6;
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
                                        stop:0 #6b7f99, stop:1 #4f647f);
            color: #ffffff;
            border: 1px solid #4f647f;
        }

        QTabBar::tab:hover:!selected {
            background: #e6eaef;
            color: #1f2933;
        }

        QPushButton#toggleTabsBtn {
            background: #ffffff;
            color: #5b6f8a;
            border: 1px solid #c7ccd4;
            padding: 8px;
            border-radius: 4px;
            font-weight: bold;
        }

        QPushButton#toggleTabsBtn:hover {
            background: #5b6f8a;
            color: #ffffff;
        }

        QWidget#contentArea {
            background: #f3f4f6;
        }

        QWidget#leftPanel {
            background: #f3f4f6;
            border-right: 1px solid #c7ccd4;
        }
    )");
}
