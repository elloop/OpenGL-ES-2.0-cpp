#pragma once
#include "elloop/inc.h"

NS_BEGIN(elloop);

struct Size
{
    Size(float width = 0, float height = 0) :
    width_(width),
    height_(height)
    {}

    float           width_;
    float           height_;
};

NS_END(elloop);