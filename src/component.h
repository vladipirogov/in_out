#ifndef COMPONENT_H
#define COMPONENT_H


template <class T, class R> class Component
{
 public:
    virtual T outStateCtrl(R input) = 0;
};

#endif // COMPONENT_H
