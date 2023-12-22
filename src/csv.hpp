#ifndef CSV_H
#define CSV_H

#include <string_view>
#include <vector>
#include <unordered_map>

#include "file.hpp"

class CSV : public File
{
public:
    CSV(const std::string_view idFilePath);
    std::vector<std::string> split(const std::string_view s, char delimiter);
    void process(const std::string_view csvFileName);
    void writeOutput();

private:
    std::unordered_map<int, std::vector<int>> m_durationMap;
};


#endif //CSV_H