#include "lcdcontroller.h"
#include <wiringPi.h>
#include <lcd.h>

#define LCD_RS 3
#define LCD_E 14
#define LCD_D4 4
#define LCD_D5 12
#define LCD_D6 13
#define LCD_D7 6

LCDController::LCDController()
{
    lcdHandle = -1;
}

bool LCDController::initialize()
{
    lcdHandle =
            lcdInit(
                2,
                16,
                4,
                LCD_RS,
                LCD_E,
                LCD_D4,
                LCD_D5,
                LCD_D6,
                LCD_D7,
                0,
                0,
                0,
                0);

    if(lcdHandle < 0)
    {
        return false;
    }

    lcdClear(lcdHandle);
    //lcdPosition i lcdPrintf  koristi umesto lcdClear ako treperi LCD

    return true;
}

void LCDController::display(
        double temp,
        const QString &status)
{
    lcdClear(lcdHandle);

    lcdPosition(lcdHandle,0,0);

    lcdPrintf(
                lcdHandle,
                "Temp:%4.1f C",
                temp);

    lcdPosition(lcdHandle,0,1);

    lcdPuts(
                lcdHandle,
                status.toStdString().c_str());
}

