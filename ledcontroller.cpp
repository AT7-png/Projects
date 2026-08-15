#include "ledcontroller.h"

#include <wiringPi.h>

#define LED_PIN 25

LEDController::LEDController()
{
    state = false;
}

bool LEDController::initialize()
{
    pinMode(LED_PIN, OUTPUT);

    digitalWrite(LED_PIN, LOW);

    return true;
}

void LEDController::on()
{
    digitalWrite(LED_PIN, HIGH);

    state = true;
}

void LEDController::off()
{
    digitalWrite(LED_PIN, LOW);

    state = false;
}

void LEDController::toggle()
{
    state = !state;

    digitalWrite(LED_PIN, state);
}