#include "temperature.h"

#include <QDir>
#include <QFile>
#include <QTextStream>

TemperatureSensor::TemperatureSensor()
{

}

bool TemperatureSensor::initialize()
{
    QDir dir("/sys/bus/w1/devices");

    QStringList list =
            dir.entryList(QStringList()
                          << "28-*",
                          QDir::Dirs);

    if(list.isEmpty())
    {
        return false;
    }

    sensorPath =
            "/sys/bus/w1/devices/"
            + list.first()
            + "/w1_slave";

    return true;
}

QString TemperatureSensor::getSensorPath() const
{
    return sensorPath;
}

double TemperatureSensor::readTemperature()
{
    QFile file(sensorPath);

    if(!file.open(QIODevice::ReadOnly))
    {
        return -999.0;
    }

    QTextStream in(&file);

    QString content = in.readAll();

    file.close();

    int index = content.indexOf("t=");

    if(index == -1)
    {
        return -999.0;
    }

    QString tempString =
            content.mid(index + 2);

    bool ok;

    double value =
            tempString.toDouble(&ok);

    if(!ok)
    {
        return -999.0;
    }

    return value / 1000.0;
}
