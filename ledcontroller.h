#ifndef LEDCONTROLLER_H
#define LEDCONTROLLER_H

class LEDController
{
public:

    LEDController();

    bool initialize();

    void on();

    void off();

    void toggle();

private:

    bool state;
};

#endif