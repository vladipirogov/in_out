#ifndef DECORATOR_H
#define DECORATOR_H

#include "component.h"

template <typename  T, typename  R> class Decorator: public Component<T, R>
{
public:
    Component<T, R>* component;
   virtual T outStateCtrl(R input) {
        return this->component->outStateCtrl(input);
    }

};

#endif // DECORATOR_H
