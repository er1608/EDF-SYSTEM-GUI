#include "mainwindow.h"
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QtCharts>

#include <QtWidgets>

void MainWindow::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
  emit resized();
}

void MainWindow::addThemeToggleButton() {
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
            background: transparent;
            color: #e2e8f0;
            border: none;
            border-radius: 25px;
            font-weight: bold;
            font-size: 20px;
            padding: 6px 14px;
        }

        QPushButton#themeToggleBtn:hover {
            background: transparent;
            color: #9DD16B;
        }

        QPushButton#themeToggleBtn:pressed {
            background: transparent;
            color: #00b4d8;
        }
    )");

  themeToggleBtn->move(15, this->height() - 70);

  connect(themeToggleBtn, &QPushButton::clicked, this,
          &MainWindow::toggleTheme);

  connect(this, &MainWindow::resized, this, [this, themeToggleBtn]() {
    themeToggleBtn->move(15, this->height() - 70);
  });
}

void MainWindow::toggleTheme() {
  _isDarkMode = !_isDarkMode;

  if (_isDarkMode) {
    applyDarkTheme();
  } else {
    applyLightTheme();
  }

  QPushButton *themeBtn = findChild<QPushButton *>("themeToggleBtn");
  if (themeBtn) {
    themeBtn->setText(_isDarkMode ? "🌙" : "🌞");
  }
}

void MainWindow::applyDarkTheme() {
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
}

