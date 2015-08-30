#pragma once
#include "elloop/inc.h"
#include "AutoReleasePool.h"

NS_BEGIN(elloop);

class Ref {
public:
    void    release() {
        --_rc;
        if (0 == _rc) {
            delete this;
        }
    }
    void    retain() {
        ++_rc;
    }
    size_t  retainCount() const {
        return _rc;
    }
    void autorelease() {
        PoolManager::getInstance()->autorelease(this);
    }
protected:
    Ref(): _rc(1) {}
    virtual ~Ref() {}
private:
    size_t   _rc;
};

NS_END(elloop);
