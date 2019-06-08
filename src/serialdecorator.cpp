#include "serialdecorator.h"

/**
 * @brief SerialDecorator::setMaxLight
 * @param val
 */
void SerialDecorator::setMaxLight(int val)
{
    (*print)(parameter, val);
    component->setMaxLight(val);
}

/**
 * @brief SerialDecorator::setMinLight
 * @param val
 */
void SerialDecorator::setMinLight(int val)
{
    (*print)(parameter, val);
    component->setMinLight(val);
}

/**
 * @brief SerialDecorator::rmtCtrlBtnOnn
 * @param val
 */
void SerialDecorator::rmtCtrlBtnOnn(int val)
{
    this->component->property.wifiIn = HIGH;
}

/**
 * @brief SerialDecorator::rmtCtrlBtnOff
 * @param val
 */
void SerialDecorator::rmtCtrlBtnOff(int val)
{
    this->component->property.wifiInOff = HIGH;
}

/**
 * @brief SerialDecorator::setCtrlMode
 * @param val
 */
void SerialDecorator::setCtrlMode(int val)
{
    this->component->property.isAutomatic = val;
}

/**
 * @brief SerialDecorator::setTimeLiteEnab
 * @param val
 */
void SerialDecorator::setTimeLiteEnab(int val)
{
    this->component->property.timeLiteEnab = val;
}

/**
 * @brief SerialDecorator::def
 * @param val
 */
void SerialDecorator::def(int val){}

void SerialDecorator::printCurrentLite(int val) {
}

/**
 * @brief SerialDecorator::SerialDecorator
 * @param component
 */
SerialDecorator::SerialDecorator(Lighting *component) : component(component)
{
    Decorator::component = this->component;

    this->map.put("maxtrs", new Foo(this, [](SerialDecorator* decorator, int value) {decorator->setMaxLight(value);}));
    this->map.put("mintrs", new Foo(this, [](SerialDecorator* decorator, int value) {decorator->setMinLight(value);}));
    this->map.put("load_1", new Foo(this, [](SerialDecorator* decorator, int value) {decorator->rmtCtrlBtnOnn(value);}));
    this->map.put("load_0", new Foo(this, [](SerialDecorator* decorator, int value) {decorator->rmtCtrlBtnOff(value);}));
    this->map.put("automode", new Foo(this, [](SerialDecorator* decorator, int value) {decorator->setCtrlMode(value);}));
    this->map.put("timeliteenab", new Foo(this, [](SerialDecorator* decorator, int value) {decorator->setTimeLiteEnab(value);}));
    this->map.put("lightmeter", new Foo(this, [](SerialDecorator* decorator, int value) {decorator->printCurrentLite(value);}));
}

/**
 * @brief SerialDecorator::outStateCtrl
 * @param input
 * @return
 */
int SerialDecorator::outStateCtrl(int input)
{
    if (foo != 0) {
        (*foo)(val);
        foo = 0;
    }
  return Decorator::outStateCtrl(input);
}

/**
 * @brief SerialDecorator::parseSerialCommand
 * @param str
 * @param value
 */
int SerialDecorator::parseSerialCommand(char *str, int value, unsigned int(*print)(char*, int))
{
    this->val = value;
    this->parameter = str;
    foo = map.get(str);
    this->print = print;
    return value;
}
