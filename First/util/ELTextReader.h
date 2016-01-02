#pragma once
#include "elloop/inc.h"
#include <string>

NS_BEGIN(elloop);

class TextReader {
public:
    static const std::string readFromFile(const std::string& fileName);
};

NS_END(elloop);