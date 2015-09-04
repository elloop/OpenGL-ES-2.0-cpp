#pragma once
#include "elloop/inc.h"

NS_BEGIN(elloop);

template <typename T>
class Singleton {
public:
    static T* getInstance();
protected:
    Singleton() {}
    virtual ~Singleton() {}
protected:
    static T* _instance;
};

template <typename T>
T* Singleton<T>::_instance = nullptr;

template <typename T>
T* Singleton<T>::getInstance() {
    if (!_instance) {
        _instance = new T;
    }
    return _instance;
}

NS_END(elloop);