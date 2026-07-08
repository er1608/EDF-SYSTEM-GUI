#ifndef AUTOCALIB_H
#define AUTOCALIB_H

#include <QDialog>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>

class MainWindow;

class AutoCalibDialog : public QDialog {
  Q_OBJECT
public:
  explicit AutoCalibDialog(MainWindow *mainWindow, const QString &lcId,
                           QWidget *parent = nullptr);
  ~AutoCalibDialog();

private slots:
  void sendAutoCalibCmd();
  void toggleStream();
  void updateStream();
  void startCalibration();
  void setInitialValue();
  void collectSample();

private:
  MainWindow *m_mainWindow;
  QString m_lcId;

  QLabel *m_lblLcId;
  QLabel *m_lblThrust;
  QSpinBox *m_numSamples;
  QDoubleSpinBox *m_sampleMass;
  QPushButton *m_btnSendCmd;
  QPushButton *m_btnStream;
  QPushButton *m_btnInitialValue;
  QPushButton *m_btnStartCalib;

  QTimer *m_streamTimer;
  QTimer *m_sampleTimer;

  int m_sampleCount;
  double m_sumThrust;
  double m_sumThrustInitial;
  bool m_isCollectingInitial;
};

#endif // AUTOCALIB_H
