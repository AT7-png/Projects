#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include "temperature.h"
#include <QDebug>
#include <QMessageBox>
#include <wiringPi.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //pocetno stanje
    ui->labelCurrentTemp->setText("0.0 C");
    ui->labelMinTemp->setText("0.0 C");
    ui->labelMaxTemp->setText("0.0 C");
    ui->labelAvgTemp->setText("0.0 C");
    ui->labelStatus->setText("NORMAL");
    ui->labelAlarm->setText("Alarm LED: OFF");

    if(wiringPiSetup() == -1)
    {
    QMessageBox::critical(
                this,
                "WiringPi",
                "Greska pri inicijalizaciji WiringPi biblioteke!");

    return;
    }

    tempTimer = new QTimer(this);
    alarmTimer = new QTimer(this);

    connect(tempTimer,
            SIGNAL(timeout()),
            this,
            SLOT(readTemperature()));

    connect(alarmTimer,
            SIGNAL(timeout()),
            this,
            SLOT(blinkAlarm()));

    currentTemp = 0.0;

    minTemp = 999.0;
    maxTemp = -999.0;

    avgTemp = 0.0;

    sumTemp = 0.0;
    sampleCount = 0;

    alarmActive = false;
    ledState = false;

    if(!sensor.initialize())
    {
        QMessageBox::critical(
                    this,
                    "DS18B20",
                    "Sensor nije pronadjen!");
        return;
    }

    if(!lcd.initialize())
    {
        QMessageBox::critical(
                    this,
                    "LCD",
                    "LCD nije inicijalizovan!");
        return;
    }

    if(!led.initialize())
    {
        QMessageBox::critical(
                    this,
                    "LED",
                    "LED nije inicijalizovana!");
        return;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveLog(double temperature)
{
    QFile file("temperature_log.txt");

    if(file.open(QIODevice::Append))
    {
        QTextStream out(&file);

        out
            << QDateTime::currentDateTime()
                  .toString("yyyy-MM-dd hh:mm:ss")
            << "  "
            << temperature
            << "\n";

        file.close();
    }
}

void MainWindow::on_btnStart_clicked()
{
    tempTimer->start(1000);
}

void MainWindow::on_btnStop_clicked()
{
    tempTimer->stop();
    alarmTimer->stop();
    led.off();
    ui->labelAlarm->setText(
                "Alarm LED: OFF");
}

void MainWindow::on_btnReset_clicked()
{
    minTemp = 999.0;
    maxTemp = -999.0;

    sumTemp = 0.0;
    sampleCount = 0;

    avgTemp = 0.0;

    ui->labelMinTemp->setText("0.0");
    ui->labelMaxTemp->setText("0.0");
    ui->labelAvgTemp->setText("0.0");
}

void MainWindow::readTemperature()
{
    currentTemp = sensor.readTemperature();

    if(currentTemp < -100)
    {
        return;
    }

    saveLog(currentTemp);

    sampleCount++;

    sumTemp += currentTemp;

    avgTemp = sumTemp / sampleCount;

    if(currentTemp < minTemp)
    {
        minTemp = currentTemp;
    }

    if(currentTemp > maxTemp)
    {
        maxTemp = currentTemp;
    }

    ui->labelCurrentTemp->setText(
                QString::number(currentTemp,'f',1)
                + " °C");

    ui->labelMinTemp->setText(
                QString::number(minTemp,'f',1)
                + " °C");

    ui->labelMaxTemp->setText(
                QString::number(maxTemp,'f',1)
                + " °C");

    ui->labelAvgTemp->setText(
                QString::number(avgTemp,'f',1)
                + " °C");

    double Tmin =
            ui->spinBoxMin->value();

    double Tmax =
            ui->spinBoxMax->value();

    if(currentTemp < Tmin)
    {
        ui->labelStatus->setText(
                    "HEATING");

        alarmActive = true;
    }
    else if(currentTemp > Tmax)
    {
        ui->labelStatus->setText(
                    "COOLING");

        alarmActive = true;
    }
    else
    {
        ui->labelStatus->setText(
                    "NORMAL");

        alarmActive = false;

        alarmTimer->stop();

        led.off();

        ui->labelAlarm->setText(
            "Alarm LED: OFF");
    }

    lcd.display(
            currentTemp,
            ui->labelStatus->text()
        );

    if(alarmActive)
    {
        if(!alarmTimer->isActive())
        {
            alarmTimer->start(500);
        }
    }
}

void MainWindow::blinkAlarm()
{
    ledState = !ledState;

    led.toggle();

    if(ledState)
    {
        ui->labelAlarm->setText(
                    "Alarm LED: ON");
    }
    else
    {
        ui->labelAlarm->setText(
                    "Alarm LED: OFF");
    }
}
