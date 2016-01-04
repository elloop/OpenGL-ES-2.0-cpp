#pragma once

#include "elloop/inc.h"

NS_BEGIN(elloop);

class ApplicationProtocol
{
public:
    virtual bool applicationDidFinishLaunching() = 0;
};

NS_END(elloop);