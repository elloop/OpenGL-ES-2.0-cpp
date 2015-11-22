#pragma  once

#include "elloop/inc.h"
#include "elgui/ELRef.h"

NS_BEGIN(elloop);

class Drawable : public Ref {
public:
    virtual void        render() {}
};

NS_END(elloop);