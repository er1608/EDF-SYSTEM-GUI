#include "mainwindow.h"
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QtCharts>

#include <QtWidgets>

void MainWindow::saveDataToPDF() {
  QDir().mkpath("log/report");
  QString dateStr =
      QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
  QString fileName = QString("log/report/Report_%1.pdf").arg(dateStr);

  QPdfWriter pdf(fileName);
  pdf.setPageSize(QPageSize(QPageSize::A4));
  pdf.setResolution(300);

  QPainter painter(&pdf);

  int pageWidth = pdf.width();
  int margin = 150;
  int y = 350;

  QPixmap image(":/images/CTUAV.png");
  int imageWidth = pageWidth * 0.5;
  int imageHeight = imageWidth * image.height() / image.width();

  painter.drawPixmap((pageWidth - imageWidth) / 2, y, imageWidth, imageHeight,
                     image);

  y += imageHeight + 150;

  QFont titleFont("Arial", 22, QFont::Bold);
  painter.setFont(titleFont);
  painter.drawText(QRect(0, y, pageWidth, 100), Qt::AlignCenter,
                   "PROP TEST RESULT");

  y += 2000;

  QFont infoFont("Arial", 11);
  painter.setFont(infoFont);

  painter.drawText(100, y,
                   "Tài liệu bảo mật cấp A2  Bản quyền thuộc về CT UAV-CT "
                   "Group  *Nghiêm cấm sao chép dưới mọi hình thức*");

  pdf.newPage();
  y = 200;

  painter.drawText(margin, y,
                   "Ngày test: " + QDate::currentDate().toString("dd/MM/yyyy"));
  y += 60;

  painter.drawText(margin, y, "Người thực hiện: ......................");
  y += 60;

  painter.drawText(margin, y, "Động cơ sử dụng: TP-4090");
  y += 60;

  painter.drawText(margin, y, "Nhiệt độ môi trường: 31°C");
  y += 120;

  QLineSeries *thrustSR = new QLineSeries();
  QLineSeries *pwmSR = new QLineSeries();
  QLineSeries *voltSR = new QLineSeries();
  QLineSeries *currSR = new QLineSeries();

  int i = 0;
  for (auto &data : bufferCopy) {
    i++;
    thrustSR->append(i, data.thrust);
    pwmSR->append(i, data.pwm);
    voltSR->append(i, data.voltage);
    currSR->append(i, data.current);
  }

  int chartWidth = pageWidth * 0.8;
  int chartHeight = chartWidth * 0.5;

  QChart *chart1 = new QChart();
  chart1->addSeries(thrustSR);
  chart1->createDefaultAxes();
  chart1->legend()->hide();

  QChartView chartView1(chart1);
  chartView1.resize(chartWidth, chartHeight);

  QPixmap pix1 = chartView1.grab();

  painter.drawPixmap((pageWidth - pix1.width()) / 2, y, pix1);

  y += chartHeight + 40;

  painter.drawText(QRect(0, y, pageWidth, 50), Qt::AlignCenter,
                   "Figure 1: Thrust Chart");

  y += 250;

  QChart *chart2 = new QChart();
  chart2->addSeries(pwmSR);
  chart2->createDefaultAxes();
  chart2->legend()->hide();

  QChartView chartView2(chart2);
  chartView2.resize(chartWidth, chartHeight);

  QPixmap pix2 = chartView2.grab();

  painter.drawPixmap((pageWidth - pix2.width()) / 2, y, pix2);

  y += chartHeight + 40;

  painter.drawText(QRect(0, y, pageWidth, 50), Qt::AlignCenter,
                   "Figure 2: Duty Cycle Chart");

  pdf.newPage();
  y = 200;

  QChart *chart3 = new QChart();
  chart3->addSeries(voltSR);
  chart3->createDefaultAxes();
  chart3->legend()->hide();

  QChartView chartView3(chart3);
  chartView3.resize(chartWidth, chartHeight);

  QPixmap pix3 = chartView3.grab();

  painter.drawPixmap((pageWidth - pix3.width()) / 2, y, pix3);

  y += chartHeight + 40;

  painter.drawText(QRect(0, y, pageWidth, 50), Qt::AlignCenter,
                   "Figure 3: Voltage Chart");

  y += 250;

  QChart *chart4 = new QChart();
  chart4->addSeries(currSR);
  chart4->createDefaultAxes();
  chart4->legend()->hide();

  QChartView chartView4(chart4);
  chartView4.resize(chartWidth, chartHeight);

  QPixmap pix4 = chartView4.grab();

  painter.drawPixmap((pageWidth - pix4.width()) / 2, y, pix4);

  y += chartHeight + 40;

  painter.drawText(QRect(0, y, pageWidth, 50), Qt::AlignCenter,
                   "Figure 4: Current Chart");

  pdf.newPage();
  y = 200;

  QFont tableTitleFont("Arial", 14, QFont::Bold);
  painter.setFont(tableTitleFont);
  painter.drawText(margin, y, "Table 1: Data Table");
  y += 150;

  int startX = (pageWidth - 2000) / 2;
  int colWidths[] = {250, 400, 450, 450, 450};
  int rowHeight = 80;

  painter.setFont(QFont("Arial", 10, QFont::Bold));
  painter.fillRect(startX, y, 2000, rowHeight, QColor(240, 240, 240));
  painter.drawRect(startX, y, 2000, rowHeight);

  QString headers[] = {"Index", "Timestamp (s)", "Duty Cycle (%)", "RPM",
                       "Thrust (N)"};
  int currX = startX;
  for (int col = 0; col < 5; ++col) {
    painter.drawText(QRect(currX, y, colWidths[col], rowHeight),
                     Qt::AlignCenter, headers[col]);
    currX += colWidths[col];
  }
  y += rowHeight;

  painter.setFont(QFont("Arial", 10));
  int maxRows = 15;
  int totalPoints = bufferCopy.size();
  int step = qMax(1, totalPoints / maxRows);

  int rowCount = 0;
  for (int idx = 0; idx < totalPoints && rowCount < maxRows; idx += step) {
    const auto &data = bufferCopy[idx];

    if (rowCount % 2 == 1) {
      painter.fillRect(startX, y, 2000, rowHeight, QColor(250, 250, 250));
    }
    painter.drawRect(startX, y, 2000, rowHeight);

    currX = startX;
    painter.drawText(QRect(currX, y, colWidths[0], rowHeight), Qt::AlignCenter,
                     QString::number(idx + 1));
    currX += colWidths[0];

    painter.drawText(QRect(currX, y, colWidths[1], rowHeight), Qt::AlignCenter,
                     QString::number(data.timestamp, 'f', 2));
    currX += colWidths[1];

    painter.drawText(QRect(currX, y, colWidths[2], rowHeight), Qt::AlignCenter,
                     QString::number(data.pwm, 'f', 1));
    currX += colWidths[2];

    painter.drawText(QRect(currX, y, colWidths[3], rowHeight), Qt::AlignCenter,
                     QString::number(data.rpm, 'f', 0));
    currX += colWidths[3];

    painter.drawText(QRect(currX, y, colWidths[4], rowHeight), Qt::AlignCenter,
                     QString::number(data.thrust, 'f', 1));
    currX += colWidths[4];

    y += rowHeight;
    rowCount++;
  }

  y += 200;

  painter.setFont(infoFont);
  painter.drawText(margin, y, "Ghi chú (Bất thường/Hiện tượng quan sát được):");
  y += 120;
  painter.drawText(
      margin, y,
      "........................................................................"
      "................................................");
  y += 100;
  painter.drawText(
      margin, y,
      "........................................................................"
      "................................................");
  y += 100;
  painter.drawText(
      margin, y,
      "........................................................................"
      "................................................");

  y += 200;

  painter.drawText(margin, y, "Người thực hiện");
  painter.drawText(pageWidth - margin - 500, y, "Trưởng phòng Kỹ thuật");

  y += 80;

  painter.drawText(margin, y, "(Ký, ghi rõ họ tên)");
  painter.drawText(pageWidth - margin - 400, y, "(Ký, ghi rõ họ tên)");

  delete chart1;
  delete chart2;
  delete chart3;
  delete chart4;

  painter.end();

  QMessageBox::information(this, "Success",
                           QString("PDF saved to %1").arg(fileName));
}

