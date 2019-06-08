#ifndef LIGHTING_H
#define LIGHTING_H
#include "component.h"

#define HIGH 0x1
#define LOW  0x0

class Lighting: public Component<int, int>
{
private:

    void(*delay)(unsigned long);

    int minLight = 0;
    int maxLight = 0;

    int lightMeterControl(int lux);
    int espControl();
    int touchButtonControl();

public:
    Lighting(int minLight, int maxLight, void(*delay)(unsigned long), unsigned long time);
    Lighting();

    struct {
         int outLite = HIGH;
         int output = LOW;
    } statuses;

    struct {
        unsigned long time;
        int wifiIn;
        int wifiInOff;
        int btnStat;
        int outWifi;
        int outBtn;
        int timeLiteEnab;
        int isAutomatic;
    } property;

    void setMinLight(int minLight);
    void setMaxLight(int maxLight);
    void setDelay(void(*delay)(unsigned long));
    void setTime(unsigned long time);

    int outStateCtrl(int lux);
};

#endif // LIGHTING_H
