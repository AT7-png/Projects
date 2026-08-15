#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <QString>

class TemperatureSensor
{
public:

    TemperatureSensor();

    bool initialize();

    double readTemperature();

    QString getSensorPath() const;

private:

    QString sensorPath;
};

#endif // TEMPERATURE_H