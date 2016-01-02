#pragma once
#include "elloop/inc.h"
#include "ELAutoReleasePool.h"

NS_BEGIN(elloop);

class Ref
{
public:
    void    release()
    {
        --rc_;
        if (0 == rc_)
        {
            delete this;
        }
    }
    void    retain()
    {
        ++rc_;
    }
    size_t  retainCount() const
    {
        return rc_;
    }
    void autorelease()
    {
        PoolManager::getInstance()->autorelease(this);
    }
protected:
    Ref() : rc_(1) {}
    virtual ~Ref() {}
private:
    size_t   rc_;
};

NS_END(elloop);
