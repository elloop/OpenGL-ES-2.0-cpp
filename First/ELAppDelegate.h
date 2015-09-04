#pragma once
#include "elloop/inc.h"
#include "ELApplication.h"

NS_BEGIN(elloop);

class AppDelegate : public Application {
public:
    AppDelegate();
    ~AppDelegate();

    bool applicationDidFinishLaunching() override;
};

NS_END(elloop);