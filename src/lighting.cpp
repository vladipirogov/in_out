#include "lighting.h"

Lighting::Lighting(int minLight, int maxLight, void(*delay)(unsigned long), unsigned long time)
{
    this->setMinLight(minLight);
    this->setMaxLight(maxLight);
    this->delay = delay;
    this->property.time = time;
}

Lighting::Lighting() {
  
}

/**
 * @brief Lighting::setMinLight
 * @param minLight
 */
void Lighting::setMinLight(int minLight)
{
    this->minLight = minLight;
    this->statuses.outLite = HIGH;
}

/**
 * @brief Lighting::setMaxLight
 * @param maxLight
 */
void Lighting::setMaxLight(int maxLight)
{

    this->maxLight = maxLight;
    this->statuses.outLite = HIGH;
}

/**
 * @brief Lighting::setDelay
 * @param delay
 */
void Lighting::setDelay(void(*delay)(unsigned long)) {
  this->delay = delay;
}

/**
 * @brief Lighting::setTime
 * @param time
 */
void Lighting::setTime(unsigned long time) {
  this->property.time = time;
}

/**
 * @brief Lighting::lightMeterControl
 * @param lux
 * @return
 */
int Lighting::lightMeterControl(int lux)
{
    if (lux < this->minLight && this->property.timeLiteEnab == HIGH) {
        this->statuses.outLite = HIGH;
      }
    if (lux > this->maxLight || this->property.timeLiteEnab == LOW) {
        this->statuses.outLite = LOW;
    }
    return this->statuses.outLite;
}

/**
 * @brief Lighting::espControl
 * @return
 */
int Lighting::espControl()
{
    if (this->property.wifiIn == HIGH) {
        this->statuses.output = HIGH;
      }
     if (this->property.wifiInOff == HIGH) {
        this->statuses.output = LOW;
    }
     return this->statuses.output;
}

/**
 * @brief Lighting::touchButtonControl
 * @return
 */
int Lighting::touchButtonControl()
{
    if (this->property.btnStat == HIGH)
      {
        if (this->statuses.output == HIGH)
        {
          this->delay(this->property.time);
          this->statuses.output = LOW;
        }
        else
        {
          this->delay(this->property.time);
          this->statuses.output = HIGH;
        }
      }
    return  this->statuses.output;
}

/**
 * @brief Lighting::outStateCtrl
 * @param lux
 * @return
 */
int Lighting::outStateCtrl(int lux)
{
    int tmpOut;
    this->lightMeterControl(lux);
    this->espControl();
    this->touchButtonControl();

    if (this->property.isAutomatic == HIGH) {
        tmpOut = this->statuses.outLite;
    }
    if (this->property.isAutomatic == LOW) {
        tmpOut = this->statuses.output;
    }
    return tmpOut;
}
