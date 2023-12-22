#ifndef FILE_H
#define FILE_H

#include <string_view>
#include <vector>

class File
{
public:
    size_t countLines(const std::string_view filePath);
    std::vector<std::string> findLists(const std::string_view path, const std::string_view name, const std::string_view extension);
};

#endif // FILE_H