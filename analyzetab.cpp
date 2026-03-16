#include "mainwindow.h"
#include <QtCharts>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QtWidgets>
#include <QTableWidget>
#include <QHeaderView>

void MainWindow::setupAnalyzeTab()
{
    auto *analyzeLayout = new QVBoxLayout(_analyzeTab);
    analyzeLayout->setContentsMargins(10, 10, 10, 10);
    analyzeLayout->setSpacing(10);

    auto *analyzeTabs = new QTabWidget(_analyzeTab);
    analyzeTabs->setTabPosition(QTabWidget::South);
    analyzeTabs->setFont(QFont("Arial", 9, QFont::Bold));

    analyzeTabs->setStyleSheet(R"(
        QTabBar::tab {
        min-width: 79px;
        height: 18px;
        }
    )");

    auto *csvPlotTab = new QWidget();
    setupPlotTab(csvPlotTab);
    analyzeTabs->addTab(csvPlotTab, "Chart");

    auto *csvTableTab = new QWidget();
    setupTableTab(csvTableTab);
    analyzeTabs->addTab(csvTableTab, "Data");

    analyzeLayout->addWidget(analyzeTabs);
}

void MainWindow::setupPlotTab(QWidget *tab)
{
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(10);

    auto *controlGroup = new QGroupBox("Control");
    controlGroup->setFont(QFont("Arial", 11, QFont::Bold));

    auto *controlLayout = new QHBoxLayout(controlGroup);

    auto *loadCSVBtn = new QPushButton("Load File");
    auto *plotBtn = new QPushButton("Plot Data");

    QFont btnFont = loadCSVBtn->font();
    btnFont.setPointSize(10);
    loadCSVBtn->setFont(btnFont);
    plotBtn->setFont(btnFont);

    auto *filePathLabel = new QLabel("No file loaded");
    filePathLabel->setStyleSheet("color: #666; font-style: italic;");

    auto *columnCombo = new QComboBox();
    columnCombo->setFont(btnFont);
    columnCombo->addItem("Select column to plot");
    columnCombo->setEnabled(false);

    controlLayout->addWidget(new QLabel("Column:"));
    controlLayout->addWidget(columnCombo);
    controlLayout->addWidget(plotBtn);
    controlLayout->addStretch();
    controlLayout->addWidget(loadCSVBtn);
    controlLayout->addWidget(filePathLabel);

    _csvSeries = new QLineSeries();

    auto *csvChart = new QChart();
    csvChart->addSeries(_csvSeries);

    auto *axisX = new QValueAxis();
    axisX->setTitleText("Index");
    axisX->setTitleFont(QFont("Arial", 9));
    axisX->setLabelFormat("%.0f");

    auto *axisY = new QValueAxis();
    axisY->setTitleText("Value");
    axisY->setTitleFont(QFont("Arial", 9));
    axisY->setLabelFormat("%.1f");

    csvChart->addAxis(axisX, Qt::AlignBottom);
    csvChart->addAxis(axisY, Qt::AlignLeft);

    _csvSeries->attachAxis(axisX);
    _csvSeries->attachAxis(axisY);

    csvChart->legend()->setVisible(false);
    csvChart->setBackgroundBrush(QBrush(QColor(27, 36, 50)));
    csvChart->setTheme(QChart::ChartThemeBlueCerulean);

    csvChart->layout()->setContentsMargins(0, 0, 0, 0);
    csvChart->setBackgroundRoundness(0);

    auto *csvChartView = new QChartView(csvChart);
    csvChartView->setRenderHint(QPainter::Antialiasing);
    csvChartView->setMinimumHeight(400);

    auto *chartContainer = new QWidget();
    auto *chartLayout = new QVBoxLayout(chartContainer);
    chartLayout->setContentsMargins(0, 0, 0, 0);
    chartLayout->setSpacing(0);

    auto *xScrollBar = new QScrollBar(Qt::Horizontal);
    xScrollBar->setRange(0, 100);
    xScrollBar->setValue(0);
    xScrollBar->setPageStep(10);
    xScrollBar->setEnabled(false);

    chartLayout->addWidget(csvChartView);
    chartLayout->setSpacing(5);
    chartLayout->addWidget(xScrollBar);

    auto *csvStatsGroup = new QGroupBox("Data Statistics");
    csvStatsGroup->setFont(QFont("Arial", 11, QFont::Bold));

    auto *csvStatsLayout = new QGridLayout(csvStatsGroup);
    csvStatsLayout->setSpacing(10);

    auto *minLabel = new QLabel("Min: --");
    auto *maxLabel = new QLabel("Max: --");
    auto *avgLabel = new QLabel("Average: --");
    auto *pointsLabel = new QLabel("Data Points: --");

    minLabel->setStyleSheet("font-weight: bold; font-size: 13px; padding: 6px; border-radius: 3px;");
    maxLabel->setStyleSheet("font-weight: bold; font-size: 13px; padding: 6px; border-radius: 3px;");
    avgLabel->setStyleSheet("font-weight: bold; font-size: 13px; padding: 6px; border-radius: 3px;");
    pointsLabel->setStyleSheet("font-weight: bold; font-size: 13px; padding: 6px; border-radius: 3px;");

    QFont statFont("Arial", 10);
    minLabel->setFont(statFont);
    maxLabel->setFont(statFont);
    avgLabel->setFont(statFont);
    pointsLabel->setFont(statFont);

    csvStatsLayout->addWidget(minLabel, 0, 1);
    csvStatsLayout->addWidget(maxLabel, 0, 2);
    csvStatsLayout->addWidget(avgLabel, 0, 3);
    csvStatsLayout->addWidget(pointsLabel, 0, 4);

    layout->addWidget(controlGroup);
    layout->addWidget(chartContainer, 1);
    layout->addWidget(csvStatsGroup);

    _csvData.clear();

    connect(loadCSVBtn, &QPushButton::clicked, this, [this, filePathLabel, columnCombo]() {
        QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                        "Open File",
                                                        QDir::homePath(),
                                                        "CSV Files (*.csv);;All Files (*.*)");

        if (!fileName.isEmpty()) {
            filePathLabel->setText(QFileInfo(fileName).fileName());
            _csvData.clear();

            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream data(&file);
                QStringList headers;
                bool firstLine = true;

                while (!data.atEnd()) {
                    QString line = data.readLine();
                    QStringList values = line.split(',');

                    if (firstLine) {
                        headers = values;
                        firstLine = false;
                    } else {
                        _csvData.append(values);
                    }
                }

                file.close();

                columnCombo->clear();
                for (int i = 0; i < headers.size(); i++) {
                    if (headers[i] != "Timestamp") {
                        columnCombo->addItem(QString("%1").arg(headers[i]), i);
                    }
                }
                columnCombo->setEnabled(true);

                if (_logTextEdit) {
                    _logTextEdit->append(QString("[CSV] Loaded %1 rows from %2").arg(_csvData.size()).arg(fileName));
                }
            }
        }
    });

    auto updateChartView = [this](const QVector<QPointF> &points, double startPos,
                                  double visibleRange, QValueAxis* axisX, QValueAxis* axisY,
                                  double yMin, double yMax) {

        _csvSeries->clear();
        for (const QPointF &point : points) {
            if (point.x() >= startPos && point.x() <= startPos + visibleRange) {
                _csvSeries->append(point);
            }
        }

        axisX->setRange(startPos, startPos + visibleRange);
        axisY->setRange(yMin, yMax);
    };

    connect(plotBtn, &QPushButton::clicked, this, [this, columnCombo, minLabel, maxLabel, avgLabel, pointsLabel, axisX, axisY, xScrollBar, updateChartView]() {

        if (columnCombo->currentIndex() < 0 || _csvData.isEmpty()) return;

        axisY->setTitleText(columnCombo->currentText());
        int colIndex = columnCombo->currentData().toInt();
        _csvSeries->clear();

        double sum = 0;
        double minVal = std::numeric_limits<double>::max();
        double maxVal = std::numeric_limits<double>::lowest();
        double xMin = std::numeric_limits<double>::max();
        double xMax = std::numeric_limits<double>::lowest();
        int validPoints = 0;

        int xColumn = -1;
        for (int i = 0; i < _csvHeaders.size(); i++) {
            QString header = _csvHeaders[i].toLower();
            if (header.contains("time") || header.contains("timestamp") ||
                header.contains("date") || header.contains("index")) {
                xColumn = i;
                break;
            }
        }

        QVector<QPointF> allPoints;

        for (int i = 0; i < _csvData.size(); i++) {
            double xValue = 0, yValue = 0;
            bool xOk = false, yOk = false;

            if (xColumn >= 0 && xColumn < _csvData[i].size()) {
                xValue = _csvData[i][xColumn].toDouble(&xOk);
                if (!xOk) {
                    xValue = i;
                    xOk = true;
                }
            } else {
                xValue = i;
                xOk = true;
            }

            if (colIndex < _csvData[i].size()) {
                yValue = _csvData[i][colIndex].toDouble(&yOk);
            }

            if (xOk && yOk) {
                allPoints.append(QPointF(xValue, yValue));
                sum += yValue;
                minVal = qMin(minVal, yValue);
                maxVal = qMax(maxVal, yValue);
                xMin = qMin(xMin, xValue);
                xMax = qMax(xMax, xValue);
                validPoints++;
            }
        }

        if (validPoints > 0) {
            static QVector<QPointF> originalPoints;
            originalPoints = allPoints;

            double totalRange = xMax - xMin;
            double visiblePercentage = 0.3;
            double visibleRange = totalRange * visiblePercentage;

            xScrollBar->setEnabled(true);
            xScrollBar->setRange(0, 100);
            xScrollBar->setValue(0);
            xScrollBar->setPageStep(qRound(visiblePercentage * 100));

            updateChartView(allPoints, 0, visibleRange, axisX, axisY, minVal, maxVal);

            static QMetaObject::Connection conn;
            if (conn) QObject::disconnect(conn);

            conn = QObject::connect(xScrollBar, &QScrollBar::valueChanged, this, [allPoints, xMin, totalRange, visiblePercentage, axisX, axisY, minVal, maxVal, this](int value) {

                double visibleRange = totalRange * visiblePercentage;
                double startPos = xMin + (totalRange - visibleRange) * (value / 100.0);

                QVector<QPointF> visiblePoints;
                for (const QPointF &point : allPoints) {
                    if (point.x() >= startPos && point.x() <= startPos + visibleRange) {
                        visiblePoints.append(point);
                    }
                }

                _csvSeries->clear();
                for (const QPointF &point : visiblePoints) {
                    _csvSeries->append(point);
                }

                axisX->setRange(startPos, startPos + visibleRange);
                axisY->setRange(minVal, maxVal);
            });

            double average = sum / validPoints;
            minLabel->setText(QString("Min: %1").arg(minVal, 0, 'f', 2));
            maxLabel->setText(QString("Max: %1").arg(maxVal, 0, 'f', 2));
            avgLabel->setText(QString("Avg: %1").arg(average, 0, 'f', 2));
            pointsLabel->setText(QString("Points: %1").arg(validPoints));
        }
    });
}

