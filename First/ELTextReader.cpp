#include "ELTextReader.h"
#include <fstream>

NS_BEGIN(elloop);

const std::string TextReader::readFromFile(const std::string& fileName) {
    assert(!fileName.empty());
    std::ifstream fileStream;
    fileStream.open(fileName, std::ios::in);
    std::string result;
    std::string line;
    if (fileStream.is_open()) {
        while (getline(fileStream, line)) {
            result.append(line + "\n");
        }
    }
    fileStream.close();
    return result;
}

NS_END(elloop);