void MainWindow::saveDataToCSV() {
  QDir().mkpath("log/csv");
  QString dateStr =
      QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
  QString fileName = QString("log/csv/system_data_%1.csv").arg(dateStr);
  QFile file(fileName);

  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream stream(&file);

    stream << "Timestamp,Thrust,RPM,Voltage,Current,PWM\n";

    for (auto it = _dataBuffer.begin(); it != _dataBuffer.end(); ++it) {
      const auto &data = *it;
      stream << data.timestamp << "," << data.thrust << "," << data.rpm << ","
             << data.current << "," << data.voltage << "," << data.pwm << "\n";
    }

    file.close();

    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    _logTextEdit->append(
        QString("[%1] DATA SAVED: %2 (%3 records)")
            .arg(timestamp, fileName, QString::number(_dataBuffer.size())));
    saveLogToCSV(
        QString("[%1] DATA SAVED: %2 (%3 records)")
            .arg(timestamp, fileName, QString::number(_dataBuffer.size())));

    QMessageBox::information(
        this, "Success",
        QString("Data saved to %1\n%2 records written")
            .arg(fileName, QString::number(_dataBuffer.size())));
  } else {
    QMessageBox::critical(this, "Error", "Could not save data to file");
  }
}

void MainWindow::saveLogToCSV(const QString &message) {
  QDir().mkpath("log/history");
  QString dateStr = QDateTime::currentDateTime().toString("yyyy-MM-dd");
  QString fileName = QString("log/history/command_log_%1.csv").arg(dateStr);
  QFile file(fileName);

  if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
    QTextStream stream(&file);
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");

    if (file.pos() == 0)
      stream << "Timestamp,Message\n";

    stream << timestamp << "," << "\"" << message << "\"" << "\n";

    file.close();
  }
}
