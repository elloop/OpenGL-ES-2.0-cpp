#pragma  once

#include "elloop/inc.h"
#include "basic/ELRef.h"

NS_BEGIN(elloop);

class Drawable : public Ref
{
public:
    virtual void render() {}
};

NS_END(elloop);