#ifndef SERIALDECORATOR_H
#define SERIALDECORATOR_H

#include "decorator.h"
#include "lighting.h"
#include <string.h>
#include "hashmap.h"

class SerialDecorator: public Decorator<int, int>
{

private:
    Lighting * component;

    class Foo
    {

    private:
        void(*func)(SerialDecorator*, int);
        SerialDecorator* decorator;
    public:
       Foo(SerialDecorator* decorator, void(*func)(SerialDecorator*, int)) {
           this->decorator = decorator;
           this->func = func;
       }

       virtual void operator () (int input) {func(decorator, input);}
    };


    void setMaxLight(int val);
    void setMinLight(int val);
    void rmtCtrlBtnOnn(int val);
    void rmtCtrlBtnOff(int val);
    void setCtrlMode(int val);
    void setTimeLiteEnab(int val);
    void def(int val);
    void printCurrentLite(int val);
    int val;
    Hashmap<Foo> map;
    Foo* foo;
    unsigned int(*print)(char*, int);
    char *parameter;



public:
    SerialDecorator(Lighting * component);
    virtual int outStateCtrl(int input);

    int parseSerialCommand(char* str, int value, unsigned int(*print)(char*, int));
    void (SerialDecorator::*func)(int);
};

#endif // SERIALDECORATOR_H
