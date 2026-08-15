#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "lcdcontroller.h"
#include "ledcontroller.h"
#include "temperature.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void readTemperature();
    void blinkAlarm();

    void on_btnStart_clicked();
    void on_btnStop_clicked();
    void on_btnReset_clicked();

private:

    Ui::MainWindow *ui;

    QTimer *tempTimer;
    QTimer *alarmTimer;

    double currentTemp;
    double minTemp;
    double maxTemp;
    double avgTemp;

    double sumTemp;
    int sampleCount;

    bool alarmActive;
    bool ledState;

    void saveLog(double temperature);

    TemperatureSensor sensor;

    LCDController lcd;

    LEDController led;
};

#endif