void MainWindow::setupTableTab(QWidget *tab)
{
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(15, 15, 15, 15);
    layout->setSpacing(15);

    auto *controlGroup = new QGroupBox("CSV Table Controls");
    controlGroup->setFont(QFont("Arial", 11, QFont::Bold));

    auto *controlLayout = new QHBoxLayout(controlGroup);

    auto *loadTableBtn = new QPushButton("Load CSV to Table");
    auto *exportTableBtn = new QPushButton("Export Table");

    QFont btnFont = loadTableBtn->font();
    btnFont.setPointSize(10);
    loadTableBtn->setFont(btnFont);
    exportTableBtn->setFont(btnFont);

    auto *tableInfoLabel = new QLabel("No data loaded");
    tableInfoLabel->setStyleSheet("color: #666; font-style: italic;");

    controlLayout->addWidget(loadTableBtn);
    controlLayout->addWidget(exportTableBtn);
    controlLayout->addWidget(loadTableBtn);
    controlLayout->addStretch();
    controlLayout->addWidget(tableInfoLabel);

    _csvTableWidget = new QTableWidget();
    _csvTableWidget->setFont(QFont("Arial", 9));

    _csvTableWidget->setStyleSheet(
        "QTableWidget {"
        "  gridline-color: #f5f5f5;"
        "  background-image: url(:/images/CTUAV.png);"
        "  background-repeat: no-repeat;"
        "  background-position: center;"
        "}"

        "QTableWidget::item {"
        "  background: rgba(255,255,255,0.85);"
        "}"
    );

    _csvTableWidget->setSortingEnabled(true);

    _csvTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    _csvTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    _csvTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    layout->addWidget(controlGroup);
    layout->addWidget(_csvTableWidget, 1);

    connect(loadTableBtn, &QPushButton::clicked, this, [this, tableInfoLabel]() {
        QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                        "Open CSV File for Table",
                                                        QDir::homePath(),
                                                        "CSV Files (*.csv);;All Files (*.*)");

        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                QStringList headers;
                int rowCount = 0;

                _csvTableWidget->clear();
                _csvTableWidget->setRowCount(0);
                _csvTableWidget->setColumnCount(0);

                QStringList allLines;
                while (!in.atEnd() && rowCount < 5000) {
                    allLines.append(in.readLine());
                    rowCount++;
                }
                file.close();

                if (!allLines.isEmpty()) {
                    headers = allLines.first().split(',');
                    _csvTableWidget->setColumnCount(headers.size());
                    _csvTableWidget->setHorizontalHeaderLabels(headers);

                    _csvTableWidget->setRowCount(allLines.size() - 1);

                    for (int row = 1; row < allLines.size(); row++) {
                        QStringList values = allLines[row].split(',');
                        for (int col = 0; col < qMin(values.size(), headers.size()); col++) {
                            auto *item = new QTableWidgetItem(values[col]);

                            bool isNumber;
                            values[col].toDouble(&isNumber);
                            if (isNumber) {
                                item->setData(Qt::UserRole, values[col].toDouble());
                            }

                            _csvTableWidget->setItem(row - 1, col, item);
                        }
                    }

                    _csvTableWidget->horizontalHeader()->setStretchLastSection(true);
                    _csvTableWidget->resizeColumnsToContents();

                    tableInfoLabel->setText(
                        QString("Loaded: %1 rows × %2 columns from %3")
                            .arg(_csvTableWidget->rowCount())
                            .arg(_csvTableWidget->columnCount())
                            .arg(QFileInfo(fileName).fileName())
                        );

                    if (_logTextEdit) {
                        _logTextEdit->append(
                            QString("[TABLE] Loaded %1x%2 table from %3")
                                .arg(_csvTableWidget->rowCount())
                                .arg(_csvTableWidget->columnCount())
                                .arg(fileName)
                            );
                    }
                }
            }
        }
    });

    connect(exportTableBtn, &QPushButton::clicked, this, [this]() {
        if (_csvTableWidget->rowCount() == 0) return;

        QString fileName = QFileDialog::getSaveFileName(nullptr,
                                                        "Export Table to CSV",
                                                        QDir::homePath() + "/table_export.csv",
                                                        "CSV Files (*.csv)");

        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);

                for (int col = 0; col < _csvTableWidget->columnCount(); col++) {
                    if (col > 0) out << ",";
                    out << _csvTableWidget->horizontalHeaderItem(col)->text();
                }
                out << "\n";

                for (int row = 0; row < _csvTableWidget->rowCount(); row++) {
                    for (int col = 0; col < _csvTableWidget->columnCount(); col++) {
                        if (col > 0) out << ",";
                        auto *item = _csvTableWidget->item(row, col);
                        if (item) {
                            out << item->text();
                        }
                    }
                    out << "\n";
                }

                file.close();

                if (_logTextEdit) {
                    _logTextEdit->append(QString("[TABLE] Exported %1 rows to %2")
                                             .arg(_csvTableWidget->rowCount())
                                             .arg(fileName));
                }
            }
        }
    });
}