void MainWindow::applyLightTheme() {
  setStyleSheet(R"(
        QMainWindow {
            background: #f5f5f5;
            color: #4a5568;
            font-family: "Segoe UI", "Roboto", sans-serif;
        }

        QScrollArea {
            border: none;
            background: #f5f5f5;
        }

        QWidget#ChartContainerWidget {
            background: #f5f5f5;
        }

        QScrollBar:vertical {
            border: none;
            background: #f5f5f5;
            width: 10px;
            margin: 0px;
        }

        QScrollBar::handle:vertical {
            background: #cbd5e0;
            min-height: 20px;
            border-radius: 5px;
        }

        QScrollBar:horizontal {
            border: none;
            background: #f5f5f5;
            height: 10px;
            margin: 0px;
        }

        QScrollBar::handle:horizontal {
            background: #cbd5e0;
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
            border: 1px solid #e5e5e5;
            border-radius: 10px;
            padding: 6px;
        }

        QGroupBox {
            background: #ffffff;
            color: #4a5568;
            border: 1px solid #e5e5e5;
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
            color: #718096;
        }

        QPushButton {
            background: qlineargradient(
                x1:0, y1:0, x2:0, y2:1,
                stop:0 rgba(4, 168, 173, 0.85),
                stop:1 rgba(4, 139, 143, 0.85)
            );
            color: #ffffff;
            border: 1px solid rgba(4, 139, 143, 0.7);
            padding: 8px 16px;
            border-radius: 6px;
            font-weight: 600;
            font-size: 11px;
        }

        QPushButton:hover {
            background: rgba(5, 194, 200, 0.9);
            color: #ffffff;
        }

        QPushButton:pressed {
            background: rgba(3, 122, 126, 0.9);
            border: 1px solid rgba(3, 122, 126, 0.8);
            color: #ffffff;
        }

        QLineEdit {
            background: #ffffff;
            color: #4a5568;
            border: 1px solid #e5e5e5;
            border-radius: 5px;
            padding: 6px 8px;
            font-size: 11px;
            selection-background-color: #edf2f7;
            selection-color: #4a5568;
        }

        QSpinBox, QDoubleSpinBox {
            background: #ffffff;
            color: #4a5568;
            border: 1px solid #ffffff;
            border-radius: 5px;
            font-size: 15px;
            selection-background-color: #edf2f7;
        }

        QSpinBox::up-button, QDoubleSpinBox::up-button {
            subcontrol-origin: border;
            subcontrol-position: top right;
            border-radius: 5px;
            background: #ffffff;
        }

        QSpinBox::down-button, QDoubleSpinBox::down-button {
            subcontrol-origin: border;
            subcontrol-position: top right;
            border-radius: 5px;
            background: #ffffff;
        }

        QSpinBox::up-button:hover, QDoubleSpinBox::up-button:hover {
            background: #edf2f7;
        }

        QSpinBox::down-button:hover, QDoubleSpinBox::down-button:hover {
            background: #edf2f7;
        }

        QSpinBox::up-arrow, QDoubleSpinBox::up-arrow {
            image: url(:/images/up.png);
            width: 8px;
            height: 8px;
        }

        QSpinBox::down-arrow, QDoubleSpinBox::down-arrow {
            image: url(:/images/down.png);
            width: 8px;
            height: 8px;
        }

        QLineEdit:focus,
        QComboBox:focus,
        QSpinBox:focus,
        QDoubleSpinBox:focus {
            border: 1px solid rgba(4, 168, 173, 0.6);
            background: #ffffff;
        }

        QSlider::groove:horizontal {
            border: 1px solid #e5e5e5;
            height: 6px;
            background: #edf2f7;
            border-radius: 4px;
        }

        QSlider::handle:horizontal {
            background: rgba(4, 168, 173, 0.8);
            border: 1px solid rgba(4, 139, 143, 0.7);
            width: 16px;
            margin: -3px 0;
            border-radius: 8px;
        }

        QLabel {
            color: #4a5568;
            font-size: 11px;
            font-weight: 500;
        }

        QTextEdit {
            background: #ffffff;
            color: #4a5568;
            border: 1px solid #e5e5e5;
            border-radius: 6px;
            font-size: 11px;
            font-family: "Consolas", "Roboto Mono", monospace;
            padding: 6px;
        }

        QTabWidget::pane {
            border: 1px solid #f5f5f5;
            background: #f5f5f5;
            padding: 4px;
        }

        QTabWidget::tab-bar {
            alignment: left;
        }

        QTabBar {
            background: transparent;
        }

        QTabBar::tab {
            background: #f5f5f5;
            color: #718096;
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
                                        stop:0 rgba(4, 168, 173, 0.85), stop:1 rgba(4, 139, 143, 0.85));
            color: #ffffff;
            border: 1px solid rgba(4, 139, 143, 0.7);
        }

        QTabBar::tab:hover:!selected {
            background: #edf2f7;
            color: #4a5568;
        }

        QPushButton#toggleTabsBtn {
            background: #ffffff;
            color: #718096;
            border: 1px solid #e5e5e5;
            padding: 8px;
            border-radius: 4px;
            font-weight: bold;
        }

        QPushButton#toggleTabsBtn:hover {
            background: rgba(4, 168, 173, 0.2);
            color: #04a8ad;
            border-color: rgba(4, 168, 173, 0.5);
        }

        QWidget#contentArea {
            background: #f5f5f5;
        }

        QWidget#leftPanel {
            background: #f5f5f5;
            border-right: 1px solid #e5e5e5;
        }

        QComboBox {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                            stop:0 rgba(4, 168, 173, 0.15), stop:1 rgba(4, 139, 143, 0.1));
            color: #4a5568;
            border: 1px solid rgba(4, 168, 173, 0.3) !important;
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
            border-top: 6px solid #04a8ad;
        }

        QComboBox QAbstractItemView {
            background: #ffffff;
            color: #4a5568;
            border: 1px solid #e5e5e5;
            selection-background-color: rgba(4, 168, 173, 0.15);
            selection-color: #4a5568;
        }

        QToolTip {
           background-color: #4a5568;
           color: #f5f5f5;
           border: 1px solid rgba(4, 168, 173, 0.5);
           border-radius: 6px;
           padding: 12px;
           font-family: 'Segoe UI', system-ui;
           font-size: 13px;
           opacity: 230;
        }

        QMenu {
            background-color: #ffffff;
            border: 1px solid #e5e5e5;
            border-radius: 5px;
            padding: 5px 0px;
        }

        QMenu::item {
            padding: 8px 30px 8px 20px;
            color: #4a5568;
            background-color: transparent;
        }

        QMenu::item:selected {
            background-color: rgba(4, 168, 173, 0.15);
            color: #4a5568;
        }

        QMenu::item:checked {
            background-color: rgba(4, 168, 173, 0.1);
            color: #4a5568;
        }

        QMenu::separator {
            height: 1px;
            background-color: #e5e5e5;
            margin: 5px 0px;
        }
    )");
}
