#ifndef LCDCONTROLLER_H
#define LCDCONTROLLER_H

#include <QString>

class LCDController
{
public:

    LCDController();

    bool initialize();

    void display(double temp,
                 const QString &status);

private:

    int lcdHandle;
};

#endif