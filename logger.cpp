#include "mainwindow.h"
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QtCharts>

#include <QtWidgets>

void MainWindow::saveDataToPDF() {
  QString fileName = QDateTime::currentDateTime().toString(
      "'Report_'yyyy-MM-dd_hh-mm-ss'.pdf'");

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

  int i = 0;
  for (auto &data : bufferCopy) {
    i++;
    thrustSR->append(i, data.thrust);
    pwmSR->append(i, data.pwm);
  }

  int chartWidth = pageWidth * 0.8;
  int chartHeight = chartWidth * 0.5;

  // ---------- CHART 1 ----------
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

  // ---------- CHART 2 ----------
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
                   "Figure 2: PWM Chart");

  y += 250;

  delete chart1;
  delete chart2;

  pdf.newPage();
  y = 200;

  painter.drawText(margin, y, "Ghi chú (Bất thường/Hiện tượng quan sát được):");
  y += 150;
  painter.drawText(margin, y,
                   "..........................................................."
                   "..........................................................."
                   ".....................................................");
  y += 150;
  painter.drawText(margin, y,
                   "..........................................................."
                   "..........................................................."
                   ".....................................................");
  y += 150;
  painter.drawText(margin, y,
                   "..........................................................."
                   "..........................................................."
                   ".....................................................");

  y += 300;

  painter.drawText(margin, y, "Người thực hiện");
  painter.drawText(pageWidth - margin - 400, y, "Trưởng phòng Kỹ thuật");

  y += 80;

  painter.drawText(margin, y, "(Ký, ghi rõ họ tên)");
  painter.drawText(pageWidth - margin - 300, y, "(Ký, ghi rõ họ tên)");

  painter.end();

  QMessageBox::information(this, "Success",
                           QString("PDF saved to %1").arg(fileName));
}

void MainWindow::saveDataToCSV() {
  QString fileName = QDateTime::currentDateTime().toString(
      "'system_data_'yyyy-MM-dd_hh-mm-ss'.csv'");
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
  QString fileName =
      QDateTime::currentDateTime().toString("'command_log_'yyyy-MM-dd'.csv'");
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
