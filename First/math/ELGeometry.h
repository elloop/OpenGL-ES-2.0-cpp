#pragma once
#include "elloop/inc.h"

NS_BEGIN(elloop);

class Size {
public:
    Size(float width = 0, float height = 0) :
        _width(width),
        _height(height)
    {
    }

    float           _width;
    float           _height;
};

NS_END(elloop);