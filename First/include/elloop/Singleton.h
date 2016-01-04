#pragma once
#include "elloop/inc.h"

NS_BEGIN(elloop);

template <typename T>
class Singleton
{
public:
    static T* getInstance();
protected:
    Singleton() {}
    virtual ~Singleton() {}
protected:
    static T* instance_;
};

template <typename T>
T* Singleton<T>::instance_ = nullptr;

template <typename T>
T* Singleton<T>::getInstance()
{
    if (!instance_)
    {
        instance_ = new T;
    }
    return instance_;
}

NS_END(elloop);